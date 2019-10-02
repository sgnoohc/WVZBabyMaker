##!/bin/env python

from condor_submit_util import submit
from time import sleep

#______________________________________________________________________________________
def get_master_list():

    dinfos = {}

    #######################################
    ######## WWW BABY MC 2016 94x #########
    #######################################

    sample_info = {

        #"/DoubleEG/Run2016B-17Jul2018_ver1-v1/MINIAOD"   : "data_Run2016Bv1_ee" , 
        "/DoubleEG/Run2016B-17Jul2018_ver2-v1/MINIAOD"   : "data_Run2016Bv2_ee" , 
        "/DoubleEG/Run2016C-17Jul2018-v1/MINIAOD"        : "data_Run2016C_ee"   , 
        "/DoubleEG/Run2016D-17Jul2018-v1/MINIAOD"        : "data_Run2016D_ee"   , 
        "/DoubleEG/Run2016E-17Jul2018-v1/MINIAOD"        : "data_Run2016E_ee"   , 
        "/DoubleEG/Run2016F-17Jul2018-v1/MINIAOD"        : "data_Run2016F_ee"   , 
        "/DoubleEG/Run2016G-17Jul2018-v1/MINIAOD"        : "data_Run2016G_ee"   , 
        "/DoubleEG/Run2016H-17Jul2018-v1/MINIAOD"        : "data_Run2016H_ee"   , 
        #"/DoubleMuon/Run2016B-17Jul2018_ver1-v1/MINIAOD" : "data_Run2016Bv1_mm" , 
        "/DoubleMuon/Run2016B-17Jul2018_ver2-v1/MINIAOD" : "data_Run2016Bv2_mm" , 
        "/DoubleMuon/Run2016C-17Jul2018-v1/MINIAOD"      : "data_Run2016C_mm"   , 
        "/DoubleMuon/Run2016D-17Jul2018-v1/MINIAOD"      : "data_Run2016D_mm"   , 
        "/DoubleMuon/Run2016E-17Jul2018-v1/MINIAOD"      : "data_Run2016E_mm"   , 
        "/DoubleMuon/Run2016F-17Jul2018-v1/MINIAOD"      : "data_Run2016F_mm"   , 
        "/DoubleMuon/Run2016G-17Jul2018-v1/MINIAOD"      : "data_Run2016G_mm"   , 
        "/DoubleMuon/Run2016H-17Jul2018-v1/MINIAOD"      : "data_Run2016H_mm"   , 
        #"/MuonEG/Run2016B-17Jul2018_ver1-v1/MINIAOD"     : "data_Run2016Bv1_em" , 
        "/MuonEG/Run2016B-17Jul2018_ver2-v1/MINIAOD"     : "data_Run2016Bv2_em" , 
        "/MuonEG/Run2016C-17Jul2018-v1/MINIAOD"          : "data_Run2016C_em"   , 
        "/MuonEG/Run2016D-17Jul2018-v1/MINIAOD"          : "data_Run2016D_em"   , 
        "/MuonEG/Run2016E-17Jul2018-v2/MINIAOD"          : "data_Run2016E_em"   , # note , v2 due to email titled [ops] Reprocessing 2017 94X with MET Recipe"
        "/MuonEG/Run2016F-17Jul2018-v1/MINIAOD"          : "data_Run2016F_em"   , 
        "/MuonEG/Run2016G-17Jul2018-v1/MINIAOD"          : "data_Run2016G_em"   , 
        "/MuonEG/Run2016H-17Jul2018-v1/MINIAOD"          : "data_Run2016H_em"   , 

        # Tribooson
        "/WWW_4F_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM"                            : "www_amcatnlo"               , 
        "/WWZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM"                               : "wwz_amcatnlo"               , 
        "/WZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM"                               : "wzz_amcatnlo"               , 
        "/ZZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM"                               : "zzz_amcatnlo"               , 
        "/VHToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM"                     : "vh_nonbb_amcatnlo"          , 

        # DY+Jets
        "/DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM"      : "dy_m1050_madgraph"          , 
        "/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext2-v1/MINIAODSIM"          : "dy_m50_madgraph"            , 
        "/ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v1/MINIAODSIM"                  : "zg_llg_amcatnlo"            , 

        # W Jets
        "/WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext2-v2/MINIAODSIM"                : "wjets_incl_madgraph"        , 
        "/WGToLNuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v1/MINIAODSIM"                 : "wg_incl_amcnlo"             ,

        # Top ttbar
        "/ST_t-channel_antitop_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM" : "stt_antitop_incdec_powheg",
        "/ST_t-channel_top_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM"     : "stt_top_incdec_powheg",
        "/ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM"      : "sts_4f_leptonic_madgraph"   , 
        "/ST_tW_antitop_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM" : "tw_antitopnofullhad_powheg" , 
        "/ST_tW_top_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM"     : "tw_topnofullhad_powheg"     , 
        "/ST_tWll_5f_LO_13TeV-MadGraph-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM"                                  : "twz_ll_madgraph"            , 
        "/TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM"                  : "ttbar_dilep_madgraph"       , 
        "/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext2-v1/MINIAODSIM"     : "ttw_lnu_amcatnlo"           , 
        "/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext2-v1/MINIAODSIM"             : "ttz_llvv_mll10_amcatnlo"    , 
        "/TTZToLL_M-1to10_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-94X_mcRun2_asymptotic_v3-v1/MINIAODSIM"                            : "ttz_ll_mll1_amcatnlo"       , 
        "/tZq_ll_4f_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v1/MINIAODSIM"                                 : "tZq_ll_madgraph"            ,
        "/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM"         : "tth_nonbb_powheg"           , 

        # ZZ
        "/GluGluHToZZTo4L_M125_13TeV_powheg2_JHUgenV6_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM"                   : "ggh_hzz4l_powheg"           , 
        "/ZZTo4L_13TeV_powheg_pythia8_ext1/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM"                                      : "zz_4l_powheg"               ,
        "/ZZTo2L2Nu_13TeV_powheg_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM"                                        : "zz_2l2v_powheg"             , 
        "/ZZTo2L2Q_13TeV_powheg_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM"                                         : "zz_2l2q_powheg"             , 
        "/GluGluToContinToZZTo2e2mu_13TeV_MCFM701_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM"                       : "ggzz_2e2m_mcfm"             ,
        "/GluGluToContinToZZTo2e2tau_13TeV_MCFM701_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM"                      : "ggzz_2e2t_mcfm"             ,
        "/GluGluToContinToZZTo2mu2tau_13TeV_MCFM701_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM"                     : "ggzz_2m2t_mcfm"             ,
        "/GluGluToContinToZZTo4e_13TeV_MCFM701_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM"                          : "ggzz_4e_mcfm"               ,
        "/GluGluToContinToZZTo4mu_13TeV_MCFM701_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM"                         : "ggzz_4m_mcfm"               ,
        "/GluGluToContinToZZTo4tau_13TeV_MCFM701_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM"                        : "ggzz_4t_mcfm"               ,

        # WW
        "/WWTo2L2Nu_13TeV-powheg/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM"                                                : "ww_2l_powheg"               , 

        # WZ
        "/WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM"                           : "wz_2l2q_amcatnlo"           ,
        "/WZTo3LNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM"                      : "wz_3lv_amcatnlo"            , 

        # VVG
        "/WZG_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM"                               : "wzg_amcatnlo"               , 
        "/WWG_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v1/MINIAODSIM"                          : "wwg_amcatnlo"               , 

        # Powheg ZH samples
        "/HZJ_HToWWTo2L2Nu_ZTo2L_M125_13TeV_powheg_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM" : "zh_ww_4l_powheg" ,
        "/GluGluZH_HToWWTo2L2Nu_ZTo2L_M125_13TeV_powheg_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM" : "ggzh_ww_4l_powheg",

        }

    dinfos["wvz_2016_94x"] = {
            "samples" : sample_info,
            "year" : 2016,
            "baby_type" : "WVZ"
            }

    dinfos["trilep_2016_94x"] = {
            "samples" : sample_info,
            "year" : 2016,
            "baby_type" : "Trilep"
            }

    dinfos["dilep_2016_94x"] = {
            "samples" : sample_info,
            "year" : 2016,
            "baby_type" : "Dilep"
            }

    dinfos["wvzmva_2016_94x"] = {
            "samples" : sample_info,
            "year" : 2016,
            "baby_type" : "WVZMVA"
            }

    # sample_info = {
    #     "/WWZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM"                               : "wwz_amcatnlo"               , 
    #     "/WZTo3LNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM"                      : "wz_3lv_amcatnlo"            , 
    #     }

    # dinfos["wvz_2016_94x"] = {
    #         "samples" : sample_info,
    #         "year" : 2016,
    #         "baby_type" : "WVZ"
    #         }

    #######################################
    ######## WWW BABY MC 2017 94x #########
    #######################################

    sample_info = {

        "/DoubleEG/Run2017B-31Mar2018-v1/MINIAOD"   : "data_Run2017B_ee",
        "/DoubleEG/Run2017C-31Mar2018-v1/MINIAOD"   : "data_Run2017C_ee",
        "/DoubleEG/Run2017D-31Mar2018-v1/MINIAOD"   : "data_Run2017D_ee",
        "/DoubleEG/Run2017E-31Mar2018-v1/MINIAOD"   : "data_Run2017E_ee",
        "/DoubleEG/Run2017F-31Mar2018-v1/MINIAOD"   : "data_Run2017F_ee",
        "/DoubleMuon/Run2017B-31Mar2018-v1/MINIAOD" : "data_Run2017B_mm",
        "/DoubleMuon/Run2017C-31Mar2018-v1/MINIAOD" : "data_Run2017C_mm",
        "/DoubleMuon/Run2017D-31Mar2018-v1/MINIAOD" : "data_Run2017D_mm",
        "/DoubleMuon/Run2017E-31Mar2018-v1/MINIAOD" : "data_Run2017E_mm",
        "/DoubleMuon/Run2017F-31Mar2018-v1/MINIAOD" : "data_Run2017F_mm",
        "/MuonEG/Run2017B-31Mar2018-v1/MINIAOD"     : "data_Run2017B_em",
        "/MuonEG/Run2017C-31Mar2018-v1/MINIAOD"     : "data_Run2017C_em",
        "/MuonEG/Run2017D-31Mar2018-v1/MINIAOD"     : "data_Run2017D_em",
        "/MuonEG/Run2017E-31Mar2018-v1/MINIAOD"     : "data_Run2017E_em",
        "/MuonEG/Run2017F-31Mar2018-v1/MINIAOD"     : "data_Run2017F_em",

        # Tribooson
        "/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM"                                         : "www_amcatnlo"               ,
        "/WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM"                                         : "wwz_amcatnlo"               ,
        "/WZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM"                                            : "wzz_amcatnlo"               ,
        "/ZZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM"                                            : "zzz_amcatnlo"               ,
        "/VHToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM"                             : "vh_nonbb_amcatnlo"          ,
        "/WWZJetsTo4L2Nu_4f_TuneCP5_13TeV_amcatnloFXFX_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM"                          : "wwz_4l2v_amcatnlo"          ,

        # DY+Jets
        "/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM"                         : "dy_m1050_madgraph"          ,
        "/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM"                       : "dy_m50_madgraph"            ,
        "/ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v3/MINIAODSIM"                             : "zg_llg_amcatnlo"            ,

        # W Jets
        "/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v2/MINIAODSIM"                             : "wjets_incl_madgraph"        ,
        "/WGToLNuG_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM"                                    : "wg_lvg_madgraph"            ,

        # Top ttbar
        "/ST_t-channel_antitop_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM"     : "stt_antitop_incdec_powheg",
        "/ST_t-channel_top_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM" : "stt_top_incdec_powheg",
        "/ST_s-channel_4f_leptonDecays_TuneCP5_PSweights_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM" : "sts_4f_leptonic_madgraph"   ,
        "/ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM"   : "tw_antitopnofullhad_powheg" ,
        "/ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM"       : "tw_topnofullhad_powheg"     ,
        "/ST_tWll_5f_LO_TuneCP5_PSweights_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM"                   : "twz_ll_madgraph"            ,
        "/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM"                               : "ttbar_dilep_madgraph"       ,
        "/TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM"                       : "ttw_lnu_amcatnlo"           ,
        "/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM"                               : "ttz_llvv_mll10_amcatnlo"    ,
        "/TTZToLL_M-1to10_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM"                                : "ttz_ll_mll1_amcatnlo"       ,
        "/tZq_ll_4f_ckm_NLO_TuneCP5_PSweights_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM"                    : "tZq_ll_madgraph"            ,
        "/ttHToNonbb_M125_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM"                          : "tth_nonbb_powheg"           ,

        # ZZ
        "/GluGluHToZZTo4L_M125_13TeV_powheg2_JHUGenV7011_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM"                   : "ggh_hzz4l_powheg"           ,
        "/ZZTo4L_13TeV_powheg_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM"                                              : "zz_4l_powheg"               ,
        "/ZZTo2L2Nu_13TeV_powheg_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM"                                                : "zz_2l2v_powheg"             ,
        "/ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM"                                   : "zz_2l2q_powheg"             ,
        "/GluGluToContinToZZTo2e2mu_13TeV_MCFM701_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM"                               : "ggzz_2e2m_mcfm"             ,
        "/GluGluToContinToZZTo2e2tau_13TeV_MCFM701_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM"                              : "ggzz_2e2t_mcfm"             ,
        "/GluGluToContinToZZTo2mu2tau_13TeV_MCFM701_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM"                             : "ggzz_2m2t_mcfm"             ,
        "/GluGluToContinToZZTo4e_13TeV_MCFM701_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM"                                  : "ggzz_4e_mcfm"               ,
        "/GluGluToContinToZZTo4mu_13TeV_MCFM701_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM"                                 : "ggzz_4m_mcfm"               ,
        "/GluGluToContinToZZTo4tau_13TeV_MCFM701_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v2/MINIAODSIM"                           : "ggzz_4t_mcfm"               ,

        # WW
        "/WWTo2L2Nu_NNPDF31_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM"                                : "ww_2l_powheg"               ,

        # WZ
        "/WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM"                                   : "wz_2l2q_amcatnlo"           ,
        "/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM"                           : "wz_3lv_amcatnlo"            ,

        # VVG
        "/WZG_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM"                                          : "wzg_amcatnlo"               ,
        "/WWG_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v1/MINIAODSIM"                                     : "wwg_amcatnlo"               ,

        # ZH Powheg
        "/HZJ_HToWWTo2L2Nu_ZTo2L_M125_13TeV_powheg_jhugen714_pythia8_TuneCP5/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM" : "zh_ww_4l_powheg",
        "/GluGluZH_HToWWTo2L2Nu_ZTo2L_M125_13TeV_powheg_pythia8_TuneCP5/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM" : "ggzh_ww_4l_powheg",
        }

    dinfos["wvz_2017_94x"] = {
            "samples" : sample_info,
            "year" : 2017,
            "baby_type" : "WVZ"
            }

    dinfos["trilep_2017_94x"] = {
            "samples" : sample_info,
            "year" : 2017,
            "baby_type" : "Trilep"
            }

    dinfos["dilep_2017_94x"] = {
            "samples" : sample_info,
            "year" : 2017,
            "baby_type" : "Dilep"
            }

    dinfos["wvzmva_2017_94x"] = {
            "samples" : sample_info,
            "year" : 2017,
            "baby_type" : "WVZMVA"
            }

    sample_info = {
        "/WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM"                                         : "wwz_amcatnlo"               ,
        "/WWZJetsTo4L2Nu_4f_TuneCP5_13TeV_amcatnloFXFX_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM"                          : "wwz_4l2v_amcatnlo"          ,
        }

    dinfos["truth_2017_94x"] = {
            "samples" : sample_info,
            "year" : 2017,
            "baby_type" : "Truth"
            }

    dinfos["wvzsig_2017_94x"] = {
            "samples" : sample_info,
            "year" : 2017,
            "baby_type" : "WVZAll"
            }

    # sample_info = {
    #     "/WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM"                                         : "wwz_amcatnlo"               ,
    #     "/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM"                           : "wz_3lv_amcatnlo"            ,
    #     }

    # dinfos["wvz_2017_94x"] = {
    #         "samples" : sample_info,
    #         "year" : 2017,
    #         "baby_type" : "WVZ"
    #         }

    ########################################
    ######## WWW BABY MC 2018 102x #########
    ########################################

    sample_info = {

        # "/EGamma/Run2018A-PromptReco-v1/MINIAOD"     : "data_Run2018Av1_ee",
        # "/EGamma/Run2018A-PromptReco-v2/MINIAOD"     : "data_Run2018Av2_ee",
        # "/EGamma/Run2018A-PromptReco-v3/MINIAOD"     : "data_Run2018Av3_ee",
        # "/EGamma/Run2018B-PromptReco-v1/MINIAOD"     : "data_Run2018Bv1_ee",
        # "/EGamma/Run2018B-PromptReco-v2/MINIAOD"     : "data_Run2018Bv2_ee",
        # "/EGamma/Run2018C-PromptReco-v1/MINIAOD"     : "data_Run2018Cv1_ee",
        # "/EGamma/Run2018C-PromptReco-v2/MINIAOD"     : "data_Run2018Cv2_ee",
        # "/EGamma/Run2018C-PromptReco-v3/MINIAOD"     : "data_Run2018Cv3_ee",
        #"/EGamma/Run2018D-PromptReco-v2/MINIAOD"       : "data_Run2018Dv2_ee",
        "/EGamma/Run2018D-22Jan2019-v2/MINIAOD"        : "data_Run2018Dv2_ee",
        # "/DoubleMuon/Run2018A-PromptReco-v1/MINIAOD" : "data_Run2018Av1_mm",
        # "/DoubleMuon/Run2018A-PromptReco-v2/MINIAOD" : "data_Run2018Av2_mm",
        # "/DoubleMuon/Run2018A-PromptReco-v3/MINIAOD" : "data_Run2018Av3_mm",
        # "/DoubleMuon/Run2018B-PromptReco-v1/MINIAOD" : "data_Run2018Bv1_mm",
        # "/DoubleMuon/Run2018B-PromptReco-v2/MINIAOD" : "data_Run2018Bv2_mm",
        # "/DoubleMuon/Run2018C-PromptReco-v1/MINIAOD" : "data_Run2018Cv1_mm",
        # "/DoubleMuon/Run2018C-PromptReco-v2/MINIAOD" : "data_Run2018Cv2_mm",
        # "/DoubleMuon/Run2018C-PromptReco-v3/MINIAOD" : "data_Run2018Cv3_mm",
        "/DoubleMuon/Run2018D-PromptReco-v2/MINIAOD"   : "data_Run2018Dv2_mm",
        # "/MuonEG/Run2018A-PromptReco-v1/MINIAOD"     : "data_Run2018Av1_em",
        # "/MuonEG/Run2018A-PromptReco-v2/MINIAOD"     : "data_Run2018Av2_em",
        # "/MuonEG/Run2018A-PromptReco-v3/MINIAOD"     : "data_Run2018Av3_em",
        # "/MuonEG/Run2018B-PromptReco-v1/MINIAOD"     : "data_Run2018Bv1_em",
        # "/MuonEG/Run2018B-PromptReco-v2/MINIAOD"     : "data_Run2018Bv2_em",
        # "/MuonEG/Run2018C-PromptReco-v1/MINIAOD"     : "data_Run2018Cv1_em",
        # "/MuonEG/Run2018C-PromptReco-v2/MINIAOD"     : "data_Run2018Cv2_em",
        # "/MuonEG/Run2018C-PromptReco-v3/MINIAOD"     : "data_Run2018Cv3_em",
        "/MuonEG/Run2018D-PromptReco-v2/MINIAOD"       : "data_Run2018Dv2_em",

        # Rereco
        "/EGamma/Run2018A-17Sep2018-v2/MINIAOD"     : "data_Run2018Av2_ee",
        "/EGamma/Run2018B-17Sep2018-v1/MINIAOD"     : "data_Run2018Bv1_ee",
        "/EGamma/Run2018C-17Sep2018-v1/MINIAOD"     : "data_Run2018Cv1_ee",

        "/DoubleMuon/Run2018A-17Sep2018-v2/MINIAOD" : "data_Run2018Av2_mm",
        "/DoubleMuon/Run2018B-17Sep2018-v1/MINIAOD" : "data_Run2018Bv1_mm",
        "/DoubleMuon/Run2018C-17Sep2018-v1/MINIAOD" : "data_Run2018Cv1_mm",

        "/MuonEG/Run2018A-17Sep2018-v1/MINIAOD"     : "data_Run2018Av1_em",
        "/MuonEG/Run2018B-17Sep2018-v1/MINIAOD"     : "data_Run2018Bv1_em",
        "/MuonEG/Run2018C-17Sep2018-v1/MINIAOD"     : "data_Run2018Cv1_em",

        # Tribosons
        "/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM"                               : "www_amcatnlo"               ,
        "/WWZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM"                                  : "wwz_amcatnlo"               ,
        "/WZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM"                                  : "wzz_amcatnlo"               ,
        "/ZZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM"                                  : "zzz_amcatnlo"               ,
        "/VHToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM"                        : "vh_nonbb_amcatnlo"          ,
        "/WWZJetsTo4L2Nu_4f_TuneCP5_13TeV_amcatnloFXFX_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM"                     : "wwz_4l2v_amcatnlo"          ,
        # "/ZZZJetsTo4L2Nu_4f_TuneCP5_13TeV_amcatnloFXFX_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM"                     : "zzz_4l2v_amcatnlo"          ,

        # Z + Jets/Gamma
        "/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM"                    : "dy_m1050_madgraph"          ,
        "/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM"                        : "dy_m50_madgraph"            ,
        "/ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM"                   : "zg_llg_amcatnlo"            ,

        # W + Jets
        "/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM"                             : "wjets_incl_madgraph"        ,
        "/WGToLNuG_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM"                               : "wg_lnug_madgraph"           ,

        # Top(/ttbar) + X
        "/ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM" : "stt_antitop_incdec_powheg",
        "/ST_t-channel_top_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM"     : "stt_top_incdec_powheg",
        "/ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-madgraph-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v4/MINIAODSIM"         : "sts_4f_leptonic_madgraph"   ,
        "/ST_tWll_5f_LO_TuneCP5_PSweights_13TeV-madgraph-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v1/MINIAODSIM"              : "twz_ll_madgraph"            ,
        "/ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v3/MINIAODSIM" : "tw_antitopnofullhad_powheg" ,
        "/ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v3/MINIAODSIM"     : "tw_topnofullhad_powheg"     ,
        "/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM"                          : "ttbar_dilep_madgraph"       ,
        "/TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM"             : "ttw_lnu_amcatnlo"           ,
        "/TTZToLL_M-1to10_TuneCP5_13TeV-amcatnlo-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM"                           : "ttz_ll_mll1_amcatnlo"       ,
        "/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM"                     : "ttz_llvv_mll10_amcatnlo"    ,
        "/tZq_ll_4f_ckm_NLO_TuneCP5_13TeV-madgraph-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM"                    : "tZq_ll_madgraph"            ,
        "/ttHToNonbb_M125_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM"                             : "tth_nonbb_powheg"           ,

        # ZZ
        "/GluGluHToZZTo4L_M125_13TeV_powheg2_JHUGenV7011_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM"                   : "ggh_hzz4l_powheg"           ,
        "/ZZTo4L_TuneCP5_13TeV_powheg_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext2-v2/MINIAODSIM"                                 : "zz_4l_powheg"               ,
        "/ZZTo2L2Nu_TuneCP5_13TeV_powheg_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM"                              : "zz_2l2v_powheg"             ,
        "/ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM"                              : "zz_2l2q_powheg"             ,
        "/GluGluToContinToZZTo2e2mu_13TeV_MCFM701_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM"                          : "ggzz_2e2m_mcfm"             ,
        "/GluGluToContinToZZTo2e2tau_13TeV_MCFM701_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM"                         : "ggzz_2e2t_mcfm"             ,
        "/GluGluToContinToZZTo2mu2tau_13TeV_MCFM701_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM"                        : "ggzz_2m2t_mcfm"             ,
        "/GluGluToContinToZZTo4e_13TeV_MCFM701_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM"                             : "ggzz_4e_mcfm"               ,
        "/GluGluToContinToZZTo4mu_13TeV_MCFM701_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM"                            : "ggzz_4m_mcfm"               ,
        "/GluGluToContinToZZTo4tau_13TeV_MCFM701_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM"                           : "ggzz_4t_mcfm"               ,

        # WW
        "/WWTo2L2Nu_NNPDF31_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM"                           : "ww_2l_powheg"               ,

        # WZ
        "/WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM"                              : "wz_2l2q_amcatnlo"           ,
        "/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM"                         : "wz_3lv_amcatnlo"            ,

        # Dibonson + Gamma
        # Missing ZZ Gamma
        "/WZG_TuneCP5_13TeV-amcatnlo-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM"                                       : "wzg_amcatnlo"               ,
        "/WWG_TuneCP5_13TeV-amcatnlo-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM"                                  : "wwg_amcatnlo"               ,

        # ZH Powheg
        "/HZJ_HToWWTo2L2Nu_ZTo2L_M125_13TeV_powheg_jhugen714_pythia8_TuneCP5/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM" : "zh_ww_4l_powheg",
        "/GluGluZH_HToWWTo2L2Nu_ZTo2L_M125_13TeV_powheg_pythia8_TuneCP5_PSweights/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM": "ggzh_ww_4l_powheg",


        }

    dinfos["wvz_2018_102x"] = {
            "samples" : sample_info,
            "year" : 2018,
            "baby_type" : "WVZ"
            }

    dinfos["trilep_2018_102x"] = {
            "samples" : sample_info,
            "year" : 2018,
            "baby_type" : "Trilep"
            }

    dinfos["dilep_2018_102x"] = {
            "samples" : sample_info,
            "year" : 2018,
            "baby_type" : "Dilep"
            }

    dinfos["wvzmva_2018_102x"] = {
            "samples" : sample_info,
            "year" : 2018,
            "baby_type" : "WVZMVA"
            }

    sample_info = {
        "/WWZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM"                                  : "wwz_amcatnlo"               ,
        "/WWZJetsTo4L2Nu_4f_TuneCP5_13TeV_amcatnloFXFX_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM"                     : "wwz_4l2v_amcatnlo"          ,
        }

    dinfos["truth_2018_102x"] = {
            "samples" : sample_info,
            "year" : 2018,
            "baby_type" : "Truth"
            }

    dinfos["wvzsig_2018_102x"] = {
            "samples" : sample_info,
            "year" : 2018,
            "baby_type" : "WVZAll"
            }

    # sample_info = {
    #     "/WWZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM"                                  : "wwz_amcatnlo"               ,
    #     "/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM"                         : "wz_3lv_amcatnlo"            ,
    #     }

    # dinfos["wvz_2018_102x"] = {
    #         "samples" : sample_info,
    #         "year" : 2018,
    #         "baby_type" : "WVZ"
    #         }

    return dinfos

