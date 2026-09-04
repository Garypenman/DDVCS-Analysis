#pragma once
#include "ModelSimple.C"
#include "ModelBH.C"
#include "ModelFull.C"

#include "SetupWeights.C"

#include "../include/FileProcessing.h"
#include "../combi/SnapshotBrufit.C"

void FitThetaPhi(//Data Directory
		 std::string datadir  = "/w/work6/home/gp140f/combirad_trees/hepmc/10x100_ddvcs_ee_bhonly_hplus/",
		 //Phasespace Directory
		 std::string sigdir = "/w/work6/home/gp140f/phasespace/trees/dilep_phsp_10_100_v2/",
		 //0=Simple, 1=BH, 2=Full
		 Int_t modelID=1)
{
  
  
  /****************************************/
  /***************Filenames****************/    
  /****************************************/
  
  //data file
  std::string datafile  = datadir + "/TCS_mc_Tree.root";
  std::string brudatafile = datadir + "/TCS_mc_Tree_brufit.root";
  if(!checkFileExists(brudatafile)){
    SnapshotBrufit(datadir,1);
  }
  
  //phasespace file
  std::string sigfile = sigdir + "/TCS_mc_Tree.root";
  std::string brusigfile = sigdir + "/TCS_mc_Tree_brufit.root";
  if(!checkFileExists(brusigfile)){
    SnapshotBrufit(sigdir,1);
  }
  
  //weights File
  std::string weightfile = sigdir + "/ImpSampleWeights.root";
  if(!checkFileExists(weightfile)){
    SetupWeights(sigdir);
  }

  
  /****************************************/
  /************Create FitManager***********/    
  /****************************************/
  FitManager fm;
  std::string outdir;
  
  /****************************************/
  /***********Apply Dilepton model**********/    
  /****************************************/
  if(modelID==0){ // Simple TCS toy model
    ModelSimple(fm);
    outdir = "fit_simple";
  }else if(modelID==1){ // BH Only, alpha mixing for F1,F2
    ModelBH(fm, weightfile);
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

  fm.LoadData("tree",brudatafile);
  //"Dilepton" is given in Model.C as name of the RooComponentsPDF
  fm.LoadSimulated("tree",brusigfile,"Dilepton");

  /**************************************************/
  /***********Choose minimiser and run***************/ 
  /**************************************************/
  //number of CPUs to split likelihood calc.
  //fm.SetUp().AddFitOption(RooFit::NumCPU(4)); 
  //std::vector<Int_t> Niters,Int_t Nburn, Float_t norm,float target,float accmin,float accmax
  auto mcmc=new BruMcmcCovariance({5000,20000,10000},100,1,0.23,0.16,0.3);
  // 2. ACTIVATE THE NEW TUNING ENGINE
  mcmc->SetTuningMode(HS::FIT::McmcTuneMode::kMappedRhat);
  // 2. Tell it to ACTUALLY RUN the tuning phase
  mcmc->TuneCovarianceStep();
  //auto mcmc=new BruMcmcCovariance({100,100,100},100,1,0.23,0.0,1.0);
  fm.SetMinimiser(mcmc);
  Here::Go(&fm);
  //Multi::Go(&fm,10);
  
}
