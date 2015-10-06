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
//
/**
 * @file   VHDPhantomZSliceHeader.cc
 * @brief  read the phantom Zslide header
 *
 * @date   7th Aug 2013
 * @author Shih-ying Huang
 * @name   Geant4.9.6-p02
 */

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4MaterialTable.hh"
#include "G4Material.hh"
#include "G4GeometryTolerance.hh"
#include "VHDPhantomZSliceHeader.hh"

//-------------------------------------------------------------
VHDPhantomZSliceHeader::VHDPhantomZSliceHeader( const VHDPhantomZSliceHeader& rhs )
{
  fNoVoxelX = rhs.GetNoVoxelX();
  fNoVoxelY = rhs.GetNoVoxelY();
  fNoVoxelZ = rhs.GetNoVoxelZ();
  fMinX = rhs.GetMinX();
  fMaxX = rhs.GetMaxX();
  fMinY = rhs.GetMinY();
  fMaxY = rhs.GetMaxY();
  fMinZ = rhs.GetMinZ();
  fMaxZ = rhs.GetMaxZ();
  fMaterialNames = rhs.GetMaterialNames(); 

}

//-------------------------------------------------------------
VHDPhantomZSliceHeader::VHDPhantomZSliceHeader( std::ifstream& fin )
{
  //----- Read material indices and names
  G4int nmate;
  G4String mateindex;
  G4String matename;
  fin >> nmate;
  //G4cout << "nmate = " << nmate << G4endl;
// #ifdef G4VERBOSE
//   G4cout << " VHDPhantomZSliceHeader reading number of materials " << nmate << G4endl;
// #endif

  for( G4int im = 0; im < nmate; im++ ){
    fin >> mateindex >> matename;
    //G4cout << "mateindx = " << mateindex << ", matename = " << matename << G4endl;
// #ifdef G4VERBOSE
//     G4cout << " VHDPhantomZSliceHeader reading material " << im << " : " << mateindex << "  " << matename << G4endl;
// #endif

    if( ! CheckMaterialExists( matename ) ) {  //check if the material specified in .g4dcm file is part of the material defined in detectorconstruction.cc
      G4cerr << "im = " << im << ": " << matename << " is found in file that is not built in the C++ code! " << G4endl;
      G4Exception("VHDPhantomZSliceHeader::VHDPhantomZSliceHeader( std::ifstream& fin )","",FatalErrorInArgument,"");
    }
    fMaterialNames.push_back(matename);
  }

  //----- Read number of voxels
  fin >> fNoVoxelX >> fNoVoxelY >> fNoVoxelZ;
// #ifdef G4VERBOSE
//   G4cout << " Number of voxels " << fNoVoxelX << " " << fNoVoxelY << " " << fNoVoxelZ << G4endl; 
// #endif

  //----- Read minimal and maximal extensions (= walls of phantom)
  fin >> fMinX >> fMaxX;
  fin >> fMinY >> fMaxY;
  fin >> fMinZ >> fMaxZ;
// #ifdef G4VERBOSE
//   G4cout << " Extension in X " << fMinX << " " << fMaxX << G4endl 
// 	 << " Extension in Y " << fMinY << " " << fMaxY << G4endl 
// 	 << " Extension in Z " << fMinZ << " " << fMaxZ << G4endl; 
// #endif

}

//-------------------------------------------------------------
G4bool VHDPhantomZSliceHeader::CheckMaterialExists( const G4String& mateName )
{
  G4bool bFound = FALSE;

  const G4MaterialTable* matTab = G4Material::GetMaterialTable();
  std::vector<G4Material*>::const_iterator matite;
  for( matite = matTab->begin(); matite != matTab->end(); matite++ ) {
    if( (*matite)->GetName() == mateName ) {
      bFound = TRUE;
      break;
    }
  }

  return bFound;
}


//-------------------------------------------------------------
void VHDPhantomZSliceHeader::operator+=( const VHDPhantomZSliceHeader& rhs )
{
  *this = *this + rhs;
}

