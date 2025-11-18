#pragma once

void MakeDetectorAssociations(rad::config::ePICDetectorReaction &cr){
  
    cr.AssociateClusters({"EcalBarrelClusters","EcalBarrelImagingClusters",
      "EcalBarrelScFiClusters",
      "EcalEndcapNClusters","EcalEndcapPClusters","EcalEndcapPInsertClusters",
      "HcalBarrelClusters","HcalEndcapNClusters","LFHCALClusters",
      "EcalFarForwardZDCClusters","HcalFarForwardZDCClusters"},
    {"energy"}); //just going to associate the cluster energy from the list of detectors

}

void ClusterSums(rad::config::ePICDetectorReaction &cr){
  
  cr.Define("cluster_energy_sum",
  	      //[](vector<vector<float>> clusters){
	      [](ROOT::RVec<ROOT::RVec<float>> clusters){
  		ROOT::RVec<float> sums;
  		sums.reserve(clusters.size());
  		for (auto &inner : clusters) {
  		  float s = 0;
  		  for (auto e : inner) s += e;
  		  sums.push_back(s);
  		}
  		return sums;
  	      },
  	      {"rec_clusters_energy"});


}
