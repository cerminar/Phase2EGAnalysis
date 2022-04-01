#ifndef __Phase2EGTriggerAnalysis_NtupleProducer_L1TEGNtupleBase_h__
#define __Phase2EGTriggerAnalysis_NtupleProducer_L1TEGNtupleBase_h__

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"

#include "TTree.h"

namespace HepPDT {
  class ParticleDataTable;
}
class MagneticField;
class HGCalTriggerGeometryBase;

struct L1TEGNtupleEventSetup {
  edm::ESHandle<MagneticField> magfield;
  edm::ESHandle<TrackerGeometry> tkgeom;
  edm::ESHandle<HGCalTriggerGeometryBase> hgcGeom;
  edm::ESHandle<HepPDT::ParticleDataTable> pdt;
};

class L1TEGNtupleBase {
public:
  L1TEGNtupleBase(const edm::ParameterSet& conf) 
        : name_(conf.getParameter<std::string>("NtupleName")),
          branch_name_prefix_(conf.getUntrackedParameter<std::string>("BranchNamePrefix", "")) {};

  virtual ~L1TEGNtupleBase(){};
  
  const std::string& name() const { return name_; }
  
  virtual void initialize(TTree&, const edm::ParameterSet&, edm::ConsumesCollector&&) = 0;
  
  virtual void fill(const edm::Event&, const L1TEGNtupleEventSetup&) {
    edm::LogWarning("NotImplemented") << "Calling ntuplizer fill(edm::Event, L1TEGNtupleEventSetup), but it is "
                                         "not implemented in the concrete class '"
                                      << name() << "'. "
                                      << "You might want to set 'accessEventSetup_' to true in order to call "
                                         "fill(edm::Event, edm::EventSetup) instead.";
  }
  
  // Kept for backward compatibility: used in L1Trigger/L1CaloTrigger/test
  virtual void fill(const edm::Event&, const edm::EventSetup&) {
    edm::LogWarning("NotImplemented")
        << "Calling ntuplizer fill(edm::Event, edm::EventSetup), but it is not implemented in the concrete class '"
        << name() << "'. "
        << "You might want to set 'accessEventSetup_' to false in order to call fill(edm::Event, "
           "L1TEGNtupleEventSetup) instead.";
  }
  bool accessEventSetup() const { return accessEventSetup_; }

  std::string branch_name_w_prefix(const std::string name) const { return branch_name_prefix_ + "_" + name; }

protected:
  virtual void clear() = 0;
  bool accessEventSetup_ = true;
  const std::string name_;

private:
  std::string branch_name_prefix_;

};

#include "FWCore/PluginManager/interface/PluginFactory.h"
typedef edmplugin::PluginFactory<L1TEGNtupleBase*(const edm::ParameterSet&)> L1TEGNtupleFactory;

#endif
