#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "L1Trigger/L1THGCalUtilities/interface/HGCalTriggerNtupleBase.h"
#include "Phase2EGTriggerAnalysis/NtupleProducer/interface/L1TEGNtupleBase.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "SimGeneral/HepPDTRecord/interface/PDTRecord.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"

class L1TEGNtupleManager : public edm::EDAnalyzer {
public:
  typedef std::unique_ptr<L1TEGNtupleBase> l1teg_ntuple_ptr;
  typedef std::unique_ptr<HGCalTriggerNtupleBase> hgc_ntuple_ptr;

public:
  explicit L1TEGNtupleManager(const edm::ParameterSet& conf);
  ~L1TEGNtupleManager() override{};
  void beginRun(const edm::Run&, const edm::EventSetup&) override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;

private:
  edm::Service<TFileService> file_service_;
  std::vector<l1teg_ntuple_ptr> l1teg_ntuples_;
  std::vector<hgc_ntuple_ptr> hgc_ntuples_;
  
  TTree* tree_;

  HGCalTriggerNtupleEventSetup hgc_ntuple_es_;
  L1TEGNtupleEventSetup l1teg_ntuple_es_;
  edm::ESGetToken<HepPDT::ParticleDataTable, PDTRecord> pdtToken_;
  edm::ESGetToken<MagneticField, IdealMagneticFieldRecord> magfieldToken_;
  edm::ESGetToken<HGCalTriggerGeometryBase, CaloGeometryRecord> hgcTriggerGeomToken_;
  edm::ESGetToken<TrackerGeometry, TrackerDigiGeometryRecord> tkGeomToken_;

};

DEFINE_FWK_MODULE(L1TEGNtupleManager);

L1TEGNtupleManager::L1TEGNtupleManager(const edm::ParameterSet& conf)
    : pdtToken_(esConsumes<HepPDT::ParticleDataTable, PDTRecord, edm::Transition::BeginRun>()),
      magfieldToken_(esConsumes<MagneticField, IdealMagneticFieldRecord, edm::Transition::BeginRun>()),
      hgcTriggerGeomToken_(esConsumes<HGCalTriggerGeometryBase, CaloGeometryRecord, edm::Transition::BeginRun>()),
      tkGeomToken_(esConsumes<TrackerGeometry, TrackerDigiGeometryRecord, edm::Transition::BeginRun>(edm::ESInputTag("", "idealForDigi"))) {
  tree_ = file_service_->make<TTree>("L1TEGTriggerNtuple", "L1TEGTriggerNtuple");
  
  const std::vector<edm::ParameterSet>& hgc_ntuple_cfgs = conf.getParameterSetVector("hgc_Ntuples");
  const std::vector<edm::ParameterSet>& l1teg_ntuple_cfgs = conf.getParameterSetVector("l1teg_Ntuples");
  
  for (const auto& ntuple_cfg : hgc_ntuple_cfgs) {
    const std::string& ntuple_name = ntuple_cfg.getParameter<std::string>("NtupleName");
    hgc_ntuples_.emplace_back(HGCalTriggerNtupleFactory::get()->create(ntuple_name, ntuple_cfg));
    hgc_ntuples_.back()->initialize(*tree_, ntuple_cfg, consumesCollector());
  }
  for (const auto& ntuple_cfg : l1teg_ntuple_cfgs) {
    const std::string& ntuple_name = ntuple_cfg.getParameter<std::string>("NtupleName");
    l1teg_ntuples_.emplace_back(L1TEGNtupleFactory::get()->create(ntuple_name, ntuple_cfg));
    l1teg_ntuples_.back()->initialize(*tree_, ntuple_cfg, consumesCollector());
  }
}

void L1TEGNtupleManager::beginRun(const edm::Run& run, const edm::EventSetup& es) {
  hgc_ntuple_es_.pdt = es.getHandle(pdtToken_);
  hgc_ntuple_es_.magfield = es.getHandle(magfieldToken_);
  hgc_ntuple_es_.geometry = es.getHandle(hgcTriggerGeomToken_);
  l1teg_ntuple_es_.magfield = es.getHandle(magfieldToken_);
  l1teg_ntuple_es_.tkgeom = es.getHandle(tkGeomToken_);
  l1teg_ntuple_es_.hgcGeom = es.getHandle(hgcTriggerGeomToken_);
}

void L1TEGNtupleManager::analyze(const edm::Event& e, const edm::EventSetup& es) {
  for (auto& hgc_ntuple : hgc_ntuples_) {
    if (hgc_ntuple->accessEventSetup()) {
      hgc_ntuple->fill(e, es);
    } else {
      hgc_ntuple->fill(e, hgc_ntuple_es_);
    }
  }
  for (auto& l1teg_ntuple : l1teg_ntuples_) {
    if (l1teg_ntuple->accessEventSetup()) {
      l1teg_ntuple->fill(e, es);
    } else {
      l1teg_ntuple->fill(e, l1teg_ntuple_es_);
    }
  }

  tree_->Fill();
}
