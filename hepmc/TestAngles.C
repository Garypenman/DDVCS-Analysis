#include "BHxsec.C"
#include "Constants.h"

using ROOT::Math::PxPyPzMVector;
using ROOT::Math::VectorUtil::boost;
using ROOT::Math::VectorUtil::RotateY;

struct kin{
  double ebeam;
  double pbeam;
  double y;
  double t;
  
  kin(double ebeam, double pbeam, double y, double t)
    : ebeam(ebeam), pbeam(pbeam), y(y), t(t) {} 
};

kin eic(18,275,0.5,0.01);
kin jlab(10.6,0,0.5,0.1);

void TestAngles(kin thekin=eic){
  
  PxPyPzMVector e(0,0,-thekin.ebeam,me);
  PxPyPzMVector p(0,0,thekin.pbeam,M);
  
  auto boost_to_tar = p.BoostToCM();
  e = boost(e,boost_to_tar);
  
  auto E=e.E();
  
  auto y = thekin.y;
  auto t = -thekin.t;
  auto Qp2 = 2.0;
  
  auto Q2 = y*y*me*me/(1-y);
  auto s = M*M + 2*M*y*E - Q2;
  
  auto phiS = 0.0;
  auto phi = 0.0;
  
  cout << "E(tar rest), y, Q2, t, s, Qp2, phiS, phi" << endl;
  cout << E << " " << y << " " << Q2 << " " << t << " " << s << " " << Qp2 << " " << phiS << " " << phi << " " << endl;
  cout << "Will randomly generate phiL and cos(thetaL) uniformly!" << endl;
  
  TH1D *hCosTheta_gen = new TH1D("hCosTheta_gen","Generated;cos(#theta_{l})",100,-1,1);
  TH1D *hPhi_gen = new TH1D("hPhi_gen","Generated;#phi_{l}",100,-M_PI,M_PI);
  TH2D *hCosTheta_Phi_gen = new TH2D("hCosTheta_Phi_gen","Generated;phi_{l} [rad];cos(#theta_{l})",100,-M_PI,M_PI,100,-1,1);
  
  TH1D *hCosTheta = new TH1D("hCosTheta","Weighted d^{4}_{BH};cos(#theta_{l})",100,-1,1);
  TH1D *hPhi = new TH1D("hPhi","Weighted d^{4}_{BH};#phi_{l}",100,-M_PI,M_PI);
  TH2D *hCosTheta_Phi = new TH2D("hCosTheta_Phi","Weighted d^{4}_{BH};phi_{l} [rad];cos(#theta_{l})",100,-M_PI,M_PI,100,-1,1);
  
  
  TH1D *ht_gen = new TH1D("ht_gen","Generated;|t| [GeV^{2}]",100,0,1);
  TH1D *hy_gen = new TH1D("hy_gen","Generated;y",100,0,1);
  TH1D *hQp2_gen = new TH1D("hQp2_gen","Generated;Q'^{2} = M_{ee}^{2} [GeV^{2}] ",100,0,20);
  TH1D *hs_gen = new TH1D("hs_gen","Generated;s [GeV^{2}]",100,-1,-1);
  
  TH1D *ht = new TH1D("ht",";|t| [GeV^{2}]",100,0,1);
  TH1D *hy = new TH1D("hy",";y",100,0,1);
  TH1D *hQp2 = new TH1D("hQp2",";Q'^{2} = M_{ee}^{2} [GeV^{2}] ",100,0,20);
  TH1D *hs = new TH1D("hs",";s [GeV^{2}]",100,-1,-1);
  
  int nev=1e6;
  TRandom3 rand;
  for (int ev=0; ev<nev; ev++){
    
    //y = rand.Uniform(0,1);
    //y = rand.expo(0.3);
    //t = rand.Uniform(0,1);
    //t = -t;
    //Qp2 = rand.Uniform(2.0, 18.0);
    //Qp2 = 100.0;
    Q2 = y*y*me*me/(1-y);
    s = M*M + 2*M*y*E - Q2;
  
    auto costheta = rand.Uniform(-1,1);
    auto phi = rand.Uniform(-M_PI,M_PI);
    hCosTheta_gen->Fill(costheta);
    hPhi_gen->Fill(phi);
    hCosTheta_Phi_gen->Fill(phi,costheta);
    hy_gen->Fill(y);
    ht_gen->Fill(-t);
    hQp2_gen->Fill(Qp2);
    hs_gen->Fill(s);
    
    auto theta = std::acos(costheta);
    auto sig = dsigma_BH(s,Qp2,t,theta,phi);
    if(sig<0 || std::isnan(sig)) {
      //cout << "problem" << endl;
      continue;
    }
    hCosTheta->Fill(costheta,sig);
    hPhi->Fill(phi,sig);
    hCosTheta_Phi->Fill(phi,costheta,sig);
    hy->Fill(y,sig);
    ht->Fill(-t,sig);
    hQp2->Fill(Qp2,sig);
    hs->Fill(s,sig);
    
  }
  
  TCanvas *c00 = new TCanvas();
  c00->Divide(2,2);
  c00->cd(1);
  hCosTheta_gen->SetMinimum(0);
  hCosTheta_gen->Draw();
  c00->cd(2);
  hPhi_gen->SetMinimum(0);
  hPhi_gen->Draw();
  c00->cd(3);
  hCosTheta->SetMinimum(0);
  hCosTheta->Draw("hist");
  c00->cd(4);
  hPhi->SetMinimum(0);
  hPhi->Draw("hist");
  
  new TCanvas();
  gPad->SetLogz();
  hCosTheta_Phi->Draw("colz");

  TCanvas *c01 = new TCanvas();
  c01->Divide(2,2);
  c01->cd(1);
  hy_gen->SetMinimum(0);
  hy_gen->Draw("hist");
  c01->cd(2);
  ht_gen->SetMinimum(0);
  ht_gen->Draw("hist");
  c01->cd(3);
  hQp2_gen->Draw("hist");
  c01->cd(4);
  hs_gen->Draw("hist");
  
  TCanvas *c02 = new TCanvas();
  c02->Divide(2,2);
  c02->cd(1)->SetLogy();
  hy->Draw("hist");
  c02->cd(2)->SetLogy();
  ht->Draw("hist");
  c02->cd(3)->SetLogy();
  hQp2->Draw("hist");
  c02->cd(4)->SetLogy();
  hs->Draw("hist");
}
