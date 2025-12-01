{
  //TFile file("sim_toys2/Toy0.root");
  TFile file("toys_test/Toy0.root");

  auto tree = file.Get<TTree>("ToyData");

  TH1D hp("plus","postive helicity",10,-TMath::Pi(),TMath::Pi());
  hp.Sumw2();
  tree->Draw("mc_Heli_Phi>>plus","mc_GammaPolCirc>0");
  
  TH1D hm("minus","negative helicity",10,-TMath::Pi(),TMath::Pi());
  hm.Sumw2();
  hm.SetLineColor(2);
  tree->Draw("mc_Heli_Phi>>minus","mc_GammaPolCirc<0","same");
  
  // TH1D hpol("pol","polarisation",100,-1,1);
  // hpol.SetLineColor(1);
  // tree->Draw("Phi>>minus","Pol<0","");
  
  auto ha= hp.GetAsymmetry(&hm);
  ha->Scale(1./0.5);
  new TCanvas();
  ha->Draw();    

  TF1 fsin("sinfunc","[0]*TMath::Sin(x)");
  ha->Fit("sinfunc");
  
}
