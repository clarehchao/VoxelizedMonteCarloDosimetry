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
 * @file   VHDMSDRunAction.cc
 * @brief  Run definition
 *
 * @date   7th Aug 2013
 * @author Shih-ying Huang
 * @name   Geant4.9.6-p02
 */
 
//=====================================================================
//
//  (Description)
//    VHDMultiSDRun Class is for accumulating scored quantities which is 
//  scored using G4MutiFunctionalDetector and G4VPrimitiveScorer.
//  Accumulation is done using G4THitsMap object.
//
//    The constructor VHDMultiSDRun(const std::vector<G4String> mfdName)
//  needs a vector filled with MultiFunctionalDetector names which
//  was assigned at instantiation of MultiFunctionalDetector(MFD).
//  Then VHDMultiSDRun constructor automatically scans primitive scorers
//  in the MFD, and obtains collectionIDs of all collections associated
//  to those primitive scorers. Futhermore, the G4THitsMap objects 
//  for accumulating during a RUN are automatically created too.
//  (*) Collection Name is same as primitive scorer name.
// 
//    The resultant information is kept inside VHDMultiSDRun objects as
//  data members.
//  std::vector<G4String> theCollName;            // Collection Name,
//  std::vector<G4int> theCollID;                 // Collection ID,
//  std::vector<G4THitsMap<G4double>*> theRunMap; // HitsMap for RUN.
//
//  The resualtant HitsMap objects are obtain using access method,
//  GetHitsMap(..).
//
//=====================================================================

#include "VHDMultiSDRun.hh"
#include "G4SDManager.hh"

#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"

//  Constructor. 
//   (The vector of MultiFunctionalDetector name has to given.)
VHDMultiSDRun::VHDMultiSDRun(const std::vector<G4String> mfdName): G4Run()
{
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  
  //=================================================
  //  Initalize RunMaps for accumulation.
  //  Get CollectionIDs for HitCollections.
  //================================definitely lost=================
  G4int Nmfd = mfdName.size();
  //G4cout << "Nmfd = " << Nmfd << G4endl;

  for ( G4int idet = 0; idet < Nmfd ; idet++){  // Loop for all MFD.
    G4String detName = mfdName[idet];
    //--- Seek and Obtain MFD objects from SDmanager.

    //G4cout << "detName = " << detName << G4endl;
    G4MultiFunctionalDetector* mfd = (G4MultiFunctionalDetector*)(SDman->FindSensitiveDetector(detName));
    //
    if ( mfd ){
	//--- Loop over the registered primitive scorers.
	//G4cout << "Number of primitives = " << mfd->GetNumberOfPrimitives() << G4endl;
	for (G4int icol = 0; icol < mfd->GetNumberOfPrimitives(); icol++){
	    // Get Primitive Scorer object.
	    G4VPrimitiveScorer* scorer = mfd->GetPrimitive(icol);
	    // collection name and collectionID for HitsCollection,
            // where type of HitsCollection is G4THitsMap in case of primitive scorer.
            // The collection name is given by <MFD name>/<Primitive Scorer name>.
	    G4String collectionName = scorer->GetName();
	    G4String fullCollectionName = detName+"/"+collectionName;
	    G4int collectionID = SDman->GetCollectionID(fullCollectionName);
	    //
	    if ( collectionID >= 0 ){
		G4cout << "++ "<<fullCollectionName<< " id " << collectionID << G4endl;
		// Store obtained HitsCollection information into data members.
		// And, creates new G4THitsMap for accumulating quantities during RUN.
		theCollName.push_back(fullCollectionName);
		theCollID.push_back(collectionID);
		//G4cout << "fullColllectionName = " << fullCollectionName << G4endl;
		//G4cout << "collectionID = " << collectionID << G4endl;
		theRunMap.push_back(new G4THitsMap<G4double>(detName,collectionName));
	    }else{
		G4cout << "** collection " << fullCollectionName << " not found. "<<G4endl;
	    }
	}
    }
  }
}

//
// Destructor
// clear all data members.
VHDMultiSDRun::~VHDMultiSDRun()
{
  //--- Clear HitsMap for RUN
  G4int Nmap = theRunMap.size();
  for ( G4int i = 0; i < Nmap; i++){
    if(theRunMap[i] ){
    	//theRunMap[i]->clear();
    	delete theRunMap[i];
    	G4cout << "RunMap # " << i << " is deleted!" << G4endl;
    }
  }
  theCollName.clear();
  theCollID.clear();
  theRunMap.clear();
  G4cout << "Destroy VHDMultiSDRun ..." << G4endl;
}


