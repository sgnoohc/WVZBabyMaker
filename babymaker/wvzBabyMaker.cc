#include "wvzBabyMaker.h"

//##############################################################################################################
wvzBabyMaker::wvzBabyMaker() : processor(0), babyMode(wvzBabyMaker::kWVZ)
{
}

wvzBabyMaker::~wvzBabyMaker() {}

//##############################################################################################################
void wvzBabyMaker::SetBabyMode(wvzBabyMaker::BabyMode bm)
{
    babyMode = bm;
}

//##############################################################################################################
void wvzBabyMaker::PrintBabyMode()
{
    switch (babyMode)
    {
        case kWVZ:
            std::cout << "Set to WVZ Baby Maker Mode" << std::endl;
            break;
        case kTrilep:
            std::cout << "Set to Trilep Baby Maker Mode" << std::endl;
            break;
        case kDilep:
            std::cout << "Set to Dilep Baby Maker Mode" << std::endl;
            break;
        case kWVZMVA:
            std::cout << "Set to WVZMVA Baby Maker Mode" << std::endl;
            break;
        case kTruth:
            std::cout << "Set to Truth Baby Maker Mode" << std::endl;
            break;
        case kWVZAll:
            std::cout << "Set to WVZAll Baby Maker Mode" << std::endl;
            break;
    }
}

//##############################################################################################################
bool wvzBabyMaker::PassEventList()
{
    switch (babyMode)
    {
        case kWVZ:
            return true;
            break;
        case kTrilep:
            return true;
            break;
        case kDilep:
            return true;
            break;
        case kWVZMVA:
            return true;
            break;
        case kTruth:
            return true;
            break;
        case kWVZAll:
            return true;
            break;
        default:
            return false;
            break;
    }
}

//##############################################################################################################
void wvzBabyMaker::SetLeptonID()
{

    // TString fullpath = looper.getCurrentFileName();
    // TString dirname = gSystem->DirName(fullpath);
    // TString basename = gSystem->BaseName(dirname);
    // basename.ReplaceAll("pythia8", " ");
    // TString samplename = RooUtil::StringUtil::split(basename)[0] + "pythia8";
    // TString nanoaodpath = "/hadoop/cms/store/user/phchang/nanoaod/" + samplename;
    // additional_muon_vars.load(nanoaodpath + "/muonsv2.dat", ":", 3);
    // additional_elec_vars.load(nanoaodpath + "/electronsv2.dat", ":", 3);

    if (gconf.year == 2016)
    {
        gconf.ea_version = 2;
        gconf.el_addlep_veto = false;
        gconf.el_reliso_veto = 0.4;
        gconf.mu_addlep_veto = false;
        gconf.mu_reliso_veto = 0.4;
    }
    else if (gconf.year == 2017)
    {
        gconf.ea_version = 4;
        gconf.el_addlep_veto = false;
        gconf.el_reliso_veto = 0.4;
        gconf.mu_addlep_veto = false;
        gconf.mu_reliso_veto = 0.4;
    }
    else if (gconf.year == 2018)
    {
        gconf.ea_version = 4;
        gconf.el_addlep_veto = false;
        gconf.el_reliso_veto = 0.4;
        gconf.mu_addlep_veto = false;
        gconf.mu_reliso_veto = 0.4;
    }
}

//##############################################################################################################
void wvzBabyMaker::ProcessObjectsPrePassSelection()
{
    // Process leptons via CoreUtil
    ProcessLeptons();

}

//##############################################################################################################
void wvzBabyMaker::ProcessObjectsPostPassSelection()
{

    // Process MET (recalculate etc.)
    ProcessMET();

    // Triggers
    ProcessTriggers();

    // Loop over gen particles
    ProcessGenParticles();

    // Loop over Jets
    ProcessJets();

    // Loop over Fat Jets
    ProcessFatJets();

    // Loop over charged particle candidates
    ProcessTracks();

}

//##############################################################################################################
void wvzBabyMaker::ProcessElectrons()
{
    switch (babyMode)
    {
        case kWVZ:
            coreElectron.process(isPt10VeryLooserThanPOGVetoElectron);
            break;
        case kTrilep:
            coreElectron.process(isPt10VeryLooserThanPOGVetoElectron);
            break;
        case kDilep:
            coreElectron.process(isPt10VeryLooserThanPOGVetoElectron);
            break;
        case kWVZMVA:
            coreElectron.process(isPt10VVVAnalysisVetoElectron);
            break;
        case kTruth:
            coreElectron.process(isPt10POGMVAwpLooseElectron);
            break;
        case kWVZAll:
            coreElectron.process(isPt10VeryLooserThanPOGVetoElectron);
            break;
    }
}

