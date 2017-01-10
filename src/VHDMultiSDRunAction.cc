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
//
// $Id: VHDMultiSDRunAction.cc,v 1.3 2006-11-18 01:37:24 asaim Exp $
// GEANT4 tag $Name: geant4-09-04-patch-02 $
// 
#include "VHDMultiSDRunAction.hh"
#include "VHDMultiSDRun.hh"

//-- In order to obtain detector information.
#include "G4RunManager.hh"
#include "VHDDetectorConstruction.hh"
#include "G4THitsMap.hh"
#include "G4UnitsTable.hh"
#include <time.h>

//=======================================================================
// VHDMultiSDRunAction
// EndOfRun definition: save the dose scorers in image volume format 
//			non-sparse format
//
//=======================================================================


// Constructor
VHDMultiSDRunAction::VHDMultiSDRunAction():G4UserRunAction()
{
  G4cout << "Construct VHDMultiSDRunAction! " << G4endl;
  // - Prepare data member for VHDMultiSDRun.
  //   vector represents a list of MultiFunctionalDetector names.
  theSDName.push_back(G4String("PhantomSD"));
  CellFluxFilterType = 0;
}

// Destructor.
VHDMultiSDRunAction::~VHDMultiSDRunAction()
{
  theSDName.clear();

  G4cout << "Destroying VHDMultiSDRunAction! " << G4endl;
}

//
//== 
G4Run* VHDMultiSDRunAction::GenerateRun()
{
  // Generate new RUN object, which is specially
  // dedicated for MultiFunctionalDetector scheme.
  //  Detail description can be found in VHDMultiSDRun.hh/cc.
  return new VHDMultiSDRun(theSDName);
}

//
//==
void VHDMultiSDRunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  
  
  G4long seed = time(0);
  G4cout << "The seed of this run = " << seed << G4endl;
  CLHEP::HepRandom::setTheSeed(seed);
  CLHEP::HepRandom::showEngineStatus();

  //DEBUGGING PURPOSE: set the desired seed to figure out what happen when things get stuck
  /*G4int index = 24;   //ufhadm_1002, kidney, run3
  G4long currentseeds[2];
  currentseeds[0] = 1340192978;
  currentseeds[1] = 1983731094;
  CLHEP::HepRandom::setTheSeeds(currentseeds,index);
  CLHEP::HepRandom::showEngineStatus();
  
  G4int index = 46;   //ufhadm_1002, liver, run3
  G4long currentseeds[2];
  currentseeds[0] = 1373916894;
  currentseeds[1] = 709120320;
  CLHEP::HepRandom::setTheSeeds(currentseeds,index);
  CLHEP::HepRandom::showEngineStatus();
  
  G4int index = 18;
  G4long currentseeds[2];
  currentseeds[0] = 1637037495;
  currentseeds[1] = 728277899;
  CLHEP::HepRandom::setTheSeeds(currentseeds,index);
  CLHEP::HepRandom::showEngineStatus(); */
}


