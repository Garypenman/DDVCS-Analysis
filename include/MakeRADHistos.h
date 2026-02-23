#pragma once

void MakeRADHistos(rad::histo::Histogrammer &histo){

  //Kinematics
  histo.Create<TH1D,double>({"Q2","Q2",100,0,2.},{"Q2"});
  histo.Create<TH1D,double>({"nu","nu",100,0,10000.},{"nu"});
  histo.Create<TH1D,double>({"xbj","xbj",100,0,1.},{"xbj"});
  histo.Create<TH1D,double>({"y","y",100,0,1.},{"y"});
  
  histo.Create<TH1D,double>({"W","W",100,0,200.},{"W"});
  histo.Create<TH1D,double>({"Whad","Whad",100,0,200.},{"Whad"});
  
  histo.Create<TH1D,double>({"MesonMass","M(e^{-},e^{+}) [GeV]",100,0,5.},{"GMass"});
  
  histo.Create<TH1D,double>({"ttop","t(p,p^{'}) [GeV^{2}]",100,0,2.0},{"t_top"});
  histo.Create<TH1D,double>({"tbot","t(p,p^{'}) [GeV^{2}]",100,0,2.0},{"t_bot"});
  histo.Create<TH1D,double>({"tptop","t' top [GeV^{2}]",100,0,2.0},{"tp_top"});
  histo.Create<TH1D,double>({"tpbot","t' bot [GeV^{2}]",100,0,2.0},{"tp_bot"});

  histo.Create<TH2D,double,double>({"tbot_mesonmass","tbot vs meson mass",100,0.0,0.2,100,0.3,5.},{"t_bot","GMass"});
  histo.Create<TH2D,double,double>({"tpbot_mesonmass","t'bot vs meson mass",100,-0.1,1.5,100,0.3,5.},{"tp_bot","GMass"});
  histo.Create<TH2D,double,double>({"tbot_W","tbot vs W",100,-0.1,1.5,100,0.,200.},{"t_bot","W"});
  histo.Create<TH2D,double,double>({"tpbot_W","t'bot vs W",100,-0.1,1.5,100,0.,200.},{"tp_bot","W"});
  histo.Create<TH2D,double,double>({"W_mesonmass","W vs meson mass",100,0.3,5.,100,0.,200.},{"GMass","W"});
  
  //CM and PR Decay Angles
  histo.Create<TH1D,double>({"cthCM","cos(#theta_{CM})",100,-1,1},{"CM_CosTheta"});
  histo.Create<TH1D,double>({"phCM","#phi_{CM}",100,-TMath::Pi(),TMath::Pi()},{"CM_Phi"});
  histo.Create<TH1D,double>({"cthPR","cos(#theta_{PR})",100,-1,1},{"PR_CosTheta"});
  histo.Create<TH1D,double>({"phPR","#phi_{PR}",100,-TMath::Pi(),TMath::Pi()},{"PR_Phi"});
  
  //exclusivity
  histo.Create<TH1D,double>({"MissMass","Mmiss [GeV]",100,-10,10},{"MissMass"});
  histo.Create<TH1D,double>({"missP","p_{miss}(e',#gamma',p')",100,0,100},{"MissP"});
  histo.Create<TH1D,double>({"missPt","p_{t,miss}(e',#gamma',p')",100,0,10},{"MissPt"});
  histo.Create<TH1D,double>({"missPz","p_{z,miss}(e',#gamma',p')",100,0,100},{"MissPz"});
  histo.Create<TH1D,double>({"missTheta","#theta_{miss}(e',#gamma',p')",100,0,1},{"MissTheta"});
  
  //semi-exclusivity
  histo.Create<TH1D,double>({"MissMass_pprime","Mmiss {e,p'}[GeV]",100,.3,5.},{"MissMass_pprime"});
  
  histo.Create<TH1D,ROOT::RVecD>({"allP","momentum of all particles",100,0,100},{"pmag"});
  // histo.Create<TH1D,ROOT::RVecD>({"eleP","momentum of electrons",100,0,100},{"elsP"});
  
  histo.Create<TH1D,double>({"scatele_phi","Azimuthal Angle of Scattered Electron",250,-TMath::Pi(),TMath::Pi()},{"phi[scat_ele]"});
  
  //recoil proton
  histo.Create<TH1D,double>({"pprime_pmag","Momentum of Recoil Proton",100,0,275},{"pmag[pprime]"});
  histo.Create<TH1D,double>({"pprime_eta","Pseudorapidity of Recoil Proton",100,-10,10},{"eta[pprime]"});
  histo.Create<TH1D,double>({"pprime_theta","Polar Angle of Recoil Proton",100,-TMath::Pi(),TMath::Pi()},{"theta[pprime]"});
  histo.Create<TH1D,double>({"pprime_phi","Azimuthal Angle of Recoil Proton",100,-TMath::Pi(),TMath::Pi()},{"phi[pprime]"});
  
  //for brufit need
  //CM_Phi Heli_theta Heli_phi GammaPolCirc=sqrt(1-epsilon)*Pol t GammaE
  histo.Create<TH1D,double>({"GammaPol","Polarisation of Virtual Photon",100,0,1},{"GammaPol"});
  histo.Create<TH1D,double>({"GammaE","Energy of Virtual Photon",100,0,18},{"GammaE"});
  histo.Create<TH1D,double>({"Heli_CosTheta","#theta decay angle",100,-TMath::Pi(),TMath::Pi()},{"Heli_CosTheta"});
  histo.Create<TH1D,double>({"Heli_Phi","#phi decay angle",100,-TMath::Pi()-1,TMath::Pi()+1},{"Heli_Phi"});
 
  //Polarisation and kinematic limits for cicular
  histo.Create<TH2D,double,double>({"y_W","W{ele missmass} vs y",100,0,1,100,0,200},{"y","W"});
  histo.Create<TH2D,double,double>({"y_Escatele","E_{e'} vs y",100,0,1,100,0,18},{"y","pmag[scat_ele]"});
  histo.Create<TH2D,double,double>({"y_CircPol","Circular Polarisation vs y",100,0,1,100,0,1},{"y","GammaPolCirc"});
  histo.Create<TH2D,double,double>({"W_CircPol","Circular Polarisation vs W{ele missmiass}",100,0,200,100,0,1},{"W","GammaPolCirc"});

}
