void Model(FitManager& fm, Int_t Nevents=1){


  /****************************************/
  /*************Load Variables*************/    
  /****************************************/
  //Watch theta limits!!! cannot go to 0 as discontinuity
  fm.SetUp().LoadVariable(Form("mc_Heli_Theta[%lf,%lf]",TMath::Pi()/10,TMath::Pi()*9/10));
  fm.SetUp().LoadVariable(Form("mc_Heli_Phi[-%lf,%lf]",TMath::Pi(),TMath::Pi()));
  fm.SetUp().LoadVariable(Form("mc_GammaPolCirc[0,-1,1]"));
  
  // fm.SetUp().LoadVariable("mc_GMass[2,0,50]");
  // fm.SetUp().LoadVariable("mc_s_photo[20,0,100000]");
  // fm.SetUp().LoadVariable("mc_t_bot[1,0,150]");
  // fm.SetUp().LoadVariable("mc_DeltaT[0.1,0.0,10]");
  
  fm.SetUp().LoadAuxVar("mc_GMass[2,0,50]");
  fm.SetUp().LoadAuxVar("mc_s_photo[20,0,100000]");
  fm.SetUp().LoadAuxVar("mc_t_bot[1,0,150]");
  fm.SetUp().LoadAuxVar("mc_DeltaT[0.1,0.0,10]");
  
 
  //fixed params
  fm.SetUp().LoadParameter("me[0.000511]");
  fm.SetUp().LoadParameter("Mp[0.938]");

  /****************************************/
  /*************Load Formula*************/    
  /****************************************/
  //short hand variables
  fm.SetUp().LoadFormula("Qp2=(@mc_GMass[] * @mc_GMass[])");
  fm.SetUp().LoadFormula("tbot= (-1) * @mc_t_bot[]"); //t comes out of rad positive for q.o.l
  fm.SetUp().LoadFormula("beta=TMath::Sqrt(1 - (4*@me[]*@me[]/@Qp2[]))");
  //fm.SetUp().LoadFormula("tau=@Qp2[] / (@mc_s_photo[] - @Mp[]*@Mp[])");
  //fm.SetUp().LoadFormula("deltaT2= (-1*@tbot[] * (1-@tau[])) - (@tau[]*@tau[]*@Mp[]*@Mp[])");
  //fm.SetUp().LoadFormula("mc_DeltaT=TMath::Sqrt(@deltaT2[])");
  fm.SetUp().LoadFormula("r2=(@mc_s_photo[] - @Qp2[] - @Mp[]*@Mp[])*(@mc_s_photo[] - @Qp2[] - @Mp[]*@Mp[]) - (4*@Qp2[]*@Mp[]*@Mp[])");
  fm.SetUp().LoadFormula("r=TMath::Sqrt(@r2[])");
  
  //TCS only term
  fm.SetUp().LoadFormula("TCS_TH=1+TMath::Cos(@mc_Heli_Theta[])*TMath::Cos(@mc_Heli_Theta[])");

  //Interference terms
  fm.SetUp().LoadFormula("INT_TH=(1+TMath::Cos(@mc_Heli_Theta[])*TMath::Cos(@mc_Heli_Theta[]))/(TMath::Sin(@mc_Heli_Theta[]))");
  fm.SetUp().LoadFormula("INT_COSPHI=(TMath::Cos(@mc_Heli_Phi[]))");
  fm.SetUp().LoadFormula("INT_hSINPHI=(@mc_GammaPolCirc[]*TMath::Sin(@mc_Heli_Phi[]))");
  
  //Bethe Heitler term
  //coefficients for full leading "L" term in BH
  //fm.SetUp().LoadFormula("BH_TH=(1 + TMath::Cos(@mc_Heli_Phi[])*TMath::Cos(@mc_Heli_Theta))/(TMath::Sin(@mc_Heli_Theta[])*TMath::Sin(@mc_Heli_Theta[]))");
  //coefficients for full leading "L" term in BH
  fm.SetUp().LoadFormula("C0=@beta[] * @r[]");
  fm.SetUp().LoadFormula("C1=@beta[] * ( (@Qp2[]*(@s[] - @Mp[]*@Mp[] - @Qp2[])) + (@tbot[]*(@s[] - @Mp[]*@Mp[] + @Qp2[])) ) / @r[]");
  fm.SetUp().LoadFormula("C2= (-2) * @beta[] * (@s[] - @Mp[]*@Mp[]) * @mc_GMass[] * @mc_DeltaT[] / @r[]");
  //fm.SetUp().LoadFormula("C1=1");
  //fm.SetUp().LoadFormula("C2=-1");
  
  fm.SetUp().LoadFormula("b=(@C1[] * TMath::Cos(@mc_Heli_Theta[])) + (@C2[] * TMath::Sin(@mc_Heli_Theta[]) * TMath::Cos(@mc_Heli_Phi[]))");
  fm.SetUp().LoadFormula("b2=(@b[] * @b[])");
  fm.SetUp().LoadFormula("L=((@Qp2[] - @tbot[])*(@Qp2[] - @tbot[]) - @b2[])/4");
  
  
  //approx form with no phi dependence
  //fm.SetUp().LoadFormula("BH_TH=(1 + TMath::Cos(@mc_Heli_Theta[])*TMath::Cos(@mc_Heli_Theta[]))/(TMath::Sin(@mc_Heli_Theta[])*TMath::Sin(@mc_Heli_Theta[]))");
  
  //1/L with cosTheta terms
  //fm.SetUp().LoadFormula("BH_TH=(1 / @L[])*(1 + TMath::Cos(@mc_Heli_Theta[])*TMath::Cos(@mc_Heli_Theta[]))");
  
  //only 1/L 
  fm.SetUp().LoadFormula("BH_TH=(1 / @L[])");
  
  //Full by-hand expansion of 1/L without C1,C2 coefficients
  //fm.SetUp().LoadFormula("BH_TH=(1 + TMath::Cos(@mc_Heli_Theta[])*TMath::Cos(@mc_Heli_Theta[]))/(TMath::Sin(@mc_Heli_Theta[])*TMath::Sin(@mc_Heli_Theta[]))");
  
  /****************************************/
  /*************Load Parameters************/    
  /****************************************/
  fm.SetUp().LoadParameter("BH[0.0,0,1]");
  fm.SetUp().LoadParameter("ImM[0.0,0,1]");
  fm.SetUp().LoadParameter("TCS[0.0,0,1]");
  
   /****************************************/
  /*************Load Constraints***********/    
  /****************************************/
 //Constrain Total contribution ==1
  fm.SetUp().LoadFormula("INT=(1 - @BH[] - @TCS[])");
  fm.SetUp().LoadFormula("ReM=TMath::Sqrt(1 - @ImM[]*@ImM[])");
  
  auto& formulas = fm.SetUp().Formulas();
  formulas.Print("v");
  
  // fm.SetUp().SetIDBranchName("UID");

  /****************************************/
  /*************Make model PDF*************/ //DONE
  /****************************************/
  fm.SetUp().FactoryPDF("RooComponentsPDF::Dilepton(0,{mc_Heli_Theta,mc_Heli_Phi,mc_GammaPolCirc},=BH;BH_TH)");
  //fm.SetUp().FactoryPDF("RooComponentsPDF::Dilepton(0,{mc_GammaPolCirc,mc_Heli_Phi,mc_Heli_Theta,mc_s_photo,mc_GMass,mc_t_bot,mc_DeltaT},=BH;BH_TH)");
}

void Model(){
}
