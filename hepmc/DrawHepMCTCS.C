void DrawHepMCTCS(){

  //------------------------------------------------------
  // Load in output of HepMCAnalysis.C
  //------------------------------------------------------

  /////THIS CAN PROBABLY JUST BE DATAFRAMES FOR CLEANESS
  
  //------------------------------------------------------
  // Setup histograms
  //------------------------------------------------------

  TH1D *h_Q2       = new TH1D("Q2","Q^{2};Q^{2} [GeV^{2}]",200,0,20);
  TH1D *h_Qp2      = new TH1D("Qp2","Qprime^{2};Qprime^{2} [GeV^{2}]",200,0,20);
  TH1D *h_t        = new TH1D("t","t;t [GeV^{2}]",200,-2,0);
  TH1D *h_xB       = new TH1D("xB","x_{B};x_{B}",200,0,1);
  TH1D *h_W        = new TH1D("W","W;W [GeV]",200,0,20);

  TH1D *h_pairMass = new TH1D("pairMass",
			      "e^{+}e^{-} invariant mass;M_{ee} [GeV]",
			      200,0,10);

  TH1D *h_theta    = new TH1D("theta",
			      "Decay #theta;#theta [rad]",
			      180,0,TMath::Pi());

  TH1D *h_phi      = new TH1D("phi",
			      "Decay #phi;#phi [rad]",
			      180,-TMath::Pi(),TMath::Pi());

  TH2D *h_t_vs_Qp2 = new TH2D("t_vs_Qp2",
			      "t vs Qprime^{2};Qprime^{2};t",
			      100,0,20,
			      100,-2,0);

  TH1D* h_check = new TH1D("pairPhotonDifference",
			   "M^{2}(ee)-M^{2}(#gamma*)",
			   200,-1e-6,1e-6);


  //------------------------------------------------------
  // Fill histograms
  //------------------------------------------------------
  h_Q2->Fill(Q2);
  h_Qp2->Fill(Qp2);
  h_t->Fill(t);
  h_xB->Fill(xB);
  h_W->Fill(W);
  
  h_pairMass->Fill(pair.M());
  
  h_theta->Fill(theta);
  h_phi->Fill(phi);
  
  h_t_vs_Qp2->Fill(Qp2,t);
  
  h_check->Fill(dM2);
  
}
