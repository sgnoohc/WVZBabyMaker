#include "babyMaker.h"

using namespace std;

//##############################################################################################################
babyMaker::babyMaker()
{
}

//##############################################################################################################
babyMaker::~babyMaker()
{
}

//##############################################################################################################
int babyMaker::ProcessCMS4(TString filepaths, int max_events, int idx, bool verbose)
{

    // Create TChain to process
    //   - From the filepaths (e.g. filepaths="file.root,file2.root,...") create a TChain
    TChain* chain = RooUtil::FileUtil::createTChain("Events", filepaths);

    // Initialize Looper
    looper.init(chain, &cms3, max_events);
//    looper.setNbadEventThreshold(0); // If there are any bad events throw std::ios_base::failure exception.
    looper.setEventIndexMap("eventindexmap.txt");

    // Initializer job index
    job_index = idx;

    //
    //
    // Run the loop !
    //
    //
    ScanChain(verbose);

    // Exit
    return 0;

}

//##############################################################################################################
std::once_flag flag_init; // a flag for std::call_once
void babyMaker::ScanChain(bool verbose)
{

    // Create output (e.g. output TFile, TTree, nevents histograms)
    CreateOutput();

    // The looper object will handle accessing each events
    // The CMS3.cc object throws std::ios_base::failure exception if the input CMS3/4 files are corrupted.
    // For Monte Carlo events, we can tolerate some percentage of failures as long as we take care of them properly.
    // For data events, this is not acceptable. So we call FATALERROR and exit the program altogether with status code = 2
    while (looper.nextEvent())
    {

        // // Try catch statement there to catch any CMS4 ntuple TTree event I/O failure
        // try
        // {

            // If verbose print progress
            if (verbose)
                cout << "[verbose] Processed " << looper.getNEventsProcessed() << " out of " << looper.getTChain()->GetEntries() << endl;

            // Some of the sample specific things need to be set prior to processing the very first event
            // Those are called inside Init, they have explicit "call_once" feature
            // The reason this is inside the loop is because it first need to access the file name before
            std::call_once(flag_init, &babyMaker::Init, this);

            // In case we need to filter out events based on the event list
            if (not PassEventList())
                continue;

            // Now process the baby ntuples
            Process();

            // Fill the gen level weights (calling after Process() in order to account for skipped events)
            FillGenWeights();

        // }
        // catch (const std::ios_base::failure& e)
        // {

        //     // If data events and a failure is detected, we need to completely fail it.
        //     // We cannot lose any single data events.
        //     if (coreSample.is2016Data(looper.getCurrentFileName()) || coreSample.is2017Data(looper.getCurrentFileName()) || coreSample.is2018Data(looper.getCurrentFileName()))
        //     {
        //         std::cout << std::endl;
        //         std::cout << "Found bad event in data" << std::endl;
        //         FATALERROR(__FUNCTION__);
        //         exit(2);
        //     }
        //     // If not data events (i.e. MC events) we can tolerate a little.
        //     // Previous Looper::setNbadEventThreshold() function setting will dictatate whether to accept or fail
        //     else
        //     {

        //         tx->clear(); // clear the TTree of any residual stuff

        //         // If the number of events that has been skipped reaches a threshold set by Looper::setNbadEventThreshold(),
        //         // we skip the input ntuple entirely. This is because some inputs are just bad and the entire file will
        //         // keep throwing exceptions.
        //         if (!looper.handleBadEvent())
        //             break;
        //     }
        // }
    }

    // Print status before exiting (for monitoring performance)
    looper.printStatus();

    // Save the output
    SaveOutput();

    return;
}

//##############################################################################################################
void babyMaker::SetEventList()
{
}

//##############################################################################################################
bool babyMaker::PassEventList()
{
    return true;
}

//##############################################################################################################
void babyMaker::Init()
{
    // get global configs
    gconf.GetConfigsFromDatasetName(looper.getCurrentFileName());

    // Provide which file it is and whether it is fast sim or not to JEC to determine which file to load
    coreJec.setJECFor(looper.getCurrentFileName(), false);

    // Provide which file it is and whether it is fast sim or not to JEC to determine which file to load
    coreFatJec.setFatJECFor(looper.getCurrentFileName());

    // Set up btagging deepcsv scalefactor machinery
    coreBtagDeepCSVSF.setup(false, gconf.year);

    // Signal or BSM samples need some specifics things to be set prior to starting the processing
    AddOutput();

    // Set lepton ID configuration via "GlobalConfig gconf"
    SetLeptonID();

    // Setup event list to filter
    SetEventList();

}

