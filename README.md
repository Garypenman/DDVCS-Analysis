# DDVCS-Analysis
Analysis machinery for the study of double deeply virtual compton scattering.

The "Process" macros in relevant directories assume a working local version of the rad analysis framework, and associated detector-rad plugins where necessary. i.e:


https://github.com/dglazier/rad


https://github.com/dglazier/epic-rad


RAD outputs files in trees with the name "rad_tree". Analysis macros will often/always then read in these rad_trees from a local storage area