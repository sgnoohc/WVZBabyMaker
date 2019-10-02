#include "JetModule.h"

//==============================================================================================================
//
// Jet Module
//
//==============================================================================================================

void wvzModule::JetModule::AddOutput()
{

    tx->createBranch<vector<LV>>("jets_p4");
    tx->createBranch<vector<float>>("jets_pt");
    tx->createBranch<vector<float>>("jets_eta");
    tx->createBranch<vector<float>>("jets_phi");
    tx->createBranch<vector<float>>("jets_mass");
    tx->createBranch<vector<float>>("jets_btag_score");

    tx->createBranch<vector<LV>>("jets_cen_p4");
    tx->createBranch<vector<float>>("jets_cen_pt");
    tx->createBranch<vector<float>>("jets_cen_eta");
    tx->createBranch<vector<float>>("jets_cen_phi");
    tx->createBranch<vector<float>>("jets_cen_mass");

    tx->createBranch<vector<LV>>("rawjets_p4");
    tx->createBranch<vector<float>>("rawjets_pt");
    tx->createBranch<vector<float>>("rawjets_eta");
    tx->createBranch<vector<float>>("rawjets_phi");
    tx->createBranch<vector<float>>("rawjets_mass");
    tx->createBranch<vector<float>>("rawjets_JEC");
    tx->createBranch<vector<float>>("rawjets_btag_score");
    tx->createBranch<vector<int>>("rawjets_passJetID");
    tx->createBranch<vector<int>>("rawjets_isORwLep");

    tx->createBranch<int>("nj");
    tx->createBranch<int>("nj_up");
    tx->createBranch<int>("nj_dn");
    tx->createBranch<int>("nb");
    tx->createBranch<int>("nb_up");
    tx->createBranch<int>("nb_dn");
    tx->createBranch<int>("nbmed");
    tx->createBranch<int>("nbmed_up");
    tx->createBranch<int>("nbmed_dn");
    tx->createBranch<int>("nbtight");
    tx->createBranch<int>("nbtight_up");
    tx->createBranch<int>("nbtight_dn");
    tx->createBranch<float>("ht");
    tx->createBranch<int>("nj_cen");
    tx->createBranch<int>("nj_cen_up");
    tx->createBranch<int>("nj_cen_dn");

    tx->createBranch<float>("weight_btagsf");
    tx->createBranch<float>("weight_btagsf_heavy_DN");
    tx->createBranch<float>("weight_btagsf_heavy_UP");
    tx->createBranch<float>("weight_btagsf_light_DN");
    tx->createBranch<float>("weight_btagsf_light_UP");
    tx->createBranch<float>("weight_med_btagsf");
    tx->createBranch<float>("weight_med_btagsf_heavy_DN");
    tx->createBranch<float>("weight_med_btagsf_heavy_UP");
    tx->createBranch<float>("weight_med_btagsf_light_DN");
    tx->createBranch<float>("weight_med_btagsf_light_UP");
    tx->createBranch<float>("weight_tight_btagsf");
    tx->createBranch<float>("weight_tight_btagsf_heavy_DN");
    tx->createBranch<float>("weight_tight_btagsf_heavy_UP");
    tx->createBranch<float>("weight_tight_btagsf_light_DN");
    tx->createBranch<float>("weight_tight_btagsf_light_UP");

}

