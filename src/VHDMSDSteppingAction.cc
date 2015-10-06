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
 * @file   VHDMSDSteppingAction.cc
 * @brief  stepping action definition
 *
 * @date   7th Aug 2013
 * @author Shih-ying Huang
 * @name   Geant4.9.6-p02
 */


#include "G4ios.hh"
#include "VHDMSDSteppingAction.hh"
#include "G4Track.hh"
#include "globals.hh"
#include "G4SteppingManager.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include <stdlib.h>
#include <stdio.h>
#include "VHDMultiSDEventAction.hh"
#include "G4RunManager.hh"
#include "G4ProcessType.hh"


VHDMSDSteppingAction::VHDMSDSteppingAction(char dname[])
{
   //set the data directory
   strcpy(datadir,dname);
   
   //ROOT histogram
   G4cout << "In VHDMSDSteppingAction constructor... initializing root histograms!!" << G4endl;
//    hVrxE_electron = new TH1F("VertexKineticEnergy","The spectrum of radionuclide decay products in electrons",1000,0.0,1.0);  //1000 bins, 1 keV per bin
//    hVrxE_electron->GetXaxis()->SetTitle("Particle Vertex Energy (MeV) ");
//    
//    hVrxE_photon = new TH1F("VertexKineticEnergy","The spectrum of radionuclide decay products in photons",1000,0.0,1.0);  //1000 bins, 1 keV per bin
//    hVrxE_photon->GetXaxis()->SetTitle("Particle Vertex Energy (MeV) ");
   
   hE_electron = new TH1F("KineticEnergy_electron","The spectrum of radionuclide decay products in electrons",1000,0.0,1.0);  //1000 bins, 1 keV per bin
   hE_electron->GetXaxis()->SetTitle("Particle Kinetic Energy (MeV) ");
   
   hE_photon = new TH1F("KineticEnergy_photon","The spectrum of radionuclide decay products in photons",1000,0.0,1.0);  //1000 bins, 1 keV per bin
   hE_photon->GetXaxis()->SetTitle("Particle Kinetic Energy (MeV) ");
   
//    char filename3[500];
//    sprintf(filename3,"%s/StepInfo.txt",datadir);
//    fpt = fopen(filename3,"w");
//    if(fpt == NULL){
// 	G4cout << "cannot open the file!" << G4endl;
// 	printf("filename: %s\n",filename3);
// 	G4Exception("VHDMSDSteppingAction.cc::VHDMSDSteppingAction, cannot open file!","cannot open file!",FatalException,"cannot open file!");
//    }

}

