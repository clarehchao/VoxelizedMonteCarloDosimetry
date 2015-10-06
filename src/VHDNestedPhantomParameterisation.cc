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
 * @file   VHDNestedParamParameterisation.cc
 * @brief  nested parameterisation definition
 *
 * @date   7th Aug 2013
 * @author Shih-ying Huang
 * @name   Geant4.9.6-p02
 */

#include "VHDNestedPhantomParameterisation.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VTouchable.hh"
#include "G4ThreeVector.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "VHDMultiSDEventAction.hh"
#include "G4RunManager.hh"

#ifdef G4VIS_USE
#include "G4VisAttributes.hh"
#endif

VHDNestedPhantomParameterisation::VHDNestedPhantomParameterisation(const G4ThreeVector& voxelSize,std::vector<G4Material*>& mat):
  G4VNestedParameterisation(), fdX(voxelSize.x()),
  fdY(voxelSize.y()), fdZ(voxelSize.z()), fMaterials(mat)
{
  // Position of voxels. 
  // x and y positions are already defined in DetectorConstruction by using
  // replicated volume. Here only we need to define is z positions of voxels.
  fMaterialIndices = 0;
  
//#ifdef G4VIS_USE
 //  ReadColourData();
 // G4cout << "Implement ReadColourData()..." << G4endl;
//#endif
  G4cout << "construct VHDNestedPhantomParameterisation ... " << G4endl;
}

VHDNestedPhantomParameterisation::~VHDNestedPhantomParameterisation()
{
   //delete all content of std::map<G4String,G4VisAttributes*> fColours
   for(std::map<G4String,G4VisAttributes*>::const_iterator it = fColours.begin(); it != fColours.end(); ++it)
   {
	//G4cout << "in the fColours loop to delete each G4VisAttribute object!" << G4endl;  //check! it is indeed deleting each G4VisAttributes in fColour map
	//delete (*it).second;  //delete all the G4VisAttributes stored in fColours map
	delete it->second;
   }
   fColours.clear();

   G4cout << "destroying VHDNestedPhantomParameterisation!" << G4endl;
}

void VHDNestedPhantomParameterisation::SetNoVoxel( unsigned int nx, unsigned int ny, unsigned int nz )
{
  fnX = nx;
  fnY = ny;
  fnZ = nz;
  //G4cout << "nx, ny,nz = " << fnX << ", " << fnY << ", " << fnZ << G4endl;
}

// Material assignment to geometry.
G4Material* VHDNestedPhantomParameterisation::ComputeMaterial(G4VPhysicalVolume* physVol, const G4int copyNoZ, const G4VTouchable* parentTouch)
{
  G4Material* mate;
  // protection for initialization and vis at idle state
  if(parentTouch==0) return fMaterials[0];

  // Copy number of voxels. 
  // Copy number of X and Y are obtained from replication number.
  // Copy nymber of Z is the copy number of current voxel.
  G4int ix = parentTouch->GetReplicaNumber(0);
  G4int iy = parentTouch->GetReplicaNumber(1);
  G4int iz = copyNoZ;

  G4int copyNo = ix + fnX*iy + fnX*fnY*iz;
  
  /*//Debugging
  const VHDMultiSDEventAction* evt =(const VHDMultiSDEventAction*)(G4RunManager::GetRunManager()->GetUserEventAction());
  if(evt->GetEvtID() == 902741)
    G4cout << "VHDNestedPhantomParameterisation::ComputeMaterial: ix,iy,iz: " << ix << ", " << iy << ", " << iz << ", copyNoZ = " << copyNoZ << G4endl;*/

  unsigned int matIndex = GetMaterialIndex(copyNo);
  //G4cout << "matIndex = " << matIndex << G4endl;

  mate = fMaterials[matIndex];

  G4String mateName = mate->GetName();
  //G4cout << "mateName = " << mateName << G4endl;

/*#ifdef G4VIS_USE
   //set the material color attribute
   std::map<G4String,G4VisAttributes*>::const_iterator ite = fColours.find(mateName);
   if(ite != fColours.end()){
   	//const G4Colour col = ((*ite).second)->GetColour();
 	//G4cout << "(*ite).first: " << (*ite).first << ", (*ite).second->color: " << col.GetRed() << ", " << col.GetGreen() << ", " << col.GetBlue() << G4endl;
 	physVol->GetLogicalVolume()->SetVisAttributes( (*ite).second ); 
 	//G4Colour check = physVol->GetLogicalVolume()->GetVisAttributes()->GetColour();
 	//G4cout << "color check: " << check.GetRed() << ", " << check.GetGreen() << ", " << check.GetBlue() << G4endl;
 	
   } 
   else{
 	physVol->GetLogicalVolume()->SetVisAttributes( (*(fColours.begin()) ).second ); // set it as unseen
   }
#endif
*/
  return mate;
}

