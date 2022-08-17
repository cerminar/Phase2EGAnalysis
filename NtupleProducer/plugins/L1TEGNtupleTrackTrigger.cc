#include "L1Trigger/L1THGCal/interface/HGCalTriggerTools.h"

#include "L1Trigger/L1TTrackMatch/interface/pTFrom2Stubs.h"

#include "DataFormats/L1TrackTrigger/interface/TTTrack.h"
#include "DataFormats/L1TrackTrigger/interface/TTTypes.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/L1TParticleFlow/interface/PFTrack.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "CommonTools/BaseParticlePropagator/interface/BaseParticlePropagator.h"
#include "FWCore/Framework/interface/ESWatcher.h"
 
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"

#include "SimTracker/TrackTriggerAssociation/interface/TTStubAssociationMap.h"
#include "SimTracker/TrackTriggerAssociation/interface/TTTrackAssociationMap.h"



#include "Phase2EGTriggerAnalysis/NtupleProducer/interface/L1TEGNtupleBase.h"

class L1TEGNtupleTrackTrigger : public L1TEGNtupleBase {
public:
  L1TEGNtupleTrackTrigger(const edm::ParameterSet& conf);
  ~L1TEGNtupleTrackTrigger() override{};
  void initialize(TTree&, const edm::ParameterSet&, edm::ConsumesCollector&&) final;
  void fill(const edm::Event &e, const L1TEGNtupleEventSetup &l1teg_es) final;
  // void fill(const edm::Event& e, const edm::EventSetup& es) final;
  typedef TTTrack<Ref_Phase2TrackerDigi_> L1TTTrackType;

private:
  void clear() final;
  // HGCalTriggerTools triggerTools_;
  std::pair<float, float> propagateToCalo(const math::XYZTLorentzVector& iMom,
                                          const math::XYZTLorentzVector& iVtx,
                                          double iCharge,
                                          double iBField);

  edm::EDGetToken track_token_;
  edm::EDGetToken decoded_track_token_;
  edm::EDGetToken ttStubMCTruthToken_;
  edm::EDGetToken ttTrackMCTruthToken_;

  bool fill_decoded_pftracks_;
  bool fill_stubs_;
  bool fill_mctruth_;

  int l1track_n_;
  std::vector<float> l1track_pt_;
  std::vector<float> l1track_pt2stubs_;

  // std::vector<float> l1track_energy_;
  std::vector<float> l1track_eta_;
  std::vector<float> l1track_phi_;
  std::vector<float> l1track_curv_;
  std::vector<float> l1track_chi2_;
  std::vector<float> l1track_chi2Red_;
  std::vector<int> l1track_nStubs_;
  
  std::vector<std::vector<int>> l1track_stubTpPid_;
  std::vector<std::vector<float>> l1track_stubTpPt_;
  std::vector<std::vector<float>> l1track_stubTpEta_;
  std::vector<std::vector<float>> l1track_stubTpPhi_;
  std::vector<std::vector<int>> l1track_stubTpGenuine_;

  std::vector<float> l1track_z0_;
  std::vector<int> l1track_charge_;
  std::vector<float> l1track_caloeta_;
  std::vector<float> l1track_calophi_;
  std::vector<int> l1track_tpmatch_;


  int pfdtk_n_;
  std::vector<float> pfdtk_pt_;
  std::vector<float> pfdtk_eta_;
  std::vector<float> pfdtk_phi_;
  std::vector<float> pfdtk_caloeta_;
  std::vector<float> pfdtk_calophi_;
  std::vector<float> pfdtk_z0_;
  std::vector<float> pfdtk_simpt_;
  std::vector<float> pfdtk_simeta_;
  std::vector<float> pfdtk_simphi_;
  std::vector<float> pfdtk_simcaloeta_;
  std::vector<float> pfdtk_simcalophi_;
  std::vector<float> pfdtk_simz0_;
  std::vector<float> pfdtk_chi2RPhi_;
  std::vector<float> pfdtk_chi2RZ_;
  std::vector<float> pfdtk_chi2Bend_;
  std::vector<int> pfdtk_hitPattern_;
  std::vector<int> pfdtk_nStubs_;
  std::vector<int> pfdtk_mvaQual_;
  std::vector<int> pfdtk_otherQual_;


  edm::ESWatcher<IdealMagneticFieldRecord> magfield_watcher_;

  HGCalTriggerTools triggerTools_;
};

DEFINE_EDM_PLUGIN(L1TEGNtupleFactory, L1TEGNtupleTrackTrigger, "L1TEGNtupleTrackTrigger");

