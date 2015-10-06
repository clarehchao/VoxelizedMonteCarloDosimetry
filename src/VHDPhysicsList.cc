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
 * @file   VHDPhysicsList.cc
 * @brief  Physics list definition
 * @date   7th Aug 2013
 * @author Shih-ying Huang
 * @name   Geant4.9.6-p02
 */

#include "VHDPhysicsList.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "VHDPhysicsListMessenger.hh"

VHDPhysicsList::VHDPhysicsList():  G4VModularPhysicsList()
{
  // default cut value  (1.0mm)
  defaultCutValue = 1.0*mm;
  //defaultCutValue = 0.5*mm;
  SetVerboseLevel(0);
  G4cout << "In VHDPhysicsList.cc, defaultCutValue = " << defaultCutValue << " mm" << G4endl;

  //use the Messenger!
  pMessenger = new VHDPhysicsListMessenger(this);
  
  //By default, the default G4 standard EM physics list is used
  emName = G4String("emstandard_default");  
  emPhysicsList = new G4EmStandardPhysics();  //verbose = 0
  RegisterPhysics(emPhysicsList);
  
  //Use G4DecayPhysics and G4RadioactiveDecayPhysics for grdm UI command usage for radioactive decay source and weak interaction physics
  //need these physics to simulate radioisotopes as a source (although one can use energy spectrum as well)
  RegisterPhysics(new G4DecayPhysics());
  RegisterPhysics(new G4RadioactiveDecayPhysics());
}

VHDPhysicsList::~VHDPhysicsList()
{
	G4cout << "Destroying VHDPhysicsList..." << G4endl;
	//delete emPhysicsList;  //doesn't like it when deleting emPhysicsList here (i suppose it's been deleted as part of PhysicsVector (?)
	delete pMessenger;
}

void VHDPhysicsList::SetCuts()
{
  //the default cut value for all particle types
  SetCutsWithDefault();
  
  G4cout << "current set cut value: " << defaultCutValue/mm << " mm" << G4endl;
}

void VHDPhysicsList::AddPhysicsList(const G4String& name)
{

  G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;	
  
  if(name == emName){
	G4cout << "Physics list, G4EmStandardPhysics, is registered!" << G4endl;
  }

  if (name == "emstandard_opt1") {
    emName = name;
    itr = find(physicsVector->begin(),physicsVector->end(),emPhysicsList); 
    if(itr != physicsVector->end()){
    	delete (*itr);
	physicsVector->erase(itr);
    	RegisterPhysics(new G4EmStandardPhysics_option1());
    }
    else
    	G4cout << "In VHDPhysicsList::AddPhysicsList, cannot find emPhysicsList in physicsVector!" << G4endl;

  }
  else if (name == "emstandard_opt2") {
    emName = name;
    itr = find(physicsVector->begin(),physicsVector->end(),emPhysicsList); 
    if(itr != physicsVector->end()){
    	delete (*itr);
	physicsVector->erase(itr);
    	RegisterPhysics(new G4EmStandardPhysics_option2());
    }
    else
    	G4cout << "In VHDPhysicsList::AddPhysicsList, cannot find emPhysicsList in physicsVector!" << G4endl;
  }
  else if (name == "emstandard_opt3") {
    emName = name;
    itr = find(physicsVector->begin(),physicsVector->end(),emPhysicsList); 
    if(itr != physicsVector->end()){
    	delete (*itr);
	physicsVector->erase(itr);
    	RegisterPhysics(new G4EmStandardPhysics_option3());
    }
    else
    	G4cout << "In VHDPhysicsList::AddPhysicsList, cannot find emPhysicsList in physicsVector!" << G4endl;
  }
  else if (name == "emstandard_opt4") {
    emName = name;
    itr = find(physicsVector->begin(),physicsVector->end(),emPhysicsList); 
    if(itr != physicsVector->end()){
    	delete (*itr);
	physicsVector->erase(itr);
    	RegisterPhysics(new G4EmStandardPhysics_option4());
    }
    else
    	G4cout << "In VHDPhysicsList::AddPhysicsList, cannot find emPhysicsList in physicsVector!" << G4endl;
  }
  else if (name == "emlivermore") {
    emName = name;
    itr = find(physicsVector->begin(),physicsVector->end(),emPhysicsList); 
    if(itr != physicsVector->end()){
    	delete (*itr);
	physicsVector->erase(itr);
    	RegisterPhysics(new G4EmLivermorePhysics());
    }
    else
    	G4cout << "In VHDPhysicsList::AddPhysicsList, cannot find emPhysicsList in physicsVector!" << G4endl;
  } else if (name == "empenelope") {
    emName = name;
    itr = find(physicsVector->begin(),physicsVector->end(),emPhysicsList); 
    if(itr != physicsVector->end()){
    	delete (*itr);
	physicsVector->erase(itr);
    	RegisterPhysics(new G4EmPenelopePhysics());
    }
    else
    	G4cout << "In VHDPhysicsList::AddPhysicsList, cannot find emPhysicsList in physicsVector!" << G4endl;
  }
  else {
    G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << " is not defined" << G4endl;
  }
}


