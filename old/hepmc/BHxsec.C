#include "Constants.h"

using ROOT::Math::PxPyPzMVector;

const double alpha = rad::constant::alpha();
const double M = rad::constant::M_pro();
const double me = rad::constant::M_ele(); 

double dsigma_BH_vec(const PxPyPzMVector &ebeam, const PxPyPzMVector &escat, const PxPyPzMVector &p, const PxPyPzMVector &pp,  const PxPyPzMVector &k,  const PxPyPzMVector &kp) {
  const double s = (ebeam+p).M2(); 
  
    auto q = ebeam-escat;
    auto Q2 = -q.M2();
    auto delt = pp-p;
    auto delt_T = sqrt(delt.Perp2());
    auto t = delt.M2();
    auto qp = k+kp;
    auto Qp2 = qp.M2();
    auto tau = Qp2 / (2*p.Dot(q));
    auto xb = Q2 / (2*p.Dot(q));

    // Scalar products (simplified placeholders)
    double a = 2.0 * (k-kp).Dot(pp); // placeholder for 2(k - k')·p'
    double b = 2.0 * (k-kp).Dot(p-pp);       // placeholder for 2(k - k')·(p - p')
      
  //return dsigma_BH(s,Qp2,t,theta,phi);
  return 0; //fix later
}

double dsigma_BH(const double s, const double Q2, const double t, const double theta, const double phi){
  
  // Form factors (dipole approximation)
  double Lambda2 = 0.71;
  double GD = 1.0 / pow(1.0 - t / Lambda2, 2);
  double Tau = -t / (4*M*M);
  double mu_p = 2.79;
  double mu_n = -1.91;
  double GMp = mu_p*GD;
  double GEp = GD;
  
  double F1p = (GEp+Tau*GMp) / (1+Tau);
  double F2p = (GMp - GEp) / (1+Tau);
  
  double F1 = F1p;
  double F2 = F2p;

  //kinematics
  double beta = sqrt(1.0 - 4.0*me*me / Q2);
  double r = sqrt((s-Q2-M*M)*(s-Q2-M*M)  - 4*Q2*M*M);
  double tau = Q2/(s-M*M);
  double delt_T2 = -t*(1-tau) - tau*tau*M*M;
  double delt_T = sqrt(delt_T2);
  
  //propagator coefficients
  double C0 = beta * r;
  double C1 = beta * (Q2*(s-M*M-Q2) + t*(s-M*M+Q2)) / r;
  double C2 = beta * (2*(s-M*M)*sqrt(Q2)*delt_T) / r;
  //cout << C0 << " " << C1 << " " << C2 << endl;
  //C0 >> C1,C2 due to large s in collider kinematics, in general
  
  
  double a = C0 * cos(theta);
  double a2 = a*a;
  double b = C1 * cos(theta) - C2 * sin(theta)*cos(phi);
  double b2 = b*b;
  double ab = a*b;
  
  //lepton propagator
  double L = ((Q2-t)*(Q2-t) - b*b)/4;
  double L0 = Q2*Q2*sin(theta)*sin(theta)/4;
  //cout << "L: " <<  L << "  L0: " << L0 << endl;
  
  
  //A term expanded
  double AK0 = (s-M*M)*(s-M*M)*delt_T*delt_T - t*(4*M*M-t)*Q2; //static
  double AK1 = -t*C1*C1; //cth
  double AK2 = -t*C0*C1; //c^2th
  double AK3 = -t*(-C0*C2); //cth sth cph
  double AK4 = -M*M*(C1*C1); //c^2th
  double AK5 = -M*M*(C2*C2); //s^2th cph
  double AK6 = -M*M*(-2*C1*C2); //cth sth cph
  
  double A0 = AK0;
  double A1 = AK1;
  double A2 = AK2 + AK4;
  double A3 = AK3 + AK6;
  double A4 = AK5;
  //cout << A0 << " " << A1 << " " << A2 << " " << A3 << " " << A4 << endl;
  
  //A and B terms
  double A = pow(s-M*M, 2) * delt_T*delt_T - t*a*(a + b) - M*M*b*b - t*(4*M*M-t)*Q2 + (me*me/L)*( pow((Q2-t)*(a+b)-(s-M*M)*b,2) + t*(4*M*M-t)*pow(Q2-t,2) );
  double B = pow(Q2+t, 2) + b*b + 8*me*me*Q2 - 4*me*me*(t+2*me*me)*pow(Q2-t, 2)/L;
  
  // Differential cross section
  double kinfac = (beta/(-t)) * alpha * alpha * alpha / (4.0 * TMath::Pi() * pow(s - M * M, 2));
  
  double GE2_red = (F1*F1 - (t/(4*M*M))*F2*F2);
  double GM2 = pow(F1 + F2, 2);

  double sigma = kinfac * (1.0/L) * ( GE2_red*A/-t + GM2*B/2.0 );
  
  if(sigma<0 || isnan(sigma)){
    cout << "Problem, unphysical sigma!" << endl;
    cout << "KinFac*1/L*(GE2_red*Aterm + GM2*Bterm)" << endl;
    cout << kinfac << " " << 1.0/L << " " << GE2_red << " " << A/-t << " " << GM2 << " " << B/2.0 << endl;
    cout << endl;
  }
  
  //sigma = 1.0/L; //test shape
  return sigma; // Units: GeV^-4
}
