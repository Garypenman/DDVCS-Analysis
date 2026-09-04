#include <iostream>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TMath.h"

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"

#include "TCSEvent.h"

using namespace std;
using namespace HepMC3;

constexpr int NPART = 8;

const char* particleNames[NPART] = {
    "eBeam",
    "eScat",
    "gammaEx",
    "pBeam",
    "gammaStar",
    "pReco",
    "eMinus",
    "ePlus"
};


void CompareHepMC(const char *infilename1 = "/w/work6/home/gp140f/epic_generator_output/DDVCS/rootfiles/9x130_ddvcs_ee_hplus.hepmc3.tree.root",
		  const char *infilename2 = "/w/work6/home/gp140f/epic_generator_output/DDVCS/afterburned/ab_9x130_ddvcs_ee_hplus.hepmc3.tree.root")
{

  //----------------------------------------------------------
  // File 1 input
  //----------------------------------------------------------
  
  TFile *infile1 = TFile::Open(infilename1);
  
  if (!infile1 || infile1->IsZombie()) {
    cout << "Cannot open file " << infilename1 << endl;
    return;
  }

  TTree *tree1 = (TTree*)infile1->Get("hepmc3_tree");

  if (!tree1) {
    cout << "Could not find TTree named 'hepmc3_tree' in " << infilename1 << endl;
    return;
  }

  
  //----------------------------------------------------------
  // File 2 input
  //----------------------------------------------------------
  
  TFile *infile2 = TFile::Open(infilename2);
  
  if (!infile2 || infile2->IsZombie()) {
    cout << "Cannot open file " << infilename2 << endl;
    return;
  }

  TTree *tree2 = (TTree*)infile2->Get("hepmc3_tree");

  if (!tree2) {
    cout << "Could not find TTree named 'hepmc3_tree' in " << infilename2 << endl;
    return;
  }

  
  //----------------------------------------------------------
  // Input branches
  //----------------------------------------------------------
  
  HepMC3::GenEventData* evt1 = nullptr;
  tree1->SetBranchAddress("hepmc3_event",
			 &evt1);
  HepMC3::GenEventData* evt2 = nullptr;
  tree2->SetBranchAddress("hepmc3_event",
			  &evt2);
  
  
  //----------------------------------------------------------
  // Histograms
  //----------------------------------------------------------
  auto ParticleNames = TCSEvent::ParticleNames;
  TH1D* h_px_A[TCSEvent::kNParticles];
  TH1D* h_py_A[TCSEvent::kNParticles];
  TH1D* h_pz_A[TCSEvent::kNParticles];

  TH1D* h_px_B[TCSEvent::kNParticles];
  TH1D* h_py_B[TCSEvent::kNParticles];
  TH1D* h_pz_B[TCSEvent::kNParticles];

  TH1D* h_px_C[TCSEvent::kNParticles];
  TH1D* h_py_C[TCSEvent::kNParticles];
  TH1D* h_pz_C[TCSEvent::kNParticles];

  TH1D* h_dpx[TCSEvent::kNParticles];
  TH1D* h_dpy[TCSEvent::kNParticles];
  TH1D* h_dpz[TCSEvent::kNParticles];

  for(int i=0;i<TCSEvent::kNParticles;i++)
    {
      h_px_A[i] = new TH1D(
			   Form("px_A_%s",ParticleNames[i]),
			   Form("%s px (A);p_{x} [GeV]",ParticleNames[i]),
			   200,-5,5);

      h_py_A[i] = new TH1D(
			   Form("py_A_%s",ParticleNames[i]),
			   Form("%s py (A);p_{y} [GeV]",ParticleNames[i]),
			   200,-5,5);

      h_pz_A[i] = new TH1D(
			   Form("pz_A_%s",ParticleNames[i]),
			   Form("%s pz (A);p_{z} [GeV]",ParticleNames[i]),
			   200,-150,150);

      h_px_B[i] = new TH1D(
			   Form("px_B_%s",ParticleNames[i]),
			   Form("%s px (B);p_{x} [GeV]",ParticleNames[i]),
			   200,-5,5);

      h_py_B[i] = new TH1D(
			   Form("py_B_%s",ParticleNames[i]),
			   Form("%s py (B);p_{y} [GeV]",ParticleNames[i]),
			   200,-5,5);

      h_pz_B[i] = new TH1D(
			   Form("pz_B_%s",ParticleNames[i]),
			   Form("%s pz (B);p_{z} [GeV]",ParticleNames[i]),
			   200,-150,150);
      
      h_px_C[i] = new TH1D(
			   Form("px_C_%s",ParticleNames[i]),
			   Form("%s px (B);p_{x} [GeV]",ParticleNames[i]),
			   200,-5,5);

      h_py_C[i] = new TH1D(
			   Form("py_C_%s",ParticleNames[i]),
			   Form("%s py (B);p_{y} [GeV]",ParticleNames[i]),
			   200,-5,5);

      h_pz_C[i] = new TH1D(
			   Form("pz_C_%s",ParticleNames[i]),
			   Form("%s pz (B);p_{z} [GeV]",ParticleNames[i]),
			   200,-150,150);

      h_dpx[i] = new TH1D(
			  Form("dpx_%s",ParticleNames[i]),
			  Form("%s #Delta p_{x};p_{x}^{A}-p_{x}^{B}",
			       ParticleNames[i]),
			  200,-1,1);

      h_dpy[i] = new TH1D(
			  Form("dpy_%s",ParticleNames[i]),
			  Form("%s #Delta p_{y};p_{y}^{A}-p_{y}^{B}",
			       ParticleNames[i]),
			  200,-1,1);

      h_dpz[i] = new TH1D(
			  Form("dpz_%s",ParticleNames[i]),
			  Form("%s #Delta p_{z};p_{z}^{A}-p_{z}^{B}",
			       ParticleNames[i]),
			  200,-1,1);
    }

  TH1D *h_dQ2 = new TH1D("h_dQ2",";#DeltaQ^{2} [GeV^{2}];Events",100,-1,-1);
  TH1D *h_dt = new TH1D("h_dt",";#Deltat [GeV^{2}];Events",100,-1,-1);
  TH1D *h_dMM2 = new TH1D("h_dMM2",";#DeltaM_{miss}^{2} [GeV^{2}/c^{4}];Events",100,-1,-1);
  
  TH1D *h_MissMass2_A = new TH1D("h_MissMass2_A","BaseFile;M_{miss}^{2};Events",100,-1,1);
  TH1D *h_MissMass2_B = new TH1D("h_MissMass2_B","ABFile;M_{miss}^{2};Events",100,-1,1);
  TH1D *h_MissMass2_C = new TH1D("h_MissMass2_C","ABFile Corrected;M_{miss}^{2};Events",100,-1,1);
  
  
  //----------------------------------------------------------
  //Event loop
  //----------------------------------------------------------
  Long64_t nEvents1 = tree1->GetEntries();
  Long64_t nEvents2 = tree2->GetEntries();
  if(nEvents1 != nEvents2){
    cout << "Mismatch in number of events between files!" << endl;
    exit(1);
  }else{
    cout << "Processing " << nEvents1 << " events" << endl;
  }


  for(Long64_t i=0;i<nEvents1;i++){

    tree1->GetEntry(i);
    tree2->GetEntry(i);

    
    //generator truth beams
    TCSEvent A = BuildTCSEvent(evt1, FrameMode::Raw);

    //afterburned actual beams (event by event)
    TCSEvent B = BuildTCSEvent(evt2, FrameMode::Raw);

    // afterburned particles corrected for crossing angle
    // but using nominal beams
    TCSEvent C = BuildTCSEvent(evt2, FrameMode::HeadOnNominal);

    //tests
    if(i == 0)
      {
	// constexpr double theta = 0.025; // 25 mrad

	// ROOT::Math::RotationY undoCA(theta);

	// auto pRecoRotVec =
	//     undoCA(B.pReco().Vect());

	// FourVec pRecoRot(
	//     pRecoRotVec.X(),
	//     pRecoRotVec.Y(),
	//     pRecoRotVec.Z(),
	//     B.pReco().E());

	std::cout << "\n=== Recoil proton ===\n";

	std::cout
	  << "Generator : "
	  << A.pReco().Px() << " "
	  << A.pReco().Py() << " "
	  << A.pReco().Pz()
	  << std::endl;

	std::cout
	  << "Afterburn : "
	  << B.pReco().Px() << " "
	  << B.pReco().Py() << " "
	  << B.pReco().Pz()
	  << std::endl;

	std::cout
	  << "UndoCrossAngle   : "
	  << C.pReco().Px() << " "
	  << C.pReco().Py() << " "
	  << C.pReco().Pz()
	  << std::endl
	  << std::endl;
	
      }
    
    
    if(i < 10){
      std::cout << "pBeam: Event " << i <<  std::endl;

      std::cout
	<< B.pBeam().Px()
	<< " "
	<< B.pBeam().Py()
	<< " "
	<< B.pBeam().Pz()
	<< std::endl;
    }
    
    
    for(int ip=0; ip<NPART; ip++)
      {
	const auto& pA = A.particles[ip];
	const auto& pB = B.particles[ip];
	const auto& pC = C.particles[ip];

	h_px_A[ip]->Fill(pA.Px());
	h_py_A[ip]->Fill(pA.Py());
	h_pz_A[ip]->Fill(pA.Pz());

	h_px_B[ip]->Fill(pB.Px());
	h_py_B[ip]->Fill(pB.Py());
	h_pz_B[ip]->Fill(pB.Pz());

	h_px_C[ip]->Fill(pC.Px());
	h_py_C[ip]->Fill(pC.Py());
	h_pz_C[ip]->Fill(pC.Pz());

	h_dpx[ip]->Fill(
			pA.Px() - pC.Px());

	h_dpy[ip]->Fill(
			pA.Py() - pC.Py());

	h_dpz[ip]->Fill(
			pA.Pz() - pC.Pz());
      }

    h_dQ2->Fill(A.Q2 - C.Q2);
    h_dt->Fill(A.t - C.t);
    h_dMM2->Fill(A.MM2 - C.MM2);
    
    h_MissMass2_A->Fill(A.MM2);
    h_MissMass2_B->Fill(B.MM2);
    h_MissMass2_C->Fill(C.MM2);
       
  }

    
  //----------------------------------------------------------
  // Draw Canvases
  //----------------------------------------------------------
  
  
  //TCanvas *c00 = new TCanvas();
    for(int i=0;i<TCSEvent::kNParticles;i++)
    {
      TCanvas *ctemp = new TCanvas(ParticleNames[i],ParticleNames[i],1920,1080);
      ctemp->Divide(2,2);
      ctemp->cd(1);
      h_px_A[i]->SetLineColor(kBlack);
      h_px_B[i]->SetLineColor(kRed);
      h_px_C[i]->SetLineColor(kBlue);
      h_px_A[i]->Draw();
      h_px_B[i]->Draw("same");
      h_px_C[i]->Draw("same");
      
      ctemp->cd(2);
      h_py_A[i]->SetLineColor(kBlack);
      h_py_B[i]->SetLineColor(kRed);
      h_py_C[i]->SetLineColor(kBlue);
      h_py_A[i]->Draw();
      h_py_B[i]->Draw("same");
      h_py_C[i]->Draw("same");
      
      ctemp->cd(3);
      h_pz_A[i]->SetLineColor(kBlack);
      h_pz_B[i]->SetLineColor(kRed);
      h_pz_C[i]->SetLineColor(kBlue);
      h_pz_A[i]->Draw();
      h_pz_B[i]->Draw("same");
      h_pz_C[i]->Draw("same");
      
    }
    
    
    TCanvas *c01 = new TCanvas("","",1920,1080);
    c01->Divide(2,2);
    c01->cd(1);
    h_MissMass2_A->SetLineColor(kBlack);
    h_MissMass2_B->SetLineColor(kRed);
    h_MissMass2_C->SetLineColor(kBlue);
  
    h_MissMass2_C->Draw("");
    h_MissMass2_A->Draw("same");
    h_MissMass2_B->Draw("same");

    TLegend *leg_MM2 =
      new TLegend(0.60, 0.70, 0.88, 0.88);

    leg_MM2->AddEntry(
		      h_MissMass2_A,
		      "Generator (Raw)",
		      "l");

    leg_MM2->AddEntry(
		      h_MissMass2_B,
		      "Afterburned (Raw)",
		      "l");

    leg_MM2->AddEntry(
		      h_MissMass2_C,
		      "Afterburned (UndoAfterBurner)",
		      "l");

    leg_MM2->SetBorderSize(0);
    leg_MM2->SetFillStyle(0);

    leg_MM2->Draw();

    c01->cd(2);
    h_dQ2->Draw();

    c01->cd(3);
    h_dt->Draw();

    c01->cd(4);
    h_dMM2->Draw();
    
}
