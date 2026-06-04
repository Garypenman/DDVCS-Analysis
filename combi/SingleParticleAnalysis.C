#include "../include/SingleParticleAnalysis.h"

void SingleParticleAnalysis(std::string infiledir = "/w/work6/home/gp140f/combirad_trees/test_run/18x275_ddvcs_ee_hplus/", std::string beam_setting = "18x275", std::string campaign = "test", std::string helicity = "hplus"){

  // ----------------------------------------------------------
  // Output
  // ----------------------------------------------------------
  std::string resultdir = "./results/"+campaign+"/"+beam_setting+"/"+helicity+"/"; 
  gSystem->Exec(("mkdir -p "+resultdir).c_str());
  std::string outfilename = "TCS_"+beam_setting+"_particles.root";
  TFile fout(outfilename.c_str(), "RECREATE");
  
  std::string tru_filename = infiledir + "TCS_tru_Tree.root";
  std::string rec_filename = infiledir + "TCS_all_rec_Tree.root";

  std::vector<ParticleConfig> particles;

  //these should go in something like
  //SetupTCSParticles.h or .C
  //scattered electron
  ParticleConfig scat_ele{
      "scat_ele", "Scattered Electron", "e'",
      11,//pdg
      50, 100,//nbins acc ,res
      0.0, 20.0, -0.1, 0.1,//pmag
      3.12, 3.14, -6, 6,//theta
      -3.14, 3.14, -3.14, 3.14,//phi
      -12.0, -6.0, -1, 1//eta
  };
  
  FiducialCuts scatele_cuts;
  scatele_cuts.use_p = true;
  scatele_cuts.p_min = 6.0;
  scatele_cuts.p_max = 12.0;
  scat_ele.fid = scatele_cuts;
  
  DerivedObservable Q2_obs{
    "Q2", "Q2", "Q^{2} [GeV^{2}]",
    0, 10,
    -0.001, 0.001
  };
  
  //decay electron
  ParticleConfig ele{
      "ele", "Decay Electron", "e^{-}",
      11,
      50, 100,
      0.0, 200.0, -0.1, 0.1,
      0, 3.14, -10, 10,
      -3.14, 3.14, -0.2, 0.2,
      -10.0, 10.0, -0.05, 0.05
  };
  
  //decay positron
  ParticleConfig pos = ele;
  pos.name = "pos"; pos.title = "Decay Positron";
  pos.label = "e^{+}"; pos.pdg = -11;
  pos.UpdateDerived();
  
  //recoil proton
  ParticleConfig pprime{
      "pprime", "Recoil Proton", "p'",
      2212,
      50, 100,
      150, 275.0, -0.1, 0.1,
      0.0, 0.01, -5, 5,
      -3.14, 3.14, -3.14, 3.14,
      4.00, 12.0, -1, 1
    };

  particles.push_back(scat_ele);
  particles.push_back(ele);
  particles.push_back(pos);
  particles.push_back(pprime);

  for (const auto& pc : particles) {
    MakeParticlePlots(pc, tru_filename, infiledir+pc.rec_filename, fout);
  }

  fout.Close();
  
  for (const auto& pc : particles) {
    DrawParticlePlots(outfilename, pc);
  }

  gSystem->Exec("pdfunite *_acceptances.pdf *_resolutions.pdf SingleParticleResults.pdf");
  gSystem->Exec(("mv *.png *.pdf *.root "+resultdir).c_str());
}
