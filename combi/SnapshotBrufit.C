#include "../include/Config.h"

void SnapshotBrufit(std::string infile = "/w/work5/home/garyp/combirad_trees/HepMC_TCS_18x275_Phasespace/TCS_mc_Tree.root", std::string outfile = "/w/work5/home/garyp/combirad_trees/HepMC_TCS_18x275_Phasespace/TCS_mc_Tree_brufit.root"){
  ROOT::RDataFrame df("tree",infile);
  df.Snapshot("tree",outfile,brufit_cols);
}
  
