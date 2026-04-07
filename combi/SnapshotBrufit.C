#include "../include/Config.h"

void SnapshotBrufit(std::string infile = "/w/work5/home/garyp/combirad_trees/HepMC_TCS_18x275_Phasespace/TCS_mc_Tree.root", std::string outfile = "/w/work5/home/garyp/combirad_trees/HepMC_TCS_18x275_Phasespace/TCS_mc_Tree_brufit.root"){
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
      },{"mc_Qp2","mc_t_bot","b"});

  auto df_filt = df_new.Filter("r2>0");
  
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
  

  df_filt.Snapshot("tree",outfile,brufit_cols,opts);

  std::cout << "Base events: " << *nstart << std::endl;
  std::cout << "Filtered events: " << *nsnap << std::endl;
  
}

void SnapshotBrufit(std::string dir, int dummy){
  
  std::string infile = dir+"TCS_mc_Tree.root";
  std::string outfile = dir+"TCS_mc_Tree_brufit.root";
  SnapshotBrufit(infile,outfile);
}  
