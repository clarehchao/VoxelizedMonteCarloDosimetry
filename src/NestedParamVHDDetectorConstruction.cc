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
 * @file   NestedParamVHDDetectorConstruction.cc
 * @brief  set up the detector geometry using G4 nested parameterization approach
 *
 * @date   4th June 2014
 * @author Shih-ying Huang
 * @name   Geant4.9.6-p02
 */

#include "globals.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "NestedParamVHDDetectorConstruction.hh"
#include "G4SDManager.hh"
#include "G4SDParticleWithEnergyFilter.hh"
#include "VHDNestedParamCellFlux.hh"
#include "VHDNestedParamPSEnergyDeposit.hh"

//#ifdef G4VIS_USE
//#include "G4VisAttributes.hh"
//#endif

NestedParamVHDDetectorConstruction::NestedParamVHDDetectorConstruction() : VHDDetectorConstruction()
{
  G4cout << "Construct NestedParamVHDDetectorConstruction!" << G4endl;
}

NestedParamVHDDetectorConstruction::~NestedParamVHDDetectorConstruction()
{
  //delete all primitive scorers by calling the destructor of each primitive scorer!
  G4int Nprim = MFDet->GetNumberOfPrimitives();
  for(G4int n = 0; n < Nprim; n++){
  	delete MFDet->GetPrimitive(n);
  }

  delete param;
  G4cout << "destroy NestedParamVHDDetectorConstruction" << G4endl;
}

void NestedParamVHDDetectorConstruction::SetMultiSensDet(G4LogicalVolume* voxel_logic)
{
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4String phantomSDname = "PhantomSD";

  MFDet = new G4MultiFunctionalDetector(phantomSDname);
  SDman->AddNewDetector(MFDet);             // Register SD to SDManager.
  voxel_logic->SetSensitiveDetector(MFDet);  // Assign SD to the logical volume.

  //==========================Total energy deposit scorer=========================================
  G4String psName = "totalEDep";
  VHDNestedParamPSEnergyDeposit* scorer0 = new VHDNestedParamPSEnergyDeposit(psName,nVoxelX,nVoxelY,nVoxelZ);
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
      VHDNestedParamCellFlux* scorer = new VHDNestedParamCellFlux(psgName,nVoxelX,nVoxelY,nVoxelZ);
      scorer->SetMaterialsOfInterest(fMaterialsOfInterest);  //define the material of interest
      scorer->Weighted(FALSE);
      scorer->SetFilter(pkinEFilter);    // Assign filter
      MFDet->RegisterPrimitive(scorer);  // Register it to MultiFunctionalDetector
  }
  engbin.clear();

  /*
  // to determine photon/electron contribution to dose
  kmin = 0.0*keV;
  G4String pname = "PhotonCellFluxAll";
  G4SDParticleWithEnergyFilter* pkinEFilter2 = new G4SDParticleWithEnergyFilter(filterName="photon filter",kmin,kmax);
  pkinEFilter2->add("gamma");
  VHDNestedParamCellFlux* scorer2 = new VHDNestedParamCellFlux(pname,nVoxelX,nVoxelY,nVoxelZ);
  scorer2->SetMaterialsOfInterest(fMaterialsOfInterest);  //define the material of interest
  scorer2->Weighted(FALSE);
  scorer2->SetFilter(pkinEFilter2);    // Assign filter
  MFDet->RegisterPrimitive(scorer2);  // Register it to MultiFunctionalDetector


  pname = "ElectronCellFluxAll";
  G4SDParticleWithEnergyFilter* pkinEFilter3 = new G4SDParticleWithEnergyFilter(filterName="electron filter",kmin,kmax);
  pkinEFilter3->add("e-");
  VHDNestedParamCellFlux* scorer3 = new VHDNestedParamCellFlux(pname,nVoxelX,nVoxelY,nVoxelZ);
  scorer3->SetMaterialsOfInterest(fMaterialsOfInterest);  //define the material of interest
  scorer3->Weighted(FALSE);
  scorer3->SetFilter(pkinEFilter3);    // Assign filter
  MFDet->RegisterPrimitive(scorer3);  // Register it to MultiFunctionalDetector*/
      
  G4cout << "end of setting up the multifunctional detectors..." << G4endl;
}

