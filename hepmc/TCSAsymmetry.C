#include <iostream>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TF1.h"

#include "HepMC3/GenEvent.h"

#include "TCSEvent.h"

using namespace std;

int FindBin(double x,
            const std::vector<double>& bins)
{
  for(unsigned int i=0;i<bins.size()-1;i++)
  {
    if(x >= bins[i] &&
       x <  bins[i+1])
      return i;
  }

  return -1;
}


int GetIndex(int iQ2,
             int it,
             int ic,
             int nt,
             int nCos)
{
    return iQ2*nt*nCos
         + it*nCos
         + ic;
}


struct AsymmetryBin
{
  TH1D* plus  = nullptr;
  TH1D* minus = nullptr;
};

struct AsymmetryBin2D
{
  TH2D* plus  = nullptr;
  TH2D* minus = nullptr;
};


void TCSAsymmetry(const char* plusFile =
		  //"/w/work6/home/gp140f/epic_generator_output/DDVCS/rootfiles/9x130_ddvcs_ee_hplus.hepmc3.tree.root",
		  "~/DDVCSdataset/test/10x100_tcs_ee_hplus.hepmc3.tree.root",
		  //"~/DDVCSdataset/test/10x100_ddvcs_ee_hplus.hepmc3.tree.root",
		  const char* minusFile =
		  //"/w/work6/home/gp140f/epic_generator_output/DDVCS/rootfiles/9x130_ddvcs_ee_hminus.hepmc3.tree.root",
		  "~/DDVCSdataset/test/10x100_tcs_ee_hminus.hepmc3.tree.root",
		  //"~/DDVCSdataset/test/10x100_ddvcs_ee_hminus.hepmc3.tree.root",
		  const bool fitclas = false)
{

  //--------------------------------------------------
  // Input
  //--------------------------------------------------

  TFile* fPlus = TFile::Open(plusFile);
  TFile* fMinus = TFile::Open(minusFile);

  if(!fPlus || !fMinus)
  {
    cout << "Failed to open files" << endl;
    return;
  }

  TTree* tPlus =
    (TTree*)fPlus->Get("hepmc3_tree");

  TTree* tMinus =
    (TTree*)fMinus->Get("hepmc3_tree");

  if(!tPlus || !tMinus)
  {
    cout << "Failed to locate trees" << endl;
    return;
  }

  HepMC3::GenEventData* evtPlus = nullptr;
  HepMC3::GenEventData* evtMinus = nullptr;

  tPlus->SetBranchAddress(
      "hepmc3_event",
      &evtPlus);

  tMinus->SetBranchAddress(
      "hepmc3_event",
      &evtMinus);

  //--------------------------------------------------
  // Binning
  //--------------------------------------------------
  std::vector<double> CLAS12_Q2Bins =  {
    2.25,
    9
  };
  std::vector<double> CLAS12_tBins =
  {
    0.15,
    0.25,
    0.34,
    0.48,
    0.8
  };
  std::vector<double> CLAS12_cosThetaBins =
    {
      -1.0,
      1.0
    };
  
  
  std::vector<double> my_Q2Bins =
  {
    2.0,
    // 8.0,
    // 14.0,
    20.0
  };

  std::vector<double> my_tBins =
  {
    0.0,
    // 0.001,
    // 0.01,
    // 0.1,
    1.0
  };

  std::vector<double> my_cosThetaBins =
  {
    -1.0,
    //    -0.5,
    0.0,
    // 0.5,
    1.0
  };

  auto Q2Bins = my_Q2Bins;
  auto tBins = my_tBins;
  auto cosThetaBins = my_cosThetaBins;
  if(fitclas){
   Q2Bins = CLAS12_Q2Bins;
   tBins = CLAS12_tBins;
   cosThetaBins = CLAS12_cosThetaBins;
  }
  
  const int nQ2 =
    Q2Bins.size()-1;

  const int nt =
    tBins.size()-1;

  const int nCos =
    cosThetaBins.size()-1;

  //--------------------------------------------------
  // Histogram booking
  //--------------------------------------------------

  vector<AsymmetryBin> bins;
  vector<AsymmetryBin2D> bins2D;
 
  for(int iQ2=0;iQ2<nQ2;iQ2++)
  {
    for(int it=0;it<nt;it++)
    {
      AsymmetryBin2D bin2D;
      TString tag2D =
	Form("Q2_%d_t_%d",
	     iQ2,it);
      
      bin2D.plus =
	new TH2D(
		 "plus_"+tag2D,
		 ";#phi [rad];Cos(#theta)",
		 10,
		 -TMath::Pi(),
		 TMath::Pi(),
		 10,
		 -1,1);

      bin2D.minus =
	new TH2D(
		 "minus_"+tag2D,
		 ";#phi [rad];Cos(#theta)",
		 10,
		 -TMath::Pi(),
		 TMath::Pi(),
		 10,
		 -1,1);

 
      bins2D.push_back(bin2D);
      
      for(int ic=0;ic<nCos;ic++)
      {
        AsymmetryBin bin;

        TString tag =
          Form("Q2_%d_t_%d_cos_%d",
               iQ2,it,ic);

        bin.plus =
          new TH1D(
              "plus_"+tag,
              ";#phi [rad];Counts",
              10,
              -TMath::Pi(),
               TMath::Pi());

        bin.minus =
          new TH1D(
              "minus_"+tag,
              ";#phi [rad];Counts",
              10,
              -TMath::Pi(),
               TMath::Pi());

        bins.push_back(bin);
      }
    }
  }
  
  
  //--------------------------------------------------
  // Fill plus helicity
  //--------------------------------------------------

  Long64_t nPlus =
    tPlus->GetEntries();

  cout
    << "Processing plus file: "
    << nPlus
    << " events"
    << endl;

  for(Long64_t i=0;i<nPlus;i++)
  {
    tPlus->GetEntry(i);

    TCSEvent evt =
      BuildTCSEvent(
          evtPlus,
          FrameMode::Raw);
    if(!PassCuts(evt))
      continue;
    
    int iQ2 =
      FindBin(evt.Qp2,Q2Bins);

    int it =
      FindBin(fabs(evt.t),tBins);

    int ic =
      FindBin(evt.cosTheta,
              cosThetaBins);

    if(iQ2 < 0 ||
       it  < 0 ||
       ic  < 0) 
      continue;
    bins2D[GetIndex(iQ2,it,0,nt,1)].plus->Fill(evt.phi, evt.cosTheta);
    bins[GetIndex(iQ2,it,ic,nt,nCos)].plus->Fill(evt.phi);
  }
  
  


  //--------------------------------------------------
  // Fill minus helicity
  //--------------------------------------------------

  Long64_t nMinus =
    tMinus->GetEntries();

  cout
    << "Processing minus file: "
    << nMinus
    << " events"
    << endl;

  for(Long64_t i=0;i<nMinus;i++)
  {
    tMinus->GetEntry(i);

    TCSEvent evt =
      BuildTCSEvent(
          evtMinus,
          FrameMode::Raw);
    if(!PassCuts(evt))
      continue;
    
    int iQ2 =
      FindBin(evt.Qp2,Q2Bins);

    int it =
      FindBin(fabs(evt.t),tBins);

    int ic =
      FindBin(evt.cosTheta,
              cosThetaBins);

    if(iQ2 < 0 ||
       it  < 0 ||
       ic  < 0)
      continue;

    bins2D[GetIndex(iQ2,it,0,nt,1)].minus->Fill(evt.phi, evt.cosTheta);
    bins[GetIndex(iQ2,it,ic,nt,nCos)].minus->Fill(evt.phi);
  }

  //--------------------------------------------------
  // Build asymmetries
  //--------------------------------------------------

  int canvasCounter = 0;
  
  TCanvas* c2D = new TCanvas("c2D",
			     "",
			     1600,
			     1200);
  c2D->Divide(nQ2,nt);
  
  for(int iQ2=0;iQ2<nQ2;iQ2++)
  {
    TCanvas* c = new TCanvas(Form("Q2_%d",iQ2),
			     "",
			     1600,
			     1200);
    
    TCanvas* cphi = new TCanvas(Form("phi_Q2_%d",iQ2),
			     "",
			     1600,
			     1200);

    if(fitclas){
      c->Divide(2,2);
      cphi->Divide(2,2);
    }else{
      c->Divide(nt,nCos);
      cphi->Divide(nt,nCos);
    }
    
    for(int it=0;it<nt;it++)
    {

      int jdx = GetIndex(iQ2, it, 0, nt, 1);
      auto title2D = Form("%.1f < Q^{'2} < %.1f, "
			  "%.3f < |t| < %.3f",
			  Q2Bins[iQ2],
			  Q2Bins[iQ2+1],
			  tBins[it],
			  tBins[it+1]);
      
      TH2D* plus2D = bins2D[jdx].plus;
      plus2D->SetTitle(title2D);
      
      TH2D* minus2D = bins2D[jdx].minus;
      minus2D->SetTitle(title2D);
      
      auto asym2D = plus2D->GetAsymmetry(minus2D);
      c2D->cd(iQ2 + nQ2*it +1)->SetLogz(0);
      asym2D->SetTitle(title2D);
      asym2D->Draw("colz");
      
      for(int ic=0;ic<nCos;ic++)
	{
	  int idx =
	    GetIndex(iQ2,it,ic,nt,nCos);

	  auto title = Form("%.1f < Q^{'2} < %.1f, "
			    "%.3f < |t| < %.3f, "
			    "%.1f < cos#theta < %.1f",
			    Q2Bins[iQ2],
			    Q2Bins[iQ2+1],
			    tBins[it],
			    tBins[it+1],
			    cosThetaBins[ic],
			    cosThetaBins[ic+1]);

	  TH1D* plus =
	    bins[idx].plus;
	  plus->SetTitle(title);
	  
	  TH1D* minus =
	    bins[idx].minus;
	  minus->SetTitle(title);

	  auto asym = plus->GetAsymmetry(minus);
	  asym->Scale(1/0.20);
	  asym->SetTitle(title);

	  // TH1D* asym = (TH1D*)plus->Clone(Form("Asym_Q2_%d_t_%d_cos_%d",
	  // 			    iQ2,it,ic));

	  // asym->Reset();

	  // for(int b=1;
	  //     b<=plus->GetNbinsX();
	  //     b++)
	  //   {
	  //     double Np =
	  // 	plus->GetBinContent(b);

	  //     double Nm =
	  // 	minus->GetBinContent(b);

	  //     if(Np + Nm <= 0)
	  // 	continue;

	  //     double A =
	  // 	(Np-Nm)/(Np+Nm);

	  //     double err =
	  // 	sqrt((1.0-A*A)
	  // 	     /(Np+Nm));

	  //     asym->SetBinContent(
	  // 			  b,A);

	  //     asym->SetBinError(
	  // 			b,err);
	  //   }
	  
	  if(fitclas)
	    c->cd(it+1);
	  else
	    c->cd(it*nCos + ic + 1);
	  
	  if(fitclas){
	    asym->SetMinimum(-0.75);
	    asym->SetMaximum(0.75);
	  }else{
	    asym->SetMinimum(-1.0);
	    asym->SetMaximum(1.0);
	  }
	  asym->Draw("E1");

	  // TF1* fit =
	  //   new TF1(Form("fit_%d",
	  // 		 canvasCounter),
	  // 	    "[0]*sin(x)",
	  // 	    -TMath::Pi(),
	  // 	    TMath::Pi());

	  // TF1* fit =
	  //   new TF1(Form("fit_%d", canvasCounter),
          //   "[0] + [1]*sin(x)",
          //   -TMath::Pi(),
          //   TMath::Pi());

	  // TF1* fit =
	  //   new TF1(Form("fit_%d", canvasCounter),
	  // 	    "[0] + [1]*sin(x) + [2]*cos(x)",
	  // 	    -TMath::Pi(),
	  // 	    TMath::Pi());

	  TF1* fit =
	    new TF1(Form("fit_%d", canvasCounter),
		    //"[0]"
		    //"+[1]*sin(x)"
		    //"+[2]*cos(x)"
		    //"+[3]*sin(2*x)"
		    "[0] + [1]*cos(2*x)",
		    -TMath::Pi(),
		    TMath::Pi());

	  // fit->SetParameter(0,1.0);
	  // fit->SetParameter(1,1.0);
	  // fit->SetParameter(2,1.0);

	  if(plus->Integral() + minus->Integral() > 50)
	    asym->Fit(fit,"Q");

	  if(fitclas)
	    cphi->cd(it+1);
	  else
	    cphi->cd(it*nCos + ic + 1);

	  plus->SetMinimum(0);
	  minus->SetMinimum(0);
	  plus->SetLineColor(kRed);
	  minus->SetLineColor(kBlue);
	  plus->Draw();
	  minus->Draw("same");
	
	}
    }
    //gStyle->SetOptStat(0);
    gStyle->SetOptFit(1);
    if(fitclas){
      c->Print("Asym_CLAS12bins.pdf");
      cphi->Print("Phi_CLAS12bins.pdf");
    }else{
      c->Print(Form("Asym_Q2_%d.pdf",iQ2));
      cphi->Print(Form("Phi_Q2_%d.pdf",iQ2));
    }
    c2D->Print("2D_CosTheta_Phi.pdf");

    //c->Close();
    //cphi->Close();
  }

  
  if(fitclas){
    gSystem->Exec("pdfunite Asym_CLAS12bins.pdf Phi_CLAS12bins.pdf 2D_CosTheta_Phi.pdfCLAS12bins.pdf");
    gSystem->Exec("rm Asym_CLAS12bins.pdf Phi_CLAS12bins.pdf 2D_CosTheta_Phi.pdf");
  }
  else{
    gSystem->Exec("pdfunite Asym_Q2*.pdf Phi_Q2*.pdf 2D_CosTheta_Phi.pdf AsymmetryBins.pdf");
    gSystem->Exec("rm Asym_Q2*.pdf Phi_Q2*.pdf 2D_CosTheta_Phi.pdf");
  }
  
  //  gSystem->Exit(0);
}
