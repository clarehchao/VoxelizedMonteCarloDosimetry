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
 * @file   VHDPhysicsListMessenger.cc
 * @brief  define the messenger for the physics list
 *
 * @date   7th Aug 2013
 * @author Shih-ying Huang
 * @name   Geant4.9.6-p02
 */

#include "VHDPhysicsListMessenger.hh"
#include "VHDPhysicsList.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"


VHDPhysicsListMessenger::VHDPhysicsListMessenger(VHDPhysicsList* pPhys)
:pPhysicsList(pPhys)
{
  physDir = new G4UIdirectory("/VHDMSDv1/phys/");
  physDir->SetGuidance("physics list commands");

  allCutCmd = new G4UIcmdWithADoubleAndUnit("/VHDMSDv1/phys/setCuts",this);
  allCutCmd->SetGuidance("Set cut for all.");
  allCutCmd->SetParameterName("cut",false);
  allCutCmd->SetUnitCategory("Length");
  allCutCmd->SetRange("cut>0.0");
  allCutCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  pListCmd = new G4UIcmdWithAString("/VHDMSDv1/phys/addPhysics",this);
  pListCmd->SetGuidance("Add modular physics list.");
  pListCmd->SetParameterName("PList",false);
  pListCmd->AvailableForStates(G4State_PreInit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

VHDPhysicsListMessenger::~VHDPhysicsListMessenger()
{
  delete allCutCmd;
  delete pListCmd;
  delete physDir;    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void VHDPhysicsListMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{       
  if( command == allCutCmd )
  {
      G4cout << "in all cut command ..." << G4endl;
      G4double cut = allCutCmd->GetNewDoubleValue(newValue);
      pPhysicsList->SetDefaultCutValue(cut);
      pPhysicsList->SetCuts();
  } 

  if( command == pListCmd )
  { 
  	G4cout << "in pList command ..." << G4endl;
  	pPhysicsList->AddPhysicsList(newValue);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
