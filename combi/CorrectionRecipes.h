#include "AnalysisManager.h"
#include "HepMCElectro.h"
#include "BasicKinematicsRDF.h"
#include "KinematicsProcElectro.h"
#include "ElectronScatterKinematics.h"
#include "gammaN_2_Spin0Spin0SpinHalf.h"
#include "DefineNames.h"
#include <TBenchmark.h>

using namespace rad::consts;

auto TCS_Mass_Corrections = [](rad::KinematicsProcessor& p) {
  p.PreModifier().FixMass(ScatEle(), M_ele());
  p.PreModifier().FixMass("ele", M_ele());
  p.PreModifier().FixMass("pos", M_ele());
  p.PreModifier().FixMass("pprime", M_pro());
};
  
auto JPsi_Mass_Corrections = [](rad::KinematicsProcessor& p) {
  //Fix reconstructed Jpsi mass after it is calculted
  p.PostModifier().FixMass("Jpsi", M_Jpsi());
};  