//  RecordEvent is called at end of event.
//  For scoring purpose, the resultant quantity in a event,
//  is accumulated during a Run.
void VHDMultiSDRun::RecordEvent(const G4Event* aEvent)
{
  numberOfEvent++;  // This is an original line.

  //check stuck event!!
  //G4int leEVT = aEvent->GetEventID();

  //=============================
  // HitsCollection of This Event
  //============================
  G4HCofThisEvent* HCE = aEvent->GetHCofThisEvent();
  if (!HCE) return;

  //=======================================================
  // Sum up HitsMap of this Event  into HitsMap of this RUN
  //=======================================================
  G4int Ncol = theCollID.size();
  
  //if(leEVT == 341922){
  //	G4cout << "In RecordEvent(..), Ncol  = " << Ncol << " (should be equal to number of events)" << G4endl;
  //}
  
  for ( G4int i = 0; i < Ncol ; i++ ){  // Loop over HitsCollection
    G4THitsMap<G4double>* EvtMap=0;
    if ( theCollID[i] >= 0 ){           // Collection is attached to HCE
      EvtMap = (G4THitsMap<G4double>*)(HCE->GetHC(theCollID[i]));
    }else{
      G4cout <<" Error EvtMap Not Found "<< i << G4endl;
    }
    if( EvtMap ){
      //=== Sum up HitsMap of this event to HitsMap of RUN.===
      *theRunMap[i] += *EvtMap;
      /*if(leEVT >= 341922){
      //======================================================
       G4cout << "the CollID = " << theCollID[i] << G4endl;
       
          std::map<G4int,G4double*>::iterator itr = EvtMap->GetMap()->begin();
          for(; itr != EvtMap->GetMap()->end(); itr++)
          {
 		G4int key = (itr->first);
 		G4double val = *(itr->second);
 		G4cout << "key, val: " << key << ", " << val << G4endl;
          }
      }*/
    }
    EvtMap->clear();
    /*if(leEVT >= 341922){
    	G4cout << "after clearing EvtMap..." << G4endl;
    }*/
   }
   /*if(leEVT >= 341922){
   	G4cout << "end of RecordEvent..." << G4endl;
   }*/
}

//=================================================================
//  Access method for HitsMap of the RUN
//
//-----
// Access HitsMap.
//  By  MultiFunctionalDetector name and Collection Name.
G4THitsMap<G4double>* VHDMultiSDRun::GetHitsMap(const G4String& detName,
					 const G4String& colName){
    G4String fullName = detName+"/"+colName;
    return GetHitsMap(fullName);
}

//-----
// Access HitsMap.
//  By full description of collection name, that is
//    <MultiFunctional Detector Name>/<Primitive Scorer Name>
G4THitsMap<G4double>* VHDMultiSDRun::GetHitsMap(const G4String& fullName){
    G4int Ncol = theCollName.size();
    for ( G4int i = 0; i < Ncol; i++){
	if ( theCollName[i] == fullName ){
	    return theRunMap[i];
	}
    }
    return NULL;
}

//-----
// - Dump All HitsMap of this RUN. (for debuging and monitoring of quantity).
//   This method calls G4THisMap::PrintAll() for individual HitsMap.
void VHDMultiSDRun::DumpAllScorer(){

  // - Number of HitsMap in this RUN.
  G4int n = GetNumberOfHitsMap();
  // - GetHitsMap and dump values.
  for ( G4int i = 0; i < n ; i++ ){
    G4THitsMap<G4double>* RunMap =GetHitsMap(i);
    if ( RunMap ) {
      G4cout << " PrimitiveScorer RUN " 
	     << RunMap->GetSDname() <<","<< RunMap->GetName() << G4endl;
      G4cout << " Number of entries " << RunMap->entries() << G4endl;
      std::map<G4int,G4double*>::iterator itr = RunMap->GetMap()->begin();
      for(; itr != RunMap->GetMap()->end(); itr++) {
	G4cout << "  copy no.: " << itr->first
	       << "  Run Value : " << *(itr->second) 
	       << G4endl;
      }
    }
  }
}


