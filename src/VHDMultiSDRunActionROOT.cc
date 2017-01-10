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
 * @file   VHDMSDRunActionROOT.cc
 * @brief  Run action definition with ROOT usage
 *
 * @date   7th Aug 2013
 * @author Shih-ying Huang
 * @name   Geant4.9.6-p02
 */


#include "VHDMultiSDRunActionROOT.hh"
#include "VHDMultiSDRun.hh"
#include "G4RunManager.hh"
#include "VHDDetectorConstruction.hh"
#include "G4THitsMap.hh"
#include "G4UnitsTable.hh"


//=======================================================================
// VHDMultiSDRunActionROOT
//  a derived class of VHDMultiSDRunAction
//  EndOfRun definition: save the dose scorers in sparse matrix format
//			 in .root format
//
//
//=======================================================================

// Constructor
VHDMultiSDRunActionROOT::VHDMultiSDRunActionROOT():VHDMultiSDRunAction()
{
  G4cout << "Construct VHDMultiSDRunActionROOT! " << G4endl;
}

// Destructor.
VHDMultiSDRunActionROOT::~VHDMultiSDRunActionROOT()
{
  G4cout << "Destroying VHDMultiSDRunActionROOT! " << G4endl;
}


void VHDMultiSDRunActionROOT::EndOfRunAction(const G4Run* aRun)
{

  //print out the total number of events during this run
  G4cout << "Number of Events in this run: " << aRun->GetNumberOfEvent() << G4endl;

  //- VHDMultiSDRun object.
  VHDMultiSDRun* MSDRun = (VHDMultiSDRun*)aRun;

  //--- Dump all socred quantities involved in VHDMultiSDRun to check for ouptput!
  //MSDRun->DumpAllScorer();
  //---

  //
  //- water phantom (Detector) Information.
  //-- Number of segments in the water phantom.

  const VHDDetectorConstruction* detector =(const VHDDetectorConstruction*)(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  fNx = detector->GetNX();
  fNy = detector->GetNY();
  fNz = detector->GetNZ();
  fNxNy = fNx*fNy;
  NEbin = detector->GetNEngbin();
  G4cout << "fNx, fNy, fNz = " << fNx << ", " << fNy << ", " << fNz << G4endl;
  G4cout << "NEbin = " << NEbin << G4endl;

  //---------------------------------------------
  // Dump accumulated quantities for this RUN.
  //  (Display only central region of x-y plane)
  //---------------------------------------------
  G4THitsMap<G4double>* totEdep = MSDRun->GetHitsMap("PhantomSD/totalEDep");
  //G4cout << "after reading the totalEDep G4THitsMap..." << G4endl;
  
  //G4THitsMap<G4double>* totNstep = MSDRun->GetHitsMap("PhantomSD/totalNofStep");

  //std::vector<G4THitsMap<G4double>*> pCurrHit;
  std::vector<G4THitsMap<G4double>*> pCellFlux;
  G4THitsMap<G4double>* photonCellFlux, *electronCellFlux;
  if (CellFluxFilterType == 1){
      
      char snamechar[50];
      for(G4int i = 0; i < NEbin; i++)
      {	
	    //std::sprintf(snamechar,"PhantomSD/PhotonSurfCurr%02d",i);
	    std::sprintf(snamechar,"PhantomSD/PhotonCellFlux%02d",i);
        G4String sname(snamechar);
	    G4THitsMap<G4double>* tmp = MSDRun->GetHitsMap(sname);
	    if(tmp != NULL){
		    pCellFlux.push_back(tmp);
		    //G4cout << "i = " << i << " push into pCellFlux!" << G4endl;
	    }
      }
  }
  else if(CellFluxFilterType == 2){
    photonCellFlux = MSDRun->GetHitsMap("PhantomSD/PhotonCellFluxAll");
    electronCellFlux = MSDRun->GetHitsMap("PhantomSD/ElectronCellFluxAll");
  }

  G4int ix,iy,iz,m;
  char fname1[700],fname2[700],fname3[700];
  
  //Save sparse data in .root files
  //write out Energy deposit sparse map for this run
  int posX,posY,posZ;
  float edep,photonfl,electronfl;
  
  TTree* EdepTree = new TTree("EdepTree","posX, posY, posZ, and Edep");
  EdepTree->Branch("posX",&posX,"posX/I");
  EdepTree->Branch("posY",&posY,"posY/I");
  EdepTree->Branch("posZ",&posZ,"posZ/I");
  EdepTree->Branch("eng",&edep,"eng/F");
  //G4cout << "after setting variables to read for EdepTree..." << G4endl;
 
//   float nstep;
//   TTree* NStepTree = new TTree("NStepTree","posX, posY, posZ, and NStep");
//   NStepTree->Branch("posX",&posX,"posX/I");
//   NStepTree->Branch("posY",&posY,"posY/I");
//   NStepTree->Branch("posZ",&posZ,"posZ/I");
//   NStepTree->Branch("nstep",&nstep,"nstep/F");
  
  std::vector<TTree*> TreeHolder;
  std::vector<float> fluence(28);   //initialize the vector to 28 ZEROs!
  TTree* PhotonCFTree, *ElectronCFTree;
  if (CellFluxFilterType == 1){
      //write out fluence vs. energy sparse format for this run
      char treename[200];
      char bname1[100];
      char bname2[100];
      TTree* letree;
      
      //set the appropriate variables to read and save in the root Tree
      for(m=0; m<NEbin; m++){
      	//sprintf(treename,"fluenceTree%0.2d",static_cast<int>(m+1));
      	std::sprintf(treename,"fluenceTree%02d",static_cast<int>(m+1));
      	letree = new TTree(treename,"posX, posY, posZ, and fluence per energy bin");
      	letree->Branch("posX",&posX,"posX/I");
      	letree->Branch("posY",&posY,"posY/I");
      	letree->Branch("posZ",&posZ,"posZ/I");
      	//sprintf(bname1,"fluence%0.2d",static_cast<int>(m+1));
      	//sprintf(bname2,"fluence%0.2d/F",static_cast<int>(m+1));
      	std::sprintf(bname1,"fluence%02d",static_cast<int>(m+1));
      	std::sprintf(bname2,"fluence%02d/F",static_cast<int>(m+1));
      	letree->Branch(bname1,&fluence[m],bname2);
	    TreeHolder.push_back(letree);
      }
  }
  else if(CellFluxFilterType == 2){
      PhotonCFTree = new TTree("PhotonCellFluxAllTree","posX, posY, posZ, and fluence of all energy");
      PhotonCFTree->Branch("posX",&posX,"posX/I");
      PhotonCFTree->Branch("posY",&posY,"posY/I");
      PhotonCFTree->Branch("posZ",&posZ,"posZ/I");
      PhotonCFTree->Branch("fluence",&photonfl,"fluence/F");

      ElectronCFTree = new TTree("ElectronCellFluxAllTree","posX, posY, posZ, and fluence of all energy");
      ElectronCFTree->Branch("posX",&posX,"posX/I");
      ElectronCFTree->Branch("posY",&posY,"posY/I");
      ElectronCFTree->Branch("posZ",&posZ,"posZ/I");
      ElectronCFTree->Branch("fluence",&electronfl,"fluence/F");
  }

  //G4bool isfluenceEmpty;
  for(iz = 0; iz < fNz; iz++){
	for(iy = 0; iy < fNy; iy++){
		//b = iy*fNx;
		for(ix = 0; ix < fNx; ix++){
			//isfluenceEmpty=FALSE;
			//indx = b + ix;
			
			//set up the posX, posY, posZ
			posX = static_cast<int>(ix);
			posY = static_cast<int>(iy);
			posZ = static_cast<int>(iz);
			
			G4double* eh1 = (*totEdep)[CopyNo(ix,iy,iz)];
			if (eh1){  //write out (x,y,z) and Edep for voxels with energy deposit..
				
				edep = static_cast< float >(*eh1);
				//printf("posX = %d, posY = %d, posZ = %d, edep = %6.8f\n",posX,posY,posZ,edep);
				EdepTree->Fill();
				//G4cout << "(" << ix << ", " << iy << ", " << iz << "): " << edep << G4endl;
			}
			
			if(CellFluxFilterType == 2){
     			G4double* eh2 = (*photonCellFlux)[CopyNo(ix,iy,iz)];
     			if (eh2){
      				photonfl = static_cast< float >(*eh2);
				    //G4cout << "PhotonFluence: (" << ix << ", " << iy << ", " << iz << "), " << photonfl << G4endl;
     				PhotonCFTree->Fill();
      			}
			
			    G4double* eh3 = (*electronCellFlux)[CopyNo(ix,iy,iz)];
     			if (eh3){
      				electronfl = static_cast< float >(*eh3);
				    //G4cout << "ElectronFluence: (" << ix << ", " << iy << ", " << iz << "), " << electronfl << G4endl;
     				ElectronCFTree->Fill();
      			}
  			}
			
			if(CellFluxFilterType == 1){
			    for(m=0; m< NEbin; m++){
				    G4double* eh2 = (*pCellFlux[m])[CopyNo(ix,iy,iz)];
				    if(eh2){
					    fluence[m] = static_cast< float >(*eh2);   //unit of cm-2
					    //printf("posX = %d, posY = %d, posZ = %d, Ebin#%d, fluence = %6.8f\n",posX,posY,posZ,static_cast<int>(m+1),fluence[m]);
					    TreeHolder[m]->Fill();
					    //isfluenceEmpty = TRUE;
					    //G4cout << "(" << ix << ", " << iy << ", " << iz << "): " << fluence[m] << G4endl;
				    }
			    }
			}
// 			if(!isfluenceEmpty){
// 				G4cout << "ix, iy, iz = " << ix << ", " << iy << ", " << iz << G4endl;
// 				posX = static_cast<int>(ix);
// 				posY = static_cast<int>(iy);
// 				posZ = static_cast<int>(iz);
// 				fluenceTree->Fill();
// 			}
			
		}
	}
  }
  
  
  //save the .root file for Edep sparse data map
  sprintf(fname1,"%s/RootData/Edep.root",dirName);
  printf("write root file: %s\n",fname1);
  TFile* outfile1 = TFile::Open(fname1,"recreate");
  EdepTree->Write();	
  outfile1->Close();
  delete EdepTree;
  EdepTree = 0;

  if(CellFluxFilterType == 1){
        for(m=0 ; m < NEbin; m++){
      	//printf("# of tree entry: %d\n",TreeHolder[m]->GetEntries());
	    //sprintf(fname2,"%s/RootData/Fluence%0.2d.root",dirName,static_cast<int>(m+1));
	    std::sprintf(fname2,"%s/RootData/Fluence%02d.root",dirName,static_cast<int>(m+1));
	    printf("write root file: %s\n",fname2);
	    TFile* outfile2 = TFile::Open(fname2,"recreate");
	    TreeHolder[m]->Write();	
	    outfile2->Close();
      }
  }
  else if(CellFluxFilterType == 2){
      //save the .root file for photon and electron sparse data map (all energy)
      sprintf(fname3,"%s/RootData/PhotonFluenceAll.root",dirName);
      printf("write root file: %s\n",fname3);
      TFile* outfile3 = TFile::Open(fname3,"recreate");
      PhotonCFTree->Write();	
      outfile3->Close();
      delete PhotonCFTree;
      PhotonCFTree = 0;

      sprintf(fname3,"%s/RootData/ElectronFluenceAll.root",dirName);
      printf("write root file: %s\n",fname3);
      outfile3 = TFile::Open(fname3,"recreate");
      ElectronCFTree->Write();	
      outfile3->Close();
      delete ElectronCFTree;
      ElectronCFTree = 0;
  }
  
  while(!TreeHolder.empty()){
	delete TreeHolder.back(), TreeHolder.pop_back();
  }
  TreeHolder.clear();
  
//   char fname3[700];
//   sprintf(fname3,"%s/RootData/NStep.root",dirName);
//   //printf("write root file: %s\n",fname1);
//   TFile* outfile3 = TFile::Open(fname3,"recreate");
//   NStepTree->Write();	
//   outfile3->Close();
//   delete NStepTree;
//   NStepTree = 0;

}

