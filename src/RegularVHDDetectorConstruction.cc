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
 * @file   RegularVHDDetectorConstruction.cc
 * @brief  set up the detector geometry using G4 regular navigation approach
 *
 * @date   7th Aug 2013
 * @author Shih-ying Huang
 * @name   Geant4.9.6-p02
 */

#include "globals.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

#include "RegularVHDDetectorConstruction.hh"
#include "VHDPhantomParameterisationColour.hh"
#include "G4SDManager.hh"
#include "G4SDParticleWithEnergyFilter.hh"
#include "VHDRegParamCellFlux.hh"
#include "VHDRegParamPSEnergyDeposit.hh"

RegularVHDDetectorConstruction::RegularVHDDetectorConstruction() : VHDDetectorConstruction()
{
  G4cout << "Construct RegularVHDDetectorConstruction!" << G4endl;
}

RegularVHDDetectorConstruction::~RegularVHDDetectorConstruction()
{

  //delete all primitive scorers by calling the destructor of each primitive scorer!
  G4int Nprim = MFDet->GetNumberOfPrimitives();
  for(G4int n = 0; n < Nprim; n++){
  	delete MFDet->GetPrimitive(n);
  }

  G4cout << "destroy RegularVHDDetectorConstruction" << G4endl;
}

void RegularVHDDetectorConstruction::SetMultiSensDet(G4LogicalVolume* voxel_logic)
{
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4String phantomSDname = "PhantomSD";

  MFDet = new G4MultiFunctionalDetector(phantomSDname);
  SDman->AddNewDetector(MFDet);             // Register SD to SDManager.
  voxel_logic->SetSensitiveDetector(MFDet);  // Assign SD to the logical volume.

  //==========================Total energy deposit scorer=========================================
  G4String psName = "totalEDep";
  VHDRegParamPSEnergyDeposit* scorer0 = new VHDRegParamPSEnergyDeposit(psName,nVoxelX,nVoxelY,nVoxelZ);
  MFDet->RegisterPrimitive(scorer0);

  //--- Cell flux for photon or electron with energy bin
  //=======================read the energy bin file ==============================//
  G4double tmp;
  std::vector<G4double> engbin;

  G4String fname;
  if(ebin == 0)
	fname = dirname + "/Energybin1.txt";   //25 energy bins; use this energy bin when using the DRFs provided by Choonsik Lee
  else
	fname = dirname + "/Energybin2.txt";  //28 energy bins; use this energy bin when using the DRFs from Wayson et al.'s datafile for newborn phantom
  G4cout << "energybin fname = " << fname << G4endl;
  std::ifstream finDF(fname);
  if(finDF.good() != 1 )
  {
     G4Exception("VHDDetectorConstruction:SetMultiSensDet(G4LogicalVolume* voxel_logic)","",FatalErrorInArgument,G4String("Invalid file name: " + fname).c_str());
  }
  finDF >> NEngbin;
  for(G4int i = 0; i < NEngbin; i++ ){
    finDF >> tmp;
    engbin.push_back(tmp);
  }
  finDF.close();
  //=======================end of reading the energy bin file ==============================//


  //==================== Construct Cell Flux scorers for a number of energy bins============//
  G4double kmin,kmax;
  G4String filterName;
  for (unsigned i = 0; i < engbin.size(); i++){
      char name[17];
      std::sprintf(name,"PhotonCellFlux%02d",i);
      G4String psgName(name);
      if(i == 0)
	kmin = 0.0*keV;
      else
        kmin = engbin[i-1]*keV;
      kmax = engbin[i]*keV;

      G4SDParticleWithEnergyFilter* pkinEFilter = new G4SDParticleWithEnergyFilter(filterName="photonE filter",kmin,kmax);
      if(electronflag)	pkinEFilter->add("e-");     //accept e- electron
      if(photonflag)	pkinEFilter->add("gamma");  // Accept gamma.
      pkinEFilter->show();        // Show accepting condition to stdout.
      
      //-----Cell Flux Scorer that store the total tracklength per volume --> per unit surface
      VHDRegParamCellFlux* scorer = new VHDRegParamCellFlux(psgName,nVoxelX,nVoxelY,nVoxelZ);
      scorer->SetMaterialsOfInterest(fMaterialsOfInterest);  //define the material of interest
      scorer->Weighted(FALSE);
      scorer->SetFilter(pkinEFilter);    // Assign filter
      MFDet->RegisterPrimitive(scorer);  // Register it to MultiFunctionalDetector
  }
  engbin.clear();
  G4cout << "end of setting up the multifunctional detectors..." << G4endl;
}