VHDMSDSteppingAction::~VHDMSDSteppingAction()
{
	//write out the root files
// 	char filename1[500];
// 	sprintf(filename1,"%s/hVrxE_electron.root",datadir);
// 	TFile* outfile1 = TFile::Open(filename1,"recreate");
// 	hVrxE_electron->Write();	
// 	delete hVrxE_electron;
// 	hVrxE_electron = 0;
// 	outfile1->Close();
// 	
// 	char filename2[500];
// 	sprintf(filename2,"%s/hVrxE_photon.root",datadir);
// 	TFile* outfile2 = TFile::Open(filename2,"recreate");
// 	hVrxE_photon->Write();	
// 	delete hVrxE_photon;
// 	hVrxE_photon = 0;
// 	outfile2->Close();

	char filename1[500];
	sprintf(filename1,"%s/RootData/hE_electron.root",datadir);
	TFile* outfile1 = TFile::Open(filename1,"recreate");
	hE_electron->Write();	
	delete hE_electron;
	hE_electron = 0;
	outfile1->Close();
	
	char filename2[500];
	sprintf(filename2,"%s/RootData/hE_photon.root",datadir);
	TFile* outfile2 = TFile::Open(filename2,"recreate");
	hE_photon->Write();	
	delete hE_photon;
	hE_photon = 0;
	outfile2->Close();
	
	//fclose(fpt);  //close writing file fpt
	
	G4cout << "destroying VHDMSDSteppingAction ..." << G4endl;

	MaterialOfInterest.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void VHDMSDSteppingAction::UserSteppingAction(const G4Step* aStep) 
{
  //this emitted decay product spectrum is inspired by the geant4.9.6/examples/extended/radioactivedecay/rdecay02
  G4Track* aTrack = aStep->GetTrack();
  G4int StepID = aTrack->GetCurrentStepNumber();
  G4int TrackID = aTrack->GetTrackID();
  //G4cout << "in stepping action...@_@" << G4endl;

  G4double energy,weight;
  if(StepID == 1 && TrackID != 1)
  {
  	//CAUTION! can only access creator process for non-primary particles or else the app crashes
  	G4String ProcName = aTrack->GetCreatorProcess()->GetProcessName();
  	//G4cout << "ProcName = " << ProcName << G4endl;
  	G4ParticleDefinition* partDef = aTrack->GetDefinition();
  	G4String partType = partDef->GetParticleType();
  	//G4cout << "partType = " << partType << G4endl;
  	if(ProcName == "RadioactiveDecay" && partType != "nucleus")
  	{
  		//G4cout << "partType = " << partType << G4endl;
		energy = aStep->GetPreStepPoint()->GetKineticEnergy();
		weight = aStep->GetPreStepPoint()->GetWeight();
		if(partDef == G4Electron::ElectronDefinition())
		{
			//G4cout << "StepID = " << StepID << ", TrackID = " << TrackID << G4endl;
			//G4cout << "In e- statement, particle name is " << partDef->GetParticleName() << G4endl;
			//G4cout << "energy = " << energy << " MeV, weight = " << weight << G4endl;
			hE_electron->Fill(energy,weight);  //inrement the histogram by weight
		}
		if(partDef == G4Gamma::GammaDefinition())
		{	
			//G4cout << "StepID = " << StepID << ", TrackID = " << TrackID << G4endl;
			//G4cout << "In gamma statement, particle name is " << partDef->GetParticleName() << G4endl;
			//G4cout << "energy = " << energy << " MeV, weight = " << weight << G4endl;
			hE_photon->Fill(energy,weight);  //inrement the histogram by weight
		}
	}
  }

/*
  //for debugging purpose: print out all the step/track info to see where th particle is at each step
  const VHDMultiSDEventAction* evt =(const VHDMultiSDEventAction*)(G4RunManager::GetRunManager()->GetUserEventAction());
  G4int event_id = evt->GetEvtID();
  //if(event_id >= 897441){
  //if(event_id == 783779){
  //if(event_id == 544734){
  if(event_id == 609563){
	G4String ProcName1 = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
	G4ParticleDefinition* partDef1 = aTrack->GetDefinition();
	G4String partType1 = partDef1->GetParticleType();
	G4double eng = aStep->GetPreStepPoint()->GetKineticEnergy();
	G4double edep = aStep->GetTotalEnergyDeposit();
	G4TrackStatus  trackinfo = aTrack->GetTrackStatus();
	G4cout << "Track status: " << trackinfo << G4endl;
	G4cout << "TrackID = " << TrackID << ", Process name: " << ProcName1 << ", particle type: " << partType1 << ", eng = " << eng << " MeV, edep = " << edep << " MeV" << G4endl;
	
	//pre-step info
	const G4VTouchable* pretouchable = aStep->GetPreStepPoint()->GetTouchable();
	if(pretouchable->GetVolume()){
	    G4String prevolName = pretouchable->GetVolume()->GetName();
	    G4String prematName =  pretouchable->GetVolume()->GetLogicalVolume()->GetMaterial()->GetName();
	    G4int preiz = pretouchable->GetReplicaNumber(0);
        G4int preix = pretouchable->GetReplicaNumber(1);
        G4int preiy = pretouchable->GetReplicaNumber(2);
        G4ThreeVector preTrans = pretouchable->GetVolume()->GetObjectTranslation();
        G4ThreeVector preP = aStep->GetPreStepPoint()->GetPosition();
        G4cout << "Pre-step position: " << preP.x() << ", " << preP.y() << ", " << preP.z() << G4endl;
        G4cout << "Pre-touchable volume translation: " << preTrans.x() << ", " << preTrans.y() << ", " << preTrans.z() << G4endl;
        G4cout << "Pre-Volume info: " << prevolName << ", " << prematName << ", (" << preix << ", " << preiy << ", " << preiz << ")" << G4endl;
        
        if(aStep->GetPreStepPoint()->GetStepStatus() != fGeomBoundary)
	        G4cout << "pre-step NOT on boundary!" << G4endl;
	    else
	        G4cout << "pre-step IS on boundary!" << G4endl;
	}
	else
	    G4cout << "pre touchable volume does not exist!" << G4endl;
	
	
	//post-step info
	const G4VTouchable* posttouchable = aStep->GetPostStepPoint()->GetTouchable();
	if (posttouchable->GetVolume()){
	    G4String postvolName = posttouchable->GetVolume()->GetName();
	    G4String postmatName =  posttouchable->GetVolume()->GetLogicalVolume()->GetMaterial()->GetName();
	    G4int postiz = posttouchable->GetReplicaNumber(0);
        G4int postix = posttouchable->GetReplicaNumber(1);
        G4int postiy = posttouchable->GetReplicaNumber(2);
        G4ThreeVector postTrans = posttouchable->GetVolume()->GetObjectTranslation();
        G4ThreeVector postP = aStep->GetPostStepPoint()->GetPosition();
        G4cout << "Post-step position: " << postP.x() << ", " << postP.y() << ", " << postP.z() << G4endl;
        G4cout << "Post-touchable volume translation: " << postTrans.x() << ", " << postTrans.y() << ", " << postTrans.z() << G4endl;
        G4cout << "Post-Volume info: " << postvolName << ", " << postmatName << ", (" << postix << ", " << postiy << ", " << postiz << ")" << G4endl;

	    if(aStep->GetPostStepPoint()->GetStepStatus() != fGeomBoundary)
	        G4cout << "post-step NOT on boundary!" << G4endl;
	    else
	        G4cout << "post-step IS on boundary!" << G4endl; 
	}
	else
	    G4cout << "post touchable volume does not exist!" << G4endl;
	
 }*/
 
 //G4bool isoverlap = touchable->GetVolume()->CheckOverlaps();
	//if(isoverlap)
	//    G4cout << "this physical volume has overlap!" << G4endl;
	//else
	//    G4cout << "no overlap in the physical volume" << G4endl;


//   const VHDMultiSDEventAction* evt =(const VHDMultiSDEventAction*)(G4RunManager::GetRunManager()->GetUserEventAction());
//   G4int event_id = evt->GetEvtID();
//   //G4cout << "eventID = " << event_id << G4endl;
//   if(event_id >= 4658000)
//   {
// 	if(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "phantom")
// 	{
// 		G4double steplen = aStep->GetStepLength();
// 		if(steplen > 0.0)
// 		{
// 			char pname[500],ptype[500];
// 			G4String ProcName1 = aTrack->GetCreatorProcess()->GetProcessName();
// 			size_t len;
// 			len = ProcName1.copy(pname,ProcName1.length(),0);
// 			pname[len] = '\0';
// 			G4ParticleDefinition* partDef1 = aTrack->GetDefinition();
// 			G4String partType1 = partDef1->GetParticleType();
// 			len = partType1.copy(ptype,partType1.length(),0);
// 			ptype[len] = '\0';
// 			G4double eng = aStep->GetPreStepPoint()->GetKineticEnergy();
// 			G4double edep = aStep->GetTotalEnergyDeposit();
// 			const G4VTouchable* touchable = aStep->GetPreStepPoint()->GetTouchable();
// 			G4int volCopyNum = touchable->GetVolume()->GetCopyNo();
// 			fprintf(fpt,"%d\t%d\t%d\t%d\t%s\t%s\t%6.8f\t%6.8f\t%6.8f\n",event_id,TrackID,StepID,volCopyNum,pname,ptype,steplen,eng/keV,edep/keV);
// 		}
// 	}
//   }
}

void VHDMSDSteppingAction::SetMaterialOfInterest(G4String dirname)
 {
 	G4String lename;
 	G4int Norgan;
 	
	G4String fname = dirname + "/CellFluxMaterialOfInterest.txt";
	std::ifstream fin(fname);
	if(fin.good() != 1 ){
		G4Exception("VHDMSDSteppingAction::SetMaterialOfInterest(G4String dirname)","",FatalErrorInArgument,G4String("Invalid file name: " + fname).c_str());
	}
 	fin >> Norgan;
 	//G4cout << "Norgan = " << Norgan << G4endl;
 	for(G4int i=0; i < Norgan; i++)
 	{
 		fin >> lename;
 		MaterialOfInterest.push_back(lename);
 		//G4cout << "lename = " << lename << G4endl;
 	}
 	fin.close();
 }
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


