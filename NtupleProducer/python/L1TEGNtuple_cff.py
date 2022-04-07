import FWCore.ParameterSet.Config as cms


from Phase2EGTriggerAnalysis.NtupleProducer.L1TEGNtuple_cfi import *

l1EGTriggerNtuplizer = hgcalTriggerNtuplizer.clone()
l1EGTriggerNtuplizer.hgc_Ntuples = cms.VPSet(
    ntuple_event,
    ntuple_triggercells,
    ntuple_multiclusters_hmvdr,
)
l1EGTriggerNtuplizer.l1teg_Ntuples = cms.VPSet(
    ntuple_l1tgen,
    ntuple_multiclusters_extra,
    ntuple_EGStaEB,
    ntuple_EGStaEE,
    ntuple_TTTracks,
    ntuple_TkEleEllEE,
    ntuple_TkEleEllEB,
    ntuple_L1TCorrEGStaEE,
    ntuple_L1TCorrTkEleEE,
    ntuple_L1TCorrTkEleEB,
    ntuple_L1TCorrTkEmEE,
    ntuple_L1TCorrTkEmEB
)


l1EGTriggerNtuplizer_egOnly = hgcalTriggerNtuplizer.clone()
l1EGTriggerNtuplizer_egOnly.hgc_Ntuples = cms.VPSet(
    ntuple_event,
)
l1EGTriggerNtuplizer_egOnly.l1teg_Ntuples = cms.VPSet(
    ntuple_l1tgen,
    ntuple_TTTracks,
    ntuple_EGStaEB,
    ntuple_EGStaEE,
    ntuple_TkEleEllEE,
    ntuple_TkEleEllEB,
    ntuple_L1TCorrEGStaEE,
    ntuple_L1TCorrTkEleEE,
    ntuple_L1TCorrTkEleEB,
    ntuple_L1TCorrTkEmEE,
    ntuple_L1TCorrTkEmEB
)

l1EGTriggerNtuplizer_l1tCorr = l1tEGNtuplizer.clone()
l1EGTriggerNtuplizer_l1tCorr.hgc_Ntuples = cms.VPSet(
    ntuple_event,
)
l1EGTriggerNtuplizer_l1tCorr.l1teg_Ntuples = cms.VPSet(
    ntuple_l1tgen,
    ntuple_TTTracks,
    ntuple_L1TCorrEGStaEE,
    ntuple_L1TCorrTkEleEE,
    ntuple_L1TCorrTkEleEB,
    ntuple_L1TCorrTkEmEE,
    ntuple_L1TCorrTkEmEB,
    ntuple_L1TCorrL2TkEle,
    ntuple_L1TCorrL2TkEm
)


l1EGTriggerNtuplizer_l1tCorrWCand = l1EGTriggerNtuplizer_l1tCorr.clone()
l1EGTriggerNtuplizer_l1tCorrWCand.l1teg_Ntuples.append = ntuple_PFCand

l1EGTriggerNtuplizer_noGen = hgcalTriggerNtuplizer.clone()
l1EGTriggerNtuplizer_noGen.hgc_Ntuples = cms.VPSet(
    ntuple_event,
)
l1EGTriggerNtuplizer_noGen.l1teg_Ntuples = cms.VPSet(
    ntuple_EGStaEB,
    ntuple_EGStaEE,
    ntuple_TTTracks,
    ntuple_TkEleEllEE,
    ntuple_TkEleEllEB,
    ntuple_L1TCorrEGStaEE,
    ntuple_L1TCorrTkEleEE,
    ntuple_L1TCorrTkEleEB,
    ntuple_L1TCorrTkEmEE,
    ntuple_L1TCorrTkEmEB
)


#
l1EGTriggerNtuples_seq = cms.Sequence(l1EGTriggerNtuplizer)
