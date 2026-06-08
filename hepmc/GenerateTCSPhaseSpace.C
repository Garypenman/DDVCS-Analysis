#include <TFile.h>
#include <TTree.h>
#include <TRandom3.h>
#include <cmath>
#include <iostream>

const double mp = 0.9382720813;

void GenerateTCSPhaseSpace(int N = 1e6,
			   double Ee = 18.0,
			   double Ep = 275.0)
{
    TRandom3 rng(0);

    double smin = (mp + 2*0.000511)*(mp + 2*0.000511);
    double smax = 4.0 * Ee * Ep;
    
    double qp2min = 2.0;
    double qp2max = 20.0;

    double tmin = 0.0001;
    double tmax = 2.0;
    
    // OUTPUT
    TFile *f = new TFile("TCSPhaseSpace.root","RECREATE");
    TTree *T = new TTree("tree","Correct TCS Phase Space");

    int event;
    double mc_s_photo, mc_Qp2, mc_t_bot, mc_ThetaHel, mc_PhiHel, mc_CosThetaHel;
    T->Branch("mc_s_photo",&mc_s_photo,"mc_s_photo/D");
    T->Branch("mc_Qp2",&mc_Qp2,"mc_Qp2/D");
    T->Branch("mc_t_bot",&mc_t_bot,"mc_t_bot/D");
    T->Branch("mc_ThetaHel",&mc_ThetaHel,"mc_ThetaHel/D");
    T->Branch("mc_CosThetaHel",&mc_CosThetaHel,"mc_CosThetaHel/D");
    T->Branch("mc_PhiHel",&mc_PhiHel,"mc_PhiHel/D");
    T->Branch("rdfentry_",&event);
    
    
    int acc = 0;
    int tries = 0;
    while (acc < N)
    {
      tries++;
      
      // Flat generation
      auto s    = rng.Uniform(smin, smax);
      auto t    = -rng.Uniform(tmin, tmax);      // physics uses negative t
      auto qp2  = rng.Uniform(qp2min, qp2max);

      auto costh = rng.Uniform(-1.0, 1.0);
      auto theta = acos(costh);
      auto phi   = rng.Uniform(-TMath::Pi(), TMath::Pi());

      // ---- compute kinematics ----
      double me=0.000511;
      double mp2=mp*mp;

      double beta2 = 1 - (4*me*me/qp2);
      if(beta2<0) continue;
      double beta = sqrt(beta2);

      double r2 = (s-qp2-mp2)*(s-qp2-mp2) - 4*qp2*mp2;
      if(r2<0) continue;
      double r=sqrt(r2);

      double tau = qp2 / (s-mp2);
      double delT2 = -t*(1-tau) - tau*tau*mp2;
      if(delT2<0) continue;
      double delT=sqrt(delT);
      
      // ---- compute b ----
      double C0 = beta*r;
      double C1 = beta * (qp2*(s-mp2-qp2) + t*(s-mp2+qp2)) / r;
      double C2 = -beta * 2*(s-mp2)*sqrt(qp2)*delT;
      double b = C1*cos(theta) + C2*sin(theta)*cos(phi);

      // ---- lepton propagator ----
      double L = ((qp2 - t)*(qp2 - t) - b*b) / 4.0;

      // physicality cut
      if (L <= 0) continue;

      mc_s_photo = s;
      mc_t_bot = -t;
      mc_Qp2 = qp2;
      mc_ThetaHel = theta;
      mc_CosThetaHel = costh;
      mc_PhiHel = phi;
      event = acc;
      
      T->Fill();
      acc++;
    }

    f->Write();
    f->Close();

    std::cout<<"Generated "<<acc<<" events in "<<tries<<" tries.\n"<<std::endl;
}
