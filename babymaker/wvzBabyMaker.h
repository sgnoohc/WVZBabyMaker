#ifndef wvzBabyMaker_h
#define wvzBabyMaker_h

#include "babyMaker.h"

class wvzBabyMaker: public babyMaker
{
    public:
        wvzBabyMaker();
        ~wvzBabyMaker();

        enum BabyMode
        {
            kWVZ = 1,
            kDilep = 2,
            kTrilep = 3,
            kWVZMVA = 4,
            kTruth = 5,
            kWVZAll = 6,
        };

        RooUtil::Processor* processor;
        RooUtil::VarMap additional_elec_vars;
        RooUtil::VarMap additional_muon_vars;

        BabyMode babyMode;
        void SetBabyMode(BabyMode bm);
        void PrintBabyMode();

        virtual bool PassEventList();

        virtual void SetLeptonID();

        virtual void ProcessObjectsPrePassSelection();
        virtual void ProcessObjectsPostPassSelection();
        virtual void ProcessElectrons();
        virtual void ProcessMuons();
        virtual void ProcessTriggers();
        virtual bool PassSelection();

        virtual void AddOutput();
        virtual void FillOutput();

        static bool isPt10Electron(int idx);
        static bool isPt10Muon(int idx);
        bool isLeptonOverlappingWithJet(int ijet);

        static bool isPt10POGMVAwpLooseElectron(int idx);
        static bool isPt10POGMVAwpHZZElectron(int idx);
        static bool isPt10POGVetoElectron(int idx);
        static bool isPt10POGVetoMuon(int idx);
        static bool isPt10LooserThanPOGVetoElectron(int idx);
        static bool isPt10LooserThanPOGVetoMuon(int idx);
        static bool isPt10VeryLooserThanPOGVetoElectron(int idx);
        static bool isPt10VeryLooserThanPOGVetoMuon(int idx);
        static bool isPt10AnalysisVetoElectron(int idx);
        static bool isPt10AnalysisVetoMuon(int idx);
        static bool isPt10AnalysisNominalElectron(int idx);
        static bool isPt10AnalysisNominalMuon(int idx);

        static bool isPt10VeryLooserThanVVVAnalysisVetoElectron(int idx);
        static bool isPt10VeryLooserThanVVVAnalysisVetoMuon(int idx);
        static bool isPt10VVVAnalysisVetoElectron(int idx);
        static bool isPt10VVVAnalysisVetoMuon(int idx);
        static bool isPt10VVVAnalysisNominalElectron(int idx);
        static bool isPt10VVVAnalysisNominalMuon(int idx);

        bool isPOGLeptonOverlappingWithJet(int ijet);
        bool isVVVAnalysisLeptonOverlappingWithJet(int ijet);
        bool isMVAPOGLeptonOverlappingWithJet(int ijet);

};

#include "TriggerModule.h"
#include "GenPartModule.h"
#include "EventModule.h"
#include "LeptonModule.h"
#include "METModule.h"
#include "JetModule.h"
#include "FatJetModule.h"
#include "AnalysisVariableModule.h"
#include "BDTInputVariableModule.h"
#include "BDTModule.h"

#endif
