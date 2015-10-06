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
 * @file   VHDNestedParamPSEnergyDeposit.cc
 * @brief  energy deposit scorer defition for Index definition for NestedParameterization geometry
 *
 * @date   3rd June 2014
 * @author Shih-ying Huang
 * @name   Geant4.9.6-p02
 */

#include "VHDNestedParamPSEnergyDeposit.hh"


VHDNestedParamPSEnergyDeposit::VHDNestedParamPSEnergyDeposit(G4String name,G4int nx, G4int ny, G4int nz)
  :G4PSEnergyDeposit(name),fNx(nx),fNy(ny),fNz(nz)
{
	fNxNy = fNx*fNy;
	//G4cout << "in PSEnergyDeposit... fNxNy = " << fNxNy << G4endl;
}

VHDNestedParamPSEnergyDeposit::~VHDNestedParamPSEnergyDeposit()
{
	if(filter){  //'filter' is a private variable that is definied in G4VPrimitiveScorer
		delete filter;  //delete it if it's defined
	}
	G4cout << "destroying VHDNestedParamPSEnergyDeposit..." << G4endl;
}

G4int VHDNestedParamPSEnergyDeposit::GetIndex(G4Step* aStep)
{
  const G4VTouchable* touchable = aStep->GetPreStepPoint()->GetTouchable();
  G4int ix = touchable->GetReplicaNumber(1);
  G4int iy = touchable->GetReplicaNumber(2);
  G4int iz = touchable->GetReplicaNumber(0);
  return ix + iy*fNx + iz*fNxNy;

}

