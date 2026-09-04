#pragma once

//#include "ePICReaction.h"
#include "ePICAssociationsManager.h" // <-- NEW: Include the manager

void BuildAssociations(rad::epic::ePICReaction &df){
  
  // --- NEW: Auxiliary Data Handling via Fluent Builder ---
  rad::epic::ePICAssociationManager assoc(df);
  
  // Extract Central Calorimeter Energies
  assoc
    .For("Central")
    .From({"EcalBarrelClusters", "EcalEndcapPClusters"}) // Optionally add "EcalEndcapNClusters" if needed!
    .Extract("energy")
    .As("cal_energy"); // Maps to "rec_cal_energy"
  
  // Central Track Times
  assoc
    .For("Central")
    .Relation("tracks") // Override default "clusters" relation
    .From({"CentralCKFTracks", "TaggerTrackerM1LocalTracks", "TaggerTrackerM2LocalTracks","ForwardRomanPotRecHits"})
    .Extract("time")
    .As("track_time");
  
  
  // Execute padding and build the unified arrays for the base dataframe
  assoc.Build();

  //Tagger Electron Flags
  df.DefineDetectorFlag("rec_from_tagger","TaggerTrackerReconstructedParticleAssociations");
  //df.DefineDetectorFlag("rec_from_tagger1","TaggerTracker
			
  df.Define("rec_not_from_tagger",
	    [](const ROOT::RVec<int>&taggerFlag){
	      ROOT::RVec<int> out(taggerFlag.size());
	      
	      for(size_t i=0;i<taggerFlag.size();++i)
		out[i] = !taggerFlag[i];
	      
	      return out;
	    },{"rec_from_tagger"});
  
  //RP Proton Flags
  //df.DefineDetectorFlag("rec_from_romanpot","ForwardRomanRawHitAssociations_rawHit");
  
  // TEMPORARY:
  // Roman Pot hit->track associations currently unavailable in eicrecon.
  df.Define("rec_pprime_rphits_time0",
       [](const ROOT::RVec<float>& t){
	 return ROOT::RVecD{
	   t.empty()
	   ? rad::consts::InvalidEntry<double>()
	   : static_cast<double>(t[0])
	 };
       },{"ForwardRomanPotRecHits.time"});
  
  // Use average RP RecHit time as proton time proxy.
  df.Define("rec_pprime_rphits_timeavg",
		    [](const ROOT::RVec<float>& times)
		    {
		      if(times.empty())
			return rad::consts::InvalidEntry<double>();

		      double avg = 0;
		      for(auto t : times) avg += t;
		      return avg /= times.size();
		    },
		    {"ForwardRomanPotRecHits.time"});
  
}
