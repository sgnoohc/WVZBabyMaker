#include "GenPartModule.h"

//==============================================================================================================
//
// GenPart Module
//
//==============================================================================================================

void wvzModule::GenPartModule::AddOutput()
{
    tx->createBranch<float>("gen_ht");
    tx->createBranch<vector<LV>>("gen_V_p4");
    tx->createBranch<vector<float>>("gen_V_pt");
    tx->createBranch<vector<float>>("gen_V_eta");
    tx->createBranch<vector<float>>("gen_V_phi");
    tx->createBranch<vector<float>>("gen_V_mass");
    tx->createBranch<vector<int>>("gen_V_id");
    tx->createBranch<vector<LV>>("gen_lep_p4");
    tx->createBranch<vector<float>>("gen_lep_pt");
    tx->createBranch<vector<float>>("gen_lep_eta");
    tx->createBranch<vector<float>>("gen_lep_phi");
    tx->createBranch<vector<float>>("gen_lep_mass");
    tx->createBranch<vector<int>>("gen_lep_id");
    tx->createBranch<vector<LV>>("gen_child_p4");
    tx->createBranch<vector<float>>("gen_child_pt");
    tx->createBranch<vector<float>>("gen_child_eta");
    tx->createBranch<vector<float>>("gen_child_phi");
    tx->createBranch<vector<float>>("gen_child_mass");
    tx->createBranch<vector<int>>("gen_child_id");
    tx->createBranch<vector<LV>>("gen_part_p4");
    tx->createBranch<vector<float>>("gen_part_pt");
    tx->createBranch<vector<float>>("gen_part_eta");
    tx->createBranch<vector<float>>("gen_part_phi");
    tx->createBranch<vector<float>>("gen_part_mass");
    tx->createBranch<vector<int>>("gen_part_id");
    tx->createBranch<vector<int>>("gen_part_mother_id");
    tx->createBranch<vector<int>>("gen_part_grandma_id");
    tx->createBranch<vector<int>>("gen_part_status");
    tx->createBranch<int>("VHchannel");
    tx->createBranch<int>("Higgschannel");
    tx->createBranch<int>("nGenTauClean");
    tx->createBranch<int>("nGenTau");
    tx->createBranch<int>("hasTau");
    tx->createBranch<int>("nLightLep");
}

