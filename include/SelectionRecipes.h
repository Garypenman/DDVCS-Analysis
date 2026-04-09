auto TCS_Selection_Recipe = [](rad::PhysicsSelection& s) {
    
    // At least reconstruct each particle
    s.AddCutRange("scatele_pmag_cut", "scat_ele_pmag", 0, 18); 
    s.AddCutMin("gprime_pmag_cut", "gprime_pmag", 0);
    s.AddCutRange("pprime_pmag_cut", "pprime_pmag", 0, 275);
    
    s.AddCutRange("Qp2_cut",    "Qp2", 0.0, 20.0); 
    
};
  
