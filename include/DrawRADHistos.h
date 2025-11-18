#pragma once

void DrawRADHistos(rad::histo::Histogrammer &histo){

  //Draw histograms
  TCanvas *c00 = new TCanvas("c00","Kinematics");
  c00->Divide(4,2);
  c00->cd(1);
  histo.DrawSame("Q2",gPad);
  c00->cd(2);
  histo.DrawSame("W",gPad);
  c00->cd(3);
  histo.DrawSame("Whad",gPad);
  c00->cd(4);
  histo.DrawSame("MesonMass",gPad);
  c00->cd(5);
  histo.DrawSame("ttop",gPad);
  c00->cd(6);
  histo.DrawSame("tbot",gPad);
  c00->cd(7);
  histo.DrawSame("tptop",gPad);
  c00->cd(8);
  histo.DrawSame("tpbot",gPad);
  c00->Print("temp00.pdf");

  TCanvas *c001 = new TCanvas("c001","Kinematics Reduced");
  c001->Divide(2,2);
  c001->cd(1)->SetLogy();
  histo.DrawSame("Q2",gPad);
  c001->cd(2);
  histo.DrawSame("W",gPad);
  c001->cd(3);
  histo.DrawSame("MesonMass",gPad);
  c001->cd(4);
  histo.DrawSame("tbot",gPad);
  c001->Print("temp001.pdf");

  TCanvas *c002 = new TCanvas("c002","Recoil Proton");
  c002->Divide(2,2);
  c002->cd(1)->SetLogy();
  histo.DrawSame("pprime_pmag",gPad);
  c002->cd(2);
  histo.DrawSame("pprime_eta",gPad);
  c002->cd(3);
  histo.DrawSame("pprime_theta",gPad);
  c002->cd(4);
  histo.DrawSame("pprime_phi",gPad);
  c002->Print("temp002.pdf");

  //histo.DrawSame("ttop",gPad);
  
  TCanvas *c01 = new TCanvas("c01","Exclusivity Plots");
  c01->Divide(3,2);
  c01->cd(1);
  histo.DrawSame("MissMass",gPad);
  c01->cd(2);
  histo.DrawSame("missP",gPad);
  c01->cd(3);
  histo.DrawSame("missPt",gPad);
  c01->cd(4);
  histo.DrawSame("missPz",gPad);
  c01->cd(5);
  histo.DrawSame("missTheta",gPad);
  //c01->cd(6);
  //histo.DrawSame("MissMass",gPad);
  c01->Print("temp01.pdf");

  TCanvas *c02 = new TCanvas("c02","2D t Distributions");
  c02->Divide(2,2);
  c02->cd(1);
  histo.Draw2DMC("tbot_mesonmass","colz",gPad);
  c02->cd(2);
  histo.Draw2DMC("tpbot_mesonmass","colz",gPad);
  c02->cd(3);
  histo.Draw2DMC("tbot_W","colz",gPad);
  c02->cd(4);
  histo.Draw2DMC("tpbot_W","colz",gPad);
  c02->Print("temp02.pdf");

  TCanvas *c03 = new TCanvas("c03","CM and PR Frame Angles");
  c03->Divide(2,2);
  c03->cd(1);
  histo.DrawSame("cthCM",gPad);
  c03->cd(2);
  histo.DrawSame("phCM",gPad);
  c03->cd(3);
  histo.DrawSame("cthPR",gPad);
  c03->cd(4);
  histo.DrawSame("phPR",gPad);
  c03->Print("temp03.pdf");

  TCanvas *c04 = new TCanvas("c04","Helicity and Polarisation");
  c04->Divide(2,2);
  c04->cd(1);
  histo.DrawSame("Heli_CosTheta",gPad);
  c04->cd(2);
  histo.DrawSame("Heli_Phi",gPad);
  c04->cd(3);
  histo.DrawSame("GammaPol",gPad);
  c04->cd(4);
  histo.DrawSame("GammaE",gPad);
  c04->Print("temp04.pdf");

  TCanvas *c05 = new TCanvas("c05","DIS Kinematics");
  c05->Divide(2,2);
  c05->cd(1);
  histo.DrawSame("Q2",gPad);
  c05->cd(2);
  histo.DrawSame("nu",gPad);
  c05->cd(3);
  histo.DrawSame("xbj",gPad);
  c05->cd(4);
  histo.DrawSame("y",gPad);
  c05->Print("temp05.pdf");

  TCanvas *c06 = new TCanvas("c06","Kin Lim CircPol Correlations");
  c06->Divide(2,2);
  c06->cd(1);
  histo.Draw2DMC("y_W","colz",gPad);
  c06->cd(2);
  histo.Draw2DMC("y_Escatele","colz",gPad);
  c06->cd(3);
  histo.Draw2DMC("y_CircPol","colz",gPad);
  c06->cd(4);
  histo.Draw2DMC("W_CircPol","colz",gPad);
  c06->Print("temp06.pdf");

}