void wvzModule::GenPartModule::FillOutput()
{
    tx->setBranch<float>("gen_ht", babymaker->coreGenPart.gen_ht);

    bool doProcess = false;
    if (babymaker->looper.getCurrentFileName().Contains("WWZ_")) doProcess = true;
    if (babymaker->looper.getCurrentFileName().Contains("ZZZ_")) doProcess = true;
    if (babymaker->looper.getCurrentFileName().Contains("WWZJets")) doProcess = true;
    if (babymaker->looper.getCurrentFileName().Contains("VHToNonbb")) doProcess = true;
    if (babymaker->looper.getCurrentFileName().Contains("HToWW")) doProcess = true;

    if (not doProcess)
        return;

    bool isHtoWW = false;
    bool isWnotFromH = false;
    bool isZthere = false;

    const vector<int>& genPart_idx = babymaker->coreGenPart.genPart_idx;
    const vector<int>& genPart_pdgId = babymaker->coreGenPart.genPart_pdgId;
    const vector<int>& genPart_status = babymaker->coreGenPart.genPart_status;
    const vector<int>& genPart_motherId = babymaker->coreGenPart.genPart_motherId;
    const vector<int>& genPart_grandmaId = babymaker->coreGenPart.genPart_grandmaId;
    const vector<LV>& genPart_p4 = babymaker->coreGenPart.genPart_p4;

    vector<LV> gen_V_p4;
    vector<int> gen_V_id;
    vector<LV> higgs_decay_p4;
    vector<int> higgs_decay_id;

    for (unsigned int i = 0; i < genPart_pdgId.size(); ++i)
    {
        if (
                (abs(genPart_pdgId[i]) == 23 && abs(genPart_motherId[i]) != 25 && genPart_status[i] == 22) or
                (abs(genPart_pdgId[i]) == 24 && abs(genPart_motherId[i]) != 25 && genPart_status[i] == 22)
                // (abs(genPart_motherId[i]) == 25 && (genPart_pdgId[i] == 23 or abs(genPart_pdgId[i]) == 24))
           )
        {
            gen_V_p4.push_back(genPart_p4[i]);
            gen_V_id.push_back(genPart_pdgId[i]);
        }
        if (
                (genPart_motherId[i] == 25 and genPart_pdgId[i] != 25)
           )
        {
            higgs_decay_p4.push_back(genPart_p4[i]);
            higgs_decay_id.push_back(genPart_pdgId[i]);
        }
    }

    if (babymaker->looper.getCurrentFileName().Contains("VHToNonbb"))
    {

        // H->XX
        if (higgs_decay_id.size() == 0)
        {
            std::cout << "Error: expected to find higgs decay but did not find any" << std::endl;
        }
        else if (higgs_decay_id.size() != 2)
        {
            std::cout << "Error: found !=2 higgs_decays" << std::endl;
        }
        else if (abs(higgs_decay_id[0]) == abs(higgs_decay_id[1]))
        {
            tx->setBranch<int>("Higgschannel", abs(higgs_decay_id[0]));
        }
        else if (abs(higgs_decay_id[0]) + abs(higgs_decay_id[1]) == 45) // H->Zgamma
        {
            tx->setBranch<int>("Higgschannel", 22);
        }
        else
        {
            std::cout << "Error: inconsistencies in decay id" << std::endl;
        }

        // Vector boson in VH
        if (gen_V_p4.size() != 1)
        {
            std::cout << "Error: found !=1 associated vector boson" << std::endl;
        }
        else
        {
            tx->setBranch<int>("VHchannel", abs(gen_V_id[0]));
        }
    }

    vector<int> tau_from_z_boson_idxs;
    vector<int> tau_from_w_boson_idxs;
    vector<int> any_tau_from_z;
    vector<int> any_tau_from_w;

    vector<int> genparticle;

    int hasTau = 0;

    for (unsigned int i = 0; i < genPart_pdgId.size(); ++i)
    {
        // Is Tau lepton
        if (abs(genPart_pdgId[i]) == 15)
        {
            if (abs(genPart_motherId[i]) == 23 and (abs(genPart_status[i]) == 2 or abs(genPart_status[i]) == 23))
            {
                tau_from_z_boson_idxs.push_back(i);
            }
            if (abs(genPart_motherId[i]) == 23)
            {
                any_tau_from_z.push_back(i);
            }
            if (abs(genPart_motherId[i]) == 24 and (abs(genPart_status[i]) == 2 or abs(genPart_status[i]) == 23))
            {
                tau_from_w_boson_idxs.push_back(i);
            }
            if (abs(genPart_motherId[i]) == 24)
            {
                any_tau_from_w.push_back(i);
            }

            hasTau = 1;
        }

        // Magic formula for the WWZ
        if (genPart_status[i]==22||genPart_status[i]==23||(((genPart_status[i]==1||genPart_status[i]==2)&&(abs(genPart_pdgId[i])==11||abs(genPart_pdgId[i])==13||abs(genPart_pdgId[i])==15||abs(genPart_pdgId[i])==12||abs(genPart_pdgId[i])==14||abs(genPart_pdgId[i])==16))&&(abs(genPart_motherId[i])==24||abs(genPart_motherId[i])==23||abs(genPart_grandmaId[i])==23)))
        {
            genparticle.push_back(i);
        }

    }

    tx->setBranch<int>("nGenTauClean", tau_from_w_boson_idxs.size() + tau_from_z_boson_idxs.size());
    tx->setBranch<int>("nGenTau", any_tau_from_w.size() + any_tau_from_z.size());
    tx->setBranch<int>("hasTau", hasTau);

    for (auto& i : genparticle)
    {
        tx->pushbackToBranch<LV>("gen_part_p4", genPart_p4[i]);
        tx->pushbackToBranch<float>("gen_part_pt", genPart_p4[i].pt());
        tx->pushbackToBranch<float>("gen_part_eta", genPart_p4[i].eta());
        tx->pushbackToBranch<float>("gen_part_phi", genPart_p4[i].phi());
        tx->pushbackToBranch<float>("gen_part_mass", genPart_p4[i].mass());
        tx->pushbackToBranch<int>("gen_part_id", genPart_pdgId[i]);
        tx->pushbackToBranch<int>("gen_part_mother_id", genPart_motherId[i]);
        tx->pushbackToBranch<int>("gen_part_grandma_id", genPart_grandmaId[i]);
        tx->pushbackToBranch<int>("gen_part_status", genPart_status[i]);
    }

    std::vector<LV> w_p4;
    std::vector<int> w_id;
    std::vector<LV> z_p4;
    std::vector<LV> wp_child_p4;
    std::vector<LV> wm_child_p4;
    std::vector<LV> z_child_p4;
    std::vector<int> wp_child_idx;
    std::vector<int> wm_child_idx;
    std::vector<int> z_child_idx;

    std::vector<LV> gen_part_p4;
    std::vector<int> gen_part_id;
    std::vector<int> gen_part_mother_id;
    std::vector<int> gen_part_grandma_id;
    std::vector<int> gen_part_status;

    for (auto& i : genparticle)
    {
        gen_part_p4.push_back(genPart_p4[i]);
        gen_part_id.push_back(genPart_pdgId[i]);
        gen_part_mother_id.push_back(genPart_motherId[i]);
        gen_part_grandma_id.push_back(genPart_grandmaId[i]);
        gen_part_status.push_back(genPart_status[i]);
    }

    w_p4.clear();
    w_id.clear();
    z_p4.clear();
    wp_child_p4.clear();
    wm_child_p4.clear();
    z_child_p4.clear();
    wp_child_idx.clear();
    wm_child_idx.clear();
    z_child_idx.clear();

    // Identifying light leptons to veto events properly
    // At the end of this it will compute the nLightLep
    // For inclusive if nLightLep == 4 it needs to be rejected
    if (babymaker->looper.getCurrentFileName().Contains("WWZ_"))
    {

        // For inclusinve WWZ sample the first three are always the W, W, Z
        w_p4.push_back(gen_part_p4[0].pt() > gen_part_p4[1].pt() ? gen_part_p4[0] : gen_part_p4[1]);
        w_p4.push_back(gen_part_p4[0].pt() > gen_part_p4[1].pt() ? gen_part_p4[1] : gen_part_p4[0]);
        w_id.push_back(gen_part_p4[0].pt() > gen_part_p4[1].pt() ? gen_part_id[0] : gen_part_id[1]);
        w_id.push_back(gen_part_p4[0].pt() > gen_part_p4[1].pt() ? gen_part_id[1] : gen_part_id[0]);
        z_p4.push_back(gen_part_p4[2]);

        // Then loop over to obtain the children information
        for (unsigned int ip = 0; ip < gen_part_p4.size(); ++ip)
        {
            if (gen_part_mother_id[ip] == 23)
            {
                z_child_p4.push_back(gen_part_p4[ip]);
                z_child_idx.push_back(ip);
            }
            if (gen_part_mother_id[ip] == 24)
            {
                if (gen_part_status[ip] == 23)
                {
                    wp_child_p4.push_back(gen_part_p4[ip]);
                    wp_child_idx.push_back(ip);
                }
            }
            if (gen_part_mother_id[ip] == -24)
            {
                if (gen_part_status[ip] == 23)
                {
                    wm_child_p4.push_back(gen_part_p4[ip]);
                    wm_child_idx.push_back(ip);
                }
            }
        }

        // In some cases the children have status == 1 so if didn't find two each then retrieve these
        if (wp_child_p4.size() != 2)
        {
            for (unsigned int ip = 0; ip < gen_part_p4.size(); ++ip)
            {
                if (gen_part_mother_id[ip] == 24)
                {
                    if (gen_part_status[ip] == 1 or (gen_part_status[ip] == 2 and abs(gen_part_id[ip]) == 15))
                    {
                        wp_child_p4.push_back(gen_part_p4[ip]);
                        wp_child_idx.push_back(ip);
                    }
                }
            }
        }

        // In some cases the children have status == 1 so if didn't find two each then retrieve these
        if (wm_child_p4.size() != 2)
        {
            for (unsigned int ip = 0; ip < gen_part_p4.size(); ++ip)
            {
                if (gen_part_mother_id[ip] == -24)
                {
                    if (gen_part_status[ip] == 1 or (gen_part_status[ip] == 2 and abs(gen_part_id[ip]) == 15))
                    {
                        wm_child_p4.push_back(gen_part_p4[ip]);
                        wm_child_idx.push_back(ip);
                    }
                }
            }
        }

        // In cases when it double counted and has more than 2 children do a clean up
        if (wm_child_p4.size() > 2) pruneChildren(wm_child_p4, wm_child_idx, w_p4[0].mass(), w_p4[1].mass());
        if (wp_child_p4.size() > 2) pruneChildren(wp_child_p4, wp_child_idx, w_p4[0].mass(), w_p4[1].mass());

        if (wp_child_p4.size() != 2)
        {
            std::cout <<  " cms3.evt_event(): " << cms3.evt_event() <<  std::endl;
            std::cout <<  " wp_child_p4.size(): " << wp_child_p4.size() <<  std::endl;
            RooUtil::warning("different than I expected");
        }
        if (wm_child_p4.size() != 2)
        {
            std::cout <<  " cms3.evt_event(): " << cms3.evt_event() <<  std::endl;
            std::cout <<  " wm_child_p4.size(): " << wm_child_p4.size() <<  std::endl;
            RooUtil::warning("different than I expected");
        }
        if (z_child_p4.size() != 2)
        {
            std::cout <<  " cms3.evt_event(): " << cms3.evt_event() <<  std::endl;
            std::cout <<  " z_child_p4.size(): " << z_child_p4.size() <<  std::endl;
            RooUtil::warning("different than I expected");
        }

    }
    if (babymaker->looper.getCurrentFileName().Contains("WWZJets"))
    {
        int evt = cms3.evt_event();
        if (abs(gen_part_id[0]) == 23)
        {
            if (abs(gen_part_id[1]) != 24)
            {
                std::cout <<  " abs(gen_part_id[1]): " << abs(gen_part_id[1]) <<  std::endl;
                RooUtil::error("different than I expected");
            }
            if (abs(gen_part_id[2]) != 24)
            {
                std::cout <<  " abs(gen_part_id[2]): " << abs(gen_part_id[2]) <<  std::endl;
                RooUtil::error("different than I expected");
            }
            w_p4.push_back(gen_part_p4[1].pt() > gen_part_p4[2].pt() ? gen_part_p4[1] : gen_part_p4[2]);
            w_p4.push_back(gen_part_p4[1].pt() > gen_part_p4[2].pt() ? gen_part_p4[2] : gen_part_p4[1]);
            w_id.push_back(gen_part_p4[1].pt() > gen_part_p4[2].pt() ? gen_part_id[1] : gen_part_id[2]);
            w_id.push_back(gen_part_p4[1].pt() > gen_part_p4[2].pt() ? gen_part_id[2] : gen_part_id[1]);
            z_p4.push_back(gen_part_p4[0]);
            for (unsigned int ip = 0; ip < gen_part_p4.size(); ++ip)
            {
                if (gen_part_mother_id[ip] == 23)
                {
                    z_child_p4.push_back(gen_part_p4[ip]);
                    z_child_idx.push_back(ip);
                }
                if (gen_part_mother_id[ip] == 24)
                {
                    if (gen_part_status[ip] == 23)
                    {
                        wp_child_p4.push_back(gen_part_p4[ip]);
                        wp_child_idx.push_back(ip);
                    }
                }
                if (gen_part_mother_id[ip] == -24)
                {
                    if (gen_part_status[ip] == 23)
                    {
                        wm_child_p4.push_back(gen_part_p4[ip]);
                        wm_child_idx.push_back(ip);
                    }
                }
            }
            if (wp_child_p4.size() != 2)
            {
                for (unsigned int ip = 0; ip < gen_part_p4.size(); ++ip)
                {
                    if (gen_part_mother_id[ip] == 24)
                    {
                        if (gen_part_status[ip] == 1)
                        {
                            wp_child_p4.push_back(gen_part_p4[ip]);
                            wp_child_idx.push_back(ip);
                        }
                    }
                }
            }
            if (wm_child_p4.size() != 2)
            {
                for (unsigned int ip = 0; ip < gen_part_p4.size(); ++ip)
                {
                    if (gen_part_mother_id[ip] == -24)
                    {
                        if (gen_part_status[ip] == 1)
                        {
                            wm_child_p4.push_back(gen_part_p4[ip]);
                            wm_child_idx.push_back(ip);
                        }
                    }
                }
            }

            // In cases when it double counted and has more than 2 children do a clean up
            if (wm_child_p4.size() > 2) pruneChildren(wm_child_p4, wm_child_idx, w_p4[0].mass(), w_p4[1].mass());
            if (wp_child_p4.size() > 2) pruneChildren(wp_child_p4, wp_child_idx, w_p4[0].mass(), w_p4[1].mass());

            if (wp_child_p4.size() != 2)
            {
                std::cout <<  " evt: " << evt <<  std::endl;
                std::cout <<  " wp_child_p4.size(): " << wp_child_p4.size() <<  std::endl;
                RooUtil::error("different than I expected");
            }
            if (wm_child_p4.size() != 2)
            {
                std::cout <<  " evt: " << evt <<  std::endl;
                std::cout <<  " wm_child_p4.size(): " << wm_child_p4.size() <<  std::endl;
                RooUtil::error("different than I expected");
            }
            if (z_child_p4.size() != 2)
            {
                std::cout <<  " evt: " << evt <<  std::endl;
                std::cout <<  " z_child_p4.size(): " << z_child_p4.size() <<  std::endl;
                RooUtil::error("different than I expected");
            }
        }
        else if(abs(gen_part_id[0]) == 24)
        {
            if (abs(gen_part_id[1]) != 24)
            {
                std::cout <<  " abs(gen_part_id[1]): " << abs(gen_part_id[1]) <<  std::endl;
                RooUtil::error("different than I expected");
            }
            if (gen_part_id[2] != -gen_part_id[3])
            {
                std::cout <<  " gen_part_id[2]: " << gen_part_id[2] <<  std::endl;
                std::cout <<  " gen_part_id[3]: " << gen_part_id[3] <<  std::endl;
                RooUtil::error("different that I expected");
            }
            w_p4.push_back(gen_part_p4[0].pt() > gen_part_p4[1].pt() ? gen_part_p4[0] : gen_part_p4[1]);
            w_p4.push_back(gen_part_p4[0].pt() > gen_part_p4[1].pt() ? gen_part_p4[1] : gen_part_p4[0]);
            w_id.push_back(gen_part_p4[0].pt() > gen_part_p4[1].pt() ? gen_part_id[0] : gen_part_id[1]);
            w_id.push_back(gen_part_p4[0].pt() > gen_part_p4[1].pt() ? gen_part_id[1] : gen_part_id[0]);
            z_p4.push_back((gen_part_p4[2] + gen_part_p4[3]));
            z_child_p4.push_back(gen_part_p4[2]);
            z_child_p4.push_back(gen_part_p4[3]);
            z_child_idx.push_back(2);
            z_child_idx.push_back(3);
            for (unsigned int ip = 0; ip < gen_part_p4.size(); ++ip)
            {
                if (gen_part_mother_id[ip] == 24)
                {
                    if (gen_part_status[ip] == 23)
                    {
                        wp_child_p4.push_back(gen_part_p4[ip]);
                        wp_child_idx.push_back(ip);
                    }
                }
                if (gen_part_mother_id[ip] == -24)
                {
                    if (gen_part_status[ip] == 23)
                    {
                        wm_child_p4.push_back(gen_part_p4[ip]);
                        wm_child_idx.push_back(ip);
                    }
                }
            }
            if (wp_child_p4.size() != 2)
            {
                for (unsigned int ip = 0; ip < gen_part_p4.size(); ++ip)
                {
                    if (gen_part_mother_id[ip] == 24)
                    {
                        if (gen_part_status[ip] == 1)
                        {
                            wp_child_p4.push_back(gen_part_p4[ip]);
                            wp_child_idx.push_back(ip);
                        }
                    }
                }
            }
            if (wm_child_p4.size() != 2)
            {
                for (unsigned int ip = 0; ip < gen_part_p4.size(); ++ip)
                {
                    if (gen_part_mother_id[ip] == -24)
                    {
                        if (gen_part_status[ip] == 1)
                        {
                            wm_child_p4.push_back(gen_part_p4[ip]);
                            wm_child_idx.push_back(ip);
                        }
                    }
                }
            }

            // In cases when it double counted and has more than 2 children do a clean up
            if (wm_child_p4.size() > 2) pruneChildren(wm_child_p4, wm_child_idx, w_p4[0].mass(), w_p4[1].mass());
            if (wp_child_p4.size() > 2) pruneChildren(wp_child_p4, wp_child_idx, w_p4[0].mass(), w_p4[1].mass());

            if (wp_child_p4.size() != 2)
            {
                std::cout <<  " evt: " << evt <<  std::endl;
                std::cout <<  " wp_child_p4.size(): " << wp_child_p4.size() <<  std::endl;
                RooUtil::error("different than I expected");
            }
            if (wm_child_p4.size() != 2)
            {
                std::cout <<  " evt: " << evt <<  std::endl;
                std::cout <<  " wm_child_p4.size(): " << wm_child_p4.size() <<  std::endl;
                RooUtil::error("different than I expected");
            }
        }
        else
        {
            std::cout <<  " abs(gen_part_id[0]): " << abs(gen_part_id[0]) <<  std::endl;
            RooUtil::error("different that I expected");
        }

    }

    int nlightlep = 0;
    for (auto& idx : wp_child_idx)
    {
        if (abs(gen_part_id[idx]) == 11 or abs(gen_part_id[idx]) == 13)
            nlightlep++;
    }
    for (auto& idx : wm_child_idx)
    {
        if (abs(gen_part_id[idx]) == 11 or abs(gen_part_id[idx]) == 13)
            nlightlep++;
    }
    for (auto& idx : z_child_idx)
    {
        if (abs(gen_part_id[idx]) == 11 or abs(gen_part_id[idx]) == 13)
            nlightlep++;
    }
    tx->setBranch<int>("nLightLep", nlightlep);

    if (w_p4.size() > 0)
    {
        tx->pushbackToBranch<LV>("gen_V_p4"      , w_p4[0]);
        tx->pushbackToBranch<float>("gen_V_pt"   , w_p4[0].pt());
        tx->pushbackToBranch<float>("gen_V_eta"  , w_p4[0].eta());
        tx->pushbackToBranch<float>("gen_V_phi"  , w_p4[0].phi());
        tx->pushbackToBranch<float>("gen_V_mass" , w_p4[0].mass());
        tx->pushbackToBranch<int>("gen_V_id"     , w_id[0]);
    }

    if (w_p4.size() > 1)
    {
        tx->pushbackToBranch<LV>("gen_V_p4", w_p4[1]);
        tx->pushbackToBranch<float>("gen_V_pt", w_p4[1].pt());
        tx->pushbackToBranch<float>("gen_V_eta", w_p4[1].eta());
        tx->pushbackToBranch<float>("gen_V_phi", w_p4[1].phi());
        tx->pushbackToBranch<float>("gen_V_mass", w_p4[1].mass());
        tx->pushbackToBranch<int>("gen_V_id", w_id[1]);
    }

    if (z_p4.size() > 0)
    {
        tx->pushbackToBranch<LV>("gen_V_p4", z_p4[0]);
        tx->pushbackToBranch<float>("gen_V_pt", z_p4[0].pt());
        tx->pushbackToBranch<float>("gen_V_eta", z_p4[0].eta());
        tx->pushbackToBranch<float>("gen_V_phi", z_p4[0].phi());
        tx->pushbackToBranch<float>("gen_V_mass", z_p4[0].mass());
        tx->pushbackToBranch<int>("gen_V_id", 23);
    }

    for (auto& idx : wp_child_idx)
    {
        tx->pushbackToBranch<LV>("gen_child_p4", gen_part_p4[idx]);
        tx->pushbackToBranch<float>("gen_child_pt", gen_part_p4[idx].pt());
        tx->pushbackToBranch<float>("gen_child_eta", gen_part_p4[idx].eta());
        tx->pushbackToBranch<float>("gen_child_phi", gen_part_p4[idx].phi());
        tx->pushbackToBranch<float>("gen_child_mass", gen_part_p4[idx].mass());
        tx->pushbackToBranch<int>("gen_child_id", gen_part_id[idx]);
        if (abs(gen_part_id[idx]) == 11 or abs(gen_part_id[idx]) == 13 or abs(gen_part_id[idx]) == 15)
        {
            tx->pushbackToBranch<LV>("gen_lep_p4", gen_part_p4[idx]);
            tx->pushbackToBranch<float>("gen_lep_pt", gen_part_p4[idx].pt());
            tx->pushbackToBranch<float>("gen_lep_eta", gen_part_p4[idx].eta());
            tx->pushbackToBranch<float>("gen_lep_phi", gen_part_p4[idx].phi());
            tx->pushbackToBranch<float>("gen_lep_mass", gen_part_p4[idx].mass());
            tx->pushbackToBranch<int>("gen_lep_id", gen_part_id[idx]);
        }
    }
    for (auto& idx : wm_child_idx)
    {
        tx->pushbackToBranch<LV>("gen_child_p4", gen_part_p4[idx]);
        tx->pushbackToBranch<float>("gen_child_pt", gen_part_p4[idx].pt());
        tx->pushbackToBranch<float>("gen_child_eta", gen_part_p4[idx].eta());
        tx->pushbackToBranch<float>("gen_child_phi", gen_part_p4[idx].phi());
        tx->pushbackToBranch<float>("gen_child_mass", gen_part_p4[idx].mass());
        tx->pushbackToBranch<int>("gen_child_id", gen_part_id[idx]);
        if (abs(gen_part_id[idx]) == 11 or abs(gen_part_id[idx]) == 13 or abs(gen_part_id[idx]) == 15)
        {
            tx->pushbackToBranch<LV>("gen_lep_p4", gen_part_p4[idx]);
            tx->pushbackToBranch<float>("gen_lep_pt", gen_part_p4[idx].pt());
            tx->pushbackToBranch<float>("gen_lep_eta", gen_part_p4[idx].eta());
            tx->pushbackToBranch<float>("gen_lep_phi", gen_part_p4[idx].phi());
            tx->pushbackToBranch<float>("gen_lep_mass", gen_part_p4[idx].mass());
            tx->pushbackToBranch<int>("gen_lep_id", gen_part_id[idx]);
        }
    }
    for (auto& idx : z_child_idx)
    {
        tx->pushbackToBranch<LV>("gen_child_p4", gen_part_p4[idx]);
        tx->pushbackToBranch<float>("gen_child_pt", gen_part_p4[idx].pt());
        tx->pushbackToBranch<float>("gen_child_eta", gen_part_p4[idx].eta());
        tx->pushbackToBranch<float>("gen_child_phi", gen_part_p4[idx].phi());
        tx->pushbackToBranch<float>("gen_child_mass", gen_part_p4[idx].mass());
        tx->pushbackToBranch<int>("gen_child_id", gen_part_id[idx]);
        if (abs(gen_part_id[idx]) == 11 or abs(gen_part_id[idx]) == 13 or abs(gen_part_id[idx]) == 15)
        {
            tx->pushbackToBranch<LV>("gen_lep_p4", gen_part_p4[idx]);
            tx->pushbackToBranch<float>("gen_lep_pt", gen_part_p4[idx].pt());
            tx->pushbackToBranch<float>("gen_lep_eta", gen_part_p4[idx].eta());
            tx->pushbackToBranch<float>("gen_lep_phi", gen_part_p4[idx].phi());
            tx->pushbackToBranch<float>("gen_lep_mass", gen_part_p4[idx].mass());
            tx->pushbackToBranch<int>("gen_lep_id", gen_part_id[idx]);
        }
    }

}