void VHDMultiSDRunAction::EndOfRunAction(const G4Run* aRun)
{

  //print out the total number of events during this run
  G4cout << "Number of Events in this run: " << aRun->GetNumberOfEvent() << G4endl;

  //- VHDMultiSDRun object
  VHDMultiSDRun* MSDRun = (VHDMultiSDRun*)aRun;
  //--- Dump all socred quantities involved in VHDMultiSDRun to debug!
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

  //std::vector<G4THitsMap<G4double>*> pCurrHit;
  std::vector<G4THitsMap<G4double>*> pCellFlux;
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

  G4int ix,iy,iz,n,m;
  char fname1[700],fname2[700];
  int b,indx;
  
  FILE *pt1,*pt2;
  float *edepimg = 0;
  int nxny = static_cast<int>(fNxNy);
  G4double zero = 0.0;
  G4double* zero_ptr = &zero;

  std::vector<float*> pcellfluxhitimg;
  if(dirName != 0){
	for(iz = 0; iz < fNz; iz++){
		//sprintf(fname1,"Edep%0.3d.raw",iz);
		std::sprintf(fname1,"%s/Edep_MultiSD/Edep%03d.raw",dirName,iz);
		//printf("fname1 = %s\n",fname1);
		pt1 = fopen(fname1,"wb");
		if(pt1 == NULL){
			printf("cannot open file %s\n",fname1);
		}
		else{
			edepimg = new float [nxny];  //dynamic allocation of array memory
			if(CellFluxFilterType == 1)
			    for(n=0; n<NEbin; n++)	pcellfluxhitimg.push_back(new float [nxny]);  //create all the image float arrays
			//G4cout << " iz = " << iz << ", after new edepimg..." << G4endl;
			for(iy = 0; iy < fNy; iy++){
				b = iy*fNx;
				for(ix = 0; ix < fNx; ix++){
					indx = b + ix;
					G4double* totED = (*totEdep)[CopyNo(ix,iy,iz)];
					if (!totED ) totED = zero_ptr;
					//instead of assigning totED = new G4double(0.0), just set it to a pointer of G4double = 0.0
					//since (*totEdep)[CopyNo(ix,iy,iz)] does not call a 'new' statement while it's accessing the recorded map
					//delete totED will be inappropriate as totED does not get a 'new' statement necessarily at every iteration
					//so, Fares suggested that assigning totED to a pointer of G4double = 0.0 is more appropriate
					//and the program will take care of its creation and deleting the memory once the program exits

					//G4cout << "totED = " << *totED*1000 << " keV!" << G4endl;  //THIS STATEMENT HAS TO BE AFTER THE IF STATEMENT!!!! 
					//THE IF STATEMENT IS TO CREATE A G4DOUBLE IF THE CopyNo in totEdep does not exist since the hitmap is recorded using std::map with copyNo and energy deposit!!!
					//edepimg[indx] = static_cast< float >((*totED)*1000);
					edepimg[indx] = static_cast< float >(*totED);   //unit of MeV
                    
                    if(CellFluxFilterType == 1){
					    for(m=0; m< NEbin; m++){
						    G4double* eh = (*pCellFlux[m])[CopyNo(ix,iy,iz)];
						    if (!eh) eh = zero_ptr;
						    pcellfluxhitimg[m][indx] = static_cast< float >(*eh);    //unit of cm-2
					    }
					}
				}
			}
			fwrite(edepimg,sizeof(float),fNxNy,pt1);
			fclose(pt1);
			delete[] edepimg;
			//G4cout << "Finish writing the edepimg files!" << G4endl;

            if(CellFluxFilterType == 1){
			    for(m=0; m<NEbin; m++){
				    std::sprintf(fname2,"%s/pCellFlux%02d/fluence%03d.raw",dirName,static_cast<int>(m+1),iz);
				    pt2 = fopen(fname2,"wb");
				    if(pt2 == NULL){
					    printf("cannot open file %s\n",fname2);
				    }
				    else{
					    fwrite(pcellfluxhitimg[m],sizeof(float),fNxNy,pt2);
					    fclose(pt2);
					    //G4cout << "Finish writing the pcurrhitimg files: " << m << G4endl;
				    }
				    delete[] pcellfluxhitimg[m];
			    }
			    pcellfluxhitimg.clear(); //MUST CLEAR THE VECTOR BEFORE USING IT AGAIN AS IT WILL KEEP ADDING THE CONTENT EVEN IF I DELETE THE ALLOCATED MEMORY!!!
      	   }
  		}
  	}
  }
  
}

void VHDMultiSDRunAction::SetRunInfo(char dname[])
{
    if(dname != NULL){
	    strcpy(dirName,dname);
        //G4cout << "dirName: " << dirName << G4endl;
        //G4cout << "dname: " << dname << G4endl;
    }
    else
        G4cout << "dname is NULL!" << G4endl;
}

