#include "GetFormFactors.C"

void DrawGraphWithBand(
    TGraphErrors *gr,
    Color_t lineColor,
    Color_t bandColor,
    float alpha = 0.30,
    int lineWidth = 2,
    bool drawAxes = true
)
{
    if (!gr) return;

    // ---- Draw error band ----
    gr->SetFillColorAlpha(bandColor, alpha);
    gr->SetFillStyle(1001);
    gr->SetMarkerStyle(0);

    TString optBand = drawAxes ? "A3" : "3";
    gr->Draw(optBand);

    // ---- Draw central line ----
    TGraphErrors *cgr = (TGraphErrors*)gr->Clone("cgr");
    cgr->SetFillStyle(0);
    cgr->SetLineColor(lineColor);
    cgr->SetLineWidth(lineWidth);
    cgr->Draw("LX SAME");
}


void DrawFF(){
  //SetMyStyle();
  gStyle->SetOptTitle(0);
  gErrorIgnoreLevel = kWarning;
  
  //theory curves
  const int nbins = 100;
  double q2_max = 2.0;
  
  TGraphErrors *gGEp = new TGraphErrors();
  TGraphErrors *gGMp = new TGraphErrors();
  TGraphErrors *gGEn = new TGraphErrors();
  TGraphErrors *gGMn = new TGraphErrors();

  TGraphErrors *gF1p = new TGraphErrors();
  TGraphErrors *gF2p = new TGraphErrors();
  TGraphErrors *gF1n = new TGraphErrors();
  TGraphErrors *gF2n = new TGraphErrors();

  TGraphErrors *galpha_p = new TGraphErrors();
  TGraphErrors *galpha_n = new TGraphErrors();
  
  for (int i=0; i<nbins; i++){
    double q2 = (q2_max/nbins) * i;

    auto ff = GetFormFactors(q2);
    
    double gep = ff.GEp;
    double gmp = ff.GMp;
    double gen = ff.GEn;
    double gmn = ff.GMn;

    double dgep = ff.dGEp;
    double dgmp = ff.dGMp;
    double dgen = ff.dGEn;
    double dgmn = ff.dGMn;

    double F1p = ff.F1p;
    double F2p = ff.F2p;
    double F1n = ff.F1n;
    double F2n = ff.F2n;

    double dF1p = ff.dF1p;
    double dF2p = ff.dF2p;
    double dF1n = ff.dF1n;
    double dF2n = ff.dF2n;

    double alpha_p = ff.alpha_p;
    double alpha_n = ff.alpha_n;
    
    galpha_p->AddPoint(q2,alpha_p);
    galpha_n->AddPoint(q2,alpha_n);

    //cout << "Q2: " << q2 << " " << F1p << " " << F2p << " " << alpha_p << endl;

    gGEp->AddPointError(q2, gep, 0, dgep);
    gGMp->AddPointError(q2, gmp, 0, dgmp);
    gGEn->AddPointError(q2, gen, 0, dgen);
    gGMn->AddPointError(q2, gmn, 0, dgmn);

    gF1p->AddPointError(q2, F1p, 0, dF1p);
    gF2p->AddPointError(q2, F2p, 0, dF2p);
    gF1n->AddPointError(q2, F1n, 0, dF1n);
    gF2n->AddPointError(q2, F2n, 0, dF2n);
 
  }

  gGEp->SetTitle("Proton Electric Form Factor;-q^{2} [GeV^{2}];G_{E}^{p}");
  gGMp->SetTitle("Proton Magnetic Form Factor;-q^{2} [GeV^{2}];G_{M}^{p}");
  gGEn->SetTitle("Neutron Electric Form Factor;-q^{2} [GeV^{2}];G_{E}^{n};");
  gGMn->SetTitle("Neutron Magnetic Form Factor;-q^{2} [GeV^{2}];G_{M}^{n}");

  gF1p->SetTitle("Proton Pauli Form Factor;-q^{2} [GeV^{2}];F_{1}^{p}");
  gF2p->SetTitle("Proton Dirac Form Factor;-q^{2} [GeV^{2}];F_{2}^{p}");
  gF1n->SetTitle("Neutron Pauli Form Factor;-q^{2} [GeV^{2}];F_{1}^{n}");
  gF2n->SetTitle("Neutron Dirac Form Factor;-q^{2} [GeV^{2}];F_{2}^{n}");
  
  galpha_p->SetTitle("Proton FF #alpha Angle;-q^{2} [GeV^{2}];#alpha_{p}");
  galpha_p->SetLineWidth(2);
  galpha_p->SetLineColor(kRed);
  
  galpha_n->SetTitle("Neutron FF #alpha Angle;-q^{2} [GeV^{2}];#alpha_{n}");
  galpha_n->SetLineWidth(2);
  galpha_n->SetLineColor(kBlue);
  
  TCanvas *c00 = new TCanvas("c00","Sach's Form Factors",1200,900);
  // --- title pad (top 7%) ---
  TPad *padTitle0 = new TPad("padTitle0", "",
			    0.0, 0.95, 1.0, 1.0);
  padTitle0->SetFillStyle(0);
  padTitle0->SetBorderSize(0);
  padTitle0->Draw();
  
  TPaveText *title0 = new TPaveText(0.25, 0.94, 0.75, 0.99, "NDC");
  title0->SetFillStyle(0);
  title0->SetBorderSize(0);
  title0->SetTextAlign(22);   // centered horizontally & vertically
  title0->SetTextFont(42);
  title0->SetTextSize(0.04);
  title0->AddText("Ye World Fit Parameterisation");
  title0->Draw();
  
  TPad *padMain0 = new TPad("padMain0", "",
			   0.0, 0.0, 1.0, 0.95);
  padMain0->Draw();
  padMain0->cd();
  padMain0->Divide(2,2);
  //c00->Divide(2,2);
  padMain0->cd(1);
  //c00->cd(1);
  gGEp->GetYaxis()->SetRangeUser(0,1.2);
  DrawGraphWithBand(gGEp, kBlue, kRed);
  padMain0->cd(2);
  //c00->cd(2);
  DrawGraphWithBand(gGMp, kBlue, kRed);
  padMain0->cd(3);
  //c00->cd(3);
  DrawGraphWithBand(gGEn, kBlue, kRed);
  padMain0->cd(4); 
  //c00->cd(4);
  DrawGraphWithBand(gGMn, kBlue, kRed);
  c00->Update();
  c00->Print("SachsEMFFs.png");


  TCanvas *c01 = new TCanvas("c01","Pauli Dirac Form Factors",1200,900);
  // --- title pad (top 7%) ---
  TPad *padTitle1 = new TPad("padTitle1", "",
			    0.0, 0.95, 1.0, 1.0);
  padTitle1->SetFillStyle(0);
  padTitle1->SetBorderSize(0);
  padTitle1->Draw();
  
  TPaveText *title1 = new TPaveText(0.25, 0.94, 0.75, 0.99, "NDDC");
  title1->SetFillStyle(0);
  title1->SetBorderSize(0);
  title1->SetTextAlign(22);   // centered horizontally & vertically
  title1->SetTextFont(42);
  title1->SetTextSize(0.04);
  title1->AddText("Decomposition to Pauli,Dirac F1,F2");
  title1->Draw();
  
  TPad *padMain1 = new TPad("padMain1", "",
			    0.0, 0.0, 1.0, 0.95);
  padMain1->Draw();
  padMain1->cd();
  padMain1->Divide(2,2);
  //c01->Divide(2,2);
  padMain1->cd(1);
  //c01->cd(1);
  DrawGraphWithBand(gF1p, kBlue, kRed);
  padMain1->cd(2);
  //c01->cd(2);
  DrawGraphWithBand(gF2p, kBlue, kRed);
  padMain1->cd(3);
  //c01->cd(3);
  DrawGraphWithBand(gF1n, kBlue, kRed);
  padMain1->cd(4);
  //c01->cd(4);
  DrawGraphWithBand(gF2n, kBlue, kRed);
  c01->Update();
  c01->Print("PauliDiracFFs.png");

  TCanvas *c02 = new TCanvas("c02","Proton Alpha Parameter",1200,900);
  galpha_p->Draw("AL");

  TCanvas *c03 = new TCanvas("c03","Neutron Alpha Parameter",1200,900);
  galpha_n->Draw("AL");
}