void wvzModule::GenPartModule::pruneChildren(std::vector<LV>& child_p4, std::vector<int>& child_idx, float wm0, float wm1)
{

    // Pruning gamma originated leptons
    std::vector<LV> tmp;
    std::vector<int> tmpidx;
    int ii = 0;
    int jj = 0;

    bool found = false;
    for (unsigned int ii = 0; ii < child_p4.size(); ++ii)
    {
        for (unsigned int jj = 1; jj < child_p4.size() - 1; ++jj)
        {
            if (ii == jj)
                continue;

            LV ip4 = child_p4.at(ii);
            LV jp4 = child_p4.at(jj);

            if (fabs((ip4 + jp4).mass() - wm0) < 0.5)
            {
                tmp.push_back(ip4);
                tmp.push_back(jp4);
                tmpidx.push_back(child_idx[ii]);
                tmpidx.push_back(child_idx[jj]);
                found = true;
                break;
            }
            else if (fabs((ip4 + jp4).mass() - wm1) < 0.5)
            {
                tmp.push_back(ip4);
                tmp.push_back(jp4);
                tmpidx.push_back(child_idx[ii]);
                tmpidx.push_back(child_idx[jj]);
                found = true;
                break;
            }
        }
        if (found)
            break;
    }

    if (found)
    {
        child_p4 = tmp;
        child_idx = tmpidx;
    }
    else
    {
        std::cout <<  " child_p4.size(): " << child_p4.size() <<  std::endl;
        std::cout <<  " child_idx.size(): " << child_idx.size() <<  std::endl;
        RooUtil::error("different than I expected");
    }
}
