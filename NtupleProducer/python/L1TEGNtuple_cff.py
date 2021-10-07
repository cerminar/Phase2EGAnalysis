import FWCore.ParameterSet.Config as cms


from Phase2EGTriggerAnalysis.NtupleProducer.L1TEGNtuple_cfi import *

l1EGTriggerNtuplizer = hgcalTriggerNtuplizer.clone()
l1EGTriggerNtuplizer.Ntuples = cms.VPSet(
    ntuple_event,
    ntuple_gen,
    ntuple_triggercells,
    ntuple_multiclusters_hmvdr,
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
l1EGTriggerNtuplizer_egOnly.Ntuples = cms.VPSet(
    ntuple_event,
    ntuple_gen,
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


l1EGTriggerNtuplizer_l1tCorr = hgcalTriggerNtuplizer.clone()
l1EGTriggerNtuplizer_l1tCorr.Ntuples = cms.VPSet(
    ntuple_event,
    ntuple_gen,
    ntuple_TTTracks,
    ntuple_L1TCorrEGStaEE,
    ntuple_L1TCorrTkEleEE,
    ntuple_L1TCorrTkEleEB,
    ntuple_L1TCorrTkEmEE,
    ntuple_L1TCorrTkEmEB
)


l1EGTriggerNtuplizer_l1tCorrWCand = l1EGTriggerNtuplizer_l1tCorr.clone()
l1EGTriggerNtuplizer_l1tCorrWCand.Ntuples.append = ntuple_PFCand

l1EGTriggerNtuplizer_noGen = hgcalTriggerNtuplizer.clone()
l1EGTriggerNtuplizer_noGen.Ntuples = cms.VPSet(
    ntuple_event,
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