//##############################################################################################################
void wvzBabyMaker::ProcessMuons()
{
    switch (babyMode)
    {
        case kWVZ:
            coreMuon.process(isPt10VeryLooserThanPOGVetoMuon);
            break;
        case kTrilep:
            coreMuon.process(isPt10VeryLooserThanPOGVetoMuon);
            break;
        case kDilep:
            coreMuon.process(isPt10VeryLooserThanPOGVetoMuon);
            break;
        case kWVZMVA:
            coreMuon.process(isPt10VVVAnalysisVetoMuon);
            break;
        case kTruth:
            coreMuon.process(isPt10POGVetoMuon);
            break;
        case kWVZAll:
            coreMuon.process(isPt10VeryLooserThanPOGVetoMuon);
            break;
    }
}

//##############################################################################################################
// Process set of triggers (sometimes it is computational expensive to compare TString so restrict to few trigger set)
void wvzBabyMaker::ProcessTriggers()
{
    switch (babyMode)
    {
        case kWVZ:
            coreTrigger.process(CoreUtil::trigger::kDilepton);
            break;
        case kTrilep:
            coreTrigger.process(CoreUtil::trigger::kDilepton);
            break;
        case kDilep:
            coreTrigger.process(CoreUtil::trigger::kDilepton);
            break;
        case kWVZMVA:
            coreTrigger.process(CoreUtil::trigger::kDilepton);
            break;
        case kTruth:
            coreTrigger.process(CoreUtil::trigger::kDilepton);
            break;
        case kWVZAll:
            coreTrigger.process(CoreUtil::trigger::kDilepton);
            break;
    }
}

