#include "HepMCElectro.h"
#include "ParticleCreator.h"
#include "ParticleGenerator.h"
#include "ParticleModifier.h"
#include "Indicing.h"
#include "Histogrammer.h"
#include "BasicKinematicsRDF.h"
#include "ReactionKinematicsRDF.h"
#include "ElectronScatterKinematicsRDF.h"
#include "gammaN_2_Spin0Spin0SpinHalfRDF.h"

#include "../include/ApplyKinematics.h"
#include "../include/MakeRADHistos.h"
#include "../include/DrawRADHistos.h"
#include "../include/FileProcessing.h"

void ProcessHepMC(const std::string infile="/w/work5/home/garyp/eic/Farm/data/EpIC_DDVCS_ee_18x275/rootfiles/18x275_ddvcs_edecay_hplus.root", 
		  const std::string outfile="tempout.root", 
		  const int beam_ele_idx=0, 
		  const int beam_ion_idx=3, 
		  const int scat_ele_idx=1, 
		  const int scat_ion_idx=5, 
		  const int lep_minus_idx=6, 
		  const int lep_plus_idx=7)
{
  using namespace rad::names::data_type; //for MC()
  
  gBenchmark->Start("Total");
  
  //ROOT::EnableImplicitMT(8);
  rad::config::HepMCElectro hepmc{"hepmc3_tree", infile };
  
  hepmc.AliasMomentumComponents();
  
  //Assign particles names and indices
  //indicing comes from ordering in hepmc file
  hepmc.setBeamIonIndex(beam_ion_idx);
  hepmc.setBeamElectronIndex(beam_ele_idx);
  hepmc.setScatElectronIndex(scat_ele_idx);
  
  hepmc.setParticleIndex("pprime",scat_ion_idx);
  hepmc.setBaryonParticles({"pprime"});
  
  //if using existing lepton pair
  //create gprime/jpsi from their sums
  rad::config::ParticleCreator particles{hepmc};
  hepmc.setParticleIndex("ele",lep_minus_idx);
  hepmc.setParticleIndex("pos",lep_plus_idx);
  particles.Sum("gprime",{"ele","pos"});
  
  //use ParticleGenerator to decay gprime to 2 M_ele particles ele,pos
  //now migrated to GenHeli.C
  //hepmc.setParticleIndex("gprime",4);
  // rad::generator::ParticleGenerator gen{hepmc};
  // gen.GenerateTwoBody({"ele0","pos0"},
  // 		      {rad::constant::M_ele(),rad::constant::M_ele()},
  // 		      "gprime");
  
  hepmc.setMesonParticles({"ele","pos"});
  //hepmc.setMesonParticles({"ele0","pos0"});

  //must call this after all particles are configured
  hepmc.makeParticleMap();
  //rad::rdf::PrintParticles(hepmc,MC());
  
  hepmc.Define("UID","rdfentry_");

  ApplyKinematics(hepmc);
  
  //Define histograms
  rad::histo::Histogrammer histo{"set1",hepmc};
  histo.Init({MC()});
  MakeRADHistos(histo);
  
  hepmc.BookLazySnapshot(outfile);
  gBenchmark->Start("processing");
  
  DrawRADHistos(histo);
  MakeOutputPDF(outfile);
  
  gBenchmark->Stop("processing");
  gBenchmark->Print("processing");
  
  //save all histograms to file
  //histo.File("HepMCDDVCS_hists.root");
  
  gBenchmark->Stop("Total");
  gBenchmark->Print("Total");
  
}
