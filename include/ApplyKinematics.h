#pragma once

#include "ConfigReaction.h"

void ApplyKinematics(rad::config::ConfigReaction &cr){

  //masses column name, {+ve particles}, {-ve particles}
  rad::rdf::MissMass(cr,"W","{scat_ele}");
  rad::rdf::Mass(cr,"Whad","{gprime,pprime}");
  rad::rdf::Mass(cr,"GMass","{gprime}");
  
  //dis kinematics
  rad::rdf::Q2(cr,"Q2");
  rad::rdf::nu(cr,"nu");
  rad::rdf::y(cr,"y");
  rad::rdf::xbj(cr,"xbj");
  
  //q2prime tau
  rad::rdf::TauPrime(cr,"tau");
  
  //t distribution, column name
  rad::rdf::TTop(cr,"t_top");
  rad::rdf::TBot(cr,"t_bot");
  rad::rdf::TPrimeBot(cr,"tp_bot");
  rad::rdf::TPrimeTop(cr,"tp_top");

  //CM production angles
  rad::rdf::CMAngles(cr,"CM");
  //Proton Rest production angles
  rad::rdf::PRAngles(cr,"PR");
  
  //exclusivity
  rad::rdf::MissMass(cr,"MissMass","{scat_ele,pprime,gprime}");
  rad::rdf::MissP(cr,"MissP","{scat_ele,pprime,gprime}");
  rad::rdf::MissPt(cr,"MissPt","{scat_ele,pprime,gprime}");
  rad::rdf::MissPz(cr,"MissPz","{scat_ele,pprime,gprime}");
  rad::rdf::MissTheta(cr,"MissTheta","{scat_ele,pprime,gprime}");
  
  //missing proton
  rad::rdf::MissMass(cr,"MissMass_pprime","{scat_ele,gprime}");
  rad::rdf::MissP(cr,"MissP_pprime","{scat_ele,gprime}");
  rad::rdf::MissPt(cr,"MissPt_pprime","{scat_ele,gprime}");
  rad::rdf::MissPz(cr,"MissPz_pprime","{scat_ele,gprime}");
  rad::rdf::MissTheta(cr,"MissTheta_pprime","{scat_ele,gprime}");
  
  //decay angles
  rad::rdf::gn2s0s0s12::HelicityAngles(cr,"Heli");
  rad::rdf::gn2s0s0s12::PhiTrento(cr);
  //photon polarisation
  rad::rdf::PolGammaStar(cr,"GammaPol");
  rad::rdf::CircPolGammaStar(cr,"GammaPolCirc");
  rad::rdf::EGammaStar(cr,"GammaE");
  

}
