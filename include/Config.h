//double checked with EpIC hepmc file
const int epic_beam_ele_idx=0; 
const int epic_beam_ion_idx=3; 
const int epic_scat_ele_idx=1; 
const int epic_scat_ion_idx=5; 
const int epic_lep_minus_idx=6; 
const int epic_lep_plus_idx=7;

//double checked with eslight hepmc file
//the lepton pair swap around event by event at hepmc level
//so actually need to figure this out not only for epic-rad analysis
//but also even at hepmc level analysis.
const int eslight_beam_ele_idx=0; 
const int eslight_beam_ion_idx=3; 
const int eslight_scat_ele_idx=2; 
const int eslight_scat_ion_idx=6; 
const int eslight_lep_minus_idx=5; 
const int eslight_lep_plus_idx=4;

//double checked with elspectro hepmc file
const int elspectro_beam_ele_idx=0; 
const int elspectro_beam_ion_idx=1; 
const int elspectro_scat_ele_idx=2; 
const int elspectro_scat_ion_idx=3; 
const int elspectro_lep_minus_idx=4; 
const int elspectro_lep_plus_idx=5;

//epic-rad beams removed from list
//check order with
//events->Scan("MCParticles.PDG","MCParticles.generatorStatus==1")
//importantly its not the "instance" in the tree scan list, but rather the absolute order
//i.e. {1,11},{5,2212},{6,11},{7,-11} -> {0,11},(1,2212},{2,11},{3,-11}
//as seen in the lines below
const int edm4hep_epic_scat_ele_idx=0;
const int edm4hep_epic_scat_ion_idx=1;
const int edm4hep_epic_lep_minus_idx=2;
const int edm4hep_epic_lep_plus_idx=3;

//the lep plus and minus seem to swap around event by event 
//for the elspectro jpsi eicrecon files. Need to figure out eventually
const int edm4hep_eslight_scat_ele_idx=0;
const int edm4hep_eslight_scat_ion_idx=3;
const int edm4hep_eslight_lep_minus_idx=1;
const int edm4hep_eslight_lep_plus_idx=2;

//currenlty dont have elspectr sim+eicrecon
//inferring from hepmc list what
//MCParticles list will look like
//update in future.
const int edm4hep_elspectro_scat_ele_idx=0;
const int edm4hep_elspectro_scat_ion_idx=1;
const int edm4hep_elspectro_lep_minus_idx=2;
const int edm4hep_elspectro_lep_plus_idx=3;


const std::string farm_dir = "/w/work5/home/garyp/eic/Farm/";
const std::string out_dir = "/w/work5/home/garyp/rad_trees/";
