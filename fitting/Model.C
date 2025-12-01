void Model(FitManager& fm, Int_t Nevents=1){


  /****************************************/
  /*************Load Variables*************/    
  /****************************************/
  //Watch theta limits!!! cannot go to 0 as discontinuity
  fm.SetUp().LoadVariable(Form("mc_Heli_Theta[%lf,%lf]",TMath::Pi()/4,TMath::Pi()*3/4));
  //fm.SetUp().LoadVariable(Form("mc_Heli_Theta[%lf,%lf]",0.,TMath::Pi()) );
  fm.SetUp().LoadVariable(Form("mc_Heli_Phi[-%lf,%lf]",TMath::Pi(),TMath::Pi()));
  fm.SetUp().LoadVariable(Form("mc_GammaPolCirc[-1,1]"));

  /****************************************/
  /*************Load Formula*************/    
  /****************************************/
  //TCS only term
  fm.SetUp().LoadFormula("TCS_TH=1+TMath::Cos(@mc_Heli_Theta[])*TMath::Cos(@mc_Heli_Theta[])");

  //Interference terms
  fm.SetUp().LoadFormula("INT_TH=(1+TMath::Cos(@mc_Heli_Theta[])*TMath::Cos(@mc_Heli_Theta[]))/(TMath::Sin(@mc_Heli_Theta[]))");
  fm.SetUp().LoadFormula("INT_COSPHI=(TMath::Cos(@mc_Heli_Phi[]))");
  fm.SetUp().LoadFormula("INT_hSINPHI=(@mc_GammaPolCirc[]*TMath::Sin(@mc_Heli_Phi[]))");
  
  fm.SetUp().LoadFormula("INT2_TH=TMath::Cos(@mc_Heli_Theta[])");
  fm.SetUp().LoadFormula("INT2_COSPHI=TMath::Cos(@mc_Heli_Phi[]*2)");
  fm.SetUp().LoadFormula("INT2_hSINPHI=(@mc_GammaPolCirc[]*TMath::Sin(@mc_Heli_Phi[]*2))");
  
  //Bethe Heitler term
  //fm.SetUp().LoadFormula("BH_TH=(1 + TMath::Cos(@mc_Heli_Phi[])*TMath::Cos(@mc_Heli_Theta))/(TMath::Sin(@mc_Heli_Theta[])*TMath::Sin(@mc_Heli_Theta[]))");
  
  fm.SetUp().LoadFormula("BH_TH=(1 + TMath::Cos(@mc_Heli_Theta[])*TMath::Cos(@mc_Heli_Theta[]))/(TMath::Sin(@mc_Heli_Theta[])*TMath::Sin(@mc_Heli_Theta[]))");

  ///)/(-TMath::Cos(@mc_Heli_Theta[]) + TMath::Sin(@mc_Heli_Theta[])*TMath::Cos(@mc_Heli_Phi[]))
  
  /****************************************/
  /*************Load Parameters************/    
  /****************************************/
  fm.SetUp().LoadParameter("BH[0.0,0,1]");
  fm.SetUp().LoadParameter("ImM[0.0,0,1]");
  fm.SetUp().LoadParameter("ReM[0.0,0,1]");
  fm.SetUp().LoadParameter("ImM2[0.0,0,1]");
  fm.SetUp().LoadParameter("ReM2[0.0,0,1]");
  fm.SetUp().LoadParameter("TCS[0.0,0,1]");
  //fm.SetUp().LoadParameter("INT[0,0,1]");
  
   /****************************************/
  /*************Load Constraints***********/    
  /****************************************/
 //Constrain Total contribution ==1
  fm.SetUp().LoadFormula("INT=(1 - @BH[] - @TCS[])");
  //fm.SetUp().LoadFormula("ImMTOT=(@ImM[]*@ImM + @ImM2[]*@ImM2[])");
  //fm.SetUp().LoadFormula("ReMTOT=TMath::Sqrt(1 - @ImMTOT[]*@ImM[]TOT)");
  //fm.SetUp().LoadFormula("@ReM[]*@ReM[] + @ReM2[]*@ReM2[] + @ImM[]*@ImM + @ImM2[]*@ImM2[] = 1");
  
  // fm.SetUp().SetIDBranchName("UID");

  /****************************************/
  /*************Make model PDF*************/ //DONE
  /****************************************/
  fm.SetUp().FactoryPDF("RooComponentsPDF::Dilepton(0,{mc_Heli_Theta,mc_Heli_Phi,mc_GammaPolCirc},=BH;BH_TH:TCS;TCS_TH:INT;ReM;INT_TH;INT_COSPHI:INT;ImM;INT_TH;INT_hSINPHI:INT;ReM2;INT2_TH;INT2_COSPHI:INT;ImM2;INT2_TH;INT2_hSINPHI)"); 
  fm.SetUp().LoadSpeciesPDF("Dilepton",Nevents);
}

void Model(){
}
