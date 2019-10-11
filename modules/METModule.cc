#include "METModule.h"

//==============================================================================================================
//
// MET Module
//
//==============================================================================================================

void wvzModule::METModule::AddOutput()
{

    tx->createBranch<float>("met_orig_pt");
    tx->createBranch<float>("met_orig_phi");
    tx->createBranch<float>("met_orig_up_pt");
    tx->createBranch<float>("met_orig_up_phi");
    tx->createBranch<float>("met_orig_dn_pt");
    tx->createBranch<float>("met_orig_dn_phi");
    tx->createBranch<LV>("met_p4");
    tx->createBranch<float>("met_pt");
    tx->createBranch<float>("met_phi");
    tx->createBranch<float>("met_up_pt");
    tx->createBranch<float>("met_up_phi");
    tx->createBranch<float>("met_dn_pt");
    tx->createBranch<float>("met_dn_phi");
    tx->createBranch<float>("met_smearup_pt");
    tx->createBranch<float>("met_smearup_phi");
    tx->createBranch<float>("met_smeardn_pt");
    tx->createBranch<float>("met_smeardn_phi");
    tx->createBranch<float>("met_pileupup_pt");
    tx->createBranch<float>("met_pileupup_phi");
    tx->createBranch<float>("met_pileupdn_pt");
    tx->createBranch<float>("met_pileupdn_phi");
    tx->createBranch<float>("met_gen_pt");
    tx->createBranch<float>("met_gen_phi");

    tx->createBranch<int>("passesMETfiltersRun2");
}

void wvzModule::METModule::FillOutput()
{

    correctMET();

    if (cms3.evt_isRealData())
    {
        tx->setBranch<float>("met_orig_pt", babymaker->coreMET.met_pt);
        tx->setBranch<float>("met_orig_phi", babymaker->coreMET.met_phi);
        tx->setBranch<float>("met_orig_up_pt", babymaker->coreMET.met_up_pt);
        tx->setBranch<float>("met_orig_up_phi", babymaker->coreMET.met_up_phi);
        tx->setBranch<float>("met_orig_dn_pt", babymaker->coreMET.met_dn_pt);
        tx->setBranch<float>("met_orig_dn_phi", babymaker->coreMET.met_dn_phi);
        tx->setBranch<LV>("met_p4", RooUtil::Calc::getLV(babymaker->coreMET.met_pt, 0, babymaker->coreMET.met_phi, 0));
        tx->setBranch<float>("met_pt", babymaker->coreMET.met_pt);
        tx->setBranch<float>("met_phi", babymaker->coreMET.met_phi);
        tx->setBranch<float>("met_up_pt", babymaker->coreMET.met_up_pt);
        tx->setBranch<float>("met_up_phi", babymaker->coreMET.met_up_phi);
        tx->setBranch<float>("met_dn_pt", babymaker->coreMET.met_dn_pt);
        tx->setBranch<float>("met_dn_phi", babymaker->coreMET.met_dn_phi);
        tx->setBranch<float>("met_smearup_pt", babymaker->coreMET.met_pt);
        tx->setBranch<float>("met_smearup_phi", babymaker->coreMET.met_phi);
        tx->setBranch<float>("met_smeardn_pt", babymaker->coreMET.met_pt);
        tx->setBranch<float>("met_smeardn_phi", babymaker->coreMET.met_phi);
        tx->setBranch<float>("met_pileupup_pt", babymaker->coreMET.met_pt);
        tx->setBranch<float>("met_pileupup_phi", babymaker->coreMET.met_phi);
        tx->setBranch<float>("met_pileupdn_pt", babymaker->coreMET.met_pt);
        tx->setBranch<float>("met_pileupdn_phi", babymaker->coreMET.met_phi);
        tx->setBranch<float>("met_gen_pt", cms3.gen_met());
        tx->setBranch<float>("met_gen_phi", cms3.gen_metPhi());
    }
    else
    {
        tx->setBranch<float>("met_orig_pt", babymaker->coreMET.met_pt);
        tx->setBranch<float>("met_orig_phi", babymaker->coreMET.met_phi);
        tx->setBranch<float>("met_orig_up_pt", babymaker->coreMET.met_up_pt);
        tx->setBranch<float>("met_orig_up_phi", babymaker->coreMET.met_up_phi);
        tx->setBranch<float>("met_orig_dn_pt", babymaker->coreMET.met_dn_pt);
        tx->setBranch<float>("met_orig_dn_phi", babymaker->coreMET.met_dn_phi);
        tx->setBranch<LV>("met_p4", RooUtil::Calc::getLV(metobj_corrected.extras.met, 0, metobj_corrected.extras.phi, 0));
        tx->setBranch<float>("met_pt", metobj_corrected.extras.met);
        tx->setBranch<float>("met_phi", metobj_corrected.extras.phi);
        tx->setBranch<float>("met_up_pt", metobj_corrected.extras.met_JECup);
        tx->setBranch<float>("met_up_phi", metobj_corrected.extras.phi_JECup);
        tx->setBranch<float>("met_dn_pt", metobj_corrected.extras.met_JECdn);
        tx->setBranch<float>("met_dn_phi", metobj_corrected.extras.phi_JECdn);
        tx->setBranch<float>("met_smearup_pt", metobj_corrected.extras.met_METup);
        tx->setBranch<float>("met_smearup_phi", metobj_corrected.extras.phi_METup);
        tx->setBranch<float>("met_smeardn_pt", metobj_corrected.extras.met_METdn);
        tx->setBranch<float>("met_smeardn_phi", metobj_corrected.extras.phi_METdn);
        tx->setBranch<float>("met_pileupup_pt", metobj_corrected.extras.met_PUup);
        tx->setBranch<float>("met_pileupup_phi", metobj_corrected.extras.phi_PUup);
        tx->setBranch<float>("met_pileupdn_pt", metobj_corrected.extras.met_PUdn);
        tx->setBranch<float>("met_pileupdn_phi", metobj_corrected.extras.phi_PUdn);
        tx->setBranch<float>("met_gen_pt", cms3.gen_met());
        tx->setBranch<float>("met_gen_phi", cms3.gen_metPhi());
    }

    tx->setBranch<int>("passesMETfiltersRun2", passesMETfiltersRun2(cms3.evt_isRealData()));
}

