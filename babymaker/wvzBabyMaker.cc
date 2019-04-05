#include "wvzBabyMaker.h"

//##############################################################################################################
wvzBabyMaker::wvzBabyMaker() : processor(0)
{
}

wvzBabyMaker::~wvzBabyMaker() {}

//##############################################################################################################
void wvzBabyMaker::SetLeptonID()
{
    if (gconf.year == 2016)
    {
        gconf.ea_version = 2;
        gconf.el_addlep_veto = true;
        gconf.el_reliso_veto = 0.4;
        gconf.mu_addlep_veto = true;
        gconf.mu_reliso_veto = 0.4;
    }
    else if (gconf.year == 2017)
    {
        gconf.ea_version = 4;
        gconf.el_addlep_veto = true;
        gconf.el_reliso_veto = 0.4;
        gconf.mu_addlep_veto = true;
        gconf.mu_reliso_veto = 0.4;
    }
    else if (gconf.year == 2018)
    {
        gconf.ea_version = 4;
        gconf.el_addlep_veto = true;
        gconf.el_reliso_veto = 0.4;
        gconf.mu_addlep_veto = true;
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

    // Loop over charged particle candidates
    ProcessTracks();

}

//##############################################################################################################
void wvzBabyMaker::ProcessElectrons()
{
    coreElectron.process(isPt10Electron);
}

//##############################################################################################################
void wvzBabyMaker::ProcessMuons()
{
    coreMuon.process(isPt10Muon);
}

//##############################################################################################################
// Goal is to pass events with at least one fat jet and one lepton
bool wvzBabyMaker::PassSelection()
{
    return true;
}

//##############################################################################################################
void wvzBabyMaker::AddOutput()
{

    // initialize the processor connected to the main output TTreeX=tx.
    processor = new RooUtil::Processor(tx);

    // Add the lepton module which handles what variables to write, and how.
    processor->AddModule(new wvzModule::TriggerModule(this));
    processor->AddModule(new wvzModule::GenPartModule(this));
    processor->AddModule(new wvzModule::EventModule(this));
    processor->AddModule(new wvzModule::LeptonModule(this));
    processor->AddModule(new wvzModule::METModule(this));
    processor->AddModule(new wvzModule::JetModule(this));

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
        if (fabs(cms3.els_etaSC()[idx]) <= 1.479)
        {
            if (!( getMVAoutput(idx)      > 0.6   )) return false;
        }
        else
        {
            if (!( getMVAoutput(idx)      > 0.    )) return false;
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
