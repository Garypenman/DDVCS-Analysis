#include "../include/Config.h"

void SnapshotBrufit(std::string infile = "/w/work6/home/gp140f/combirad_trees/HepMC_TCS_18x275_Phasespace/TCS_mc_Tree.root", std::string outfile = "/w/work6/home/gp140f/combirad_trees/HepMC_TCS_18x275_Phasespace/TCS_mc_Tree_brufit.root"){

  ROOT::EnableImplicitMT(8);
  ROOT::RDataFrame df("tree",infile);
  
  auto df_new0 = df.Define("r2",[](double s, double Qp2){
      double M=0.938;
      double M2=M*M;
      return (s-Qp2-M2)*(s-Qp2-M2) - 4*Qp2*M2;
    },{"mc_s_photo","mc_Qp2"});
  
  auto df_new = df_new0.Filter("r2>0")
    .Define("r", [] (double r2){
      return sqrt(r2);
    },{"r2"})
    .Define("beta", [](double Qp2){
      double me = 0.000511;
      return sqrt(1 - (4*me*me/Qp2));
    },{"mc_Qp2"})
    .Define("C0", [](double beta, double r){
      return beta*r;
    },{"beta","r"})
    .Define("C1", [](double beta, double Qp2, double s, double tpos, double r){
      auto t = -tpos;
      double M = 0.938;
      double M2=M*M;
      auto C1 = beta * (Qp2*(s-M2-Qp2) + t*(s-M2+Qp2)) / r;
      return C1;
    },{"beta","mc_Qp2","mc_s_photo","mc_t_bot","r"})
    .Define("C2", [](double beta, double s, double Qp, double delT, double r){
      double M = 0.938;
      double M2=M*M;
      auto C2 = -beta * 2*(s-M2)*Qp*delT / r;
      return C2;
    },{"beta","mc_s_photo","mc_Qp","mc_DeltaT","r"})
    .Define("b", [](double C1, double C2, double theta, double phi){
      return C1*cos(theta) + C2*sin(theta)*cos(phi);
    },{"C1","C2","mc_ThetaHel","mc_PhiHel"})
    .Define("a", [](double C0, double theta){
      return C0*cos(theta);
    },{"C0","mc_ThetaHel"})
    .Define("L", [](double Qp2, double tpos, double b){
      auto t=-tpos;
      return ((Qp2-t)*(Qp2-t) - b*b) / 4;
    },{"mc_Qp2","mc_t_bot","b"})
    .Define("A0",[](double s, double deltaT){
      double M = 0.938;
      double M2=M*M;
      return pow((s-M2)*deltaT,2);
    },{"mc_s_photo","mc_DeltaT"}).
    Define("A1",[](double tpos, double a, double b){
      auto t=-tpos;
      return t*a*(a+b);
    },{"mc_t_bot","a","b"}).
    Define("A2",[](double b){
      double M = 0.938;
      return M*M*b*b;
    },{"b"})
    .Define("A3",[](double tpos, double Qp2){
      auto t=-tpos;
      double M = 0.938;
      double M2=M*M;
      return t*(4*M2-t)*Qp2;
    },{"mc_t_bot","mc_Qp2"})
    .Define("A4",[](double Qp2, double tpos, double a, double b, double s){
      auto t=-tpos;
      double M = 0.938;
      double M2=M*M;
      return pow((Qp2-t)*(a+b) - (s-M2)*b,2);
    },{"mc_Qp2","mc_t_bot","a","b","mc_s_photo"})
    .Define("A5",[](double tpos, double Qp2){
      auto t=-tpos;
      double M = 0.938;
      double M2=M*M;
      return t*(4*M2-t)*pow(Qp2-t,2);
    },{"mc_t_bot","mc_Qp2"})
    .Define("A",[](double A0, double A1, double A2, double A3, double A4, double A5, double L){
      double me = 0.000511;
      return A0-A1-A2-A3-((me*me/L)*(A4+A5));
    },{"A0","A1","A2","A3","A4","A5","L"})
    .Define("B0",[](double Qp2, double tpos){
      auto t=-tpos;
      return pow(Qp2+t,2);
    },{"mc_Qp2","mc_t_bot"})
    .Define("B1",[](double b){
      return b*b;
    },{"b"})
    .Define("B2",[](double Qp2){
      double me = 0.000511;
      return 8*me*me*Qp2;
    },{"mc_Qp2"})
    .Define("B3",[](double tpos, double Qp2, double L){
      double me = 0.000511;
      auto t=-tpos;
      return 4*me*me*(t+2*me*me)*pow(Qp2-t,2)/L;
    },{"mc_t_bot","mc_Qp2","L"})
    .Define("B",[](double B0, double B1, double B2, double B3){
      return B0+B1+B2-B3;
    },{"B0","B1","B2","B3"})
    .Define("BHprefac",[](double beta, double s, double tpos, double Qp2, double L){
      double M = 0.938;
      double M2=M*M;
      auto alpha = 1.0/137.0;
      auto alpha3 = alpha*alpha*alpha;
      auto t=-tpos;
      return alpha3*beta / (4*TMath::Pi()*pow(s-M2,2)*(-t*L));
    },{"beta","mc_s_photo","mc_t_bot","mc_Qp2","L"})
    .Define("F1","1.00") //placeholder
    .Define("F2","1.79")
    .Define("d4sigma",[](double prefac, double A, double B, double F1, double F2, double tpos){
      auto t=-tpos;
      double M = 0.938;
      double M2=M*M;
      auto tau = t/(4*M2);
      return prefac*((F1*F1 - tau*F2*F2)*A/(-t) + pow(F1+F2,2)*B/2);
    },{"BHprefac","A","B","F1","F2","mc_t_bot"});
    
    
  
    
    
    
  auto df_filt = df_new.Filter("r2>0");
  df_filt = df_filt.Filter("d4sigma>0");
  
  auto nstart = df.Count();
  auto nsnap = df_filt.Count();
  
  ROOT::RDF::RSnapshotOptions opts;
  opts.fLazy = false;
  opts.fOverwriteIfExists = true;
  opts.fMode = "RECREATE";
  
  brufit_cols.push_back("r");
  brufit_cols.push_back("r2");
  brufit_cols.push_back("beta");
  brufit_cols.push_back("C0");
  brufit_cols.push_back("C1");
  brufit_cols.push_back("C2");
  brufit_cols.push_back("b");
  brufit_cols.push_back("a");
  brufit_cols.push_back("L");
  brufit_cols.push_back("A0");
  brufit_cols.push_back("A1");
  brufit_cols.push_back("A2");
  brufit_cols.push_back("A3");
  brufit_cols.push_back("A4");
  brufit_cols.push_back("A5");
  brufit_cols.push_back("B0");
  brufit_cols.push_back("B1");
  brufit_cols.push_back("B2");
  brufit_cols.push_back("B3");
  brufit_cols.push_back("A");
  brufit_cols.push_back("B");
  brufit_cols.push_back("F1");
  brufit_cols.push_back("F2");
  brufit_cols.push_back("BHprefac");
  brufit_cols.push_back("d4sigma");
  

  df_filt.Snapshot("tree",outfile,brufit_cols,opts);

  std::cout << "Base events: " << *nstart << std::endl;
  std::cout << "Filtered events: " << *nsnap << std::endl;
  
}

void SnapshotBrufit(std::string dir, int dummy){
  
  std::string infile = dir+"TCS_mc_Tree.root";
  std::string outfile = dir+"TCS_mc_Tree_brufit.root";
  SnapshotBrufit(infile,outfile);
}  