//------------------------------------------------------------------
unsigned int VHDNestedPhantomParameterisation::GetMaterialIndex( unsigned int copyNo ) const
{
  return *(fMaterialIndices+copyNo); //so *(fMaterialIndices+copyNo) is the same as fMaterialIndices[copyNo]
  //THIS IS DEREFERENCING IN C++!!! ex: a[5] = 0 means a [offset of 5] = 0, and *(a+5) = 0 means pointed by (a+5) = 0
}

// Number of Materials
// Material scanner is required for preparing physics tables and so on before 
// starting simulation, so that G4 has to know number of materials.
//
G4int VHDNestedPhantomParameterisation::GetNumberOfMaterials() const
{
  return fMaterials.size();
}

//
// GetMaterial
//  This is needed for material scanner and realizing geometry.
G4Material* VHDNestedPhantomParameterisation::GetMaterial(G4int i) const
{
  return fMaterials[i];
}

//
// Transformation of voxels.
//
void VHDNestedPhantomParameterisation::ComputeTransformation(const G4int copyNoZ, G4VPhysicalVolume* physVol) const
{
  G4ThreeVector position(0.,0.,(2*copyNoZ+1)*fdZ - fdZ*fnZ);
  physVol->SetTranslation(position);
  
  /*// For debugging
  const VHDMultiSDEventAction* evt =(const VHDMultiSDEventAction*)(G4RunManager::GetRunManager()->GetUserEventAction());
  if(evt->GetEvtID() == 902741)
    G4cout << "copyNoZ: " << copyNoZ << ", position: " << position.x() << ", " << position.y() << ", " << position.z() << G4endl;*/
  
}

//
// Dimensions are always same
//
void VHDNestedPhantomParameterisation::ComputeDimensions( G4Box& box, const G4int, const G4VPhysicalVolume* ) const
{
  box.SetXHalfLength(fdX);
  box.SetYHalfLength(fdY);
  box.SetZHalfLength(fdZ);
}

void VHDNestedPhantomParameterisation::ReadColourData()
{
  //----- Add a G4VisAttributes for materials not defined in file;
  G4VisAttributes* blankAtt = new G4VisAttributes;
  blankAtt->SetVisibility( FALSE );
  fColours["Default"] = blankAtt;
  
  //----- Read file
  //std::ifstream fin("/home/shuang/g4work/VoxelizedHumanDose/ColourMap.dat");
  std::ifstream fin("ColourMap.dat");
  G4int nMate;
  G4String mateName;
  G4double cred, cgreen, cblue, copacity;
  fin >> nMate;
  for( G4int ii = 0; ii < nMate; ii++ ){
    fin >> mateName >> cred >> cgreen >> cblue >> copacity;
    copacity = 0.0;
    G4cout << mateName << ": " << cred << ", " << cgreen << ", " << cblue << ", " << copacity << G4endl;
    G4Colour colour( cred, cgreen, cblue, copacity );
    //G4VisAttributes* visAtt = new G4VisAttributes( colour );
    //visAtt = new G4VisAttributes;
    G4VisAttributes* visAtt = new G4VisAttributes(colour);
    //instead of setting the opacity; one needs to actively set materials of interest to invisible
    if(mateName == "Air")   visAtt->SetVisibility(false);
    if(mateName == "Muscle")   visAtt->SetVisibility(false);
    //if(mateName == "ResidualSoftTissue")   visAtt->SetVisibility(false);
    if(mateName == "Skin")   visAtt->SetVisibility(false);
    visAtt->SetForceWireframe(FALSE);
    visAtt->SetForceSolid(TRUE);
    fColours[mateName] = visAtt;
    //G4Colour col = visAtt->GetColour();
    //G4cout << "in ReadColourData: visAtt->colour: " << col.GetRed() << ", " << col.GetGreen() << ", " << col.GetBlue() << G4endl;
  }
  fin.close();
  
  //delete blankAtt;
  //delete visAtt;
}