L1TEGNtupleTrackTrigger::L1TEGNtupleTrackTrigger(const edm::ParameterSet& conf)
    : L1TEGNtupleBase(conf) {
      accessEventSetup_ = false;
    }

void L1TEGNtupleTrackTrigger::initialize(TTree& tree,
                                         const edm::ParameterSet& conf,
                                         edm::ConsumesCollector&& collector) {
  track_token_ =
      collector.consumes<std::vector<TTTrack<Ref_Phase2TrackerDigi_>>>(conf.getParameter<edm::InputTag>("TTTracks"));

  fill_decoded_pftracks_ = conf.getParameter<bool>("fillPFDecodedTracks");
  fill_stubs_ = conf.getParameter<bool>("fillStubsInfo");
  fill_mctruth_ = conf.getParameter<bool>("fillTrackingParticleTruth");

  if(fill_decoded_pftracks_) {
    decoded_track_token_ = 
      collector.consumes<std::vector<l1t::PFTrack>>(conf.getParameter<edm::InputTag>("PFDecodedTracks"));
  }

  if(fill_mctruth_) {
    ttStubMCTruthToken_ = 
      collector.consumes<TTStubAssociationMap<Ref_Phase2TrackerDigi_> >(conf.getParameter<edm::InputTag>("MCTruthStubInputTag"));

    ttTrackMCTruthToken_ = 
      collector.consumes<TTTrackAssociationMap<Ref_Phase2TrackerDigi_> >(conf.getParameter<edm::InputTag>("MCTruthTrackInputTag"));
  }


  tree.Branch(branch_name_w_prefix("n").c_str(), &l1track_n_, branch_name_w_prefix("n/I").c_str());
  tree.Branch(branch_name_w_prefix("pt").c_str(), &l1track_pt_);
  tree.Branch(branch_name_w_prefix("pt2stubs").c_str(), &l1track_pt2stubs_);

  // tree.Branch("l1track_energy", &l1track_energy_);
  tree.Branch(branch_name_w_prefix("eta").c_str(), &l1track_eta_);
  tree.Branch(branch_name_w_prefix("phi").c_str(), &l1track_phi_);
  tree.Branch(branch_name_w_prefix("curv").c_str(), &l1track_curv_);
  tree.Branch(branch_name_w_prefix("chi2").c_str(), &l1track_chi2_);
  tree.Branch(branch_name_w_prefix("chi2Red").c_str(), &l1track_chi2Red_);
  if(fill_stubs_) {
    tree.Branch(branch_name_w_prefix("nStubs").c_str(), &l1track_nStubs_);
    if(fill_mctruth_) {
      tree.Branch(branch_name_w_prefix("stubTpPid").c_str(), &l1track_stubTpPid_);
      tree.Branch(branch_name_w_prefix("stubTpPt").c_str(), &l1track_stubTpPt_);
      tree.Branch(branch_name_w_prefix("stubTpEta").c_str(), &l1track_stubTpEta_);
      tree.Branch(branch_name_w_prefix("stubTpPhi").c_str(), &l1track_stubTpPhi_);
      tree.Branch(branch_name_w_prefix("stubTpGenuine").c_str(), &l1track_stubTpGenuine_);
    }
  }
  tree.Branch(branch_name_w_prefix("z0").c_str(), &l1track_z0_);
  tree.Branch(branch_name_w_prefix("charge").c_str(), &l1track_charge_);
  tree.Branch(branch_name_w_prefix("caloeta").c_str(), &l1track_caloeta_);
  tree.Branch(branch_name_w_prefix("calophi").c_str(), &l1track_calophi_);

  if(fill_mctruth_) {
    tree.Branch(branch_name_w_prefix("tpmatch").c_str(), &l1track_tpmatch_);    
  }
  
  if(fill_decoded_pftracks_) {
    tree.Branch("pfdtk_n", &pfdtk_n_, "pfdtk_n/I");
    tree.Branch("pfdtk_pt", &pfdtk_pt_);
    tree.Branch("pfdtk_eta", &pfdtk_eta_);
    tree.Branch("pfdtk_phi", &pfdtk_phi_);
    tree.Branch("pfdtk_caloeta", &pfdtk_caloeta_);
    tree.Branch("pfdtk_calophi", &pfdtk_calophi_);
    tree.Branch("pfdtk_z0", &pfdtk_z0_);
    tree.Branch("pfdtk_simpt", &pfdtk_simpt_);
    tree.Branch("pfdtk_simeta", &pfdtk_simeta_);
    tree.Branch("pfdtk_simphi", &pfdtk_simphi_);
    tree.Branch("pfdtk_simcaloeta", &pfdtk_simcaloeta_);
    tree.Branch("pfdtk_simcalophi", &pfdtk_simcalophi_);
    tree.Branch("pfdtk_simz0", &pfdtk_simz0_);
    tree.Branch("pfdtk_chi2RPhi", &pfdtk_chi2RPhi_);
    tree.Branch("pfdtk_chi2RZ", &pfdtk_chi2RZ_);
    tree.Branch("pfdtk_chi2Bend", &pfdtk_chi2Bend_);
    tree.Branch("pfdtk_hitPattern", &pfdtk_hitPattern_);
    tree.Branch("pfdtk_nStubs", &pfdtk_nStubs_);
    tree.Branch("pfdtk_mvaQual", &pfdtk_mvaQual_);
    tree.Branch("pfdtk_otherQual", &pfdtk_otherQual_);
  }
  
}

