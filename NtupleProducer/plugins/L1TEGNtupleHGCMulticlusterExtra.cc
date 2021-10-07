#include "DataFormats/L1THGCal/interface/HGCalMulticluster.h"
#include "L1Trigger/Phase2L1ParticleFlow/interface/HGC3DClusterEgID.h"
#include "L1TEGNtupleBase.h"


class L1TEGNtupleHGCMulticlusterExtra : public L1TEGNtupleBase {
public:
  L1TEGNtupleHGCMulticlusterExtra(const edm::ParameterSet& conf);
  ~L1TEGNtupleHGCMulticlusterExtra() override{};
  void initialize(TTree&, const edm::ParameterSet&, edm::ConsumesCollector&&) final;
  void fill(const edm::Event& e, const edm::EventSetup& es) final;

private:
  void clear() final;

  edm::EDGetToken multiclusters_token_;
  l1tpf::HGC3DClusterEgID emVsPionID_;
  l1tpf::HGC3DClusterEgID emVsPUID_;

  int cl3d_n_;
  std::vector<uint32_t> cl3d_id_;
  std::vector<uint32_t> cl3d_passEmVsPU_;
  std::vector<float> cl3d_EmVsPUOut_;
  std::vector<uint32_t> cl3d_passEmVsPi_;
  std::vector<float> cl3d_EmVsPiOut_;
  
};

DEFINE_EDM_PLUGIN(HGCalTriggerNtupleFactory, L1TEGNtupleHGCMulticlusterExtra, "L1TEGNtupleHGCMulticlusterExtra");

L1TEGNtupleHGCMulticlusterExtra::L1TEGNtupleHGCMulticlusterExtra(const edm::ParameterSet& conf)
    : L1TEGNtupleBase(conf),
    emVsPionID_(conf.getParameter<edm::ParameterSet>("emVsPionID")),
    emVsPUID_(conf.getParameter<edm::ParameterSet>("emVsPUID")) {}    
        

void L1TEGNtupleHGCMulticlusterExtra::initialize(TTree& tree,
                                                    const edm::ParameterSet& conf,
                                                    edm::ConsumesCollector&& collector) {
  multiclusters_token_ =
      collector.consumes<l1t::HGCalMulticlusterBxCollection>(conf.getParameter<edm::InputTag>("Multiclusters"));

  if (!emVsPionID_.method().empty()) {
    emVsPionID_.prepareTMVA();
  }
  if (!emVsPUID_.method().empty()) {
    emVsPUID_.prepareTMVA();
  }

  tree.Branch(branch_name_w_prefix("nextra").c_str(), &cl3d_n_,  branch_name_w_prefix("_nextra/I").c_str());
  tree.Branch(branch_name_w_prefix("idcheck").c_str(), &cl3d_id_);
  tree.Branch(branch_name_w_prefix("passEmVsPU").c_str(), &cl3d_passEmVsPU_);
  tree.Branch(branch_name_w_prefix("EmVsPUOut").c_str(), &cl3d_EmVsPUOut_);
  tree.Branch(branch_name_w_prefix("passEmVsPi").c_str(), &cl3d_passEmVsPi_);
  tree.Branch(branch_name_w_prefix("EmVsPiOut").c_str(), &cl3d_EmVsPiOut_);
  
}

void L1TEGNtupleHGCMulticlusterExtra::fill(const edm::Event& e, const edm::EventSetup& es) {
  // retrieve clusters 3D
  edm::Handle<l1t::HGCalMulticlusterBxCollection> multiclusters_h;
  e.getByToken(multiclusters_token_, multiclusters_h);
  const l1t::HGCalMulticlusterBxCollection& multiclusters = *multiclusters_h;

  clear();
  for (auto cl3d_itr = multiclusters.begin(0); cl3d_itr != multiclusters.end(0); cl3d_itr++) {
    cl3d_n_++;
    cl3d_id_.emplace_back(cl3d_itr->detId());

    l1t::PFCluster cluster;
    bool passEmVsPU = false;
    bool passPFEmVsPion = false;
    if (!emVsPUID_.method().empty()) {
      passEmVsPU = emVsPUID_.passID(*cl3d_itr, cluster);
    }
    if (!emVsPionID_.method().empty()) {
      passPFEmVsPion = emVsPionID_.passID(*cl3d_itr, cluster);
    }
    cl3d_passEmVsPU_.push_back(passEmVsPU);
    cl3d_EmVsPUOut_.push_back(cluster.egVsPUMVAOut());
    cl3d_passEmVsPi_.push_back(passPFEmVsPion);
    cl3d_EmVsPiOut_.push_back(cluster.egVsPionMVAOut());

  }
}

void L1TEGNtupleHGCMulticlusterExtra::clear() {
  cl3d_n_ = 0;
  cl3d_id_.clear();
  cl3d_passEmVsPU_.clear();
  cl3d_EmVsPUOut_.clear();
  cl3d_passEmVsPi_.clear();
  cl3d_EmVsPiOut_.clear();

}
