#include "L1Trigger/L1THGCalUtilities/interface/HGCalTriggerNtupleBase.h"

class L1TEGNtupleBase : public HGCalTriggerNtupleBase {
public:
  L1TEGNtupleBase(const edm::ParameterSet& conf)
      : HGCalTriggerNtupleBase(conf),
        branch_name_prefix_(conf.getUntrackedParameter<std::string>("BranchNamePrefix", "")) {}
  ~L1TEGNtupleBase() override{};

  std::string branch_name_w_prefix(const std::string name) const { return branch_name_prefix_ + "_" + name; }

private:
  std::string branch_name_prefix_;
};
