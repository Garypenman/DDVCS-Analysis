#pragma once
#include "Model.C"
#include "ModelV2.C"

void FitThetaPhi(TString filebase = "t1", Int_t modelID=2){

  
  /****************************************/
  /***************Filenames****************/    
  /****************************************/
  TString signalfiledir = "HepMC_TCS_10x100_Phasespace";
  TString outdir = "fit_out/";

  if(filebase=="")
    signalfiledir += "/";
  else
    signalfiledir += "_" + filebase + "/";

  //10x100 full epic
  //TString datafile  = "/w/work6/home/gp140f/combirad_trees/HepMC_TCS_10x100_hplus/TCS_mc_Tree.root";
  //10x100 BH only
  TString datafile  = "/w/work6/home/gp140f/combirad_trees/HepMC_TCS_10x100_BHONLY_hplus/TCS_mc_Tree.root";

  //phasespace file
  TString sigfile = "/w/work6/home/gp140f/combirad_trees/"+signalfiledir+"TCS_mc_Tree_brufit.root";
  //sigfile = "../hepmc/TCSPhaseSpace.root";
  /****************************************/
  /************Create FitManager***********/    
  /****************************************/
  FitManager fm;
  fm.SetUp().SetOutDir(outdir);
  // fm.SetUp().SetIDBranchName("UID");
  
  /****************************************/
  /***********Apply Dilepton model**********/    
  /****************************************/
  if(modelID==0){ //simple TCS toy model, ModelV0.C TBD
    //ModelV0(fm);
  }else if(modelID==1){ //F1,F1 full BH, need rename
      Model(fm);
    }else if(modelID==2){ // alpha mixing BH
      ModelV2(fm);
    }else{ //add more as needed
    std::cout << "No matching fit model ID" << endl;
    exit(1);
  }

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
  //fm.LoadSimulated("tree",datafile,"Dilepton");
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