//##############################################################################################################
bool wvzBabyMaker::PassSelection()
{
    if (babyMode == kWVZ)
    {
        if (coreElectron.index.size() + coreMuon.index.size() < 4)
            return false;
        int nVetoLep = 0;
        int nLepPt25 = 0;
        for (auto& iel : coreElectron.index)
        {
            if (cms3.els_p4()[iel].pt() > 25.)
                nLepPt25++;
            if (isPt10POGVetoElectron(iel))
                nVetoLep++;
        }
        for (auto& imu : coreMuon.index)
        {
            if (cms3.mus_p4()[imu].pt() > 25.)
                nLepPt25++;
            if (isPt10POGVetoMuon(imu))
                nVetoLep++;
        }
        if (nVetoLep < 2)
            return false;
        if (nLepPt25 < 2)
            return false;
        return true;
    }
    else if (babyMode == kTrilep)
    {
        if (coreElectron.index.size() + coreMuon.index.size() < 3)
            return false;
        int nVetoLep = 0;
        int nLepPt25 = 0;
        for (auto& iel : coreElectron.index)
        {
            if (cms3.els_p4()[iel].pt() > 25.)
                nLepPt25++;
            if (isPt10POGVetoElectron(iel))
                nVetoLep++;
        }
        for (auto& imu : coreMuon.index)
        {
            if (cms3.mus_p4()[imu].pt() > 25.)
                nLepPt25++;
            if (isPt10POGVetoMuon(imu))
                nVetoLep++;
        }
        if (nVetoLep < 2)
            return false;
        if (nLepPt25 < 2)
            return false;
        return true;
    }
    else if (babyMode == kDilep)
    {
        if (coreElectron.index.size() + coreMuon.index.size() < 2)
            return false;
        int nLepPt25 = 0;
        int nLepPt15 = 0;
        for (auto& iel : coreElectron.index)
        {
            if (cms3.els_p4()[iel].pt() > 25.)
                nLepPt25++;
            if (cms3.els_p4()[iel].pt() > 15.)
                nLepPt15++;
        }
        for (auto& imu : coreMuon.index)
        {
            if (cms3.mus_p4()[imu].pt() > 25.)
                nLepPt25++;
            if (cms3.mus_p4()[imu].pt() > 15.)
                nLepPt15++;
        }
        if (not (nLepPt25 >= 1 and nLepPt15 >= 1))
            return false;
        return true;
    }
    else if (babyMode == kWVZMVA)
    {
        if (coreElectron.index.size() + coreMuon.index.size() < 4)
            return false;
        int nVetoLep = 0;
        int nLepPt25 = 0;
        for (auto& iel : coreElectron.index)
        {
            if (cms3.els_p4()[iel].pt() > 25.)
                nLepPt25++;
            if (isPt10VVVAnalysisVetoElectron(iel))
                nVetoLep++;
        }
        for (auto& imu : coreMuon.index)
        {
            if (cms3.mus_p4()[imu].pt() > 25.)
                nLepPt25++;
            if (isPt10VVVAnalysisVetoMuon(imu))
                nVetoLep++;
        }
        if (nVetoLep < 4)
            return false;
        if (nLepPt25 < 2)
            return false;
        return true;
    }
    else if (babyMode == kTruth)
    {
        return true;
    }
    else if (babyMode == kWVZAll)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//##############################################################################################################
void wvzBabyMaker::AddOutput()
{

    // initialize the processor connected to the main output TTreeX=tx.
    processor = new RooUtil::Processor(tx);

    // Add the lepton module which handles what variables to write, and how.
    if (babyMode == kTruth)
    {
        processor->AddModule(new wvzModule::GenPartModule(this));
    }
    else
    {
        processor->AddModule(new wvzModule::TriggerModule(this));
        processor->AddModule(new wvzModule::GenPartModule(this));
        processor->AddModule(new wvzModule::EventModule(this));
        processor->AddModule(new wvzModule::LeptonModule(this));
        processor->AddModule(new wvzModule::METModule(this));
        processor->AddModule(new wvzModule::JetModule(this));
        processor->AddModule(new wvzModule::FatJetModule(this));
        processor->AddModule(new wvzModule::AnalysisVariableModule(this));
        if (babyMode == kWVZ or babyMode == kWVZAll or babyMode == kWVZMVA)
        {
            processor->AddModule(new wvzModule::BDTInputVariableModule(this));
            processor->AddModule(new wvzModule::BDTModule(this));
        }
    }

    // Now create the outputs to the ttree
    processor->AddOutputs();

}

//##############################################################################################################
void wvzBabyMaker::FillOutput()
{
    processor->FillOutputs();
}

//##############################################################################################################
// Very Loose Lepton ID
bool wvzBabyMaker::isPt10Electron(int idx)
{
    if (!( cms3.els_p4()[idx].pt() > 10.          )) return false;
    if (gconf.year == 2017 or gconf.year == 2018)
    {
        if (!( isMVAHZZNoIsofall17(idx, true)     )) return false;
    }
    else
    {
        if (fabs(cms3.els_etaSC()[idx]) < 0.8)
        {
            if (!( cms3.els_VIDSpring16HZZMvaValue().at(idx) > -0.870 )) return false;
        }
        else if (fabs(cms3.els_etaSC()[idx]) <= 1.479)
        {
            if (!( cms3.els_VIDSpring16HZZMvaValue().at(idx) > -0.838 )) return false;
        }
        else
        {
            if (!( cms3.els_VIDSpring16HZZMvaValue().at(idx) > -0.763 )) return false;
        }
    }
    if (!( fabs(cms3.els_p4()[idx].eta()) < 2.5   )) return false;
    if (!( eleRelIso03EA(
                    idx,
                    gconf.ea_version,
                    gconf.el_addlep_veto)
                < gconf.el_reliso_veto            )) return false;
    if (!( fabs(cms3.els_dzPV()[idx]) < 0.1       )) return false;
    if (!( fabs(cms3.els_dxyPV()[idx]) < 0.05     )) return false;
    return true;
}

//##############################################################################################################
// Very Loose Lepton ID
bool wvzBabyMaker::isPt10Muon(int idx)
{
    if (!( cms3.mus_p4()[idx].pt() > 10.        )) return false;
    if (!( isLooseMuonPOG(idx)                  )) return false;
    if (!( fabs(cms3.mus_p4()[idx].eta()) < 2.5 )) return false;
    if (!( muRelIso03EA(
                    idx,
                    gconf.ea_version,
                    gconf.mu_addlep_veto)
                < gconf.mu_reliso_veto          )) return false;
    if (!( fabs(cms3.mus_dzPV()[idx]) < 0.1     )) return false;
    if (!( fabs(cms3.mus_dxyPV()[idx]) < 0.05   )) return false;
    return true;
}

//##############################################################################################################
// Very Loose Lepton ID
bool wvzBabyMaker::isPt10POGVetoElectron(int idx)
{
    if (!( cms3.els_p4()[idx].pt() > 10.          )) return false;
    if (!( isVetoElectronPOGfall17_v2(idx)        )) return false;
    if (!( fabs(cms3.els_p4()[idx].eta()) < 2.5   )) return false;
    if (fabs(cms3.els_etaSC()[idx]) <= 1.479)
    {
        if (!( fabs(cms3.els_dzPV()[idx]) < 0.1       )) return false;
        if (!( fabs(cms3.els_dxyPV()[idx]) < 0.05     )) return false;
    }
    else
    {
        if (!( fabs(cms3.els_dzPV()[idx]) < 0.2       )) return false;
        if (!( fabs(cms3.els_dxyPV()[idx]) < 0.1      )) return false;
    }
    if (!( fabs(cms3.els_ip3d()[idx] / cms3.els_ip3derr()[idx]) < 4. )) return false;
    return true;
}

//##############################################################################################################
// Very Loose Lepton ID
bool wvzBabyMaker::isPt10LooserThanPOGVetoElectron(int idx)
{
    if (!( cms3.els_p4()[idx].pt() > 10.          )) return false;
    // if (!(isVetoElectronPOGfall17_v2(idx)        )) return false;
    if (!( isVetoElectronPOGfall17noIso_v2(idx)   )) return false;
    if (!( eleRelIso03EA(idx, 4) < 0.6            )) return false;
    if (!( fabs(cms3.els_p4()[idx].eta()) < 2.5   )) return false;
    if (fabs(cms3.els_etaSC()[idx]) <= 1.479)
    {
        if (!( fabs(cms3.els_dzPV()[idx]) < 0.1       )) return false;
        if (!( fabs(cms3.els_dxyPV()[idx]) < 0.05     )) return false;
    }
    else
    {
        if (!( fabs(cms3.els_dzPV()[idx]) < 0.2       )) return false;
        if (!( fabs(cms3.els_dxyPV()[idx]) < 0.1      )) return false;
    }
    if (!( fabs(cms3.els_ip3d()[idx] / cms3.els_ip3derr()[idx]) < 4. )) return false;
    return true;
}

//##############################################################################################################
// Very Loose Lepton ID
bool wvzBabyMaker::isPt10VeryLooserThanPOGVetoElectron(int idx)
{
    if (!( cms3.els_p4()[idx].pt() > 10.          )) return false;
    // if (!(isVetoElectronPOGfall17_v2(idx)        )) return false;
    if (!( isVetoElectronPOGfall17noIso_v2(idx)   )) return false;
    // if (!( eleRelIso03EA(idx, 4) < 0.6            )) return false;
    if (!( fabs(cms3.els_p4()[idx].eta()) < 2.5   )) return false;
    if (fabs(cms3.els_etaSC()[idx]) <= 1.479)
    {
        if (!( fabs(cms3.els_dzPV()[idx]) < 0.1       )) return false;
        if (!( fabs(cms3.els_dxyPV()[idx]) < 0.05     )) return false;
    }
    else
    {
        if (!( fabs(cms3.els_dzPV()[idx]) < 0.2       )) return false;
        if (!( fabs(cms3.els_dxyPV()[idx]) < 0.1      )) return false;
    }
    // if (!( fabs(cms3.els_ip3d()[idx] / cms3.els_ip3derr()[idx]) < 4. )) return false;
    return true;
}

//##############################################################################################################
// Very Loose Lepton ID
bool wvzBabyMaker::isPt10POGMVAwpLooseElectron(int idx)
{
    if (!( cms3.els_p4()[idx].pt() > 10.          )) return false;
    if (!( isMVAwpLooseIsofall17V2(idx)           )) return false;
    if (!( fabs(cms3.els_p4()[idx].eta()) < 2.5   )) return false;
    if (fabs(cms3.els_etaSC()[idx]) <= 1.479)
    {
        if (!( fabs(cms3.els_dzPV()[idx]) < 0.1       )) return false;
        if (!( fabs(cms3.els_dxyPV()[idx]) < 0.05     )) return false;
    }
    else
    {
        if (!( fabs(cms3.els_dzPV()[idx]) < 0.2       )) return false;
        if (!( fabs(cms3.els_dxyPV()[idx]) < 0.1      )) return false;
    }
    return true;
}

//##############################################################################################################
// Slightly Loose Lepton ID
bool wvzBabyMaker::isPt10POGMVAwpHZZElectron(int idx)
{
    if (!( cms3.els_p4()[idx].pt() > 10.          )) return false;
    if (!( isMVAHZZIsofall17V2(idx)         )) return false;
    if (!( fabs(cms3.els_p4()[idx].eta()) < 2.5   )) return false;
    if (fabs(cms3.els_etaSC()[idx]) <= 1.479)
    {
        if (!( fabs(cms3.els_dzPV()[idx]) < 0.1       )) return false;
        if (!( fabs(cms3.els_dxyPV()[idx]) < 0.05     )) return false;
    }
    else
    {
        if (!( fabs(cms3.els_dzPV()[idx]) < 0.2       )) return false;
        if (!( fabs(cms3.els_dxyPV()[idx]) < 0.1      )) return false;
    }
    return true;
}

//##############################################################################################################
// Very Loose Lepton ID
bool wvzBabyMaker::isPt10POGVetoMuon(int idx)
{
    if (!( cms3.mus_p4()[idx].pt() > 10.        )) return false;
    if (!( isMediumMuonPOG(idx)                 )) return false;
    if (!( fabs(cms3.mus_p4()[idx].eta()) < 2.4 )) return false;
    if (!( muRelIso04DB(idx)  < 0.25            )) return false;
    if (fabs(cms3.mus_p4()[idx].eta()) <= 1.479)
    {
        if (!( fabs(cms3.mus_dzPV()[idx]) < 0.1       )) return false;
        if (!( fabs(cms3.mus_dxyPV()[idx]) < 0.05     )) return false;
    }
    else
    {
        if (!( fabs(cms3.mus_dzPV()[idx]) < 0.2       )) return false;
        if (!( fabs(cms3.mus_dxyPV()[idx]) < 0.1      )) return false;
    }
    if (!( fabs(cms3.mus_ip3d()[idx] / cms3.mus_ip3derr()[idx]) < 4. )) return false;
    return true;
}

//##############################################################################################################
// Very Loose Lepton ID
bool wvzBabyMaker::isPt10LooserThanPOGVetoMuon(int idx)
{
    if (!( cms3.mus_p4()[idx].pt() > 10.        )) return false;
    if (!( isMediumMuonPOG(idx)                 )) return false;
    if (!( fabs(cms3.mus_p4()[idx].eta()) < 2.4 )) return false;
    if (!( muRelIso04DB(idx)  < 0.8             )) return false;
    if (fabs(cms3.mus_p4()[idx].eta()) <= 1.479)
    {
        if (!( fabs(cms3.mus_dzPV()[idx]) < 0.1       )) return false;
        if (!( fabs(cms3.mus_dxyPV()[idx]) < 0.05     )) return false;
    }
    else
    {
        if (!( fabs(cms3.mus_dzPV()[idx]) < 0.2       )) return false;
        if (!( fabs(cms3.mus_dxyPV()[idx]) < 0.1      )) return false;
    }
    if (!( fabs(cms3.mus_ip3d()[idx] / cms3.mus_ip3derr()[idx]) < 4. )) return false;
    return true;
}

//##############################################################################################################
// Very Loose Lepton ID
bool wvzBabyMaker::isPt10VeryLooserThanPOGVetoMuon(int idx)
{
    if (!( cms3.mus_p4()[idx].pt() > 10.        )) return false;
    if (!( isMediumMuonPOG(idx)                 )) return false;
    if (!( fabs(cms3.mus_p4()[idx].eta()) < 2.4 )) return false;
    // if (!( muRelIso04DB(idx)  < 0.8             )) return false;
    if (fabs(cms3.mus_p4()[idx].eta()) <= 1.479)
    {
        if (!( fabs(cms3.mus_dzPV()[idx]) < 0.1       )) return false;
        if (!( fabs(cms3.mus_dxyPV()[idx]) < 0.05     )) return false;
    }
    else
    {
        if (!( fabs(cms3.mus_dzPV()[idx]) < 0.2       )) return false;
        if (!( fabs(cms3.mus_dxyPV()[idx]) < 0.1      )) return false;
    }
    // if (!( fabs(cms3.mus_ip3d()[idx] / cms3.mus_ip3derr()[idx]) < 4. )) return false;
    return true;
}

//##############################################################################################################
// Veto ID of the analysis
bool wvzBabyMaker::isPt10AnalysisVetoElectron(int idx)
{
    if (not( isPt10POGVetoElectron(idx) )) return false;
    return true;
}

//##############################################################################################################
// Veto ID of the analysis
bool wvzBabyMaker::isPt10AnalysisVetoMuon(int idx)
{
    if (not( isPt10POGVetoMuon(idx) )) return false;
    return true;
}

//##############################################################################################################
// Nominal ID of the analysis
bool wvzBabyMaker::isPt10AnalysisNominalElectron(int idx)
{
    if (not( isPt10POGVetoElectron(idx)        )) return false;
    if (not( isMediumElectronPOGfall17_v2(idx) )) return false;
    return true;
}

//##############################################################################################################
// Nominal ID of the analysis
bool wvzBabyMaker::isPt10AnalysisNominalMuon(int idx)
{
    if (not( isPt10POGVetoMuon(idx)      )) return false;
    if (not( muRelIso04DB(idx)  < 0.15   )) return false;
    return true;
}

//##############################################################################################################
// Very Loose Lepton ID
bool wvzBabyMaker::isPt10VeryLooserThanVVVAnalysisVetoElectron(int idx)
{
    if (!( cms3.els_p4()[idx].pt() > 10.          )) return false;
    if (!( isMVAwpLooseNoIsofall17V2(idx, true)   )) return false;
    if (!( fabs(cms3.els_p4()[idx].eta()) < 2.5   )) return false;
    if (!( fabs(cms3.els_dxyPV()[idx]) < 0.05     )) return false;
    if (!( fabs(cms3.els_dzPV()[idx]) < 0.1       )) return false;
    // if (!( eleRelIso03EA(idx, 4, true) < 0.4      )) return false;
    return true;
}

//##############################################################################################################
// Very Loose Lepton ID
bool wvzBabyMaker::isPt10VeryLooserThanVVVAnalysisVetoMuon(int idx)
{
    if (!( cms3.mus_p4()[idx].pt() > 10.          )) return false;
    if (!( isLooseMuonPOG(idx)                    )) return false;
    if (!( fabs(cms3.mus_p4()[idx].eta()) < 2.4   )) return false;
    if (!( fabs(cms3.mus_dxyPV()[idx]) < 0.05     )) return false;
    if (!( fabs(cms3.mus_dzPV()[idx]) < 0.1       )) return false;
    return true;
}

//##############################################################################################################
// Very Loose Lepton ID
bool wvzBabyMaker::isPt10VVVAnalysisVetoElectron(int idx)
{
    if (!( isPt10VeryLooserThanVVVAnalysisVetoElectron(idx) )) return false;
    if (!( eleRelIso03EA(idx, 4, true) < 0.4                )) return false;
    return true;
}

//##############################################################################################################
// Very Loose Lepton ID
bool wvzBabyMaker::isPt10VVVAnalysisVetoMuon(int idx)
{
    if (!( isPt10VeryLooserThanVVVAnalysisVetoMuon(idx) )) return false;
    if (!( muRelIso03EA(idx, 4, true) < 0.4             )) return false;
    return true;
}

//##############################################################################################################
// Very Loose Lepton ID
bool wvzBabyMaker::isPt10VVVAnalysisNominalElectron(int idx)
{
    if (!( isPt10VVVAnalysisVetoElectron(idx) )) return false;
    if (!( eleRelIso03EA(idx, 4, true) < 0.4  )) return false;
    return true;
}

//##############################################################################################################
// Very Loose Lepton ID
bool wvzBabyMaker::isPt10VVVAnalysisNominalMuon(int idx)
{
    if (!( isPt10VVVAnalysisVetoMuon(idx)   )) return false;
    if (!( isMediumMuonPOG(idx)             )) return false;
    if (!( muRelIso04DB(idx)  < 0.15        )) return false;
    return true;
}

//##############################################################################################################
bool wvzBabyMaker::isLeptonOverlappingWithJet(int ijet)
{
    bool is_overlapping = false;
    int idx = coreJet.index[ijet];
    const LV& jet_p4 = cms3.pfjets_p4()[idx];

    for (unsigned ilep = 0; ilep < cms3.els_p4().size(); ++ilep)
    {
        const LV& p4 = cms3.els_p4()[ilep];
        if (!( isPt10Electron(ilep) )) continue;
        if (ROOT::Math::VectorUtil::DeltaR(jet_p4, p4) < 0.4)
        {
            is_overlapping = true;
            break;
        }
    }

    for (unsigned ilep = 0; ilep < cms3.mus_p4().size(); ++ilep)
    {
        const LV& p4 = cms3.mus_p4()[ilep];
        if (!( isPt10Muon(ilep) )) continue;
        if (ROOT::Math::VectorUtil::DeltaR(jet_p4, p4) < 0.4)
        {
            is_overlapping = true;
            break;
        }
    }

    if (is_overlapping)
        return true;

    return false;
}

//##############################################################################################################
bool wvzBabyMaker::isPOGLeptonOverlappingWithJet(int ijet)
{
    bool is_overlapping = false;
    int idx = coreJet.index[ijet];
    const LV& jet_p4 = cms3.pfjets_p4()[idx];

    for (unsigned ilep = 0; ilep < cms3.els_p4().size(); ++ilep)
    {
        const LV& p4 = cms3.els_p4()[ilep];
        if (!( isPt10POGVetoElectron(ilep) )) continue;
        if (ROOT::Math::VectorUtil::DeltaR(jet_p4, p4) < 0.4)
        {
            is_overlapping = true;
            break;
        }
    }

    for (unsigned ilep = 0; ilep < cms3.mus_p4().size(); ++ilep)
    {
        const LV& p4 = cms3.mus_p4()[ilep];
        if (!( isPt10POGVetoMuon(ilep) )) continue;
        if (ROOT::Math::VectorUtil::DeltaR(jet_p4, p4) < 0.4)
        {
            is_overlapping = true;
            break;
        }
    }

    if (is_overlapping)
        return true;

    return false;
}

//##############################################################################################################
bool wvzBabyMaker::isVVVAnalysisLeptonOverlappingWithJet(int ijet)
{
    bool is_overlapping = false;
    int idx = coreJet.index[ijet];
    const LV& jet_p4 = cms3.pfjets_p4()[idx];

    for (unsigned ilep = 0; ilep < cms3.els_p4().size(); ++ilep)
    {
        const LV& p4 = cms3.els_p4()[ilep];
        if (!( isPt10VVVAnalysisVetoElectron(ilep) )) continue;
        if (ROOT::Math::VectorUtil::DeltaR(jet_p4, p4) < 0.4)
        {
            is_overlapping = true;
            break;
        }
    }

    for (unsigned ilep = 0; ilep < cms3.mus_p4().size(); ++ilep)
    {
        const LV& p4 = cms3.mus_p4()[ilep];
        if (!( isPt10VVVAnalysisVetoMuon(ilep) )) continue;
        if (ROOT::Math::VectorUtil::DeltaR(jet_p4, p4) < 0.4)
        {
            is_overlapping = true;
            break;
        }
    }

    if (is_overlapping)
        return true;

    return false;
}

//##############################################################################################################
bool wvzBabyMaker::isMVAPOGLeptonOverlappingWithJet(int ijet)
{
    bool is_overlapping = false;
    int idx = coreJet.index[ijet];
    const LV& jet_p4 = cms3.pfjets_p4()[idx];

    for (unsigned ilep = 0; ilep < cms3.els_p4().size(); ++ilep)
    {
        const LV& p4 = cms3.els_p4()[ilep];
        if (!( isPt10POGMVAwpLooseElectron(ilep) )) continue;
        if (ROOT::Math::VectorUtil::DeltaR(jet_p4, p4) < 0.4)
        {
            is_overlapping = true;
            break;
        }
    }

    for (unsigned ilep = 0; ilep < cms3.mus_p4().size(); ++ilep)
    {
        const LV& p4 = cms3.mus_p4()[ilep];
        if (!( isPt10POGVetoMuon(ilep) )) continue;
        if (ROOT::Math::VectorUtil::DeltaR(jet_p4, p4) < 0.4)
        {
            is_overlapping = true;
            break;
        }
    }

    if (is_overlapping)
        return true;

    return false;
}