//______________________________________________________________________________________________
void wvzModule::METModule::correctMET()
{
    metobj.extras.met = metobj.extras.met_original = metobj.extras.met_raw
    = metobj.extras.met_METup = metobj.extras.met_METdn
    = metobj.extras.met_JERup = metobj.extras.met_JERdn
    = metobj.extras.met_PUup = metobj.extras.met_PUdn
    = metobj_corrected.extras.met = metobj_corrected.extras.met_original = metobj_corrected.extras.met_raw
    = metobj_corrected.extras.met_METup = metobj_corrected.extras.met_METdn
    = metobj_corrected.extras.met_JERup = metobj_corrected.extras.met_JERdn
    = metobj_corrected.extras.met_PUup = metobj_corrected.extras.met_PUdn
    = babymaker->coreMET.met_pt; // This is ptmiss from the MET recipe alone (nominal).

    metobj.extras.phi = metobj.extras.phi_original = metobj.extras.phi_raw
    = metobj.extras.phi_METup = metobj.extras.phi_METdn
    = metobj.extras.phi_JECup = metobj.extras.phi_JECdn
    = metobj.extras.phi_JERup = metobj.extras.phi_JERdn
    = metobj.extras.phi_PUup = metobj.extras.phi_PUdn
    = metobj_corrected.extras.phi = metobj_corrected.extras.phi_original = metobj_corrected.extras.phi_raw
    = metobj_corrected.extras.phi_METup = metobj_corrected.extras.phi_METdn
    = metobj_corrected.extras.phi_JECup = metobj_corrected.extras.phi_JECdn
    = metobj_corrected.extras.phi_JERup = metobj_corrected.extras.phi_JERdn
    = metobj_corrected.extras.phi_PUup = metobj_corrected.extras.phi_PUdn
    = babymaker->coreMET.met_phi; // Nominal MET phi from MET recipe alone

    metobj.extras.met_JECup = metobj_corrected.extras.met_JECup = babymaker->coreMET.met_up_pt; // MET JES up
    metobj.extras.met_JECdn = metobj_corrected.extras.met_JECdn = babymaker->coreMET.met_dn_pt; // MET JES dn
    metobj.extras.phi_JECup = metobj_corrected.extras.phi_JECup = babymaker->coreMET.met_up_phi; // MET phi JES up
    metobj.extras.phi_JECdn = metobj_corrected.extras.phi_JECdn = babymaker->coreMET.met_dn_phi; // MET phi JES dn

    metcorrector.correctMET(cms3.gen_met(), cms3.gen_metPhi(), &metobj_corrected, false);

}
