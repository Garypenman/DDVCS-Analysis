#pragma once
#include "Model.C"

void ToyMaker(int nev=10000, double BH=0.5, double ImM=0.3, double ImM2=0.3, double ReM=0.3, double ReM2=0.3, double TCS=0.1){
  //Start from large file of "flat/phase space" events
  //TString sigfile  = "flat/Toy0.root";
  //TString treename = "ToyData";
  
  TString sigfile  = "/w/work5/home/garyp/rad_trees/HepMC_ddvcs_ee_18x275_mixed_flat.root";
  TString treename = "rad_tree";
  
  TString outdir = "toys_test/";
  
  /****************************************/
  /************Create ToyManager***********/    
  /****************************************/
  ToyManager toyman(1);
  toyman.SetUp().SetOutDir(outdir);
  toyman.SetUp().SetIDBranchName("UID");
  /****************************************/
  /***********Apply Dilepton model**********/    
  /****************************************/
 
  Model(toyman,nev); //create toy data with 100 events

  /**************************************************/
  /**************Load PhaseSpace data****************/
  /**************************************************/
  //We will sample from these events with accept/reject
  //LoadSimulated(Treename,Filename,PDFname)
  toyman.LoadSimulated(treename,sigfile,"Dilepton");

  /**************************************************/
  /************Set Toy Parameter Values**************/
  /**************************************************/
  toyman.SetUp().SetParVal("BH",BH); //Bethe Heitler contribution
  toyman.SetUp().SetParVal("ImM",ImM); //Imaginery part of M (ReM^2 =1-ImM^2)
  toyman.SetUp().SetParVal("ImM2",ImM2);
  toyman.SetUp().SetParVal("ReM",ReM);
  toyman.SetUp().SetParVal("ReM2",ReM2);
  toyman.SetUp().SetParVal("TCS",TCS); //TCS contribution
 
  /**************************************************/
  /**************Create Toy  data****************/
  /**************************************************/
  Here::Go(&toyman);


  // //do a sanity check fit
 //  auto toyfitter = toyman.Fitter();
 // //********************************************
 // // Perform "fit" with an MCMC sampler
 //  //a tree MCMCTree is included in the Results*.root file
 //  auto mcmc=new BruMcmcCovariance(20000,1000,1,0.23,0.16,0.3);
 //  ////mcmc->TurnOffCovariance();//BruMcmcCovariance only, do not proceed with covariance based sampling, just perform basic stepping
 //  //toyfitter->SetMinimiser(mcmc);
  
 //    /**************************************************/
 //  /************Set Toy Parameter Values**************/
 //  /**************************************************/
 //  toyfitter->SetUp().SetParVal("BH",0.5); //Bethe Heitler contribution
 //  toyfitter->SetUp().SetParVal("ImM",0.25); //Imaginery part of M (ReM^2 =1-ImM^2)
 //  toyfitter->SetUp().SetParVal("TCS",0.25); //TCS contribution

 //  Here::Go(toyfitter.get());
 
}