//-------------------------------------------------------------
VHDPhantomZSliceHeader VHDPhantomZSliceHeader::operator+( const VHDPhantomZSliceHeader& rhs )
{
  //----- Check that both slices has the same dimensions
  if( fNoVoxelX != rhs.GetNoVoxelX() || fNoVoxelY != rhs.GetNoVoxelY() ) {
    G4cerr << "VHDPhantomZSliceHeader error adding two slice headers: !!! Different number of voxels: " 
	   << "  X= " << fNoVoxelX << " =? " << rhs.GetNoVoxelX() 
	   << "  Y=  " << fNoVoxelY << " =? " << rhs.GetNoVoxelY()  
	   << "  Z=  " << fNoVoxelZ << " =? " << rhs.GetNoVoxelZ() 
	   << G4endl;
    G4Exception("VHDPhantomZSliceHeader::operator+=( const VHDPhantomZSliceHeader& rhs )","",FatalErrorInArgument,"");
  }
  //----- Check that both slices has the same extensions
  if( fMinX != rhs.GetMinX() || fMaxX != rhs.GetMaxX() || fMinY != rhs.GetMinY() || fMaxY != rhs.GetMaxY() ) {
    G4cerr << "VHDPhantomZSliceHeader error adding two slice headers: !!! Different extensions: " 
	   << "  Xmin= " << fMinX << " =? " << rhs.GetMinX() 
	   << "  Xmax= " << fMaxX << " =? " << rhs.GetMaxX() 
	   << "  Ymin= " << fMinY << " =? " << rhs.GetMinY() 
	   << "  Ymax= " << fMaxY << " =? " << rhs.GetMaxY() 
	   << G4endl;
    G4Exception("VHDPhantomZSliceHeader::operator+=( const VHDPhantomZSliceHeader& rhs )","",FatalErrorInArgument,"");
  }
  
  //----- Check that both slices has the same materials
  std::vector<G4String> fMaterialNames2 = rhs.GetMaterialNames();
  if( fMaterialNames.size() != fMaterialNames2.size() ) {
    G4cerr << "Eror adding two slice headers: !!! Different number of materials: " << fMaterialNames.size() << " =? " << fMaterialNames2.size() << G4endl;
    G4Exception("VHDPhantomZSliceHeader::operator+=( const VHDPhantomZSliceHeader& rhs )","",FatalErrorInArgument,"");
  }
  for( unsigned int ii = 0; ii < fMaterialNames.size(); ii++ ) {
    if( fMaterialNames[ii] != fMaterialNames2[ii] ) {
      G4cerr << "Error adding two slice headers: !!! Different material number " << ii << " : " << fMaterialNames[ii] << " =? " << fMaterialNames2[ii] << G4endl;
      G4Exception("VHDPhantomZSliceHeader::operator+=( const VHDPhantomZSliceHeader& rhs )","",FatalErrorInArgument,"");
    }
  }
   
  //----- Check that the slices are contiguous in Z
  if( std::fabs( fMinZ - rhs.GetMaxZ() ) > G4GeometryTolerance::GetInstance()->GetRadialTolerance() && 
      std::fabs( fMaxZ - rhs.GetMinZ() ) > G4GeometryTolerance::GetInstance()->GetRadialTolerance() ){
    G4cerr << "Error adding two slice headers: !!! Slices are not contiguous in Z "
	   << "  Zmin= " << fMinZ << " & " << rhs.GetMinZ() 
	   << "  Zmax= " << fMaxZ << " & " << rhs.GetMaxZ() 
	   << G4endl;
    G4Exception("VHDPhantomZSliceHeader::operator+=( const VHDPhantomZSliceHeader& rhs )","",FatalErrorInArgument,"");
  }

  //----- Build slice header copying first one
  VHDPhantomZSliceHeader temp( *this );

  //----- Add data from second slice header
  temp.SetMinZ( std::min( fMinZ, rhs.GetMinZ() ) );
  temp.SetMaxZ( std::max( fMaxZ, rhs.GetMaxZ() ) );
  temp.SetNoVoxelZ( fNoVoxelZ + rhs.GetNoVoxelZ() );

  return temp;
}
