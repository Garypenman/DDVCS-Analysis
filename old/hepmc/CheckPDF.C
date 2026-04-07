#include "BHxsec.C"
void CheckPDF(){

  std::string infile="/w/work5/home/garyp/rad_trees/HepMC_ddvcs_ee_18x275_mixed_flat.root";
  //infile="/w/work5/home/garyp/rad_trees/HepMC_ddvcs_ee_18x275_mixed.root";
  
  ROOT::RDataFrame df_base("rad_tree",infile);
  auto df = df_base.Define("mc_Qp2","mc_GMass*mc_GMass");
  df = df.Define("sigma",[](double s,double Q2,double t,double theta,double phi){
      return dsigma_BH(s,Q2,-t,theta,phi);
    },{"mc_s_photo","mc_Qp2","mc_t_bot","mc_Heli_Theta","mc_Heli_Phi"});
  auto df_safe = df.Filter("sigma>=0 && (!std::isnan(sigma))");
  auto df_unsafe = df.Filter("sigma<0 || std::isnan(sigma)");
  
  auto nev = df.Count();
  auto nev_safe = df_safe.Count();
  
  //lazy snapshots
  ROOT::RDF::RSnapshotOptions opts;
  opts.fLazy = true;
  auto cols = df.GetColumnNames();
  df_safe.Snapshot("rad_tree","../fitting/safesnap.root",cols,opts);
  
  //book histograms
  auto hs = df_safe.Histo2D({"hs","",100,-1,-1,100,-1,-1},"mc_s_photo","sigma");
  auto hQp2 = df_safe.Histo2D({"hQp2","",100,-1,-1,100,-1,-1},"mc_Qp2","sigma");
  auto ht = df_safe.Histo2D({"ht","",100,-1,-1,100,-1,-1},"mc_t_bot","sigma");
  auto hth = df_safe.Histo2D({"hth","",100,-1,-1,100,-1,-1},"mc_Heli_Theta","sigma");
  auto hphi = df_safe.Histo2D({"hphix","",100,-1,-1,100,-1,-1},"mc_Heli_Phi","sigma");
  
  //triget with foreach
  df_unsafe.Foreach([](double s,double Q2,double t,double theta,double phi, double sigma){
      std:: cout << "sigma: " << sigma << " s: " << s << " Qp2: " << Q2 << " t: " << t << " theta: " << theta << " phi: " << phi << std::endl;
    },{"mc_s_photo","mc_Qp2","mc_t_bot","mc_Heli_Theta","mc_Heli_Phi","sigma"});
  
  //draw after trigger
  new TCanvas();
  hs->DrawCopy("colz");
  new TCanvas();
  hQp2->DrawCopy("colz");
  new TCanvas();
  ht->DrawCopy("colz");
  new TCanvas();
  hth->DrawCopy("colz");
  new TCanvas();
  hphi->DrawCopy("colz");
    
  //final prints
  cout << "Ntotal: " << *nev << "   Nsafe: " << *nev_safe << endl;
  }
