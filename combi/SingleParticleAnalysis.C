#include "../include/SingleParticleAnalysis.h"

#include "../include/DrawHelperFuncs.h"
#include "../include/DrawEpicInfo.h"

std::string file1 = "/w/work6/home/gp140f/tcs_bg_merge/nobg_rad/";
std::string outdir1 = "./results/NoBGMerge/9x130/hplus";

std::string file2 = "/w/work6/home/gp140f/combirad_trees/epic/9x130_ddvcs_ee_hplus/";
std::string outdir2 = "./results/july/9x130/hplus/";

void SingleParticleAnalysis(std::string infiledir = file2,
			    std::string outfiledir = outdir2,
			    int ebeam = 9, int pbeam = 130, std::string helicity="hplus", std::string fcampaign="26.07.1"){

gErrorIgnoreLevel = kError;
  set_ePIC_style();

  // ----------------------------------------------------------
  // Metadata
  // ----------------------------------------------------------
  int Ecom = sqrt(4*ebeam*pbeam); //sqrt(s)
  string Eset = to_string(ebeam) + "x" + to_string(pbeam);
  double Ngen = 1e5;
  double xsec = 5.0; //nb
  double dxsec = 0.1; //nb
  double lumi_fb = 1;
  double lumi_nb = lumi_fb * 1e6; //fb-1 to nb-1
  // double scale = xsec*Lumi / Ngen;
  // double dscale = dxsec*Lumi;

  
  // ----------------------------------------------------------
  // Files
  // ----------------------------------------------------------
  std::string tru_filename = infiledir + "TCS_tru_Tree.root";
  std::string rec_filename = infiledir + "TCS_all_rec_Tree.root";
  gSystem->Exec(("mkdir -p "+outfiledir).c_str());
  std::string outfilename = outfiledir+"/TCS_"+Eset+"_particles.root";
  TFile fout(outfilename.c_str(), "RECREATE");
  
  
  std::vector<ParticleConfig> particles;

  //these should go in something like
  //SetupTCSParticles.h or .C
  //scattered electron
  ParticleConfig scat_ele{
      "scat_ele", "Scattered Electron", "e'",
      11, //pdg
      50, 100, //nbins acc ,res
      0.0, 10.0, -0.1, 0.1, //pmag
      -1.0, 1.0, -1.0, 1.0, //px
      -1.0, 1.0, -1.0, 1.0, //py
      -10.0, 0.0, -0.1, 0.1, //pz
      3.12, 3.14, -6, 6, //theta
      -3.14, 3.14, -3.14, 3.14, //phi
      -12.0, -6.0, -1, 1//eta
  };
  
  FiducialCuts scatele_cuts;
  scatele_cuts.use_p = true;
  scatele_cuts.p_min = 0.0;
  scatele_cuts.p_max = 9.0;
  scat_ele.fid = scatele_cuts;

  
  //decay electron
  ParticleConfig ele{
      "ele", "Decay Electron", "e^{-}",
      11,
      50, 100,
      0.0, 80.0, -0.1, 0.1, //pmag
      -2.0, 2.0, -0.1, 0.1, //px
      -2.0, 2.0, -0.1, 0.1, //py
      -30.0, 30.0, -0.1, 0.1, //pz
      0, 3.14, -10, 10,
      -3.14, 3.14, -0.2, 0.2,
      -10.0, 10.0, -0.05, 0.05
  };

  FiducialCuts ele_cuts;
  ele_cuts.use_p = true;
  ele_cuts.p_min = 0.0;
  ele_cuts.p_max = 100.0;
  ele.fid = ele_cuts;

  
  //virtual photon
  ParticleConfig gprime{
      "gprime", "Virtual Photon", "#gamma^{*}",
      22,
      50, 100,
      0.0, 90.0, -0.1, 0.1, //pmag
      -2.0, 2.0, -0.1, 0.1, //px
      -2.0, 2.0, -0.1, 0.1, //py
      -20.0, 80.0, -0.1, 0.1, //pz
      0, 3.14, -10, 10,
      -3.14, 3.14, -0.2, 0.2,
      -10.0, 10.0, -0.05, 0.05
  };

  FiducialCuts gprime_cuts;
  gprime_cuts.use_p = true;
  gprime_cuts.p_min = 0.0;
  gprime_cuts.p_max = 90.0;
  gprime.fid = gprime_cuts;

  
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
      0.0, 150.0, -0.1, 0.1, //pmag
      -5.0, 5.0, -5.0, 5.0, //px
      -5.0, 5.0, -5.0, 5.0, //py
      50.0, 150.0, -0.1, 0.1, //pz
      0.0, 0.01, -5, 5,
      -3.14, 3.14, -3.14, 3.14,
      4.00, 12.0, -1, 1
    };

  FiducialCuts pprime_cuts;
  pprime_cuts.use_p = true;
  pprime_cuts.p_min = 0.0;
  pprime_cuts.p_max = 130.0;
  pprime.fid = pprime_cuts;
  
  particles.push_back(scat_ele);
  particles.push_back(ele);
  particles.push_back(pos);
  particles.push_back(gprime);
  particles.push_back(pprime);

  for (const auto& pc : particles) {
    MakeParticlePlots(pc, tru_filename, infiledir+pc.rec_filename, fout);
  }

  fout.Close();
  
  for (const auto& pc : particles) {
    DrawParticlePlots(outfilename, pc);
  }

  std::string cmd = "pdfunite ";
  
  for (const auto& pc : particles)
    cmd += pc.name + "_summary.pdf ";
  
  cmd += "SingleParticleResults.pdf";
  
  gSystem->Exec(cmd.c_str());
  gSystem->Exec(("mv *.png *.pdf "+outfiledir).c_str());
}
