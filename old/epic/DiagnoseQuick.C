void DiagnoseQuick(){

  TString filename="/w/work5/home/garyp/rad_trees/MCReconDetector_ddvcs_ee_18x275_hminus.root";
  ROOT::RDataFrame dfbase("rad_tree",filename);
  using ROOT::VecOps::RVec;
  
  auto df = dfbase.Define("tru_pmag_beamele",[](RVec<double> pmag,int idx){
	return pmag[idx];
      },{"tru_pmag","beam_ele"})
    .Define("rec_pmag_beamele",[](RVec<double> pmag,int idx){
	return pmag[idx];
      },{"rec_pmag","beam_ele"})
    .Define("tru_pmag_beamion",[](RVec<double> pmag,int idx){
	return pmag[idx];
      },{"tru_pmag","beam_ion"})
    .Define("rec_pmag_beamion",[](RVec<double> pmag,int idx){
	return pmag[idx];
      },{"rec_pmag","beam_ion"})
    .Define("tru_pmag_scatele",[](RVec<double> pmag,int idx){
	return pmag[idx];
      },{"tru_pmag","scat_ele"})
    .Define("rec_pmag_scatele",[](RVec<double> pmag,int idx){
	return pmag[idx];
      },{"rec_pmag","scat_ele"})
    .Define("tru_pmag_gprime",[](RVec<double> pmag,int idx){
	return pmag[idx];
      },{"tru_pmag","gprime"})
    .Define("rec_pmag_gprime",[](RVec<double> pmag,int idx){
	return pmag[idx];
      },{"rec_pmag","gprime"})
    .Define("tru_pmag_pprime",[](RVec<double> pmag,int idx){
	return pmag[idx];
      },{"tru_pmag","pprime"})
    .Define("rec_pmag_pprime",[](RVec<double> pmag,int idx){
	return pmag[idx];
      },{"rec_pmag","pprime"});
  
  
  //if the hacked particles dont exist maked dummy intmax to not break
  //compilation
  if(!df.HasColumn("calc_scatele")) {
    df = df.Define("calc_scatele",  [](){ return std::numeric_limits<int>::max(); });
    df = df.Define("tru_calc_scatele",  [](){ return std::numeric_limits<int>::max(); });
    df = df.Define("rec_calc_scatele",  [](){ return std::numeric_limits<int>::max(); });
  }
  
  if(!df.HasColumn("calc_gprime")) {
    df = df.Define("calc_gprime",  [](){ return std::numeric_limits<int>::max(); });
    df = df.Define("tru_calc_gprime",  [](){ return std::numeric_limits<int>::max(); });
    df = df.Define("rec_calc_gprime",  [](){ return std::numeric_limits<int>::max(); });
    
  }
  
  
  if(!df.HasColumn("calc_pprime")) {
    df = df.Define("calc_pprime",  [](){ return std::numeric_limits<int>::max(); });
    df = df.Define("tru_calc_pprime",  [](){ return std::numeric_limits<int>::max(); });
    df = df.Define("rec_calc_pprime",  [](){ return std::numeric_limits<int>::max(); });
    
  }
  
  //get the hacked calculated particles
  //or if we just put dummy intmax then return -1 pmags
  //keep in mind that the idx needs to be espeicifcally
  //tru_ or rec_ in each case since adding particles after map
  //disrupts the ordering of the particle list
  df = df
    .Define("tru_pmag_calc_pprime",[](RVec<double> pmag,int idx){
	if (!(idx==std::numeric_limits<int>::max())) {
	  return pmag[idx];
	}
	return -1.0;
      }, {"tru_pmag", "tru_calc_pprime"})
    .Define("rec_pmag_calc_pprime",[](RVec<double> pmag,int idx){
	if (!(idx==std::numeric_limits<int>::max())) {
	  return pmag[idx];
	}
	return -1.0;
      }, {"rec_pmag", "rec_calc_pprime"})
    .Define("tru_pmag_calc_scatele",[](RVec<double> pmag,int idx){
	if (!(idx==std::numeric_limits<int>::max())) {
	  return pmag[idx];
	}
	return -1.0;
      }, {"tru_pmag", "tru_calc_scatele"})
    .Define("rec_pmag_calc_scatele",[](RVec<double> pmag,int idx){
	if (!(idx==std::numeric_limits<int>::max())) {
	  return pmag[idx];
	}
	return -1.0;
      }, {"rec_pmag", "rec_calc_scatele"})
    .Define("tru_pmag_calc_gprime",[](RVec<double> pmag,int idx){
	if (!(idx==std::numeric_limits<int>::max())) {
	  return pmag[idx];
	}
	return -1.0;
      }, {"tru_pmag", "tru_calc_gprime"})
    .Define("rec_pmag_calc_gprime",[](RVec<double> pmag,int idx){
	if (!(idx==std::numeric_limits<int>::max())) {
	  return pmag[idx];
	}
	return -1.0;
      }, {"rec_pmag", "rec_calc_gprime"});
    
    
  
    
  
  
  
  auto htru_pmag_beamele = df.Histo1D({"htru_pmag_beamele","TRUTH BEAM ELE",100,17,19},"tru_pmag_beamele");
  auto hrec_pmag_beamele = df.Histo1D({"hrec_pmag_beamele","RECO BEAM ELE",100,17,19},"rec_pmag_beamele");

  auto htru_pmag_beamion = df.Histo1D({"htru_pmag_beamion","TRUTH BEAM PRO",100,274,276},"tru_pmag_beamion");
  auto hrec_pmag_beamion = df.Histo1D({"hrec_pmag_beamion","RECO BEAM PRO",100,274,276},"rec_pmag_beamion");

  auto htru_pmag_scatele = df.Histo1D({"htru_pmag_scatele","TRUTH SCAT ELE",100,0,20},"tru_pmag_scatele");
  auto hrec_pmag_scatele = df.Histo1D({"hrec_pmag_scatele","RECO SCAT ELE",100,0,20},"rec_pmag_scatele");
  
  auto htru_pmag_gprime = df.Histo1D({"htru_pmag_gprime","TRUTH GPRIME",100,0,25},"tru_pmag_gprime");
  auto hrec_pmag_gprime = df.Histo1D({"hrec_pmag_gprime","RECO GPRIME",100,0,25},"rec_pmag_gprime");

  auto htru_pmag_pprime = df.Histo1D({"htru_pmag_pprime","TRUTH PRO PRIME",100,0,300},"tru_pmag_pprime");
  auto hrec_pmag_pprime = df.Histo1D({"hrec_pmag_pprime","RECO PRO PRIME",100,0,300},"rec_pmag_pprime");

  auto htru_pmag_calc_pprime = df.Histo1D({"htru_pmag_calc_pprime","TRUTH CALC PPRIME",100,0,300},"tru_pmag_calc_pprime");
  auto hrec_pmag_calc_pprime = df.Histo1D({"hrec_pmag_calc_pprime","RECO CALC PPRIME",100,0,300},"rec_pmag_calc_pprime");
  
  auto htru_pmag_calc_gprime = df.Histo1D({"htru_pmag_calc_gprime","TRUTH CALC GPRIME",100,0,25},"tru_pmag_calc_gprime");
  auto hrec_pmag_calc_gprime = df.Histo1D({"hrec_pmag_calc_gprime","RECO CALC GPRIME",100,0,25},"rec_pmag_calc_gprime");
  
  auto htru_pmag_calc_scatele = df.Histo1D({"htru_pmag_calc_scatele","TRUTH CALC SCAT ELE",100,0,20},"tru_pmag_calc_scatele");
  auto hrec_pmag_calc_scatele = df.Histo1D({"hrec_pmag_calc_scatele","RECO CALC SCAT ELE",100,0,20},"rec_pmag_calc_scatele");

  
  TCanvas *c00 = new TCanvas("c00","Particle Momenta",1500,1200);
  c00->Divide(5,4);
  c00->cd(1);
  htru_pmag_beamele->DrawCopy();
  c00->cd(2);
  htru_pmag_beamion->DrawCopy();
  c00->cd(3);
  htru_pmag_scatele->DrawCopy();
  c00->cd(4);
  htru_pmag_gprime->DrawCopy();
  c00->cd(5);
  htru_pmag_pprime->DrawCopy();
  
  //TCanvas *c01 = new TCanvas("c01","Reco Particles");
  //c01->Divide(3,2);
  c00->cd(6);
  hrec_pmag_beamele->DrawCopy();
  c00->cd(7);
  hrec_pmag_beamion->DrawCopy();
  c00->cd(8);
  hrec_pmag_scatele->DrawCopy();
  c00->cd(9);
  hrec_pmag_gprime->DrawCopy();
  c00->cd(10);
  hrec_pmag_pprime->DrawCopy();
  
  c00->cd(13);
  htru_pmag_calc_scatele->DrawCopy();
  c00->cd(14);
  htru_pmag_calc_gprime->DrawCopy();
  c00->cd(15);
  htru_pmag_calc_pprime->DrawCopy();
  
  c00->cd(18);
  hrec_pmag_calc_scatele->DrawCopy();
  c00->cd(19);
  hrec_pmag_calc_gprime->DrawCopy();
  c00->cd(20);
  hrec_pmag_calc_pprime->DrawCopy();
  
}
