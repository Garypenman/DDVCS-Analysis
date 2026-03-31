//#pragma once
//#include "Model.C"

//void FitThetaPhi(){
{
  
  /****************************************/
  /***************Filenames****************/    
  /****************************************/
  //TString datafile  = "sim_toys2/Toy0.root";
  //TString datafile  = "toys_test/Toy0.root";
  //TString datafile  = "/w/work5/home/garyp/rad_trees/HepMC_ddvcs_ee_18x275_mixed.root";
  TString datafile  = "/w/work5/home/garyp/combirad_trees/HepMC_TCS_18x275_hmixed/TCS_mc_Tree.root";
  
  //TString sigfile  = "eepData.root";
  //TString sigfile = "/w/work5/home/garyp/rad_trees/HepMC_ddvcs_ee_18x275_mixed_flat.root";
  TString sigfile = "/w/work5/home/garyp/combirad_trees/HepMC_TCS_18x275_Phasespace/TCS_mc_Tree_brufit.root";
  TString outdir = "fit_out/";
  
  /****************************************/
  /************Create FitManager***********/    
  /****************************************/
  FitManager fm;
  fm.SetUp().SetOutDir(outdir);
  // fm.SetUp().SetIDBranchName("UID");
  
  /****************************************/
  /***********Apply Dilepton model**********/    
  /****************************************/
  Model(fm); 

  /**************************************************/  
  /********************Make bins*********************/ 
  /**************************************************/
  
  // Double_t tbinLimits[] = {0,0.01,0.1,1.}; // tbins
  // fm.Bins().LoadBinVar("mc_t_bot",3,tbinLimits);			  

  /**************************************************/
  /****************Load data and MC******************/
  /**************************************************/

  fm.LoadData("tree",datafile);
  //fm.LoadData("tree",sigfile);
   //"Dilepton" is given in Model.C as name of the RooComponentsPDF
  fm.LoadSimulated("tree",datafile,"Dilepton");
  //fm.LoadSimulated("tree",sigfile,"Dilepton");

  /**************************************************/
  /***********Choose minimiser and run***************/ 
  /**************************************************/
  //number of CPUs to split likelihood calc.
  //fm.SetUp().AddFitOption(RooFit::NumCPU(4)); 
  //std::vector<Int_t> Niters,Int_t Nburn, Float_t norm,float target,float accmin,float accmax
  auto mcmc=new BruMcmcCovariance({5000,20000,10000},100,1,0.23,0.16,0.3);
  fm.SetMinimiser(mcmc);
  Here::Go(&fm);
  //Multi::Go(&fm,10);
  
}
