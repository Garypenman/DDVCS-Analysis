#pragma once

#include <string>
#include <vector>
#include <cmath>
#include <TString.h>

//particle struct here
struct ParticleConfig {
  std::string name;
  std::string title;
  std::string label;
  int pdg;
  
  double acc = -1.0;      // integrated acceptance
  double acc_err = 0.0;   // optional binomial error
  double fidu_acc = -1.0; // integrated acceptance in fiducial cuts
  double fidu_acc_err = 0.0;
  
  // files (automatically set)
  std::string tru_filename;
  std::string rec_filename;
  
  // branches (auto-filled)
  std::string tru_p;
  std::string tru_theta;
  std::string tru_phi;
  std::string tru_eta;

  std::string rec_p;
  std::string rec_theta;
  std::string rec_phi;
  std::string rec_eta;

  std::string res_p;
  std::string res_theta;
  std::string res_phi;
  std::string res_eta;
  
  // axis ranges
  int nbins_acc, nbins_res;
  double p_min, p_max, p_min_res, p_max_res;
  double t_min, t_max, t_min_res, t_max_res;
  double phi_min, phi_max, phi_min_res, phi_max_res;
  double eta_min, eta_max, eta_min_res, eta_max_res;

  // ----------------------------------------------------------
  // Constructor
  // ----------------------------------------------------------
  ParticleConfig(const std::string& pname, const std::string& ptitle, const std::string& plabel,
		 int Ipdg,
                 int nbacc, int nbres,
		 double pmin, double pmax, double pminres, double pmaxres,
                 double tmin, double tmax, double tminres, double tmaxres,
		 double phimin, double phimax, double phiminres, double phimaxres,
                 double etamin, double etamax, double etaminres, double etamaxres)
    : name(pname), title(ptitle), label(plabel),
      pdg(Ipdg),
      nbins_acc(nbacc), nbins_res(nbres),
      p_min(pmin), p_max(pmax), p_min_res(pminres), p_max_res(pmaxres),
      t_min(tmin), t_max(tmax), t_min_res(tminres), t_max_res(tmaxres),
      phi_min(phimin), phi_max(phimax), phi_min_res(phiminres), phi_max_res(phimaxres),
      eta_min(etamin), eta_max(etamax), eta_min_res(etaminres), eta_max_res(etamaxres)
  {
    UpdateDerived();
  }
  void UpdateDerived(){

    // automatic file naming
    tru_filename = "TCS_" + name + "_tru_Tree.root";
    rec_filename = "TCS_" + name + "_rec_Tree.root";
  
    // automatic branch naming
    tru_p     = "tru_" + name + "_pmag";
    tru_theta = "tru_" + name + "_theta";
    tru_phi   = "tru_" + name + "_phi";
    tru_eta   = "tru_" + name + "_eta";

    rec_p     = "rec_" + name + "_pmag";
    rec_theta = "rec_" + name + "_theta";
    rec_phi   = "rec_" + name + "_phi";
    rec_eta   = "rec_" + name + "_eta";

    res_p     = "res_" + name + "_pmag";
    res_theta = "res_" + name + "_theta";
    res_phi   = "res_" + name + "_phi";
    res_eta   = "res_" + name + "_eta";
  }
};
