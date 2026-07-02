#pragma once
#include "ModelSimple.C"
#include "ModelBH.C"
#include "ModelFull.C"

void FitThetaPhi(//Data File
		 TString datafile  = "/w/work6/home/gp140f/combirad_trees/10x100_ddvcs_ee_bhonly_hplus/TCS_mc_Tree.root",
		 //Phasespace File
		 TString sigfile = "/w/work6/home/gp140f/phasespace/trees/dilep_phsp_10_100_b5_10M//TCS_mc_Tree_brufit.root",
		 //0=Simple, 1=BH, 2=Full
		 Int_t modelID=1)
{
  
  
  /****************************************/
  /***************Filenames****************/    
  /****************************************/
  
  
  //10x100 BH only is macro default
  //10x100 full epic
  //TString datafile  = "/w/work6/home/gp140f/combirad_trees/HepMC_TCS_10x100_hplus/TCS_mc_Tree.root";
  
  //phasespace file
  //sigfile = "../hepmc/TCSPhaseSpace.root";
  /****************************************/
  /************Create FitManager***********/    
  /****************************************/
  FitManager fm;
  std::string outdir;
  // fm.SetUp().SetIDBranchName("UID");
  
  /****************************************/
  /***********Apply Dilepton model**********/    
  /****************************************/
  if(modelID==0){ // Simple TCS toy model
    ModelSimple(fm);
    outdir = "fit_simple";
  }else if(modelID==1){ // BH Only, alpha mixing for F1,F2
    ModelBH(fm);
    outdir = "fit_bh";
  }else if(modelID==2){ // Full BH+TCS+INT - TBD
    ModelFull(fm);
    outdir = "fit_full";
  }else{ //add more as needed
    std::cout << "No matching fit model ID" << endl;
    exit(1);
  }

  fm.SetUp().SetOutDir(outdir);
  
  /**************************************************/  
  /********************Make bins*********************/ 
  /**************************************************/
  
  // Double_t tbinLimits[] = {0,0.01,0.1,1.}; // tbins
  // fm.Bins().LoadBinVar("mc_t_bot",3,tbinLimits);			  

  /**************************************************/
  /****************Load data and MC******************/
  /**************************************************/

  fm.LoadData("tree",datafile);
  //"Dilepton" is given in Model.C as name of the RooComponentsPDF
  fm.LoadSimulated("tree",sigfile,"Dilepton");

  /**************************************************/
  /***********Choose minimiser and run***************/ 
  /**************************************************/
  //number of CPUs to split likelihood calc.
  //fm.SetUp().AddFitOption(RooFit::NumCPU(4)); 
  //std::vector<Int_t> Niters,Int_t Nburn, Float_t norm,float target,float accmin,float accmax
  auto mcmc=new BruMcmcCovariance({5000,20000,10000},100,1,0.23,0.16,0.3);
  //auto mcmc=new BruMcmcCovariance({100,100,100},100,1,0.23,0.0,1.0);
  fm.SetMinimiser(mcmc);
  Here::Go(&fm);
  //Multi::Go(&fm,10);
  
}
