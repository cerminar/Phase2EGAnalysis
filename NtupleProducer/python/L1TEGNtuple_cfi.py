import FWCore.ParameterSet.Config as cms

# cluster extras
from L1Trigger.L1THGCalUtilities.hgcalTriggerNtuples_cfi import *
from L1Trigger.Phase2L1ParticleFlow.l1tPFClustersFromHGC3DClusters_cfi import *


l1tEGNtuplizer = cms.EDAnalyzer(
    "L1TEGNtupleManager",
    Ntuples = cms.VPSet()
)


ntuple_multiclusters_hmvdr = ntuple_multiclusters.clone()
ntuple_multiclusters_hmvdr.Prefix = cms.untracked.string('Cl3D')


ntuple_l1tgen = cms.PSet(
    NtupleName = cms.string('L1TEGNtupleGen'),
    GenPU = cms.InputTag("addPileupInfo"),
    GenParticles = cms.InputTag("genParticles"),
    MCEvent = cms.InputTag("generatorSmeared"),
    SimTracks = cms.InputTag("g4SimHits"),
    SimVertices = cms.InputTag("g4SimHits"),
    particleFilter = cms.PSet(
        EMin = cms.double(0.1),
        chargedPtMin = cms.double(0.1),
        etaMax = cms.double(3.1),
        invisibleParticles = cms.vint32(),
        protonEMin = cms.double(100000),
        rMax = cms.double(129.0),
        zMax = cms.double(317.0)
    )
)


ntuple_multiclusters_extra = cms.PSet(
    NtupleName=cms.string('L1TEGNtupleHGCMulticlusterExtra'),
    Multiclusters=ntuple_multiclusters_hmvdr.Multiclusters,
    BranchNamePrefix=ntuple_multiclusters_hmvdr.Prefix,
    emVsPionID=l1tPFClustersFromHGC3DClusters.emVsPionID,
    emVsPUID=l1tPFClustersFromHGC3DClusters.emVsPUID
)

# tracks
ntuple_TTTracks = cms.PSet(
    NtupleName=cms.string('L1TEGNtupleTrackTrigger'),
    TTTracks=cms.InputTag("l1tTTTracksFromTrackletEmulation", "Level1TTTracks"),
    fillPFDecodedTracks=cms.bool(False),
    fillStubsInfo=cms.bool(True),
    fillTrackingParticleTruth=cms.bool(False),
    PFDecodedTracks=cms.InputTag("l1ctLayer1HGCal", 'DecodedTK'),
    MCTruthStubInputTag=cms.InputTag("TTStubAssociatorFromPixelDigis", "StubAccepted"),
    MCTruthTrackInputTag=cms.InputTag("TTTrackAssociatorFromPixelDigis", "Level1TTTracks"),  # MCTruth input
    BranchNamePrefix=cms.untracked.string("l1Trk")
)

# barrel
ntuple_EGStaEB = cms.PSet(
    NtupleName=cms.string('L1TEGNtupleEgamma'),
    Egamma=cms.InputTag("l1tEGammaClusterEmuProducer"),
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
    Egamma=cms.InputTag('l1tLayer1EG:L1EgEE'),
    BranchNamePrefix=cms.untracked.string("EGStaEE")
)


ntuple_L1TCorrTkEleEE = cms.PSet(
    NtupleName=cms.string('L1TEGNtupleTkElectrons'),
    TkElectrons=cms.InputTag("l1tLayer1EG", "L1TkEleEE"),
    BranchNamePrefix=cms.untracked.string("TkEleEE")
)

ntuple_L1TCorrTkEleEB = cms.PSet(
    NtupleName=cms.string('L1TEGNtupleTkElectrons'),
    TkElectrons=cms.InputTag("l1tLayer1EG", "L1TkEleEB"),
    BranchNamePrefix=cms.untracked.string("TkEleEB")
)



ntuple_L1TCorrTkEmEE = cms.PSet(
    NtupleName=cms.string('L1TEGNtupleTkEm'),
    TkEms=cms.InputTag("l1tLayer1EG", "L1TkEmEE"),
    BranchNamePrefix=cms.untracked.string("TkEmEE")
)

ntuple_L1TCorrL2TkEm = cms.PSet(
    NtupleName=cms.string('L1TEGNtupleTkEm'),
    TkEms=cms.InputTag("l1tLayer2EG", "L1CtTkEm"),
    BranchNamePrefix=cms.untracked.string("L2TkEm")
)

ntuple_L1TCorrL2TkEle = cms.PSet(
    NtupleName=cms.string('L1TEGNtupleTkElectrons'),
    TkElectrons=cms.InputTag("l1tLayer2EG", "L1CtTkElectron"),
    BranchNamePrefix=cms.untracked.string("L2TkEle")
)


ntuple_L1TCorrTkEmEB = cms.PSet(
    NtupleName=cms.string('L1TEGNtupleTkEm'),
    TkEms=cms.InputTag("l1tLayer1EG", "L1TkEmEB"),
    BranchNamePrefix=cms.untracked.string("TkEmEB")
)

ntuple_PFCand = cms.PSet(
    NtupleName=cms.string('L1TEGNtuplePFCands'),
    L1PFObjects=cms.InputTag("l1tLayer1", "PF"),
    BranchNamePrefix=cms.untracked.string("PFCand")
)