//##############################################################################################################
void babyMaker::CreateOutput()
{
    ofile = new TFile(Form("output_%d.root", job_index), "recreate");
    t = new TTree("t", "All events");
    tx = new RooUtil::TTreeX(t);

    // Below are things related to event weights which must be done before skipping events
    h_neventsinfile = new TH1F("h_neventsinfile", "", 16, 0, 16);
    h_neventsinfile->SetBinContent(0, looper.getTChain()->GetEntries()); // this is the bin with value = -1 underflow

    // Below are things related to event weights which must be done before skipping events
    tx->createBranch<Int_t>("run");
    tx->createBranch<Int_t>("lumi");
    tx->createBranch<unsigned long long>("evt");
    tx->createBranch<int>("isData");
    tx->createBranch<float>("evt_scale1fb");
    tx->createBranch<int>("nevents");
    tx->createBranch<float>("genps_weight");
    tx->createBranch<float>("xsec_br");
    tx->createBranch<int>("evt_passgoodrunlist");
    tx->createBranch<TString>("CMS4path");
    tx->createBranch<int>("CMS4index");

    tx->createBranch<float>("weight_fr_r1_f1");
    tx->createBranch<float>("weight_fr_r1_f2");
    tx->createBranch<float>("weight_fr_r1_f0p5");
    tx->createBranch<float>("weight_fr_r2_f1");
    tx->createBranch<float>("weight_fr_r2_f2");
    tx->createBranch<float>("weight_fr_r2_f0p5");
    tx->createBranch<float>("weight_fr_r0p5_f1");
    tx->createBranch<float>("weight_fr_r0p5_f2");
    tx->createBranch<float>("weight_fr_r0p5_f0p5");
    tx->createBranch<float>("weight_pdf_up");
    tx->createBranch<float>("weight_pdf_down");
    tx->createBranch<float>("weight_alphas_down");
    tx->createBranch<float>("weight_alphas_up");
    tx->createBranch<float>("weight_nominal");

}

//##############################################################################################################
void babyMaker::AddOutput()
{
}

//##############################################################################################################
void babyMaker::SetLeptonID()
{
}

//##############################################################################################################
void babyMaker::Process()
{

    // Process objects needed to determine preselection
    ProcessObjectsPrePassSelection();

    // If the event fails preselection skip
    if (!PassSelection())
        return;

    // Process other non-lepton objects via CoreUtil
    ProcessObjectsPostPassSelection();

    // Fill the basic event information
    FillEventInfo();

    // Fill the output ttree
    FillOutput();

}

//##############################################################################################################
void babyMaker::FillGenWeights()
{
    if (cms3.evt_isRealData())
        return;

    FillGenWeightsNominal();
}

//##############################################################################################################
void babyMaker::FillGenWeightsNominal()
{
    float sum_of_pdf_weights = 0;
    float average_of_pdf_weights = 0;

    // Things that don't depend on PDF
    h_neventsinfile->Fill(0., 1.); // event count
    h_neventsinfile->Fill(15, (cms3.genps_weight() > 0) - (cms3.genps_weight() < 0)); // event count
    tx->setBranch<float>("weight_fr_r1_f1"     , cms3.gen_LHEweight_QCDscale_muR1_muF1());
    tx->setBranch<float>("weight_fr_r1_f2"     , cms3.gen_LHEweight_QCDscale_muR1_muF2());
    tx->setBranch<float>("weight_fr_r1_f0p5"   , cms3.gen_LHEweight_QCDscale_muR1_muF0p5());
    tx->setBranch<float>("weight_fr_r2_f1"     , cms3.gen_LHEweight_QCDscale_muR2_muF1());
    tx->setBranch<float>("weight_fr_r2_f2"     , cms3.gen_LHEweight_QCDscale_muR2_muF2());
    tx->setBranch<float>("weight_fr_r2_f0p5"   , cms3.gen_LHEweight_QCDscale_muR2_muF0p5());
    tx->setBranch<float>("weight_fr_r0p5_f1"   , cms3.gen_LHEweight_QCDscale_muR0p5_muF1());
    tx->setBranch<float>("weight_fr_r0p5_f2"   , cms3.gen_LHEweight_QCDscale_muR0p5_muF2());
    tx->setBranch<float>("weight_fr_r0p5_f0p5" , cms3.gen_LHEweight_QCDscale_muR0p5_muF0p5());
    tx->setBranch<float>("weight_pdf_up"       , cms3.gen_LHEweight_PDFVariation_Up());
    tx->setBranch<float>("weight_pdf_down"     , cms3.gen_LHEweight_PDFVariation_Dn());
    tx->setBranch<float>("weight_alphas_down"  , cms3.gen_LHEweight_AsMZ_Up());
    tx->setBranch<float>("weight_alphas_up"    , cms3.gen_LHEweight_AsMZ_Dn());
    tx->setBranch<float>("weight_nominal"      , cms3.genHEPMCweight());
    h_neventsinfile->Fill(1  , tx->getBranch<float>("weight_fr_r1_f1"));
    h_neventsinfile->Fill(2  , tx->getBranch<float>("weight_fr_r1_f2"));
    h_neventsinfile->Fill(3  , tx->getBranch<float>("weight_fr_r1_f0p5"));
    h_neventsinfile->Fill(4  , tx->getBranch<float>("weight_fr_r2_f1"));
    h_neventsinfile->Fill(5  , tx->getBranch<float>("weight_fr_r2_f2"));
    h_neventsinfile->Fill(6  , tx->getBranch<float>("weight_fr_r2_f0p5"));
    h_neventsinfile->Fill(7  , tx->getBranch<float>("weight_fr_r0p5_f1"));
    h_neventsinfile->Fill(8  , tx->getBranch<float>("weight_fr_r0p5_f2"));
    h_neventsinfile->Fill(9  , tx->getBranch<float>("weight_fr_r0p5_f0p5"));
    h_neventsinfile->Fill(10 , tx->getBranch<float>("weight_pdf_up"));
    h_neventsinfile->Fill(11 , tx->getBranch<float>("weight_pdf_down"));
    h_neventsinfile->Fill(12 , tx->getBranch<float>("weight_alphas_down"));
    h_neventsinfile->Fill(13 , tx->getBranch<float>("weight_alphas_up"));
    h_neventsinfile->Fill(14 , tx->getBranch<float>("weight_nominal"));
}

