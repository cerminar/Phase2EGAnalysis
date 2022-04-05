# Phase2EGAnalysis

## Ntuple production

An example config file in 
[Phase2EGTriggerAnalysis/NtupleProducer/test/step1_L1TCorr_EGNtuples.py](NtupleProducer/test/step1_L1TCorr_EGNtuples.py)

runs the ntuple production and the L1T correlator (Layer-1 and Layer-2 e/g) emulator (and corresponding inputs).

A few flags are available in the file to control re-processing of input objects:

* `doHgcTPS = True` re-runs HGC TPs;
* `doAllL1Emu = True` runs all the L1 tasks on which the Correlator emulator depends on;
* `doTrackTrigger = True` runs the track trigger TPs;
* `doCaloEG = True` runs the L1 Calo inputs for the barrel.
