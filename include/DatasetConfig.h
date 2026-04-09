#ifndef DATASETCONFIG_H
#define DATASETCONFIG_H

#include <string>
#include <stdexcept>

#include "../include/Config.h"

struct DatasetConfig {
  
  // User-provided identifiers
  std::string reaction;
  std::string decay;
  std::string helicity;
  std::string config;
  std::string campaign;
  
  // Auto-generated metadata
  std::string xrdfsPath;
  int beam_ele_idx;
  int beam_ion_idx;
  int scat_ele_idx;
  int scat_ion_idx;
  int lep_minus_idx;
  int lep_plus_idx;
  int pdg;

DatasetConfig(const std::string& reac,
	      const std::string& dec,
	      const std::string& hel,
	      const std::string& cfg,
	      const std::string& camp)
: reaction(reac), decay(dec), helicity(hel), config(cfg), campaign(camp)
  {
    // PDG
    if (decay == "edecay" || decay == "ee") pdg = 11;
    else if (decay == "mudecay" || decay == "mumu") pdg = 13;
    else throw std::runtime_error("Unknown decay: " + decay);

    // Index selection
    if (reaction == "ddvcs") {
      beam_ele_idx  = edm4hep_epic_beam_ele_idx;
      beam_ion_idx  = edm4hep_epic_beam_ion_idx;
      scat_ele_idx  = edm4hep_epic_scat_ele_idx;
      scat_ion_idx  = edm4hep_epic_scat_ion_idx;
      lep_minus_idx = edm4hep_epic_lep_minus_idx;
      lep_plus_idx  = edm4hep_epic_lep_plus_idx;
    }
    else if (reaction == "jpsi") {
      beam_ele_idx  = edm4hep_eslight_beam_ele_idx;
      beam_ion_idx  = edm4hep_eslight_beam_ion_idx;
      scat_ele_idx  = edm4hep_eslight_scat_ele_idx;
      scat_ion_idx  = edm4hep_eslight_scat_ion_idx;
      lep_minus_idx = edm4hep_eslight_lep_minus_idx;
      lep_plus_idx  = edm4hep_eslight_lep_plus_idx;
    }
    else {
      throw std::runtime_error("Unknown reaction: " + reaction);
    }

    // XRDFS path construction
    std::string base = "/volatile/eic/EPIC/RECO/" + campaign +
      "/epic_craterlake/EXCLUSIVE/";

    if (reaction == "ddvcs") {
      xrdfsPath = base +
	"DDVCS_ABCONV/EpIC1.1.6-1.0/" +
	config + "/q2_0_10/" + decay + "/" + helicity + "/";
    }
    else if (reaction == "jpsi") {
      xrdfsPath = base +
	"PHOTOPRODUCTION_JPSI_ABCONV/coherent_ep/"
	"eSTARlight1.3.0-1.0/" +
	config + "/q2_nocut/" + decay + "/";
    }
  }
};

#endif // DATASETCONFIG_H
