#include "AnalysisManager.h"
#include "HepMCElectro.h"
#include "BasicKinematicsRDF.h"
#include "KinematicsProcElectro.h"
#include "ElectronScatterKinematics.h"
#include "gammaN_2_Spin0Spin0SpinHalf.h"
#include "DefineNames.h"
#include <TBenchmark.h>

#include "ePICFileStreamer.h"
#include "../include/FileProcessing.h"

#include "TopologyRecipes.h"
#include "CorrectionRecipes.h"
#include "HistogramRecipes.h"
#include "SelectionRecipes.h"

const std::string my_out_dir = "/w/work5/home/garyp/combirad_trees/HepMC_TCS_18x275_Phasespace";

void ProcessHEPMCTCSCombi(std::vector<std::string> infiles={"/w/work5/home/garyp/phasespace/dilep_phsp_18_275_M1.5GeV.root"}, 
			  std::string outdir = my_out_dir,
			  const int beam_ele_idx = 0,
			  const int beam_ion_idx = 1,
			  const int Role_ScatEle  = 2, 
			  const int Role_Recoil   = 3,
			  const int Role_DecayEle = 4, 
			  const int Role_DecayPos = 5, 
			  const int lep_PDG = 11) 
{
  using namespace rad;
  using namespace rad::consts::data_type; 

  gBenchmark->Start("df");

  using Reaction = HepMCElectro;
  using Processor = KinematicsProcElectro;
  
  AnalysisManager<Reaction,Processor>  mgr{
    "TCS",
      "hepmc3_tree",
      infiles};
  
  mgr.SetOutputDir(outdir);
  auto& rad_df = mgr.Reaction();
 
  rad_df.SetupMC();
   
  rad_df.SetBeamIonIndex(beam_ion_idx);
  rad_df.SetBeamElectronIndex(beam_ele_idx);
  rad_df.SetParticleIndex(consts::ScatEle(), Role_ScatEle);
  rad_df.SetParticleIndex("ele", Role_DecayEle); 
  rad_df.SetParticleIndex("pos", Role_DecayPos); 
  rad_df.SetParticleIndex("pprime", Role_Recoil); 

  rad_df.MakeCombinations();

  mgr.AddStream(MC(),""); 
  
  // [A] Topology
  auto topology_recipe = TCS_Exclusive_Topology<Processor>;
  
  // [B] REC-SPECIFIC CORRECTIONS
  auto correction_recipe = TCS_Mass_Corrections;
    
  // [C] SELECTION CUTS
  auto selection_recipe = TCS_Selection_Recipe;
  
  // [D] HISTOGRAMS
  auto histogram_recipe =  TCS_Histogram_Recipe;
  
  mgr.ConfigureKinematics(topology_recipe);
  //mgr.ConfigureKinematics(correction_recipe);
  //mgr.ConfigureSelection(selection_recipe);
  mgr.ConfigureHistograms(histogram_recipe);
  
  //rad::rdf::PrintParticles(rad_df, MC());
  //rad::PrintDefinedColumnNames(mgr.Reaction().CurrFrame());
  mgr.Snapshot();

  //Diagnostics helpers
  // rad::rdf::PrintParticles(rad_df, MC());
  rad::PrintDefinedColumnNames(mgr.Reaction().CurrFrame());
  mgr.PrintDiagnostics();
  //PrintDefinedColumnNames will give a list of all columns we can print at this point
  //optional 3rd argument = number of events to print for
  
  
  gBenchmark->Start("analysis");
  mgr.Run();
  gBenchmark->Stop("analysis");
  gBenchmark->Print("analysis");
 }


//---Glob Overload---
void ProcessHEPMCTCSCombi(const std::string& infile_glob,
			  std::string outdir = my_out_dir,
			  const int beam_ele_idx = 0,
			  const int beam_ion_idx = 1,
			  const int Role_ScatEle = 2,
			  const int Role_Recoil = 3,
			  const int Role_DecayEle = 4,
			  const int Role_DecayPos = 5,
			  const int lep_PDG = 11)
{
  auto infiles = GlobToVector(infile_glob);
  
  // Forward all arguments to the real implementation
  ProcessHEPMCTCSCombi(infiles,
		       outdir,
		       beam_ele_idx,
		       beam_ion_idx,
		       Role_ScatEle,
		       Role_Recoil,
		       Role_DecayEle,
		       Role_DecayPos,
		       lep_PDG);
}

