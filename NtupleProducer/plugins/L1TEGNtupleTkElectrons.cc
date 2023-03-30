#include "Phase2EGTriggerAnalysis/NtupleProducer/interface/L1TEGNtupleBase.h"

#include "DataFormats/L1TCorrelator/interface/TkElectron.h"
#include "DataFormats/L1TCorrelator/interface/TkElectronFwd.h"

class L1TEGNtupleTkElectrons : public L1TEGNtupleBase {
public:
  L1TEGNtupleTkElectrons(const edm::ParameterSet& conf);
  ~L1TEGNtupleTkElectrons() override{};
  void initialize(TTree&, const edm::ParameterSet&, edm::ConsumesCollector&&) final;
  void fill(const edm::Event& e, const edm::EventSetup& es) final;

private:
  void clear() final;

  edm::EDGetToken tkEle_token_;

  int tkEle_n_;
  std::vector<float> tkEle_pt_;
  std::vector<float> tkEle_energy_;
  std::vector<float> tkEle_eta_;
  std::vector<float> tkEle_phi_;
  std::vector<float> tkEle_hwQual_;
  std::vector<float> tkEle_tkIso_;
  std::vector<float> tkEle_pfIso_;
  std::vector<float> tkEle_puppiIso_;
  std::vector<float> tkEle_tkChi2_;
  std::vector<float> tkEle_tkPt_;
  std::vector<float> tkEle_tkZ0_;
  std::vector<float> tkEle_compBDTScore_;

};

DEFINE_EDM_PLUGIN(L1TEGNtupleFactory, L1TEGNtupleTkElectrons, "L1TEGNtupleTkElectrons");

L1TEGNtupleTkElectrons::L1TEGNtupleTkElectrons(const edm::ParameterSet& conf)
    : L1TEGNtupleBase(conf) {}

void L1TEGNtupleTkElectrons::initialize(TTree& tree,
                                            const edm::ParameterSet& conf,
                                            edm::ConsumesCollector&& collector) {
  tkEle_token_ = collector.consumes<l1t::TkElectronCollection>(conf.getParameter<edm::InputTag>("TkElectrons"));

  tree.Branch(branch_name_w_prefix("n").c_str(), &tkEle_n_, branch_name_w_prefix("n/I").c_str());
  tree.Branch(branch_name_w_prefix("pt").c_str(), &tkEle_pt_);
  tree.Branch(branch_name_w_prefix("energy").c_str(), &tkEle_energy_);
  tree.Branch(branch_name_w_prefix("eta").c_str(), &tkEle_eta_);
  tree.Branch(branch_name_w_prefix("phi").c_str(), &tkEle_phi_);
  tree.Branch(branch_name_w_prefix("hwQual").c_str(), &tkEle_hwQual_);
  tree.Branch(branch_name_w_prefix("tkIso").c_str(), &tkEle_tkIso_);
  tree.Branch(branch_name_w_prefix("pfIso").c_str(), &tkEle_pfIso_);
  tree.Branch(branch_name_w_prefix("puppiIso").c_str(), &tkEle_puppiIso_);
  tree.Branch(branch_name_w_prefix("tkChi2").c_str(), &tkEle_tkChi2_);
  tree.Branch(branch_name_w_prefix("tkPt").c_str(), &tkEle_tkPt_);
  tree.Branch(branch_name_w_prefix("tkZ0").c_str(), &tkEle_tkZ0_);
  tree.Branch(branch_name_w_prefix("compBDTScore").c_str(), &tkEle_compBDTScore_);

}

void L1TEGNtupleTkElectrons::fill(const edm::Event& e, const edm::EventSetup& es) {
  // retrieve towers
  edm::Handle<l1t::TkElectronCollection> tkEle_h;
  e.getByToken(tkEle_token_, tkEle_h);
  const l1t::TkElectronCollection& tkEle_collection = *tkEle_h;

  // triggerTools_.eventSetup(es);
  clear();
  // std::cout << branch_name_w_prefix("") << " # ele: " << tkEle_collection.size() << std::endl;
  for (auto tkele_itr : tkEle_collection) {
    tkEle_n_++;
    tkEle_pt_.emplace_back(tkele_itr.pt());
    tkEle_energy_.emplace_back(tkele_itr.energy());
    tkEle_eta_.emplace_back(tkele_itr.eta());
    tkEle_phi_.emplace_back(tkele_itr.phi());
    // tkEle_hwQual_.emplace_back(tkele_itr.EGRef()->hwQual());
    tkEle_hwQual_.emplace_back(tkele_itr.hwQual());
    tkEle_tkIso_.emplace_back(tkele_itr.trkIsol());
    tkEle_pfIso_.emplace_back(tkele_itr.pfIsol());
    tkEle_puppiIso_.emplace_back(tkele_itr.puppiIsol());
    tkEle_tkChi2_.emplace_back(tkele_itr.trkPtr()->chi2());
    tkEle_tkPt_.emplace_back(tkele_itr.trkPtr()->momentum().perp());
    tkEle_tkZ0_.emplace_back(tkele_itr.trkPtr()->POCA().z());
    tkEle_compBDTScore_.emplace_back(tkele_itr.compositeBdtScore());

  }
}

void L1TEGNtupleTkElectrons::clear() {
  tkEle_n_ = 0;
  tkEle_pt_.clear();
  tkEle_energy_.clear();
  tkEle_eta_.clear();
  tkEle_phi_.clear();
  tkEle_hwQual_.clear();
  tkEle_tkIso_.clear();
  tkEle_pfIso_.clear();
  tkEle_puppiIso_.clear();
  tkEle_tkChi2_.clear();
  tkEle_tkPt_.clear();
  tkEle_tkZ0_.clear();
  tkEle_compBDTScore_.clear();
}
