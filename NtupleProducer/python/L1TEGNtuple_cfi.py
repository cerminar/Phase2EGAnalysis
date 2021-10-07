import FWCore.ParameterSet.Config as cms

# cluster extras
from L1Trigger.L1THGCalUtilities.hgcalTriggerNtuples_cfi import *
from L1Trigger.Phase2L1ParticleFlow.pfClustersFromHGC3DClusters_cfi import *


ntuple_multiclusters_hmvdr = ntuple_multiclusters.clone()
ntuple_multiclusters_hmvdr.Prefix = cms.untracked.string('Cl3D')


ntuple_multiclusters_extra = cms.PSet(
    NtupleName=cms.string('L1TEGNtupleHGCMulticlusterExtra'),
    Multiclusters=ntuple_multiclusters_hmvdr.Multiclusters,
    BranchNamePrefix=ntuple_multiclusters_hmvdr.Prefix,
    emVsPionID=pfClustersFromHGC3DClusters.emVsPionID,
    emVsPUID=pfClustersFromHGC3DClusters.emVsPUID
)

# tracks
ntuple_TTTracks = cms.PSet(
    NtupleName=cms.string('L1TEGNtupleTrackTrigger'),
    TTTracks=cms.InputTag("TTTracksFromTrackletEmulation", "Level1TTTracks"),
    fillPFDecodedTracks=cms.bool(False),
    PFDecodedTracks=cms.InputTag("l1ctLayer1HGCal", 'DecodedTK'),
    BranchNamePrefix=cms.untracked.string("l1Trk")
)

# barrel
ntuple_EGStaEB = cms.PSet(
    NtupleName=cms.string('L1TEGNtupleEgamma'),
    Egamma=cms.InputTag("L1EGammaClusterEmuProducer"),
    BranchNamePrefix=cms.untracked.string("EGStaEB")
)

# endcap TDR simulation
ntuple_EGStaEE = cms.PSet(
    NtupleName=cms.string('L1TEGNtupleEgamma'),
    Egamma=cms.InputTag('l1EGammaEEProducer:L1EGammaCollectionBXVWithCuts'),
    BranchNamePrefix=cms.untracked.string("SimEGStaEE")
)

# ntuple_TkEleEE = cms.PSet(
#     NtupleName=cms.string('L1TEGNtupleTkElectrons'),
#     TkElectrons=cms.InputTag("L1TkElectronsHGC","EG"),
#     BranchNamePrefix=cms.untracked.string("TkEleEE")
# )
#
# ntuple_TkEleEB = cms.PSet(
#     NtupleName=cms.string('L1TEGNtupleTkElectrons'),
#     TkElectrons=cms.InputTag("L1TkElectronsCrystal","EG"),
#     BranchNamePrefix=cms.untracked.string("TkEleEB")
# )

ntuple_TkEleEllEE = cms.PSet(
    NtupleName=cms.string('L1TEGNtupleTkElectrons'),
    TkElectrons=cms.InputTag("L1TkElectronsEllipticMatchHGC", "EG"),
    BranchNamePrefix=cms.untracked.string("SimTkEleEE")
)

ntuple_TkEmEB = cms.PSet(
    NtupleName=cms.string('L1TEGNtupleTkEm'),
    TkEms=cms.InputTag("L1TkPhotonsCrystal", "EG"),
    BranchNamePrefix=cms.untracked.string("SimTkEmEB")
)

ntuple_TkEmEE = cms.PSet(
    NtupleName=cms.string('L1TEGNtupleTkEm'),
    TkEms=cms.InputTag("L1TkPhotonsHGC", "EG"),
    BranchNamePrefix=cms.untracked.string("SimTkEmEE")
)

ntuple_TkEleEllEB = cms.PSet(
    NtupleName=cms.string('L1TEGNtupleTkElectrons'),
    TkElectrons=cms.InputTag("L1TkElectronsEllipticMatchCrystal", "EG"),
    BranchNamePrefix=cms.untracked.string("SimTkEleEB")
)

# L1T correlator emulation
ntuple_L1TCorrEGStaEE = cms.PSet(
    NtupleName=cms.string('L1TEGNtupleEgamma'),
    Egamma=cms.InputTag('l1ctLayer1EG:L1EgEE'),
    BranchNamePrefix=cms.untracked.string("EGStaEE")
)


ntuple_L1TCorrTkEleEE = cms.PSet(
    NtupleName=cms.string('L1TEGNtupleTkElectrons'),
    TkElectrons=cms.InputTag("l1ctLayer1EG", "L1TkEleEE"),
    BranchNamePrefix=cms.untracked.string("TkEleEE")
)

ntuple_L1TCorrTkEleEB = cms.PSet(
    NtupleName=cms.string('L1TEGNtupleTkElectrons'),
    TkElectrons=cms.InputTag("l1ctLayer1EG", "L1TkEleEB"),
    BranchNamePrefix=cms.untracked.string("TkEleEB")
)

ntuple_L1TCorrTkEmEE = cms.PSet(
    NtupleName=cms.string('L1TEGNtupleTkEm'),
    TkEms=cms.InputTag("l1ctLayer1EG", "L1TkEmEE"),
    BranchNamePrefix=cms.untracked.string("TkEmEE")
)


ntuple_L1TCorrTkEmEB = cms.PSet(
    NtupleName=cms.string('L1TEGNtupleTkEm'),
    TkEms=cms.InputTag("l1ctLayer1EG", "L1TkEmEB"),
    BranchNamePrefix=cms.untracked.string("TkEmEB")
)

ntuple_PFCand = cms.PSet(
    NtupleName=cms.string('L1TEGNtuplePFCands'),
    L1PFObjects=cms.InputTag("l1ctLayer1", "PF"),
    BranchNamePrefix=cms.untracked.string("PFCand")
)
