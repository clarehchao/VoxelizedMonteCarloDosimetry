$Id$
-------------------------------------------------------------------

     =========================================================================
     Geant4 - an Object-Oriented Toolkit for Simulation in High Energy Physics
     =========================================================================

                     VoxelizedHumanDoseMultiSDv3


Author: Shih-ying (Clare) Huang
Institution: University of California, San Francisco
Organization: Physics Research Laboratory (Principle Investigator: Prof. Youngho Seo)
-------------------------------------------------------------------
 0- Introduction

 This application performs internal dosimetry calculation for user-specific geometry and source distribution by tallying dose information per voxel in the defined geometry.

Improvement from VoxelizedHumanDoseMultiSDv1 include:
- 
-


	
 1- GEOMETRY DEFINITION
	
   The geometry is constructed in the VHDDetectorConstruction class derived from the Geant4 
   abstract class, G4VUserDetectorConstruction. The VHDDetectorConstruction class does the following:
   [a] Initialize and define elements and tissue composition needed for a given volume with its defined
       materials
       ==> an material array is built to convert material index to material type specified in the
           simulation)
   [b] Read in the volume data files (.g4m) and merge all the data from each image slice via 
       VHDPhantomZSliceHeader
   [c] Construct the voxelized container based on the voxel size of the given volume
   [d] Construct the phantom implemented by its derived class RegularVHDDetectorConstruction and 
       NestedParamVHDDetectorConstruction (per user's choice)

   Note:
   [1] Prior to running this application, geometry files will need to be generated in the .g4m format 
       (see Matlab or python code that generates these files)
   [2] There are several scorers defined as well to tally the physical quantities of interest such as
       - VHDMSDCellFlux and VHDMSDCellFlux_RegParam
       - VHDPSEnergyDeposit and VHDPSEnergyDeposit_RegParam
       - VHDMSDNofStep was implemented as an optional scorer; it's only defined for the nested 
         parameterized geometry definition (see GetIndex(....) in its class definition)
       - these derived classes have different definition in the member function "GetIndx(....)" for 
         nested parameterized vs. regular parameterized geometry
       - future improvement will re-structure the usage of these derived classes using factory design 
         pattern
   
		
 2- PHYSICS LIST
 
    This application uses the modular physics lists as a starter to define the physics needed for 
    dosimetry purpose.  Since this application is designed to work with any
    form of targeted radionuclide therapy or other radiation therapy, the option to select the 
    appropriate physics lists defined in Geant4 are necessary.  By default,
    G4EmStandardPhysics, G4DecayPhysics, and G4RadioactiveDecayPhysics are registered in the user-
    defined physicslist class.  A physics messenger class, VHDPhysicsListMessenger,
    was defined for users to select physics lists via macro or command line interactively.
 	 
 3- PRIMARY GENERATOR
  
   The primary generator is defined in the VHDPrimaryGeneratorAction class, which does the following..
   [a] Declare a particle gun for primary generation
   [b] Read the position probability map defined by the user (.g4m)
   [c] Sample the particle position and momentum using Rejection sampling and built-in function that 
       generates isotropic momemtum (this can replaced by using G4ParticleSource)
   [d] FIRE away with the primary particles!
   
     
 4- DETECTOR RESPONSE & SCORING
    In order to score quantities of interest, the following optinal user-defined actions were defined
    [a] VHDMSDSteppingAction: Set up a tally to keep track of energy histogram of electrons and photons 
        (this info is used as part of estimating skeletal dosimetry based on UF/NCI methods)
    [b] VHDMultiSDEventAction: Set up only for printing out event ID during a simultion
    [c] VHDMSDRunAction: set up the framework to keep updating the scorers from event to event (each 
        score is treated as a class and a container of scorers is interfaced for event-by-event update
    [d] VHDMultiSDRunAction: EndOfRunAction was defined to save the final scorer results in .raw 
        or .root format (defined in the derived class VHDMSDRunActionROOT)
    

 A- VISUALISATION

   this version of the VHDMSD does not turn on any visualization related function.  
   See VoxelizedHumanDoseMultiSDv2 to run simulations to visualize the geometry or run-time      
   visualization.
 
 C- HOW TO RUN

    - Execute VoxelizedHumanDoseMultiSDv1  in the 'batch' mode from macro files 
      (without visualization): see the bash script in scripts/VHDMSDv1MultiRun.sh 
      VHDMSDv1MultiRun.sh [OrganName] [Run1] [Run2]
      [OrganName]: the name of source organ for this dose simultion
      [Run1]: the ID# of the starting simulation run
      [Run2]: the ID# of the ending simulatiion run
    - To process the .root files output from VHDMSDv1, do the following:
      [a] start root > root
      [b] run the root processing code in /rootC/Root2Dat_EdepTree.C, Root2Dat_SrcEngHIST.C, etc.by 
          .x rootC/Root2Dat_EdepTree.C (".......")
      [c] appropriate .dat files will be generated for any data post-processing


	