void L1TEGNtupleTrackTrigger::fill(const edm::Event &ev, const L1TEGNtupleEventSetup &l1teg_es) {
  // the L1Tracks
  edm::Handle<std::vector<L1TTTrackType>> l1TTTrackHandle;
  ev.getByToken(track_token_, l1TTTrackHandle);

  edm::Handle<std::vector<l1t::PFTrack>> pfDecodedTrackHandle;
  if(fill_decoded_pftracks_) {
    ev.getByToken(decoded_track_token_, pfDecodedTrackHandle);
  }

  edm::Handle<TTStubAssociationMap<Ref_Phase2TrackerDigi_> > MCTruthTTStubHandle;
  edm::Handle<TTTrackAssociationMap<Ref_Phase2TrackerDigi_> > MCTruthTTTrackHandle;
  if(fill_mctruth_) {
    ev.getByToken(ttStubMCTruthToken_, MCTruthTTStubHandle);
    ev.getByToken(ttTrackMCTruthToken_, MCTruthTTTrackHandle);
  }

  float fBz = l1teg_es.magfield->inTesla(GlobalPoint(0, 0, 0)).z();


  // geometry needed to call pTFrom2Stubs
  const TrackerGeometry* tGeom = l1teg_es.tkgeom.product();

  triggerTools_.setGeometry(l1teg_es.hgcGeom.product());

  clear();
  for (auto trackIter = l1TTTrackHandle->begin(); trackIter != l1TTTrackHandle->end(); ++trackIter) {
    edm::Ptr<TTTrack<Ref_Phase2TrackerDigi_> > l1track_ptr(l1TTTrackHandle, l1track_n_);

    l1track_n_++;
    l1track_pt_.emplace_back(trackIter->momentum().perp());
    l1track_pt2stubs_.emplace_back(pTFrom2Stubs::pTFrom2(trackIter, tGeom));

    // l1track_energy_.emplace_back(trackIter->energy());
    l1track_eta_.emplace_back(trackIter->momentum().eta());
    l1track_phi_.emplace_back(trackIter->momentum().phi());
    l1track_curv_.emplace_back(trackIter->rInv());
    l1track_chi2_.emplace_back(trackIter->chi2());
    l1track_chi2Red_.emplace_back(trackIter->chi2Red());
    
    
    // edm::Ptr<TrackingParticle> my_tp = MCTruthTTTrackHandle->findTrackingParticlePtr(trackIter);

    
    if(fill_stubs_) {
      l1track_nStubs_.emplace_back(trackIter->getStubRefs().size());

      if(fill_mctruth_) {
        std::vector<int> stub_tp_id;
        std::vector<float> stub_tp_pt;
        std::vector<float> stub_tp_eta;
        std::vector<float> stub_tp_phi;
        std::vector<int> stub_tp_genuine;

        for(const auto& stub: trackIter->getStubRefs()) {
          edm::Ptr<TrackingParticle> my_tp = MCTruthTTStubHandle->findTrackingParticlePtr(stub);

          int stub_genuine = MCTruthTTStubHandle->isGenuine(stub);

          if (my_tp.isNull() == false) {
            int tmp_eventid = my_tp->eventId().event();

            if (tmp_eventid > 0) {
              // std::cout << "  this is PU TrackingParticle" << std::endl;
              
              stub_tp_id.push_back(999);
              stub_tp_pt.push_back(0);
              stub_tp_eta.push_back(0);
              stub_tp_phi.push_back(0);
              stub_tp_genuine.push_back(stub_genuine);

              
              continue;
            }
            
            stub_tp_id.push_back(my_tp->pdgId());
            stub_tp_pt.push_back(my_tp->p4().pt());
            stub_tp_eta.push_back(my_tp->p4().eta());
            stub_tp_phi.push_back(my_tp->p4().phi() );
            stub_tp_genuine.push_back(stub_genuine);

            // std::cout << "   TP PID: " << my_tp->pdgId()
            //            << " pt: " << my_tp->p4().pt()
            //            << " eta: " << my_tp->p4().eta()
            //            << " phi: " << my_tp->p4().phi() 
            //            << " genuine: " << stub_genuine
            //            << std::endl;
                       
          } else {
            // std::cout << "   No matched TrackingParticle " << std::endl;
            stub_tp_id.push_back(-999);
            stub_tp_pt.push_back(0);
            stub_tp_eta.push_back(0);
            stub_tp_phi.push_back(0);
            stub_tp_genuine.push_back(stub_genuine);
          }

        }
        l1track_stubTpPid_.push_back(stub_tp_id);
        l1track_stubTpPt_.push_back(stub_tp_pt);
        l1track_stubTpEta_.push_back(stub_tp_eta);
        l1track_stubTpPhi_.push_back(stub_tp_phi);
        l1track_stubTpGenuine_.push_back(stub_tp_genuine);

      }

    }
    // FIXME: need to be configuratble?
    // int nParam_ = 4;
    float z0 = trackIter->POCA().z();  //cm
    int charge = trackIter->rInv() > 0 ? +1 : -1;

    reco::Candidate::PolarLorentzVector p4p(
        trackIter->momentum().perp(), trackIter->momentum().eta(), trackIter->momentum().phi(), 0);  // no mass ?
    reco::Particle::LorentzVector p4(p4p.X(), p4p.Y(), p4p.Z(), p4p.E());
    reco::Particle::Point vtx(0., 0., z0);

    auto caloetaphi = propagateToCalo(p4, math::XYZTLorentzVector(0., 0., z0, 0.), charge, fBz);

    l1track_z0_.emplace_back(z0);
    l1track_charge_.emplace_back(charge);
    l1track_caloeta_.emplace_back(caloetaphi.first);
    l1track_calophi_.emplace_back(caloetaphi.second);
    
    ///--- Get quality of L1 track based on truth info.
    /// (N.B. "genuine" tracks are used for official L1 track efficiency measurements).

    /// Exactly one (i.e. not 0 or >= 2) unique TP contributes to every stub on the track.
    /// (even if it is not the principle TP in a stub, or contributes to only one cluster
    /// in the stub, it still counts).
    /// N.B. If cfg param getAllowOneFalse2SStub() is true, then one incorrect stub in
    /// a 2S module is alowed
    /// ISSUE: a track with 4 stubs can be accepted if only 3 of its stubs are correct!
    /// ISSUE: isLooselyGenuine() must also be true. So if 2 TPs match track, one with
    /// an incorrect PS stub, both isGenuine() & isLooselyGenuine() will be false!
    
    if(fill_mctruth_) {
      int tmp_trk_genuine = MCTruthTTTrackHandle->isGenuine(l1track_ptr);
      int tmp_trk_loose = MCTruthTTTrackHandle->isLooselyGenuine(l1track_ptr);
      int tmp_trk_unknown = MCTruthTTTrackHandle->isUnknown(l1track_ptr);
      int tmp_trk_combinatoric = MCTruthTTTrackHandle->isCombinatoric(l1track_ptr);
      
      unsigned int tpmatch = (tmp_trk_combinatoric) + (tmp_trk_unknown << 1) + (tmp_trk_loose << 2) + (tmp_trk_genuine << 3);
      l1track_tpmatch_.push_back(tpmatch);
      // std::cout << "Track Genuine: " << tmp_trk_genuine
      // << " loose-genuine: " << tmp_trk_loose
      //   /// More than one stub on track is "unknown".
      // << " unknown: " << tmp_trk_unknown
      //   /// Both isLooselyGenuine() & isUnknown() are false.
      // << " combinatorics: " << tmp_trk_combinatoric 
      // << " TP Match: " << tpmatch
      // <<std::endl;
    }
  }
  
  if(fill_decoded_pftracks_) {
    for(auto dtk: *pfDecodedTrackHandle) {
      pfdtk_n_++;
      pfdtk_pt_.emplace_back(dtk.pt());
      pfdtk_eta_.emplace_back(dtk.eta());
      pfdtk_phi_.emplace_back(dtk.phi());
      pfdtk_caloeta_.emplace_back(dtk.caloEta());
      pfdtk_calophi_.emplace_back(dtk.caloPhi());
      pfdtk_z0_.emplace_back(dtk.vz());

      auto tk = dtk.track();
      
      float z0 = tk->POCA().z();  //cm
      int charge = tk->rInv() > 0 ? +1 : -1;
      reco::Candidate::PolarLorentzVector p4p(
          tk->momentum().perp(), tk->momentum().eta(), tk->momentum().phi(), 0);  // no mass ?
      reco::Particle::LorentzVector p4(p4p.X(), p4p.Y(), p4p.Z(), p4p.E());
      reco::Particle::Point vtx(0., 0., z0);

      auto caloetaphi = propagateToCalo(p4, math::XYZTLorentzVector(0., 0., z0, 0.), charge, fBz);
      
      pfdtk_simpt_.emplace_back(tk->momentum().perp());
      pfdtk_simeta_.emplace_back(tk->momentum().eta());
      pfdtk_simphi_.emplace_back(tk->momentum().phi());

      pfdtk_simcaloeta_.emplace_back(caloetaphi.first);
      pfdtk_simcalophi_.emplace_back(caloetaphi.second);
      pfdtk_simz0_.emplace_back(z0);

      // we access the following info directly from the ttrack word
      pfdtk_chi2RPhi_.emplace_back( dtk.trackWord().getChi2RPhi());
      pfdtk_chi2RZ_.emplace_back(dtk.trackWord().getChi2RZ());
      pfdtk_chi2Bend_.emplace_back(dtk.trackWord().getBendChi2());
      pfdtk_hitPattern_.emplace_back(dtk.trackWord().getHitPattern());
      pfdtk_nStubs_.emplace_back(dtk.trackWord().getNStubs());
      pfdtk_mvaQual_.emplace_back(dtk.trackWord().getMVAQuality());
      pfdtk_otherQual_.emplace_back(dtk.trackWord().getMVAOther());


    }  
  }
}

