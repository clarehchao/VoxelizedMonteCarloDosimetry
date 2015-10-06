## Voxelized Monte Calor Dosimety Simulations

1. Author: Shih-ying (Clare) Huang
2. Institution: University of California, San Francisco
3. Organization: Physics Research Laboratory (Principle Investigator: Prof. Youngho Seo)
4. Software: 
	- Geant4 - an Object-Oriented Toolkit for Simulation in High Energy Physics
	- VoxelizedHumanDoseMultiSDv3

### Introduction

This application performs internal dosimetry calculation for user-specific geometry and source distribution by tallying dose information per voxel in the defined geometry.

Improvement from VoxelizedHumanDoseMultiSDv1 include:
- Added a factory design pattern class for DetectorConstruction class 
- Added a Factory design pattern class for RunAction class

#### GEOMETRY DEFINITION

The geometry is constructed in the VHDDetectorConstruction class derived from the Geant4 abstract class, G4VUserDetectorConstruction. The VHDDetectorConstruction class does the following:

1. Initialize and define elements and tissue composition needed for a given volume with its defined materials
2. Read in the volume data files (.g4m) and merge all the data from each image slice via VHDPhantomZSliceHeader
3. Construct the voxelized container based on the voxel size of the given volume
4. Construct the phantom implemented by its derived class RegularVHDDetectorConstruction or NestedParamVHDDetectorConstruction (per user's choice via DetectorContFactory class)

Note:
- Prior to running this application, geometry files will need to be generated in the .g4m format (see [Image-based Dosimetry Python Toolkit] (https://github.com/clarehchao/ImageBasedDosimetryTool))
- There are several scorers defined as well to tally the physical quantities of interest such as
	- VHDMSDCellFlux and VHDMSDCellFlux_RegParam
	- VHDPSEnergyDeposit and VHDPSEnergyDeposit_RegParam
	- VHDMSDNofStep was implemented as an optional scorer; it's only defined for the nested 
         parameterized geometry definition (see GetIndex(....) in its class definition)
	- these derived classes have different definition in the member function "GetIndx(....)" for nested parameterized vs.regular parameterized geometry (DetectorContFactory class will use one of the geoemtry option)

#### PHYSICS LIST

This application uses the modular physics lists as a starter to define the physics needed for dosimetry purpose.  Since this application is designed to work with any form of targeted radionuclide therapy or other radiation therapy, the option to select the appropriate physics lists defined in Geant4 are necessary.  By default, G4EmStandardPhysics, G4DecayPhysics, and G4RadioactiveDecayPhysics are registered in the user-defined physicslist class.  A physics messenger class, VHDPhysicsListMessenger, was defined for users to select physics lists via macro or command line interactively.
 	 
#### PRIMARY GENERATOR

The primary generator is defined in the VHDPrimaryGeneratorAction class, which does the following..
1. Declare a particle gun for primary generation
2. Read the position probability map defined by the user (.g4m)
3. Sample the particle position and momentum using Rejection sampling and built-in function that generates isotropic momemtum (this can replaced by using G4ParticleSource)
4. FIRE away with the primary particles!

#### DETECTOR RESPONSE & SCORING

In order to score quantities of interest, the following optinal user-defined actions were defined

1. VHDMSDSteppingAction: Set up a tally to keep track of energy histogram of electrons and photons (this info is used as part of estimating skeletal dosimetry based on UF/NCI methods)
2. VHDMultiSDEventAction: Set up only for printing out event ID during a simultion
3. VHDMSDRunAction: set up the framework to keep updating the scorers from event to event (each score is treated as a class and a container of scorers is interfaced for event-by-event update
4. VHDMultiSDRunAction: EndOfRunAction was defined to save the final scorer results in .raw or .root format (defined in the derived class VHDMSDRunActionROOT)
5. RunActionFactory class will select which RunAction to use for the run (VHDMultiRunAction or VHDMultiRunActionROOT class)
    

#### MC Simulation Setup:
To modify any MC-related setup, see the file macro/I131_Emphysics2.mac
```
# Set the isotope for the source particle, e.g. I-131
/gun/ion 53 131 0 0  #A (atomicNumber), Z (atomicMass), Q (charge of ion in unit of e), E (excitation energy in kev)
/grdm/nucleusLimits 131 131 53 53 # restrict radioactive decay to I-131

# Set the # of total events in the MC simulation
/run/beamOn 1000000 # number of particles

# Set up the physics list option: emstandard_opt1, emstandard_opt2, emstandard_opt3, emstandard_opt4, emlivermore, or empenelope
/VHDMSDv1/phys/addPhysics emstandard_opt4
```

##### HOW TO RUN

1. Execute VoxelizedHumanDoseMultiSDv3  in the 'batch' mode from macro files (without visualization)
```
# See the bash script in scripts/VHDMSDv3MultiRun.sh 

> VHDMSDv3MultiRun.sh [Geo_id] [SrcOrganName] [Run1] [Run2]
# [Geo_id]: the name of the geometry used in the Monte Carlo simulations
# [SrcOrganName]: the name of source organ for this dose simultion
# [Run1]: the ID # of the starting simulation run
# [Run2]: the ID # of the ending simulatiion run

# For multiple Monte Carlo simulations:

# Set the source organ and geo_id appropriately in multijob.sh
SO=Brain
geoname=MIBGPT1_segCT

# Submit multiple MC simulations
> source multijob.sh > multijoblog.txt

```
2. To process the .root files output from VHDMSDv3, do the following to generate appropriate .dat files for data post-processing:
```
# start the application Root
> root

# Run the root processing code in rootC/Root2Dat\_EdepTree.C, Root2Dat\_SrcEngHIST.C, etc.
> .x rootC/Root2Dat_EdepTree.C (".......")
```
