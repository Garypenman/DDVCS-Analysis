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

std::string file1 = "/w/work6/home/gp140f/epic_generator_output/DDVCS/rootfiles/9x130_ddvcs_ee_hplus.hepmc3.tree.root";
std::string file2 = "/w/work6/home/gp140f/epic_generator_output/DDVCS/afterburned/ab_9x130_ddvcs_ee_hplus.hepmc3.tree.root";

void HepMCTCSAnalysis(std::string infilename = file2)
{

  //----------------------------------------------------------
  // Input
  //----------------------------------------------------------
  
  TFile *infile = TFile::Open(infilename.c_str());

  if (!infile || infile->IsZombie()) {
    cout << "Cannot open file " << infilename << endl;
    return;
  }

  TTree *tree = (TTree*)infile->Get("hepmc3_tree");

  if (!tree) {
    cout << "Could not find TTree named 'hepmc3_tree'" << endl;
    return;
  }

  
  //----------------------------------------------------------
  // Input branches
  //----------------------------------------------------------

  Int_t particles_;

  HepMC3::GenEventData* evtdata = nullptr;
  tree->SetBranchAddress("hepmc3_event",
			 &evtdata);

  
  //----------------------------------------------------------
  // Histograms
  //----------------------------------------------------------

  TH1D *hpprime_px = new TH1D("hpprime_px","HepMC Analysis Recoil Proton; px_{p'} [GeV/c];Events",100,-2,2);
  TH1D *hpprime_py = new TH1D("hpprime_py","HepMC Analysis Recoil Proton; py_{p'} [GeV/c];Events",100,-2,2);
  TH1D *hpprime_pz = new TH1D("hpprime_pzz","HepMC Analysis Recoil Proton; pz_{p'} [GeV/c];Events",100,40,150);
  TH2D *h2d_pprime_px_py = new TH2D("h2d_pprime_px_py","HepMC Analysis Recoil Proton; px; py",250,-0.5,0.5,250,-0.5,0.5);
  
  TH1D *hpprime_phi = new TH1D("hpprime_phi","HepMC Analysis Recoil Proton; #phi_{p'} [rad];Events",100,-3.14,3.14);
  TH1D *hpprime_theta = new TH1D("hpprime_theta","HepMC Analysis Recoil Proton; #theta_{p'} [rad];Events",100,-0.01,0.01);
  TH2D *h2d_pprime_theta_phi = new TH2D("h2d_pprime_theta_phi","HepMC Analysis Recoil Proton; #theta_{p'} [rad]; #phi_{p'} [rad]",100,0.0,0.01,100,-3.14,3.14);

  TH1D *hCosThetaHel = new TH1D("hCosThetaHel","HepMC; cos(#theta_{l}); Events",100,-1,1);
  TH1D *hPhiHel = new TH1D("hPhi","HepMC; #phi_{l}; Events",100,-TMath::Pi(),TMath::Pi());
  
  TH1D *hepsilon = new TH1D("hepsilon","Virtual Photon Linear Polarisation; #epsilon; Events",100,0,1);
  TH1D *hcircPol = new TH1D("hcircPol","Virtual Photon Circular Polarisation; #circ; Events",100,0,1);
  
  //----------------------------------------------------------
  //Event loop
  //----------------------------------------------------------

  Long64_t nEvents = tree->GetEntries();
  cout << "Processing "
       << nEvents
       << " events"
       << endl;

  for(Long64_t i=0;i<nEvents;i++)
    {
      tree->GetEntry(i);
      //TCSEvent evt = BuildTCSEvent(evtdata, FrameMode::Raw); // if pure HepMC
      TCSEvent evt = BuildTCSEvent(evtdata, FrameMode::HeadOnNominal); // if afterburned or crossing angle

      hpprime_px->Fill(evt.pprime().Px());
      hpprime_py->Fill(evt.pprime().Py());
      hpprime_pz->Fill(evt.pprime().Pz());
      h2d_pprime_px_py->Fill(evt.pprime().Px(),evt.pprime().Py()); 
      hpprime_phi->Fill(evt.pprime().Phi());
      hpprime_theta->Fill(evt.pprime().Theta());
      h2d_pprime_theta_phi->Fill(evt.pprime().Theta(), evt.pprime().Phi());

      hCosThetaHel->Fill(evt.cosTheta);
      hPhiHel->Fill(evt.phi);
      
      hepsilon->Fill(evt.epsilon);
      hcircPol->Fill(evt.circPol);
    }
  
  //----------------------------------------------------------
  // Draw Histograms
  //----------------------------------------------------------
  
  TCanvas* c00 = new TCanvas("c00","c00",1000,700);
  hpprime_phi->SetMinimum(0);
  hpprime_phi->Draw();
  c00->Close();
  
  TCanvas* c01 = new TCanvas("c01","c01",1000,700);
  hpprime_theta->Draw();
  c01->Close();
  
  TCanvas* c02 = new TCanvas("c02","c02",1000,700);
  h2d_pprime_theta_phi->Draw("colz");
  c02->Close();
  
  TCanvas *c03 = new TCanvas("c03","c03",1000,700);
  c03->Divide(3,1);
  c03->cd(1);
  hpprime_px->Draw();
  c03->cd(2);
  hpprime_py->Draw();
  c03->cd(3)->SetLogy(0);
  hpprime_pz->Draw();
  c03->Close();
  
  TCanvas *c04 = new TCanvas("c04","c04",1000,700);
  h2d_pprime_px_py->Draw("colz");
  c04->Close();
  
  TCanvas *c05 = new TCanvas("c05","c05",1000,700);
  c05->Divide(2,1);
  c05->cd(1);
  hepsilon->Draw();
  c05->cd(2);
  hcircPol->Draw();

  TCanvas *c06 = new TCanvas("c06","c06",1000,700);
  c06->Divide(2,1);
  c06->cd(1);
  hCosThetaHel->Draw();
  c06->cd(2);
  hPhiHel->SetMinimum(0);
  hPhiHel->Draw();

}
