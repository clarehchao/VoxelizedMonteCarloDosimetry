//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************

/**
 * @file   VoxelizedHumanDoseMultiSDv3.cc
 * @brief  calculate the dose to a geometry using voxelized geometry of human phantoms or clinical data with voxelized source distribution
 *	  
 * @date   3rd June 2014
 * @author Shih-ying Huang
 * @name   Geant4.9.6-p02
 */

#include "VHDDetectorConstruction.hh"
#include "VHDPrimaryGeneratorAction.hh"
#include "DetectorContFactory.hh"
#include "RunActionFactory.hh"
#include "VHDPhysicsList.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "VHDMultiSDEventAction.hh"
#include "VHDMultiSDRunAction.hh"
#include "VHDMultiSDRunActionROOT.hh"
#include "VHDMSDSteppingAction.hh"

int main(int argc,char **argv) {

  time_t start,end;
  double diffT;
  time(&start);   //get execution start time


  //===READ THE INPUT PARAMETERS=========
  G4String GEOtype = argv[1];
  G4String GEOdir = argv[2];
  G4String GEOname = argv[3];
  G4String SRCMPdir = argv[4];
  G4String SRCMPname = argv[5];
  G4int isSRCMPsparse = atoi(argv[6]);
  G4String DATAdir = argv[7];
  G4int elceh = atoi(argv[8]);
  G4int photoneh = atoi(argv[9]);
  G4String RUNACTtype = argv[10];
  G4int ebin = atoi(argv[11]);  //ebin == 0, use 25 energy bins; ebin == 1, use 28 energy bins
  G4String OOItype = argv[12];
  //===END OF READING INPUT PARAMETERS====
 

  //choose a random number generator
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

  // Run manager
  G4RunManager * runManager = new G4RunManager;
  G4cout << "after the runmanager!" << G4endl;

  //define the detector setup and what directory to read the image files
  DetectorContFactory* theDetFactory = new DetectorContFactory();
  VHDDetectorConstruction* theGeometry = theDetFactory->GetDetectorConstr(GEOtype);
  G4String geodirname = GEOdir + "/" + GEOname;
  theGeometry->SetDirName(geodirname);
  theGeometry->SetParticleFlag(elceh,photoneh);
  theGeometry->SetEnergyBinOption(ebin);
  theGeometry->SetOOItype(OOItype);
  runManager->SetUserInitialization(theGeometry);
  G4cout << "geodirname: " << geodirname << ", after the geometry!" << G4endl;
  
  //set up the physics list and the primary source parameter
  runManager->SetUserInitialization(new VHDPhysicsList);
  G4cout << "after the PhysicsList!" << G4endl;

  //user-defined source distribution
  G4String srcmpdirname = SRCMPdir + "/" + SRCMPname;
  VHDPrimaryGeneratorAction* primgen = new VHDPrimaryGeneratorAction(srcmpdirname,isSRCMPsparse);
  G4cout << "after the PrimaryGenerator!" << G4endl;
  runManager->SetUserAction(primgen);

  //Define data directory name
  size_t len;
  char datadrive[300];
  //len = datadirname.copy(datadrive,datadirname.length(),0);
  len = DATAdir.copy(datadrive,DATAdir.length(),0);
  datadrive[len] = '\0';

  //User SteppingAction  
  VHDMSDSteppingAction* step = new VHDMSDSteppingAction(datadrive);
  //step->SetMaterialOfInterest(geodirname);
  runManager->SetUserAction(step);
  G4cout << "after VHDMSDSteppingAction!" << G4endl;

  //User EventAction
  runManager->SetUserAction(new VHDMultiSDEventAction);
  G4cout << "after VHDMultiSDEventAction!" << G4endl;

  //=====================================================================
  //USING multifunction sensitive detector in RunAction Definition
  //=====================================================================
  RunActionFactory* theRunActFactory = new RunActionFactory();
  VHDMultiSDRunAction* run = theRunActFactory->GetRunAction(RUNACTtype);
  run->SetRunInfo(datadrive);
  runManager->SetUserAction(run);
  G4cout << "after VHDMultiSDRunAction!" << G4endl;
  //=====================================================================


  /*//initialize RunManager in the macro I131_EMPhysics2.mac instead of here in the code
  runManager->Initialize();  //Initialize G4 kernel
  G4cout << "after Initialize runManager for the 1st time!" << G4endl;*/
  
  //get the pointer to the User Interface manager 
  G4UImanager * UIman = G4UImanager::GetUIpointer();
  G4cout << "argc = " << argc << G4endl;
  G4String execommand = "/control/execute ";
  G4String macfile;
  
  macfile = argv[13];
  G4cout << "In batch mode: execute " << macfile << "! knut Knut KNUT!!!" << G4endl;
  UIman->ApplyCommand(execommand+macfile);

  // END OF THE APPLICATION
  time(&end);   //get execution end time
  diffT = difftime(end,start);
  G4cout << "It took " << diffT << " seconds to complete this execution!" << G4endl;
  G4cout << "W00t! Finish this VoxelizedHumanDoseMultiSDv1 simulation!" << G4endl;

  delete theDetFactory;
  delete theRunActFactory;
  delete runManager;
  return 0;
}