void RegularVHDDetectorConstruction::ConstructPhantom()
{
#ifdef G4VERBOSE
  G4cout << "In RegularVHDDetectorConstruction::ConstructPhantom " << G4endl;
#endif

  //----- Create parameterisation 
  VHDPhantomParameterisationColour* param = new VHDPhantomParameterisationColour();

  //----- Set voxel dimensions
  param->SetVoxelDimensions( voxelHalfDimX, voxelHalfDimY, voxelHalfDimZ );
  
  G4cout << "voxel dimension: " << voxelHalfDimX << ", " << voxelHalfDimY << ", " << voxelHalfDimZ << G4endl;

  //----- Set number of voxels 
  param->SetNoVoxel( nVoxelX, nVoxelY, nVoxelZ );
  G4cout << "container voxel size: " << param->GetNoVoxelX() << ", " << param->GetNoVoxelY() << ", " << param->GetNoVoxelZ() << G4endl;

  //----- Set list of materials
  //param->SetMaterials( fMaterials );
  param->SetMaterials(fOriginalMaterials);


  //----- Set list of material indices: for each voxel it is a number that correspond to the index of its material in the vector of materials defined above
  param->SetMaterialIndices( fMateIDs );

  //----- Define voxel logical volume
  G4Box* voxel_solid = new G4Box( "Voxel", voxelHalfDimX, voxelHalfDimY, voxelHalfDimZ);
  
  //G4cout << "voxel solid size: " << voxelHalfDimX << ", " << voxelHalfDimY << ", " << voxelHalfDimZ << G4endl;
  //G4LogicalVolume* voxel_logic = new G4LogicalVolume(voxel_solid,fMaterials[0],"VoxelLogical",0,0,0); // material is not relevant, it will be changed by the ComputeMaterial method of the parameterisation
  
  G4LogicalVolume* voxel_logic = new G4LogicalVolume(voxel_solid,fOriginalMaterials[0],"VoxelLogical",0,0,0); // material is not relevant, it will be changed by the

  //--- Assign the container volume of the parameterisation
  param->BuildContainerSolid(container_phys);

  //--- Assure yourself that the voxels are completely filling the container volume
  param->CheckVoxelsFillContainer( container_solid->GetXHalfLength(), 
                                   container_solid->GetYHalfLength(), 
                                   container_solid->GetZHalfLength());

  //----- The G4PVParameterised object that uses the created parameterisation should be placed in the container logical volume
  G4PVParameterised * phantom_phys = new G4PVParameterised("phantom",voxel_logic,container_logic,kXAxis, nVoxelX*nVoxelY*nVoxelZ, param);
  //G4PVParameterised * phantom_phys = new G4PVParameterised("phantom",voxel_logic,container_logic,kUndefined, nVoxelX*nVoxelY*nVoxelZ, param);   //create xx amount of G4Replica
  // if axis is set as kUndefined instead of kXAxis, GEANT4 will do an smart voxel optimisation (not needed if G4RegularNavigation is used)

  //----- Set this physical volume as having a regular structure of type 1, so that G4RegularNavigation is used
  phantom_phys->SetRegularStructureId(1); // if not set, G4VoxelNavigation will be used instead 

  SetMultiSensDet(voxel_logic);

}