#################################################################################################################################################################################################
# SUBMIT JOBS
#################################################################################################################################################################################################


if __name__ == "__main__":

    import argparse
    import sys

    parser = argparse.ArgumentParser(description="Submitting Baby Maker Jobs to Condor")
    parser.add_argument('-p' , '--production' , dest='production' , help='To submit ALL jobs for production'        , default=False         , action='store_true')
    parser.add_argument('-t' , '--test'       , dest='test'       , help='To submit test jobs (5 jobs each sample)' , default=False         , action='store_true')
    parser.add_argument(       '--tag'        , dest='tag'        , help='job tag'                                                          , required=True      )
    parser.add_argument(       '--sample_sets', dest='sample_sets', help='comma separated sample sets (e.g. WWW2018, WWW2017, FR2017, ...)' , required=True      )

    args = parser.parse_args()

    if args.test and args.production:
        parser.print_help()
        print "ERROR: Option conflict. Can't have both -t,--test and -p,--production at the same time."
        sys.exit(1)

    if not args.test and not args.production:
        parser.print_help()
        print "ERROR: Option missing. Choose either -t,--test or -p,--production."
        sys.exit(1)

    if args.test:
        print "Submitting a test job (i.e. 5 jobs max each sample) with tag =", args.tag

    if args.production:
        print "Submitting a production job (i.e. ALL jobs per sample) with tag =", args.tag

    grand_master_list = get_master_list()

    # filter out grand master list and form a master list to be passed on to submit function
    master_list = {}
    if "WVZ2018" in args.sample_sets:
        master_list["wvz_2018_102x"] = grand_master_list["wvz_2018_102x"]
    if "WVZ2017" in args.sample_sets:
        master_list["wvz_2017_94x"] = grand_master_list["wvz_2017_94x"]
    if "WVZ2016" in args.sample_sets:
        master_list["wvz_2016_94x"] = grand_master_list["wvz_2016_94x"]
    if "Trilep2018" in args.sample_sets:
        master_list["trilep_2018_102x"] = grand_master_list["trilep_2018_102x"]
    if "Trilep2017" in args.sample_sets:
        master_list["trilep_2017_94x"] = grand_master_list["trilep_2017_94x"]
    if "Trilep2016" in args.sample_sets:
        master_list["trilep_2016_94x"] = grand_master_list["trilep_2016_94x"]
    if "Dilep2018" in args.sample_sets:
        master_list["dilep_2018_102x"] = grand_master_list["dilep_2018_102x"]
    if "Dilep2017" in args.sample_sets:
        master_list["dilep_2017_94x"] = grand_master_list["dilep_2017_94x"]
    if "Dilep2016" in args.sample_sets:
        master_list["dilep_2016_94x"] = grand_master_list["dilep_2016_94x"]
    if "WVZMVA2018" in args.sample_sets:
        master_list["wvzmva_2018_102x"] = grand_master_list["wvzmva_2018_102x"]
    if "WVZMVA2017" in args.sample_sets:
        master_list["wvzmva_2017_94x"] = grand_master_list["wvzmva_2017_94x"]
    if "WVZMVA2016" in args.sample_sets:
        master_list["wvzmva_2016_94x"] = grand_master_list["wvzmva_2016_94x"]
    if "Truth2018" in args.sample_sets:
        master_list["truth_2018_102x"] = grand_master_list["truth_2018_102x"]
    if "Truth2017" in args.sample_sets:
        master_list["truth_2017_94x"] = grand_master_list["truth_2017_94x"]
    if "Truth2016" in args.sample_sets:
        master_list["truth_2016_94x"] = grand_master_list["truth_2016_94x"]
    if "WVZSIG2018" in args.sample_sets:
        master_list["wvzsig_2018_102x"] = grand_master_list["wvzsig_2018_102x"]
    if "WVZSIG2017" in args.sample_sets:
        master_list["wvzsig_2017_94x"] = grand_master_list["wvzsig_2017_94x"]

    submit(master_list, args.tag, dotestrun=args.test)

#eof
