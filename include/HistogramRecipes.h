#include "ePICFileStreamer.h"
#include "../include/FileProcessing.h"

auto TCS_Histogram_Recipe = [](rad::histo::Histogrammer& h) {
    //kinematics
    h.Create("Q2",";Q^2[GeV^{2}]", 100, 0, 1.0, "Q2");
    h.Create("nu",";#nu [GeV]",100,0,10000.,"nu");
    h.Create("xbj",";xbj",100,0,1.,"xbj");
    h.Create("y",";y",100,0,1.,"y");
    
    h.Create("W",";W (electro) [GeV]",100,0,200.,"W");
    h.Create("Whad","W (hadronic) [GeV]",100,0,200.,"Whad");
    
    h.Create("Qp", ";Q' = M_{e+e-} [GeV]", 100, 0.0, 5.0, "Qp");
    h.Create("Qp2", ";Q'^{2} = M_{e+e-} [GeV^2]", 100, 0.0, 25.0, "Qp2");
    
    h.Create("ttop","t(p,p^') [GeV^2]",100,0,2.0,"t_top");
    h.Create("tbot","t(p,p^') [GeV^2]",100,0,2.0,"t_bot");
    // h.Create("tptop","t' top [GeV^2]",100,0,2.0,"tp_top");
    // h.Create("tpbot","t' bot [GeV^2]",100,0,2.0,"tp_bot");
    
    h.Create2D("ttop_Qp2",";t_{eXBE} [GeV^{2}]; Q'^{2} [GeV^{2}]",100, 0.0, 2.0, 100, 0.0, 25.0,"t_top","Qp2");
    h.Create2D("tbot_Qp2",";t_{BABE} [GeV^{2}]; Q'^{2} [GeV^{2}]",100, 0.0, 2.0, 100, 0.0, 25.0,"t_bot","Qp2");
    
    h.Create2D("httop_W2",";t_{eXBE} [GeV^{2}]; W^{2} [GeV^{2}]",100, 0.0, 2.0, 100, 0.0, 200.0, "t_top","W2");
    h.Create2D("htbot_W2had",";t_{BABE} [GeV^{2}]; W^{2} [GeV^{2}]",100, 0.0, 2.0, 100, 0.0, 200.0, "t_bot", "W2");
    
    h.Create2D("W2_Qp2","",100, 0.0, 25., 100, 0.0, 200.0, "Qp2","W");
 
    //CM and PR Decay Angles
    h.Create("CosThetaCM","cos(#theta_CM)",100,-1,1,"CosThetaCM");
    h.Create("PhiCM","#phi_CM",100,-TMath::Pi(),TMath::Pi(),"PhiCM");
    //h.Create("CosThetaPR","cos(#theta_{PR})",100,-1,1,"CosThetaPR");
    //h.Create("PhiPR","#phi_{PR}",100,-TMath::Pi(),TMath::Pi(),"PhiPR");
  
    //exclusivity
    h.Create("MissMass","Mmiss [GeV]",100,-10,10,"MissMass");
    h.Create("MissMass2",     "Missing Mass squared; [GeV]", 100, -50, 50, "MissMass2");
    //h.Create("missP","p_miss(e',#gamma',p')",100,0,100,"miss_pmag");
    h.Create("missPt","p_t,miss(e',#gamma',p')",100,0,10,"MissPt");
    //h.Create("missPz","p_z,miss(e',#gamma',p')",100,0,100,"MissPz");
    //h.Create("missTheta","#theta_miss(e',#gamma',p')",100,0,1,"miss_theta");
  
    //semi-exclusivity
    h.Create("MissMass_pprime","Missing Mass of Proton; M_{miss,p'} [GeV/c^{2}]",100,-5., 5., "MissMass_pprime");
    h.Create("MissMass2_pprime", "Missing Mass of Proton squared; M^{2}_{miss,p'} [GeV^{2}/c^{4}]", 100, -25., 25., "MissMass2_pprime");
    
    //scattered electron
    h.Create("scat_ele_pmag","Momentum of Scattered Electron; p_{e'} [GeV/c]",100,0,18,"scat_ele_pmag");
    h.Create("scat_ele_eta","Pseudorapidity of Scattered Electron; #eta_{e'}",100,-10,10,"scat_ele_eta");
    h.Create("scat_ele_theta","Polar Angle of Scattered Electron; #theta_{e'} [rad]",100, 0.0,TMath::Pi(),"scat_ele_theta");
    h.Create("scat_ele_phi","Azimuthal Angle of Scattered Electron; #phi_{e'} [rad]",100,-TMath::Pi(),TMath::Pi(),"scat_ele_phi");
  
    //decay ele
    h.Create("ele_pmag","Momentum of Decay Ele; p_{e-} [GeV/c]",100,0,275,"ele_pmag");
    h.Create("ele_eta","Pseudorapidity of Decay Ele; #eta_{e-}",100,-10,10,"ele_eta");
    h.Create("ele_theta","Polar Angle of Decay Ele; #theta_{e-} [rad]",100, 0.0,TMath::Pi(),"ele_theta");
    h.Create("ele_phi","Azimuthal Angle of Decay Ele; #theta_{e-} [rad]",100,-TMath::Pi(),TMath::Pi(),"ele_phi");
    
    //decay pos
    h.Create("pos_pmag","Momentum of Decay Pos; p_{e+} [GeV/c]",100,0,275,"pos_pmag");
    h.Create("pos_eta","Pseudorapidity of Decay Pos; #eta_{e+}",100,-10,10,"pos_eta");
    h.Create("pos_theta","Polar Angle of Decay Pos; #theta_{e+} [rad]",100, 0.0,TMath::Pi(),"pos_theta");
    h.Create("pos_phi","Azimuthal Angle of Decay Pos; #phi_{e+} [rad]",100,-TMath::Pi(),TMath::Pi(),"pos_phi");
    
    //recoil proton
    h.Create("pprime_pmag","Momentum of Recoil Proton; p_{p'} [GeV/c]",100,0,275,"pprime_pmag");
    h.Create("pprime_eta","Pseudorapidity of Recoil Proton; #eta_{p'}",100,-10,10,"pprime_eta");
    h.Create("pprime_theta","Polar Angle of Recoil Proton; #theta_{p'} [rad]",100, 0.0,TMath::Pi(),"pprime_theta");
    h.Create("pprime_phi","Azimuthal Angle of Recoil Proton; #phi_{p'} [rad]",100,-TMath::Pi(),TMath::Pi(),"pprime_phi");
  
    //for brufit need
    //CM_Phi Heli_theta Heli_phi GammaPolCirc=sqrt(1-epsilon)*Pol t GammaE
    h.Create("GammaPol","Polarisation of Virtual Photon;#epsilon",100,0,1,"GammaPol");
    h.Create("GammaE","Energy of Virtual Photon;E_{#gamma} [GeV]",100,0,18,"GammaE");
    h.Create("CosThetaHel","CosTheta Decay Angle;cos(#theta_{l})",100,-1,1,"CosThetaHel");
    h.Create("ThetaHel","Theta Decay Angle;#theta_{l} [rad]",100, 0.0,TMath::Pi(),"ThetaHel");
    h.Create("PhiHel","Phi Decay Angle;#phi_{l} [rad]",100,-TMath::Pi()-1,TMath::Pi()+1,"PhiHel");
 
    //Polarisation and kinematic limits for cicular
    h.Create2D("y_W","Wele missmass vs y",100,0,1,100,0,200,"y","W");
    h.Create2D("y_Escatele","E_e' vs y",100,0,1,100,0,18,"y","scat_ele_pmag");
    h.Create2D("y_CircPol","Circular Polarisation vs y",100,0,1,100,0,1,"y","GammaPolCirc");
    h.Create2D("W_CircPol","Circular Polarisation vs Wele missmiass",100,0,200,100,0,1,"W","GammaPolCirc");

};
  
