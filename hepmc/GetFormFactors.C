#include "../Ye_Fit_Data/GetFF.C"
#include "/home/gp140f/SBS-Analysis/GEn/src/TheoryFunctions.cxx"

struct FormFactors{
  double q2;
  double gd;
  double tau;
  double alpha_p;
  double alpha_n;
  double F1p, dF1p;
  double F2p, dF2p;
  double F1n, dF1n;
  double F2n, dF2n;
  double GEp, dGEp;
  double GMp, dGMp;
  double GEn, dGEn;
  double GMn, dGMn;
};

FormFactors GetFormFactors(double q2){

  FormFactors ff;
  ff.q2 = q2;
  
  double gd = GetGD(q2);
  ff.gd = gd;
  
  double gep, dgep;
  double gmp, dgmp;
  double gen, dgen;
  double gmn, dgmn;
  
  GetFF(1,q2,&gep,&dgep);
  GetFF(2,q2,&gmp,&dgmp);
  GetFF(3,q2,&gen,&dgen);
  GetFF(4,q2,&gmn,&dgmn);

  double gep0 = 1.0;
  double gmp0 = 2.79284356;
  double gen0 = 1.0;
  double gmn0 = -1.91304272;
  
  gep = gep*gd*gep0;
  gmp = gmp*gd*gmp0;
  gen = gen*gd*gen0;
  gmn = gmn*gd*gmn0;
  
  dgep = dgep*gd*gep0;
  dgmp = dgmp*gd*gmp0;
  dgen = dgen*gd*gen0;
  dgmn = dgmn*gd*gmn0;

  ff.GEp = gep;
  ff.dGEp = dgep;
  ff.GMp = gmp;
  ff.dGMp = dgmp;

  ff.GEn = gen;
  ff.dGEn = dgen;
  ff.GMn = gmn;
  ff.dGMn = dgmn;
  
  double F1p,F2p;
  double dF1p,dF2p;
  double F1n,F2n;
  double dF1n,dF2n;
  
  double Mp = 0.938;
  double tau = q2 / (4*Mp*Mp);
    
  F1p = (gep + tau*gmp) / (1+tau);
  F2p = (gmp - gep) / (1+tau);
  dF1p = sqrt(pow(dgep/gep,2)+pow(dgmp/gmp,2))*F1p;
  dF2p = sqrt(pow(dgep/gep,2)+pow(dgmp/gmp,2))*F2p;

  F1n = (gen + tau*gmn) / (1+tau);
  F2n = (gmn - gen) / (1+tau);
  dF1n = sqrt(pow(dgen/gen,2)+pow(dgmn/gmn,2))*F1n;
  dF2n = sqrt(pow(dgen/gen,2)+pow(dgmn/gmn,2))*F2n;

  ff.F1p = F1p;
  ff.dF1p = dF1p;
  ff.F2p = F2p;
  ff.dF2p = dF2p;

  ff.F1n = F1n;
  ff.dF1n = dF1n;
  ff.F2n = F2n;
  ff.dF2n = dF2n;

  //tan = sin / cos
  //F1 = sqrt(S) cos alpha
  //F2 = sqrt(S) sin alpha
  //F2/F1 = tan(alpha)
  ff.alpha_p = atan(F2p/F1p);
  ff.alpha_n = atan(F2n/F1n);
    
  return ff;
}
