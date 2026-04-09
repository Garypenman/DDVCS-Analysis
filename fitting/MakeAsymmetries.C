{
  TFile pfile("/w/work6/home/gp140f/combirad_trees/HepMC_TCS_18x275_hplus/TCS_mc_Tree.root");
  TFile mfile("/w/work6/home/gp140f/combirad_trees/HepMC_TCS_18x275_hminus/TCS_mc_Tree.root");
 
  auto ptree = pfile.Get<TTree>("tree");
  auto mtree = mfile.Get<TTree>("tree");

  TH1D hp("plus","postive helicity",10,-TMath::Pi(),TMath::Pi());
  hp.Sumw2();
  ptree->Draw("mc_PhiHel>>plus","");
  
  TH1D hm("minus","negative helicity",10,-TMath::Pi(),TMath::Pi());
  hm.Sumw2();
  hm.SetLineColor(2);
  mtree->Draw("mc_PhiHel>>minus","","same");
  
  // TH1D hpol("pol","polarisation",100,-1,1);
  // hpol.SetLineColor(1);
  // tree->Draw("Phi>>minus","Pol<0","");
  
  auto ha= hp.GetAsymmetry(&hm);
  //ha->Scale(1./0.5);
  new TCanvas();
  ha->Draw();    

  TF1 *f = new TF1("sinfunc","[0]*cos([1]*x)");
  f->SetParameters(1,1);
  f->SetLineColor(kRed);
  ha->Fit(f,"","",-TMath::Pi(),TMath::Pi());
  
}
