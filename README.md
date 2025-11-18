# DDVCS-Analysis
Analysis machinery for the study of double deeply virtual compton scattering.


The "Process" macros in relevant directories assume a working local version of the rad analysis framework, and associated detector-rad plugins where necessary. i.e:


https://github.com/dglazier/rad


https://github.com/dglazier/epic-rad


ProcessHepMC.C: MC data from HepMC file.\
ProcessMCMatched.C: MC matched rec data and truth branches. \
ProcessMCMatchedDetector.C: MC matched rec data and truth branches, with detector associations. \
ProcessMCRecon.C: Rec data (no mc matching) and truth branches. \
ProcessMCReconDetector.C: Rec data (no mc matching) and truth branches, with detector associations. \
ProcessRecon.C: Rec data only. Designed for analysis of "real" files i.e. from experiment. \
(TBD FUTURE) ProcessReconDetector.C: Rec data only with detector associations. Designed for analysis of "real files" i.e. from experiment.


RAD outputs files in trees with the name "rad_tree". Analysis macros will often/always then read in these rad_trees from a local storage area