void L1TEGNtupleTrackTrigger::clear() {
  l1track_n_ = 0;
  l1track_pt_.clear();
  l1track_pt2stubs_.clear();
  // l1track_energy_.clear();
  l1track_eta_.clear();
  l1track_phi_.clear();
  l1track_curv_.clear();
  l1track_chi2_.clear();
  l1track_chi2Red_.clear();
  l1track_nStubs_.clear();
  l1track_z0_.clear();
  l1track_charge_.clear();
  l1track_caloeta_.clear();
  l1track_calophi_.clear();
  l1track_tpmatch_.clear();
  
  pfdtk_n_ = 0;
  pfdtk_pt_.clear();
  pfdtk_eta_.clear();
  pfdtk_phi_.clear();
  pfdtk_caloeta_.clear();
  pfdtk_calophi_.clear();
  pfdtk_z0_.clear();
  pfdtk_simpt_.clear();
  pfdtk_simeta_.clear();
  pfdtk_simphi_.clear();
  pfdtk_simcaloeta_.clear();
  pfdtk_simcalophi_.clear();
  pfdtk_simz0_.clear();

  
}

// #include "FastSimulation/Particle/interface/RawParticle.h"

std::pair<float, float> L1TEGNtupleTrackTrigger::propagateToCalo(const math::XYZTLorentzVector& iMom,
                                                                     const math::XYZTLorentzVector& iVtx,
                                                                     double iCharge,
                                                                     double iBField) {
  BaseParticlePropagator prop = BaseParticlePropagator(RawParticle(iMom, iVtx, iCharge), 0., 0., iBField);
  prop.setPropagationConditions(129.0, triggerTools_.getLayerZ(1), false);
  prop.propagate();
  double ecalShowerDepth = reco::PFCluster::getDepthCorrection(prop.particle().momentum().E(), false, false);
  math::XYZVector point = math::XYZVector(prop.particle().vertex()) +
                          math::XYZTLorentzVector(prop.particle().momentum()).Vect().Unit() * ecalShowerDepth;
  // math::XYZVector point  = particle.vertex();
  // math::XYZVector point = math::XYZVector(particle.vertex())+math::XYZTLorentzVector(particle.momentum()).Vect().Unit()*ecalShowerDepth;
  return std::make_pair(point.eta(), point.phi());
}
