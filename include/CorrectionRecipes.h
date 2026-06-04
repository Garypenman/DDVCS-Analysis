#pragma once

#include "AnalysisManager.h"
#include "HepMCElectro.h"
#include "BasicKinematicsRDF.h"
#include "KinematicsProcElectro.h"
#include "ElectronScatterKinematics.h"
#include "gammaN_2_Spin0Spin0SpinHalf.h"
#include "DefineNames.h"
#include <TBenchmark.h>

using namespace rad::consts;

auto TCS_MassP_Corrections = [](rad::KinematicsProcessor& p) {
  p.PreModifier().FixMassWithP(ScatEle(), M_ele());
  p.PreModifier().FixMassWithP("ele", M_ele());
  p.PreModifier().FixMassWithP("pos", M_ele());
  p.PreModifier().FixMassWithP("pprime", M_pro());
};

auto TCS_MassE_Corrections = [](rad::KinematicsProcessor& p) {
  p.PreModifier().FixMassWithE(ScatEle(), M_ele());
  p.PreModifier().FixMassWithE("ele", M_ele());
  p.PreModifier().FixMassWithE("pos", M_ele());
  p.PreModifier().FixMassWithE("pprime", M_pro());
};
   
auto JPsi_Mass_Corrections = [](rad::KinematicsProcessor& p) {
  //Fix reconstructed Jpsi mass after it is calculted
  p.PostModifier().FixMassWithP("Jpsi", M_Jpsi());
};  
