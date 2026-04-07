template <typename T>
void Defines(T &df, std::string type){
  using ROOT::VecOps::RVec;

  df = df
    .Define(Form("%s_pmag_scatele",type.c_str()), [](RVec<double> pmag, int idx){
  	return pmag[idx];
      },{Form("%s_pmag",type.c_str()),"scat_ele"});

  
}


template <typename T>
void Takes(T &df, std::string type){

  cout << "hello world" << endl;

}


void TestBeamFX(){
  
  TString file="/w/work5/home/garyp/rad_trees/HepMC_ddvcs_ee_18x275_minus.root";
  TString fileAB="/w/work5/home/garyp/rad_trees/AB_HepMC_ddvcs_ee_18x275_minus.root";
  TString fileSIM="/w/work5/home/garyp/rad_trees/MCReconDetector_ddvcs_ee_18x275_hminus.root";
  
  
  ROOT::RDataFrame dfbase("rad_tree",file);
  ROOT::RDataFrame dfbaseAB("rad_tree",fileAB);
  ROOT::RDataFrame dfbaseSIM("rad_tree",fileSIM);
  
  
  auto nev=10000;
  auto df = dfbase.Range(nev);
  auto dfAB = dfbaseAB.Range(nev);
  auto dfSIM = dfbaseSIM.Range(nev);
  
  
  Defines(df,"mc");
  Defines(dfAB,"mc");
  Defines(dfSIM,"tru");
  Defines(dfSIM,"rec");
  
  
  auto vmc_Q2 = df.Take<double>("mc_Q2");
  auto vmcAB_Q2 = dfAB.Take<double>("mc_Q2");
  auto vtru_Q2 = dfSIM.Take<double>("tru_Q2");
  auto vrec_Q2 = dfSIM.Take<double>("rec_Q2");
  
  auto vmc_pmag_scatele = df.Take<double>("mc_pmag_scatele");
  auto vmcAB_pmag_scatele = dfAB.Take<double>("mc_pmag_scatele");
  auto vtru_pmag_scatele = dfSIM.Take<double>("tru_pmag_scatele");
  auto vrec_pmag_scatele = dfSIM.Take<double>("rec_pmag_scatele");
  
  TH1D* hresQ2_noAB = new TH1D("hresQ2_noAB", "Reconstructed - HepMC Truth;#DeltaQ^{2}[GeV^{2}]",100,-0.001,0.001);
  TH1D* hresQ2 = new TH1D("hresQ2", "Reconstructed - MCParticles Truth;#DeltaQ^{2}[GeV^{2}]",100,-0.001,0.001);

  TH1D* hres_pmag_scatele_noAB = new TH1D("hrespmag_scatele_noAB", "Reconstructed - HepMC Truth;#Deltap/p",100,-0.3,0.3);
  TH1D* hres_pmag_scatele = new TH1D("hrespmag_scatele", "Reconstructed - MCParticles Truth;#Deltap/p",100,-0.3,0.3);
  
  
  TH2D *h2d_Q2_mc_mcAB = new TH2D("h2d_Q2_mc_mcAB","HepMC vs AB HepMC;log_{10}(Q^{2})_{HepMC} [GeV^{2}]; log_{10}(Q^{2})_{MC,AB} [GeV^{2}]",100,-8,1,100,-8,1);
  TH2D *h2d_Q2_mc_tru = new TH2D("h2d_Q2_mc_tru","HepMC vs MCParticles;log_{10}(Q^{2})_{HepMC} [GeV^{2}]; log_{10}(Q^{2})_{MCParticles} [GeV^{2}]",100,-8,1,100,-8,1);
  
  TH2D *h2d_pmag_scatele_mc_tru = new TH2D("h2d_pmag_scatele_mc_tru","HepMC vs MCParticles",100,0,20,100,0,20);
  
  cout << vmc_Q2->size() << endl;
  cout << vmcAB_Q2->size() << endl;
  cout << vtru_Q2->size() << endl;
  cout << vrec_Q2->size() << endl;
  
  for(size_t ev=0; ev<vmc_Q2->size(); ev++){
    
    auto mc_Q2 = vmc_Q2->at(ev);
    auto mcAB_Q2 = vmcAB_Q2->at(ev);
    auto tru_Q2 = vtru_Q2->at(ev);
    auto rec_Q2 = vrec_Q2->at(ev);
    
  //   auto mc_pmag_scatele = vmc_pmag_scatele->at(ev);
  //   auto tru_pmag_scatele = vtru_pmag_scatele->at(ev);
  //   auto rec_pmag_scatele = vrec_pmag_scatele->at(ev);
  //   if(!(rec_pmag_scatele>0)) continue;
    
  //   hresQ2_noAB->Fill(mc_Q2-rec_Q2);
  //   hresQ2->Fill(tru_Q2-rec_Q2);
    
    h2d_Q2_mc_tru->Fill(log10(mc_Q2), log10(tru_Q2));
    h2d_Q2_mc_mcAB->Fill(log10(mc_Q2), log10(mcAB_Q2));
    
  //   hres_pmag_scatele_noAB->Fill((mc_pmag_scatele - rec_pmag_scatele)/mc_pmag_scatele);
  //   hres_pmag_scatele->Fill((tru_pmag_scatele - rec_pmag_scatele)/tru_pmag_scatele);
    
  //   h2d_pmag_scatele_mc_tru->Fill(mc_pmag_scatele, tru_pmag_scatele);
    
  }
  
  // TCanvas* c00 = new TCanvas();
  // c00->Divide(2,1);
  // c00->cd(1);
  // hresQ2_noAB->Draw();
  // c00->cd(2);
  // hresQ2->Draw();
  
  // TCanvas* c01 = new TCanvas();
  // c01->Divide(2,1);
  // c01->cd(1);
  // hres_pmag_scatele_noAB->Draw();
  // c01->cd(2);
  // hres_pmag_scatele->Draw();

  // new TCanvas();
  // h2d_pmag_scatele_mc_tru->Draw("colz");

  new TCanvas();
  h2d_Q2_mc_tru->Draw("colz");
  new TCanvas();
  h2d_Q2_mc_mcAB->Draw("colz");
  

}