//##############################################################################################################
void babyMaker::ProcessLeptons()
{
    // Loop over electrons
    ProcessElectrons();

    // Loop over muons
    ProcessMuons();

}

//##############################################################################################################
void babyMaker::ProcessElectrons()
{
}

//##############################################################################################################
void babyMaker::ProcessMuons()
{
}

//##############################################################################################################
bool babyMaker::PassSelection()
{
    return true;
}

//##############################################################################################################
void babyMaker::ProcessObjectsPrePassSelection()
{
    // Process leptons via CoreUtil
    ProcessLeptons();

    // Loop over fatJets
    ProcessFatJets();
}

//##############################################################################################################
void babyMaker::ProcessObjectsPostPassSelection()
{
    // Triggers
    ProcessTriggers();

    // Loop over gen particles
    ProcessGenParticles();

    // Loop over Jets
    ProcessJets();

    // Process MET (recalculate etc.)
    ProcessMET();

    // Loop over charged particle candidates
    ProcessTracks();
}

//##############################################################################################################
void babyMaker::ProcessTriggers() { coreTrigger.process(); }

//##############################################################################################################
void babyMaker::ProcessGenParticles() { coreGenPart.process(); }

//##############################################################################################################
void babyMaker::ProcessJets() { coreJet.process(coreJec); }

//##############################################################################################################
void babyMaker::ProcessFatJets() { coreFatJet.process(coreFatJec); }

//##############################################################################################################
void babyMaker::ProcessMET() { coreMET.process(coreJec); }

//##############################################################################################################
void babyMaker::ProcessTracks() { coreTrack.process(); }

//##############################################################################################################
void babyMaker::FillEventInfo()
{

    tx->setBranch<Int_t>("run", cms3.evt_run());
    tx->setBranch<Int_t>("lumi", cms3.evt_lumiBlock());
    tx->setBranch<unsigned long long>("evt", cms3.evt_event());
    tx->setBranch<int>("isData", cms3.evt_isRealData());
    if (cms3.evt_isRealData())
    {
        tx->setBranch<float>("evt_scale1fb", 1);
        tx->setBranch<int>("nevents", 1);
        tx->setBranch<float>("genps_weight", 1);
        tx->setBranch<float>("xsec_br", 1);
        tx->setBranch<int>("evt_passgoodrunlist", goodrun(cms3.evt_run(), cms3.evt_lumiBlock()));
    }
    else
    {
        float scale1fb = coreDatasetInfo.getScale1fb();
        float xsec = coreDatasetInfo.getXsec();
        int nevents = coreDatasetInfo.getNEvents();
        tx->setBranch<float>("evt_scale1fb", scale1fb);
        tx->setBranch<int>("nevents", nevents);
        tx->setBranch<float>("genps_weight", cms3.genps_weight());
        tx->setBranch<float>("xsec_br", xsec);
        tx->setBranch<int>("evt_passgoodrunlist", true);
    }
    tx->setBranch<TString>("CMS4path", looper.getCurrentFileName());
    tx->setBranch<int>("CMS4index", looper.getCurrentEventIndex());

}

//##############################################################################################################
void babyMaker::FillOutput()
{
    tx->fill();
}

//##############################################################################################################
void babyMaker::SaveOutput()
{

    ofile->cd();

    // This is always saved
    h_neventsinfile->Write();

    // Write the TTree
    t->Write();

}

//---------------==================-----------------==================-----------------=================----------
//---------------==================-----------------==================-----------------=================----------
//---------------==================-----------------==================-----------------=================----------
//---------------==================-----------------==================-----------------=================----------
//---------------==================-----------------==================-----------------=================----------

//##############################################################################################################
// FATAL error code
void babyMaker::FATALERROR(const char* funcname)
{
    std::cout <<  " cms3.evt_run(): " << cms3.evt_run() <<  " cms3.evt_lumiBlock(): " << cms3.evt_lumiBlock() <<  " cms3.evt_event(): " << cms3.evt_event() <<  std::endl;
    cout << "FATAL ERROR: I Should never be here! In function : " << funcname << endl;
}

//##############################################################################################################
void babyMaker::sortVecP4(vector<LV>& v)
{
    std::sort(v.begin(), v.end(), [](const LV& a, const LV& b){return a.pt() > b.pt();});
}

//eof