void wvzModule::JetModule::FillOutput()
{

    // Variables to compute and fill to the ttree output
    int nj = 0;
    int nj_up = 0;
    int nj_dn = 0;
    int nj_cen = 0;
    int nj_cen_up = 0;
    int nj_cen_dn = 0;
    int nb = 0;
    int nb_up = 0;
    int nb_dn = 0;
    int nbmed = 0;
    int nbmed_up = 0;
    int nbmed_dn = 0;
    int nbtight = 0;
    int nbtight_up = 0;
    int nbtight_dn = 0;
    float ht = 0;

    // For deep csv btagging sf
    std::vector<double> deepcsv_sf_jet_pt;
    std::vector<double> deepcsv_sf_jet_eta;
    std::vector<double> deepcsv_sf_jet_deepCSV;
    std::vector<int> deepcsv_sf_jet_flavour;

    // Loop over selected jets
    // coreJet.index contains index to the jets in cms3.pfjets_p4() vector
    for (unsigned ijet = 0; ijet < babymaker->coreJet.index.size(); ++ijet)
    {
        // Get the index
        int idx = babymaker->coreJet.index[ijet];

        // Get the JEC correction
        float corr = babymaker->coreJet.corrs[ijet];

        // Jet ID
        bool passJetID = false;
        if (gconf.year == 2016)
        {
            passJetID = isLoosePFJet_Summer16_v1(idx);
        }
        else if (gconf.year == 2017)
        {
            passJetID = isTightPFJet_2017_v1(idx);
        }
        else if (gconf.year == 2018)
        {
            passJetID = isTightPFJet_2018_v1(idx);
        }

        // Get the b-tagging value
        float current_btag_score_val = cms3.pfjets_pfDeepCSVJetTagsprobbPlusprobbb()[ijet];

        LV rawjet = cms3.pfjets_p4()[idx] * cms3.pfjets_undoJEC()[idx];
        tx->pushbackToBranch<LV>("rawjets_p4", rawjet);
        tx->pushbackToBranch<float>("rawjets_pt", rawjet.pt());
        tx->pushbackToBranch<float>("rawjets_eta", rawjet.eta());
        tx->pushbackToBranch<float>("rawjets_phi", rawjet.phi());
        tx->pushbackToBranch<float>("rawjets_mass", rawjet.mass());
        tx->pushbackToBranch<float>("rawjets_JEC", corr);
        tx->pushbackToBranch<float>("rawjets_btag_score", current_btag_score_val);
        tx->pushbackToBranch<int>("rawjets_passJetID", passJetID);
        tx->pushbackToBranch<int>("rawjets_isORwLep", babymaker->isPOGLeptonOverlappingWithJet(ijet));

        if (not passJetID)
            continue;

        // Get the JEC shift
        float shift = babymaker->coreJet.shifts[ijet];

        // Get the b-tagging value
        int hadron_flavor = cms3.pfjets_hadronFlavour()[ijet];

        LV jet = cms3.pfjets_p4()[idx] * cms3.pfjets_undoJEC()[idx] * corr;
        LV jet_up = jet * (1. + shift);
        LV jet_dn = jet * (1. - shift);

        // Check whether this jet overlaps with any of the leptons
        switch (babymaker->babyMode)
        {
            case wvzBabyMaker::kWVZ:
                if (babymaker->isPOGLeptonOverlappingWithJet(ijet)) continue;
                break;
            case wvzBabyMaker::kDilep:
                if (babymaker->isPOGLeptonOverlappingWithJet(ijet)) continue;
                break;
            case wvzBabyMaker::kTrilep:
                if (babymaker->isPOGLeptonOverlappingWithJet(ijet)) continue;
                break;
            case wvzBabyMaker::kWVZMVA:
                if (babymaker->isVVVAnalysisLeptonOverlappingWithJet(ijet)) continue;
                break;
            case wvzBabyMaker::kWVZAll:
                if (babymaker->isPOGLeptonOverlappingWithJet(ijet)) continue;
                break;
            case wvzBabyMaker::kTruth:
                if (babymaker->isPOGLeptonOverlappingWithJet(ijet)) continue;
                break;
        }

        // The pt of the jet
        float pt = jet.pt();

        // Require pt minimum requirement of 30 GeV for safety
        if (jet.pt() > 30)
        {

            // Increase the counter for the number of jets
            nj++;
            ht += pt;

            if (fabs(jet.eta()) < 2.5)
                nj_cen++;

        }

        // Require pt minimum requirement of 30 GeV for safety
        if (jet_up.pt() > 30)
        {

            // Increase the counter for the number of jets
            nj_up++;

            if (fabs(jet.eta()) < 2.5)
                nj_cen_up++;

        }

        // Require pt minimum requirement of 30 GeV for safety
        if (jet_dn.pt() > 30)
        {

            // Increase the counter for the number of jets
            nj_dn++;

            if (fabs(jet.eta()) < 2.5)
                nj_cen_dn++;

        }

        float btag_loose_threshold = gconf.WP_DEEPCSV_LOOSE;
        float btag_medium_threshold = gconf.WP_DEEPCSV_MEDIUM;
        float btag_tight_threshold = gconf.WP_DEEPCSV_TIGHT;

        // Counting N-btag jets: Require minimum of 20 GeV for increased acceptance
        if (jet.pt() > 20. and fabs(jet.eta()) < 2.4)
        {

            // Increase the counter for the number of b-jets
            if (current_btag_score_val >= btag_loose_threshold) nb++;
            if (current_btag_score_val >= btag_medium_threshold) nbmed++;
            if (current_btag_score_val >= btag_tight_threshold) nbtight++;

            // we will stick with loose b-tagging
            deepcsv_sf_jet_pt.push_back(jet.pt());
            deepcsv_sf_jet_eta.push_back(jet.eta());
            deepcsv_sf_jet_deepCSV.push_back(current_btag_score_val);
            deepcsv_sf_jet_flavour.push_back(abs(hadron_flavor));

        }

        // Counting N-btag jets: Require minimum of 20 GeV for increased acceptance
        if (jet_up.pt() > 20. and fabs(jet_up.eta()) < 2.4)
        {

            // Increase the counter for the number of b-jets
            if (current_btag_score_val >= btag_loose_threshold) nb_up++;
            if (current_btag_score_val >= btag_medium_threshold) nbmed_up++;
            if (current_btag_score_val >= btag_tight_threshold) nbtight_up++;

        }

        // Counting N-btag jets: Require minimum of 20 GeV for increased acceptance
        if (jet_dn.pt() > 20. and fabs(jet_dn.eta()) < 2.4)
        {

            // Increase the counter for the number of b-jets
            if (current_btag_score_val >= btag_loose_threshold) nb_dn++;
            if (current_btag_score_val >= btag_medium_threshold) nbmed_dn++;
            if (current_btag_score_val >= btag_tight_threshold) nbtight_dn++;

        }

        // Adding jets to the container
        if (jet.pt() > 30.)
        {
            tx->pushbackToBranch<LV>("jets_p4", jet);
            tx->pushbackToBranch<float>("jets_pt", jet.pt());
            tx->pushbackToBranch<float>("jets_eta", jet.eta());
            tx->pushbackToBranch<float>("jets_phi", jet.phi());
            tx->pushbackToBranch<float>("jets_mass", jet.mass());
            tx->pushbackToBranch<float>("jets_btag_score", current_btag_score_val);
        }

        // Adding jets to the container
        if (jet.pt() > 30. and fabs(jet.eta()) < 2.5)
        {
            tx->pushbackToBranch<LV>("jets_cen_p4", jet);
            tx->pushbackToBranch<float>("jets_cen_pt", jet.pt());
            tx->pushbackToBranch<float>("jets_cen_eta", jet.eta());
            tx->pushbackToBranch<float>("jets_cen_phi", jet.phi());
            tx->pushbackToBranch<float>("jets_cen_mass", jet.mass());
        }


    } // End looping over jets

    double wgt_btagsf = 0;
    double wgt_btagsf_hf_up = 0;
    double wgt_btagsf_hf_dn = 0;
    double wgt_btagsf_lf_up = 0;
    double wgt_btagsf_lf_dn = 0;
    double wgt_btagsf_fs_up = 0;
    double wgt_btagsf_fs_dn = 0;
    int WP = 0; // Loose working point 1 or 2 for med or tight
    babymaker->coreBtagDeepCSVSF.getBTagWeight( WP, deepcsv_sf_jet_pt, deepcsv_sf_jet_eta, deepcsv_sf_jet_deepCSV, deepcsv_sf_jet_flavour, wgt_btagsf, wgt_btagsf_hf_up, wgt_btagsf_hf_dn, wgt_btagsf_lf_up, wgt_btagsf_lf_dn, wgt_btagsf_fs_up, wgt_btagsf_fs_dn );
    double wgt_med_btagsf = 0;
    double wgt_med_btagsf_hf_up = 0;
    double wgt_med_btagsf_hf_dn = 0;
    double wgt_med_btagsf_lf_up = 0;
    double wgt_med_btagsf_lf_dn = 0;
    double wgt_med_btagsf_fs_up = 0;
    double wgt_med_btagsf_fs_dn = 0;
    WP = 1; // Loose working point 1 or 2 for med or tight
    babymaker->coreBtagDeepCSVSF.getBTagWeight( WP, deepcsv_sf_jet_pt, deepcsv_sf_jet_eta, deepcsv_sf_jet_deepCSV, deepcsv_sf_jet_flavour, wgt_med_btagsf, wgt_med_btagsf_hf_up, wgt_med_btagsf_hf_dn, wgt_med_btagsf_lf_up, wgt_med_btagsf_lf_dn, wgt_med_btagsf_fs_up, wgt_med_btagsf_fs_dn );
    double wgt_tight_btagsf = 0;
    double wgt_tight_btagsf_hf_up = 0;
    double wgt_tight_btagsf_hf_dn = 0;
    double wgt_tight_btagsf_lf_up = 0;
    double wgt_tight_btagsf_lf_dn = 0;
    double wgt_tight_btagsf_fs_up = 0;
    double wgt_tight_btagsf_fs_dn = 0;
    WP = 2; // Loose working point 1 or 2 for med or tight
    babymaker->coreBtagDeepCSVSF.getBTagWeight( WP, deepcsv_sf_jet_pt, deepcsv_sf_jet_eta, deepcsv_sf_jet_deepCSV, deepcsv_sf_jet_flavour, wgt_tight_btagsf, wgt_tight_btagsf_hf_up, wgt_tight_btagsf_hf_dn, wgt_tight_btagsf_lf_up, wgt_tight_btagsf_lf_dn, wgt_tight_btagsf_fs_up, wgt_tight_btagsf_fs_dn );

    tx->setBranch<int>("nj", nj);
    tx->setBranch<int>("nj_up", nj_up);
    tx->setBranch<int>("nj_dn", nj_dn);
    tx->setBranch<int>("nb", nb);
    tx->setBranch<int>("nb_up", nb_up);
    tx->setBranch<int>("nb_dn", nb_dn);
    tx->setBranch<int>("nbmed", nbmed);
    tx->setBranch<int>("nbmed_up", nbmed_up);
    tx->setBranch<int>("nbmed_dn", nbmed_dn);
    tx->setBranch<int>("nbtight", nbmed);
    tx->setBranch<int>("nbtight_up", nbmed_up);
    tx->setBranch<int>("nbtight_dn", nbmed_dn);
    tx->setBranch<float>("ht", ht);
    tx->setBranch<int>("nj_cen", nj_cen);
    tx->setBranch<int>("nj_cen_up", nj_cen_up);
    tx->setBranch<int>("nj_cen_dn", nj_cen_dn);

    tx->setBranch<float>("weight_btagsf"         , wgt_btagsf);
    tx->setBranch<float>("weight_btagsf_heavy_DN", wgt_btagsf_hf_dn);
    tx->setBranch<float>("weight_btagsf_heavy_UP", wgt_btagsf_hf_up);
    tx->setBranch<float>("weight_btagsf_light_DN", wgt_btagsf_lf_dn);
    tx->setBranch<float>("weight_btagsf_light_UP", wgt_btagsf_lf_up);
    tx->setBranch<float>("weight_med_btagsf"         , wgt_med_btagsf);
    tx->setBranch<float>("weight_med_btagsf_heavy_DN", wgt_med_btagsf_hf_dn);
    tx->setBranch<float>("weight_med_btagsf_heavy_UP", wgt_med_btagsf_hf_up);
    tx->setBranch<float>("weight_med_btagsf_light_DN", wgt_med_btagsf_lf_dn);
    tx->setBranch<float>("weight_med_btagsf_light_UP", wgt_med_btagsf_lf_up);
    tx->setBranch<float>("weight_tight_btagsf"         , wgt_tight_btagsf);
    tx->setBranch<float>("weight_tight_btagsf_heavy_DN", wgt_tight_btagsf_hf_dn);
    tx->setBranch<float>("weight_tight_btagsf_heavy_UP", wgt_tight_btagsf_hf_up);
    tx->setBranch<float>("weight_tight_btagsf_light_DN", wgt_tight_btagsf_lf_dn);
    tx->setBranch<float>("weight_tight_btagsf_light_UP", wgt_tight_btagsf_lf_up);

    tx->sortVecBranchesByPt("jets_p4",
            {
            "jets_pt",
            "jets_eta",
            "jets_phi",
            "jets_mass",
            "jets_btag_score"
            },{},{});

    tx->sortVecBranchesByPt("jets_cen_p4",
            {
            "jets_cen_pt",
            "jets_cen_eta",
            "jets_cen_phi",
            "jets_cen_mass"
            },{},{});

    tx->sortVecBranchesByPt("rawjets_p4",
            {
            "rawjets_pt",
            "rawjets_eta",
            "rawjets_phi",
            "rawjets_mass",
            "rawjets_JEC"
            "rawjets_btag_score"
            },
            {
            "rawjets_passJetID",
            "rawjets_isORwLep"
            }
            ,{});

}
