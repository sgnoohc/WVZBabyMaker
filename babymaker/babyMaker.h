#ifndef babyMaker_h
#define babyMaker_h

// C++
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <mutex>
#include <algorithm>

// ROOT
#include "TChain.h"
#include "TDirectory.h"
#include "TTreeCache.h"
#include "Math/VectorUtil.h"
#include "TVector2.h"
#include "TBenchmark.h"
#include "TLorentzVector.h"
#include "TH2.h"
#include "TH3.h"
#include "TROOT.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TH2.h"
#include "TString.h"
#include "TMVA/Reader.h"
#include "Math/LorentzVector.h"
#include "Math/GenVector/LorentzVector.h"

// CORE
#include "CMS3.h"
#include "Base.h"
#include "OSSelections.h"
#include "SSSelections.h"
#include "VVVSelections.h"
#include "ElectronSelections.h"
#include "IsolationTools.h"
#include "JetSelections.h"
#include "MuonSelections.h"
#include "IsoTrackVeto.h"
#include "PhotonSelections.h"
#include "TriggerSelections.h"
#include "VertexSelections.h"
#include "MCSelections.h"
#include "MetSelections.h"
#include "SimPa.h"
#include "Tools/MT2/MT2.h"
#include "Tools/hemJet.h"
#include "Tools/utils.h"
#include "Tools/goodrun.h"
#include "Tools/btagsf/BTagCalibrationStandalone.h"
#include "Tools/btagsf/BTagCalibrationStandalone.h"
#include "Tools/datasetinfo/getDatasetInfo.h"

// RooUtil
#include "rooutil.h"

// CoreUtil
#include "jec.h"
#include "btag.h"
#include "btagdeepcsv.h"
#include "puwgt.h"
#include "genpart.h"
#include "trigger.h"
#include "electron.h"
#include "muon.h"
#include "grl.h"
#include "datasetinfo.h"
#include "jet.h"
#include "met.h"
#include "track.h"
#include "fatjet.h"
#include "sample.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;

// `````````````````````````````````````````````````````````````````````````````````````````````````````````````
//
//
//
// Baby Maker Class
//
//
//
// ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,

class babyMaker
{

public:

    CoreUtil::jec coreJec;
    CoreUtil::jec coreFatJec;
    CoreUtil::grl coreGRL;
    CoreUtil::btag coreBtagSF;
    CoreUtil::btag coreBtagSFFastSim;
    CoreUtil::btagdeepcsv coreBtagDeepCSVSF;
    CoreUtil::puwgt corePUWgt;
    CoreUtil::trigger coreTrigger;
    CoreUtil::genpart coreGenPart;
    CoreUtil::electron coreElectron;
    CoreUtil::muon coreMuon;
    CoreUtil::datasetinfo coreDatasetInfo;
    CoreUtil::jet coreJet;
    CoreUtil::fatjet coreFatJet;
    CoreUtil::met coreMET;
    CoreUtil::track coreTrack;
    CoreUtil::sample coreSample;

    TFile* ofile;
    TTree* t;
    TH1F* h_neventsinfile;
    RooUtil::TTreeX* tx;
    RooUtil::Looper<CMS3> looper;
    RooUtil::EventList eventlist;

    std::string filename;

    // Job
    int job_index;

    babyMaker();
    virtual ~babyMaker();

    int ProcessCMS4(TString filepaths, int max_events = -1, int index = 1, bool verbose = false);

    void ScanChain(TString filepaths, int max_events = -1, bool verbose = false);
    void Init();
    void SetYear();
    void CreateOutput();
    virtual void SetEventList();
    virtual bool PassEventList();
    virtual void AddOutput();
    virtual void SetLeptonID();

    void Process();
    void FillGenWeights();
    void FillGenWeightsNominal();
    void ProcessLeptons();
    virtual void ProcessElectrons();
    virtual void ProcessMuons();

    virtual bool PassSelection();

    virtual void ProcessObjectsPrePassSelection();
    virtual void ProcessObjectsPostPassSelection();
    virtual void ProcessTriggers();
    void ProcessGenParticles();
    void ProcessJets();
    void ProcessFatJets();
    void ProcessMET();
    void ProcessTracks();

    virtual void FillOutput();
    virtual void FillEventInfo();

    void SaveOutput();

    // Tools
    static void sortVecP4(std::vector<LV>& p4s);
    void FATALERROR(const char* funcname="");

    // Sample handling
    TString SampleNiceName() { return coreSample.nicename(looper.getCurrentFileName()); }

    //
    // Processor units
    //

};


#endif
