void Model(FitManager& fm, Int_t Nevents=1){


  /****************************************/
  /*************Load Variables*************/    
  /****************************************/
  //Watch theta limits!!! cannot go to 0 as discontinuity
  //fm.SetUp().LoadVariable(Form("mc_Heli_Theta[%lf,%lf]",TMath::Pi()/4,TMath::Pi()*3/4));
  //fm.SetUp().LoadVariable(Form("mc_Heli_Theta[%lf,%lf]",0.,TMath::Pi()) );
  fm.SetUp().LoadVariable(Form("mc_Heli_Phi[-%lf,%lf]",TMath::Pi(),TMath::Pi()));
  fm.SetUp().LoadVariable(Form("mc_GammaPolCirc[-1,1]"));
  //fm.SetUp().LoadVariable(Form("mc_Heli_Theta[%lf,%lf]",TMath::Pi()/4,TMath::Pi()*3/4));
  
  fm.SetUp().LoadVariable("mc_Heli_CosTheta[-0.8,0.8]");
  fm.SetUp().LoadFormula("mc_Heli_Theta=TMath::ACos(@mc_Heli_CosTheta[])");
  
  fm.SetUp().LoadAuxVar("mc_t_top[0,2]");
  
  //generating flat phase space in other vars
  fm.SetUp().LoadVariable("cm_costheta[-1,1]");
  fm.SetUp().LoadFormula("cm_sintheta=TMath::Sin(TMath::ACos(@cm_costheta[]))");
  
  //fm.SetUp().LoadVariable("mc_GMass[1.4142141,4.2425992]");
  //fm.SetUp().LoadVariable("mc_t_bot[0.0001,1.9964108]");
  //fm.SetUp().LoadVariable("mc_s_photo[5.7304514,19799.769]");
  //fm.SetUp().LoadVariable("mc_DeltaT[0.0012470404,1.4117276]");
  
  //fixed params
  fm.SetUp().LoadParameter("y[0.5]");
  fm.SetUp().LoadParameter("Mp[0.938]");
  //fm.SetUp().LoadParameter("me[0.000511]");
  //fm.SetUp().LoadParameter("Erest[10551.3]");
  fm.SetUp().LoadParameter("beta[1]");
  fm.SetUp().LoadParameter("s[9900]");
  fm.SetUp().LoadParameter("mc_GMass[1.414]");
  fm.SetUp().LoadParameter("mc_t_bot[0.01]");
  //fm.SetUp().LoadParameter("mc_DeltaT[0.1]");
  
  //fm.SetUp().LoadFormula("Q2=@y[]*@y[]*@me[]*@me[] / (1-@y[])");
  //fm.SetUp().LoadFormula("s=@Mp[]*@Mp[] + 2*@Mp[]*@y[]*@Erest[] - @Q2[]");
  
  
  /****************************************/
  /*************Load Formula*************/    
  /****************************************/
  //short hand variables
  fm.SetUp().LoadFormula("Qp2=(@mc_GMass[] * @mc_GMass[])");
  fm.SetUp().LoadFormula("tbot= (-1) * @mc_t_bot[]"); //t comes out of rad positive for q.o.l
  //fm.SetUp().LoadFormula("s=@mc_s_photo[]");
  
  //fm.SetUp().LoadFormula("beta=TMath::Sqrt(1 - (4*@me[]*@me[]/@Qp2[]))");
  fm.SetUp().LoadFormula("tau=@Qp2[] / (@s[] - @Mp[]*@Mp[])");
  fm.SetUp().LoadFormula("deltaT2=-1*@tbot[] * (1-@tau[]) - @tau[]*@tau[]*@Mp[]*@Mp[]");
  fm.SetUp().LoadFormula("mc_DeltaT=TMath::Sqrt(@deltaT2[])");
  fm.SetUp().LoadFormula("r2=(@s[] - @Qp2[] - @Mp[]*@Mp[])*(@s[] - @Qp2[] - @Mp[]*@Mp[]) - (4*@Qp2[]*@Mp[]*@Mp[])");
  fm.SetUp().LoadFormula("r=TMath::Sqrt(@r2[])");
  
  
  //TCS only term
  fm.SetUp().LoadFormula("TCS_TH=1+TMath::Cos(@mc_Heli_Theta[])*TMath::Cos(@mc_Heli_Theta[])");

  //Interference terms
  fm.SetUp().LoadFormula("INT_TH=(1+TMath::Cos(@mc_Heli_Theta[])*TMath::Cos(@mc_Heli_Theta[]))/(TMath::Sin(@mc_Heli_Theta[]))");
  fm.SetUp().LoadFormula("INT_COSPHI=(TMath::Cos(@mc_Heli_Phi[]))");
  fm.SetUp().LoadFormula("INT_hSINPHI=(@mc_GammaPolCirc[]*TMath::Sin(@mc_Heli_Phi[]))");
  
  //Bethe Heitler term
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
  
  //fm.SetUp().LoadFormula("BH_TH=(1 + TMath::Cos(@mc_Heli_Theta[])*TMath::Cos(@mc_Heli_Theta[]))/(TMath::Sin(@mc_Heli_Theta[])*TMath::Sin(@mc_Heli_Theta[]))");
  //fm.SetUp().LoadFormula("BH_TH=(1 / @L[])*(1 + TMath::Cos(@mc_Heli_Theta[])*TMath::Cos(@mc_Heli_Theta[]))");
  fm.SetUp().LoadFormula("BH_TH=(1 / @L[])");
  //fm.SetUp().LoadFormula("BH_TH=(1 + TMath::Cos(@mc_Heli_Theta[])*TMath::Cos(@mc_Heli_Theta[]))/(TMath::Sin(@mc_Heli_Theta[])*TMath::Sin(@mc_Heli_Theta[]) * (1 + TMath::Cos(@mc_Heli_Phi[])*TMath::Cos(@mc_Heli_Phi[]) - TMath::Cos(@mc_Heli_Phi[])/TMath::Tan(@mc_Heli_Theta[])))");
  
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
  //fm.SetUp().FactoryPDF("RooComponentsPDF::Dilepton(0,{Theta,Phi,Pol},=BH;BH_TH:TCS;TCS_TH:INT;ReM;INT_TH;INT_COSPHI:INT;ImM;INT_TH;INT_hSINPHI)"); 
  fm.SetUp().FactoryPDF("RooComponentsPDF::Dilepton(0,{Theta,Phi,Pol},=BH;BH_TH)"); 
  fm.SetUp().LoadSpeciesPDF("Dilepton",Nevents);
}

void Model(){
}
