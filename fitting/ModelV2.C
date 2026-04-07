void ModelV2(FitManager& fm, Int_t Nevents=10000){


  /****************************************/
  /*************Load Variables*************/    
  /****************************************/
  //Watch theta limits!!! cannot go to 0 as discontinuity
  fm.SetUp().LoadVariable(Form("mc_ThetaHel[%lf,%lf]",TMath::Pi()/10,TMath::Pi()*9/10));
  fm.SetUp().LoadVariable(Form("mc_PhiHel[-%lf,%lf]",TMath::Pi(),TMath::Pi()));
  fm.SetUp().LoadVariable(Form("mc_GammaPolCirc[0,-1,1]"));
  
  fm.SetUp().LoadVariable("mc_Qp[2,1.5,5]");
  fm.SetUp().LoadVariable("mc_s_photo[100,6,20000]");
  fm.SetUp().LoadVariable("mc_t_bot[0.01,0.0001,1]");
  fm.SetUp().LoadVariable("mc_DeltaT[0.01,0.0001,1]");
  
  // fm.SetUp().LoadConstant("mc_Qp[2]");
  // fm.SetUp().LoadConstant("mc_s_photo[100]");
  // fm.SetUp().LoadConstant("mc_t_bot[0.01]");
  // fm.SetUp().LoadConstant("mc_DeltaT[0.01]");
  
  //fixed params
  fm.SetUp().LoadConstant("me[0.000511]");
  fm.SetUp().LoadConstant("Mp[0.938]");
  fm.SetUp().LoadConstant("pi[3.14159]");
  fm.SetUp().LoadConstant("alpha3[1.0/(137.0*137.0*137.0)]");
  
  /****************************************/
  /*************Load Formula*************/    
  /****************************************/
  //short hand variables
  fm.SetUp().LoadFormula("Qp2=(@mc_Qp[] * @mc_Qp[])");
  fm.SetUp().LoadFormula("tbot= (-1) * @mc_t_bot[]"); //t comes out of rad positive for q.o.l
  fm.SetUp().LoadFormula("beta=TMath::Sqrt(1 - (4*@me[]*@me[]/@Qp2[]))");
  //fm.SetUp().LoadFormula("tau=@Qp2[] / (@mc_s_photo[] - @Mp[]*@Mp[])");
  //fm.SetUp().LoadFormula("deltaT2= (-1*@tbot[] * (1-@tau[])) - (@tau[]*@tau[]*@Mp[]*@Mp[])");
  //fm.SetUp().LoadFormula("mc_DeltaT=TMath::Sqrt(@deltaT2[])");
  fm.SetUp().LoadFormula("r2=(@mc_s_photo[] - @Qp2[] - @Mp[]*@Mp[])*(@mc_s_photo[] - @Qp2[] - @Mp[]*@Mp[]) - (4*@Qp2[]*@Mp[]*@Mp[])");
  fm.SetUp().LoadFormula("r=TMath::Sqrt(@r2[])");
  
  //Bethe Heitler term
  //coefficients for full leading "L" term in BH
  fm.SetUp().LoadFormula("C0=@beta[] * @r[]");
  fm.SetUp().LoadFormula("C1=@beta[] * ( (@Qp2[]*(@mc_s_photo[] - @Mp[]*@Mp[] - @Qp2[])) + (@tbot[]*(@mc_s_photo[] - @Mp[]*@Mp[] + @Qp2[])) ) / @r[]");
  fm.SetUp().LoadFormula("C2=(-2) * @beta[] * (@mc_s_photo[] - @Mp[]*@Mp[]) * @mc_Qp[] * @mc_DeltaT[] / @r[]");
  //fm.SetUp().LoadFormula("C1=1");
  //fm.SetUp().LoadFormula("C2=-1");
  
  fm.SetUp().LoadFormula("a=@C0[] * TMath::Cos(@mc_ThetaHel[])");
  fm.SetUp().LoadFormula("b=(@C1[] * TMath::Cos(@mc_ThetaHel[])) + (@C2[] * TMath::Sin(@mc_ThetaHel[]) * TMath::Cos(@mc_PhiHel[]))");
  fm.SetUp().LoadFormula("b2=(@b[] * @b[])");
  fm.SetUp().LoadFormula("L=((@Qp2[] - @tbot[])*(@Qp2[] - @tbot[]) - @b2[])/4");
  
  //kinematic prefactor
  fm.SetUp().LoadFormula("BHprefac=@alpha3[]*@beta[] / (4*@pi[]*(@mc_s_photo[] - @Mp[]*@Mp[])*(@mc_s_photo[] - @Mp[]*@Mp[]) * (-1*@tbot[]*@L[]))");
  
  //BH A Terms
  fm.SetUp().LoadFormula("A0=(@mc_s_photo[] - @Mp[]*@Mp[])*(@mc_s_photo[] - @Mp[]*@Mp[])*@mc_DeltaT[]*@mc_DeltaT[]");
  fm.SetUp().LoadFormula("A1=@tbot[]*@a[]*(@a[]+@b[])");
  fm.SetUp().LoadFormula("A2=@Mp[]*@Mp[]*@b[]*@b[]");
  fm.SetUp().LoadFormula("A3=@tbot[]*(4*@Mp[]*@Mp[]-@tbot[])*@Qp2[]");
  //terms in m_l/L
  fm.SetUp().LoadFormula("sqA4=(@Qp2[]-@tbot[])*(@a[]+@b[]) - (@mc_s_photo[] - @Mp[]*@Mp[])*@b[]");
  fm.SetUp().LoadFormula("A4=@sqA4[]*@sqA4[]");
  fm.SetUp().LoadFormula("A5=@tbot[]*(4*@Mp[]*@Mp[]-@tbot[])*(@Qp2[]-@tbot[])*(@Qp2[]-@tbot[])");

  fm.SetUp().LoadFormula("A=@A0[]-@A1[]-@A2[]-@A3[] + (@me[]/@L[])*(@A4[]+@A5[])");
  
    
  //BH B Terms
  fm.SetUp().LoadFormula("B0=(@Qp2[]+@tbot[])*(@Qp2[]+@tbot[])");
  fm.SetUp().LoadFormula("B1=@b2[]");
  fm.SetUp().LoadFormula("B2=8*@me[]*@me[]*@Qp2[]");
  fm.SetUp().LoadFormula("B3=4*@me[]*@me[]*(@tbot[]+2*@me[]*@me[])*(@Qp2[]-@tbot[])*(@Qp2[]-@tbot[])/@L[]");
  fm.SetUp().LoadFormula("B=@B0[]+@B1[]+@B2[]-@B3[]");

  //approx form with no phi dependence
  //fm.SetUp().LoadFormula("BH_TH=(1 + TMath::Cos(@mc_ThetaHel[])*TMath::Cos(@mc_ThetaHel[]))/(TMath::Sin(@mc_ThetaHel[])*TMath::Sin(@mc_ThetaHel[]))");
  
  //only 1/L 
  //fm.SetUp().LoadFormula("BH_TH=(1 / @L[])");
  
  //1/L with cosTheta terms and kinematic prefactor
  //fm.SetUp().LoadFormula("BH_TH=(@BHprefac[]*@beta[] / (-1*@tbot[]*@L[])) * (1 + TMath::Cos(@mc_ThetaHel[])*TMath::Cos(@mc_ThetaHel[]))");

  //Full BH Expressions
  fm.SetUp().LoadFormula("BHF1A=@BHprefac[] * @A[]/(-1*@tbot[])");
  fm.SetUp().LoadFormula("BHF1B=@BHprefac[] * @B[]/2");
  fm.SetUp().LoadFormula("BHF2A=@BHprefac[] * @A[]/(4*@Mp[]*@Mp[])");
  fm.SetUp().LoadFormula("BHF2B=@BHprefac[] * @B[]/2");
  fm.SetUp().LoadFormula("BHF1F2=@BHprefac[] * @B[]");
  
  //Brufit code would be something like
  // Load a single angle parameter alpha
  fm.SetUp().LoadParameter("alpha[0.0,-1.57,1.57]"); // Limits roughly -pi/2 to pi/2
  
  // Define the trigonometric weights (there is a nice symmetry to this!)
  fm.SetUp().LoadFormula("wF1sq=(TMath::Cos(@alpha[]) * TMath::Cos(@alpha[]))");
  fm.SetUp().LoadFormula("wF2sq=(TMath::Sin(@alpha[]) * TMath::Sin(@alpha[]))");
  fm.SetUp().LoadFormula("wF1F2=(TMath::Cos(@alpha[]) * TMath::Sin(@alpha[]))");
 
  
  //TCS only term
  //fm.SetUp().LoadFormula("TCSprefac=@alpha3[] / (8*@pi[]*@mc_s_photo[]*@mc_s_photo[])");
  //fm.SetUp().LoadFormula("TCS_TH=(@TCSprefac[]/(4*@Qp2[]))*(1+TMath::Cos(@mc_ThetaHel[])*TMath::Cos(@mc_ThetaHel[]))");
  //fm.SetUp().LoadFormula("TCS_TH=1+TMath::Cos(@mc_ThetaHel[])*TMath::Cos(@mc_ThetaHel[])");

  // //Interference terms
  //fm.SetUp().LoadFormula("INT_TH=(1+TMath::Cos(@mc_ThetaHel[])*TMath::Cos(@mc_ThetaHel[]))/(TMath::Sin(@mc_ThetaHel[]))");
  //fm.SetUp().LoadFormula("INT_COSPHI=(TMath::Cos(@mc_PhiHel[]))");
  //fm.SetUp().LoadFormula("INT_hSINPHI=(@mc_GammaPolCirc[]*TMath::Sin(@mc_PhiHel[]))");
  
  /****************************************/
  /*************Load Parameters************/    
  /****************************************/
  //fm.SetUp().LoadParameter("BH[1.0,0.0,1]");
  // fm.SetUp().LoadParameter("F1[0.5,0.01,1]");
  // fm.SetUp().LoadParameter("F2[0.5,0.01,1.7928]");
  //fm.SetUp().LoadParameter("F1F2[0.0,0.0,1]");
  
  //fm.SetUp().LoadParameter("ImM[0.0,0,1]");
  //fm.SetUp().LoadParameter("TCS[0.0,0,1]");
  
   /****************************************/
  /*************Load Constraints***********/    
  /****************************************/
 //Constrain Total contribution ==1
  //fm.SetUp().LoadFormula("INT=(1 - @BH[] - @TCS[])");
  //fm.SetUp().LoadFormula("ReM=TMath::Sqrt(1 - @ImM[]*@ImM[])");
  //fm.SetUp().LoadFormula("F1F2=(TMath::Sqrt(@F1[])*TMath::Sqrt(@F2[]))");
  
  auto& formulas = fm.SetUp().Formulas();
  formulas.Print("v");
  
  //fm.SetUp().SetIDBranchName("UID");
  fm.SetUp().SetIDBranchName("rdfentry_");

  /****************************************/
  /*************Make model PDF*************/
  /****************************************/
  //fm.SetUp().FactoryPDF("BruComponentsPDF::Dilepton(0,{mc_ThetaHel,mc_PhiHel},=BH;BH_TH)");
  //fm.SetUp().FactoryPDF("BruComponentsPDF::Dilepton(0,{mc_GammaPolCirc,mc_PhiHel,mc_ThetaHel,mc_s_photo,mc_Qp,mc_t_bot,mc_DeltaT},=BH;BH_TH)");
  
  //BH Only
  //fm.SetUp().FactoryPDF("BruComponentsPDF::Dilepton(0,{mc_GammaPolCirc,mc_PhiHel,mc_ThetaHel,mc_s_photo,mc_Qp,mc_t_bot,mc_DeltaT},=F1;BHF1A:F1;BHF1B:F2;BHF2A:F2;BHF2B:F1F2;BHF1F2)");
  //fm.SetUp().FactoryPDF("BruComponentsPDF::Dilepton(0,{mc_GammaPolCirc,mc_PhiHel,mc_ThetaHel,mc_s_photo,mc_Qp,mc_t_bot,mc_DeltaT},=F1;BHF1A)");
  
  //BH+TCS+INT
  //fm.SetUp().FactoryPDF("BruComponentsPDF::Dilepton(0,{mc_GammaPolCirc,mc_PhiHel,mc_ThetaHel,mc_s_photo,mc_Qp,mc_t_bot,mc_DeltaT},=BH;BH_A1:TCS;TCS_TH:INT;ReM;INT_TH;INT_COSPHI:INT;ImM;INT_TH;INT_hSINPHI)");


  // Apply them to the PDF
  fm.SetUp().FactoryPDF("BruComponentsPDF::Dilepton(0,{mc_GammaPolCirc,mc_PhiHel,mc_ThetaHel,mc_s_photo,mc_Qp,mc_t_bot,mc_DeltaT},=wF1sq;BHF1A:wF1sq;BHF1B:wF2sq;BHF2A:wF2sq;BHF2B:wF1F2;BHF1F2)");
  
  fm.SetUp().LoadSpeciesPDF("Dilepton",Nevents);
}

void ModelV2(){
}