//-------------------------------------------------------------
void NestedParamVHDDetectorConstruction::ConstructPhantom()
{
//#ifdef G4VERBOSE
  G4cout << "NestedParamVHDDetectorConstruction::ConstructPhantom " << G4endl;
  G4cout << "nx,ny,nz: " << nVoxelX << ", " << nVoxelY << ", " << nVoxelZ << G4endl;
  G4cout << "voxelhalfX, voxelhalfY, voxelhalfZ: " << voxelHalfDimX << ", " << voxelHalfDimY << ", " << voxelHalfDimZ << G4endl;
//#endif

  //----- Replication of Water Phantom Volume.
  //--- Y Slice
  G4String yRepName("RepY");
  G4VSolid* solYRep = new G4Box(yRepName,nVoxelX*voxelHalfDimX,voxelHalfDimY,nVoxelZ*voxelHalfDimZ);
  G4LogicalVolume* logYRep =new G4LogicalVolume(solYRep,air,yRepName);
  new G4PVReplica(yRepName,logYRep,container_logic,kYAxis,nVoxelY,voxelHalfDimY*2.);
  //G4PVReplica* repy = new G4PVReplica(yRepName,logYRep,container_logic,kYAxis,nVoxelY,voxelHalfDimY*2.);
  //G4cout << "RepY No of replica: " << repy->GetMultiplicity() << G4endl;
  
//#ifdef G4VIS_USE  
//  logYRep->SetVisAttributes(G4Colour(1.0,0.0,0.0));
//#endif

  //--- X Slice
  G4String xRepName("RepX");
  G4VSolid* solXRep = new G4Box(xRepName,voxelHalfDimX,voxelHalfDimY,nVoxelZ*voxelHalfDimZ);
  G4LogicalVolume* logXRep = new G4LogicalVolume(solXRep,air,xRepName);
  new G4PVReplica(xRepName,logXRep,logYRep,kXAxis,nVoxelX,voxelHalfDimX*2.);
  //G4PVReplica* repx = new G4PVReplica(xRepName,logXRep,logYRep,kXAxis,nVoxelX,voxelHalfDimX*2.);
  //G4cout << "RepX No of replica: " << repx->GetMultiplicity() << G4endl;

//#ifdef G4VIS_USE  
//  logXRep->SetVisAttributes(G4Colour(0.0,1.0,0.0));
//#endif
  
  
  //----- Voxel solid and logical volumes
  //--- Z Slice
  G4VSolid* solVoxel = new G4Box("phantom",voxelHalfDimX,voxelHalfDimY,voxelHalfDimZ);
  G4LogicalVolume* logicVoxel = new G4LogicalVolume(solVoxel,air,"phantom");
//#ifdef G4VIS_USE
//  logicVoxel->SetVisAttributes(G4Colour(0.0,0.0,1.0));
//#endif
  
  // Parameterisation for transformation of voxels.
  //  (voxel size is fixed in this example. 
  //    e.g. nested parameterisation handles material and transfomation of voxels.)
  // must set up the param appropriately before passing it into G4PVPamameterised!!!!
  G4ThreeVector voxelSize(voxelHalfDimX,voxelHalfDimY,voxelHalfDimZ);
  param = new VHDNestedPhantomParameterisation(voxelSize,fOriginalMaterials);
  param->SetMaterialIndices( fMateIDs );
  param->SetNoVoxel( nVoxelX, nVoxelY, nVoxelZ );

  new G4PVParameterised("phantom",    // their name
			logicVoxel, // their logical volume
			logXRep,      // Mother logical volume
			//kZAxis,       // Are placed along this axis 
			kUndefined,        // Are placed along this axis 
			nVoxelZ,      // Number of cells
			param);       // Parameterisation

 /*param_phys = new G4PVParameterised("phantom",    // their name
			logicVoxel,   // their logical volume
			logXRep,      // Mother logical volume
			kZAxis,       // Are placed along this axis 
			kUndefined,        // Are placed along this axis 
			//nVoxelZ,      // Number of cells
			param,        // Parameterisation
			true);        // pSurfChk = true to check */ 

  SetMultiSensDet(logicVoxel);
}
