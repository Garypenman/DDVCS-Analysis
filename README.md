# DDVCS-Analysis

    This repository contains the analysis pipeline for DDVCS/TCS physics events generated with the EPIC event generator. It operates on HepMC- and EDM4hep-level outputs and produces combined helicity ROOT trees and analysis-ready datasets.

    This repository is intended to be used alongside a companion repository:

    DDVCSdataset
    - Handles XML generation and EPIC event production
    - Produces raw .hepmc files

    DDVCS-Analysis (this repository)
    - Handles all post-processing and physics analysis

    As well as necessary auxillary repositories
    
    https://github.com/dglazier/rad


    https://github.com/dglazier/epic-rad

    ------------------------------------------------------------
    WORKFLOW OVERVIEW
    ------------------------------------------------------------

    The full workflow is:

    DDVCSdataset
      -> EPIC generator
      -> .hepmc files
      -> DoAB.sh conversion/afterburning
      -> .hepmc3.tree.root
      -> DDVCS-Analysis processing
      -> combined helicity ROOT trees
      -> edm4hep output analysis

    ------------------------------------------------------------
    DATA FLOW
    ------------------------------------------------------------

    1. Event generation (DDVCSdataset)

    - XML files are generated with make_xml.sh
    - EPIC produces files:
      *.hepmc


    2. Conversion and afterburning

    Using tool:

    NHPGlasgow/eic_utilities/macros/DoAB.sh

    Run in directory containing .hepmc files:

      ./DoAB.sh

    This produces:

    - ./rootfiles/*.hepmc3.tree.root
      (direct conversion of HepMC to ROOT trees)

    - ./afterburned/*.hepmc3.tree.root
      (afterburned trees including radiative corrections)


    3. Generator-level analysis (RAD)

    Handled by ProcessHEPMCTCSCombi.C

    This uses the RAD framework, specifically:

    - HepmcReaction()

    It performs:
    - particle selection
    - kinematic reconstruction
    - observable extraction


    4. Helicity processing

    Handled by MixHelicityTrees.C

    This processes:
    - hplus samples
    - hminus samples

    Outputs:
    - plus helicity tree
    - minus helicity tree
    - mixed helicity tree

    Output files are created in directories:
      <outdir>_hplus/
      <outdir>_hminus/
      <outdir>_hmixed/

    Each contains:
      TCS_mc_Tree.root


    5. Workflow orchestration

    Main control is via RunEverything.C

    Functions include:

    - RunHepMC()
      Runs HepMC-level processing

    - RunCampaigns()
      Optional detector-level processing

    Typical usage from ROOT:

      root -l
      .L RunEverything.C
      RunEverything();


    ------------------------------------------------------------
    NAMING CONVENTION
    ------------------------------------------------------------

    All datasets follow:

      <ebeam>x<pbeam>_ddvcs_<decay>[_<mode>]_<helicity>

    Examples:

      9x130_ddvcs_ee_hplus.hepmc
      9x130_ddvcs_ee_bhonly_hminus.hepmc

    Where:

    - ebeam, pbeam:
      beam energies in GeV

    - decay:
      ee or mumu

    - mode:
      bhonly or all

    - helicity:
      hplus or hminus


    Important note:

    The "all" amplitude is implicit and has no tag.

    So:
      all    -> no "_all" in filename
      bhonly -> explicitly "_bhonly"

    Examples:

      9x130_ddvcs_ee_hplus        (ALL)
      9x130_ddvcs_ee_bhonly_hplus (BH-only)


    ------------------------------------------------------------
    DIRECTORY STRUCTURE
    ------------------------------------------------------------

    Typical layout:

    epic_generator_output/
      DDVCS/
        rootfiles/
          *.hepmc3.tree.root

    combirad_trees/
      <config>_ddvcs_<decay>[_mode]_hplus/
      <config>_ddvcs_<decay>[_mode]_hminus/
      <config>_ddvcs_<decay>[_mode]_hmixed/


    ------------------------------------------------------------
    RUNNING THE ANALYSIS
    ------------------------------------------------------------

    Start ROOT:

      root -l

    Run everything:

      RunEverything();

    Or run only HepMC stage:

      RunHepMC();


    RunHepMC does the following:

    - loops over allowed beam configurations
    - loops over decay channels (ee, mumu)
    - loops over subprocesses (all, bhonly)
    - constructs file names based on naming convention
    - checks existence of hplus and hminus files
    - calls MixHelicityTrees


    ------------------------------------------------------------
    ANALYSIS FRAMEWORKS
    ------------------------------------------------------------

    RAD (generator level)

    Used in ProcessHEPMCTCSCombi.C

    Main class:
      HepmcReaction()

    Handles generator-level physics analysis.


    ePIC + RAD (detector level)

    Used when analysing detector simulation output.

    Main class:
      ePICReaction()

    Input:
      afterburned .hepmc3.tree.root files


    ------------------------------------------------------------
    REQUIREMENTS
    ------------------------------------------------------------

    - ROOT (with multithreading enabled)
    - HepMC3
    - RAD framework
    - EPIC generator outputs (.hepmc files)
    - DoAB.sh conversion tool


    ------------------------------------------------------------
    DESIGN NOTES
    ------------------------------------------------------------

    - Input files are always checked before processing
    - Both helicities must exist for combination
    - Output directory creation is handled internally
    - Naming consistency is critical for automation
    - "all" mode is implicit in filenames


    ------------------------------------------------------------
    RELATED REPOSITORIES AND TOOLS
    ------------------------------------------------------------

    DDVCSdataset
    - XML generation and EPIC simulation

    NHPGlasgow/eic_utilities
    - HepMC conversion and afterburning

    RAD framework
    - Physics analysis backend


    ------------------------------------------------------------
    EXAMPLE FLOW
    ------------------------------------------------------------

    9x130_ddvcs_ee_hplus.hepmc
      -> DoAB.sh
      -> rootfiles/9x130_ddvcs_ee_hplus.hepmc3.tree.root
      -> MixHelicityTrees
      -> combirad_trees/9x130_ddvcs_ee_hplus/TCS_mc_Tree.root


    ------------------------------------------------------------
    SUMMARY
    ------------------------------------------------------------

    This repository provides a complete analysis pipeline for DDVCS and TCS studies:

    - EPIC generator produces HepMC events
    - Utils convert them to ROOT trees
    - RAD performs generator-level analysis
    - Helicity mixing produces final datasets

    The pipeline is fully script-driven and designed for reproducibility and scalability.


    ------------------------------------------------------------
    QUICK USAGE
    ------------------------------------------------------------

    1. Generate events using DDVCSdataset
    2. Run DoAB.sh in the HepMC directory
    3. Start ROOT and run:

       RunHepMC();




