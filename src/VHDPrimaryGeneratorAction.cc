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
 * @file   VHDPrimaryGeneratorAction.cc
 * @brief  primary generator action defition
 *
 * @date   7th Aug 2013
 * @author Shih-ying Huang
 * @name   Geant4.9.6-p02
 */



#include "VHDPrimaryGeneratorAction.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"
#include "globals.hh"
//#include "VHDMultiSDEventAction.hh"
//#include "G4RunManager.hh"
//#include "G4GeneralParticleSource.hh"
//#include "G4SingleParticleSource.hh"

#include "G4UnitsTable.hh"
#include <stdlib.h>
#include <stdio.h>
#include "CLHEP/Random/RandFlat.h"

using namespace std;


//VHDPrimaryGeneratorAction::VHDPrimaryGeneratorAction(const G4String& dname, char dirtag[50], char mactag[50],G4int runcount)
VHDPrimaryGeneratorAction::VHDPrimaryGeneratorAction(const G4String& dname, const G4int isSparse)
{
   //pgun = new G4GeneralParticleSource();
   pgun = new G4ParticleGun(); 
   MinTheta = 0.;
   MaxTheta = pi;
   MinPhi = 0.;
   MaxPhi = twopi;

   //set source map directory: read Sparse sourcemap format or volume sourcemap format
   if(isSparse==1)
	SetSourceProbMapSparse(dname);
   else
   	SetSourceProbMap(dname);
   G4cout << "in VHDPrimaryGeneratorAction: dname = " << dname << G4endl;

//   size_t len;
//   char dnamechar[300];
//   len = dname.copy(dnamechar,dname.length(),0);
//   dnamechar[len] = '\0';
// 
//    sprintf(filename1,"%s/positiondata.txt",dnamechar);
//    fpt1 = fopen(filename1,"w");
//    if(fpt1 == NULL){
// 	G4cout << "cannot open the file!" << G4endl;
// 	printf("filename: %s\n",filename1);
// 	G4Exception("VHDPrimaryGeneratorAction.cc::VHDPrimaryGeneratorAction, cannot open file!","cannot open file!",FatalException,"cannot open file!");
//    }

   
   //printf("%s, %s, %d\n",dirtag,mactag,runcount);
//    //sprintf(filename1,"/home/shuang/g4work/VoxelizedHumanDose/PositionIM/%s/%s/Run%d/positiondata.txt",dirtag,mactag,runcount);
//    sprintf(filename1,"PositionIM/%s/%s/Run%d/positiondata.txt",dirtag,mactag,runcount);
//    fpt1 = fopen(filename1,"w");
//    if(fpt1 == NULL){
// 	G4cout << "cannot open the file!" << G4endl;
// 	printf("filename: %s\n",filename1);
// 	G4Exception("VHDPrimaryGeneratorAction.cc::VHDPrimaryGeneratorAction, cannot open file!","cannot open file!",FatalException,"cannot open file!");
//    }

//    //sprintf(filename2,"/home/shuang/g4work/VoxelizedHumanDose/PositionIM/%s/%s/Run%d/nxnynzdata.txt",dirtag,mactag,runcount);
//    sprintf(filename2,"PositionIM/%s/%s/Run%d/nxnynzdata.txt",dirtag,mactag,runcount);
//    fpt2 = fopen(filename2,"w");
//    if(fpt2 == NULL){
// 	G4cout << "cannot open the file!" << G4endl;
// 	printf("filename: %s\n",filename2);
// 	G4Exception("VHDPrimaryGeneratorAction.cc::VHDPrimaryGeneratorAction, cannot open file!","cannot open file!",FatalException,"cannot open file!");
//    }

//    //sprintf(filename3,"/home/shuang/g4work/VoxelizedHumanDose/PositionIM/%s/%s/Run%d/particlegundata.txt",dirtag,mactag,runcount);
//    sprintf(filename3,"PositionIM/%s/%s/Run%d/particlegundata.txt",dirtag,mactag,runcount);
//    fpt3 = fopen(filename3,"w");
//    if(fpt3 == NULL){
// 	G4cout << "cannot open the file!" << G4endl;
// 	printf("filename: %s\n",filename3);
// 	G4Exception("VHDPrimaryGeneratorAction.cc::VHDPrimaryGeneratorAction, cannot open file!","cannot open file!",FatalException,"cannot open file!");
//    }

//    sprintf(filename3,"PositionIM/%s/%s/Run%d/theProbAccum.txt",dirtag,mactag,runcount);
//    fpt3 = fopen(filename3,"w");
//    if(fpt3 == NULL){
// 	G4cout << "cannot open the file!" << G4endl;
// 	G4Exception("VHDPrimaryGeneratorAction.cc::VHDPrimaryGeneratorAction, cannot open file!","cannot open file!",FatalException,"cannot open file!");
//    }
   
}


VHDPrimaryGeneratorAction::~VHDPrimaryGeneratorAction()
{
  delete pgun;
  probAccum.clear();
  theProbAccum.clear();
  
  G4cout << "destroying VHDPrimaryGeneratorAction" << G4endl;
  //fclose(fpt1);
//   fclose(fpt2);
//   fclose(fpt3);
}

//
void VHDPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{ 
  //G4cout << "beginning of generateprimaries..." << G4endl;
  position = GeneratePosition();
  momentum = GenerateIsotropicMomentum();
  
  //const VHDMultiSDEventAction* evt =(const VHDMultiSDEventAction*)(G4RunManager::GetRunManager()->GetUserEventAction());
  //G4int event_id = evt->GetEvtID();
  //if(event_id == 783779)
  //  G4cout << "primary particle position: " << position.x() << ", " << position.y() << ", " << position.z() << G4endl;


/*  position.setX(18.4209);
  position.setY(35.7517);
  position.setZ(12.0);

  momentum.setX(-0.644857);
  momentum.setY(0.583443);
  momentum.setZ(0.493714);
*/
  //G4cout << "after generating momentum..." << G4endl;
  pgun->SetParticlePosition(position);
  pgun->SetParticleMomentumDirection(momentum);
  pgun->GeneratePrimaryVertex(anEvent);
  
  //when using radioactive decay physics, particle energy is all 0 despite its decay products (i think it's printing out the energy of the "nucleus" particle
  //G4cout << "Primary particle energy: " << pgun->GetParticleEnergy() << G4endl;

  //G4int leEVT = anEvent->GetEventID();
  //if(leEVT >= 341922){
  //	G4cout << "momentum: " << momentum.x() << ", " << momentum.y() << ", " << momentum.z() << G4endl;
  //	G4cout << "position: " << position.x() << ", " << position.y() << ", " << position.z() << G4endl;
  //}

 //fprintf(fpt1,"%6.8f\t%6.8f\t%6.8f\t%6.8f\t%6.8f\t%6.8f\n",static_cast<float>(position.x()),static_cast<float>(position.y()),static_cast<float>(position.z()),static_cast<float>(momentum.x()),static_cast<float>(momentum.y()),static_cast<float>(momentum.z()));

  //G4ThreeVector pos = pgun->GetParticlePosition();
  //G4ThreeVector mom = pgun->GetParticleMomentumDirection();
//   G4double eng = pgun->GetParticleEnergy();
//   G4String ptype = pgun->GetParticleDefinition()->GetParticleType();
//   G4cout << "eng = " << eng << ", ptype = " << ptype << G4endl;

//   size_t len;
//   char ptypechar[300];
//   len = ptype.copy(ptypechar,ptype.length(),0);
//   ptypechar[len] = '\0';

//   pgun->SetParticlePosition(position);
//   // set angular distribution: isotropic
//   G4SPSAngDistribution *angDist = pgun->GetCurrentSource()->GetAngDist();
//   angDist->SetAngDistType("iso");
//   pgun->GeneratePrimaryVertex(anEvent);
//   G4ThreeVector pos = pgun->GetCurrentSource()->GetParticlePosition();
//   G4ThreeVector mom = pgun->GetCurrentSource()->GetParticleMomentumDirection();
//   G4double eng = pgun->GetCurrentSource()->GetParticleEnergy();
  //G4cout << "particle energy: " << eng << G4endl;
//   fprintf(fpt3,"%6.8f\t%6.8f\t%6.8f\t%6.8f\t%6.8f\t%6.8f\t%6.8f\n",static_cast<float>(pos.x()),static_cast<float>(pos.y()),static_cast<float>(pos.z()),static_cast<float>(mom.x()),static_cast<float>(mom.y()),static_cast<float>(mom.z()),static_cast<float>(eng));

  //fprintf(fpt1,"%s\t%6.8f\n",ptypechar,static_cast<float>(eng));

}

void VHDPrimaryGeneratorAction::SetSourceProbMap(const G4String& dirname)
{
  // open Data.dat to get all the files to read 

  G4String datafname = dirname + "/Data.dat";
  std::ifstream fin(datafname);
  if(!fin.is_open()) ErrorFileNotFound(datafname);
  G4cout << "reading from " << datafname << G4endl;
  if(fin.eof()){
	ErrorFileTooShort(datafname);
  }

  G4String fname1,fname2;
  fin >> fNoFiles;
  
  nVoxels = 0;
  NVoxelZ = 0;
  theProbSum = 0;
  for(nfile = 0; nfile < fNoFiles; nfile++)
  {
    fin >> fname1;
    //--- Read one data file
    fname2 = dirname + "/" + fname1;
    ReadDoseMapFile(fname2);
    //G4cout << "read fname2: " << fname2 << G4endl;
  }
  fin.close();

  //-- Normalize probabilities
  std::map<G4double,G4int>::iterator ite;
  //G4cout << "theProbSum = " << theProbSum << G4endl;
// 	//G4cout << "mateID = " << mateID << G4endl;
  for(ite = probAccum.begin(); ite != probAccum.end(); ite++)
  {
    theProbAccum[ (*ite).first / theProbSum ] = (*ite).second;
  }

//   for(ite = theProbAccum.begin(); ite!= theProbAccum.end(); ite++)
//   {
// 	fprintf(fpt3,"%6.8f\t%d\n",static_cast<float>((*ite).first),static_cast<int>((*ite).second));
//   }
//   fclose(fpt3);
  
  G4cout << "NVoxelX = " << NVoxelX << ", NVoxelY = " << NVoxelY << ", NVoxelZ = " << NVoxelZ << G4endl;
  G4cout << "dX = " << dX << ", dY = " << dY << ", dZ = " << dZ << G4endl;
  G4cout << "offsetX = " << offsetX << ", offsetY = " << offsetY << ", offsetZ = " << offsetZ << G4endl;
  G4cout << "theProbSum = " << theProbSum << G4endl;
}


void VHDPrimaryGeneratorAction::ReadDoseMapFile(const G4String& fname)
{
  std::ifstream fin(fname.c_str(), std::ios_base::in);  //ios_base::in ==> open file for reading
  if(!fin.is_open()) ErrorFileNotFound(fname);

  G4int nz;
  fin >> NVoxelX >> NVoxelY >> nz;
  fin >> Xmin >> Xmax;
  fin >> Ymin >> Ymax;
  fin >> Zmin >> Zmax;

  if(nfile == 0){
	NVoxelXY = NVoxelX * NVoxelY;
	dX = (Xmax - Xmin)/NVoxelX;
	dY = (Ymax - Ymin)/NVoxelY;
	dZ = (Zmax - Zmin)/nz;
// 	dX = (Xmax - Xmin)/(NVoxelX-1);
// 	dY = (Ymax - Ymin)/(NVoxelY-1);
// 	dZ = (Zmax - Zmin)/nz;
	offsetX = Xmin + dX/2.;
	offsetY = Ymin + dY/2.;
	offsetZ = Zmin + dZ/2.;
// 	offsetX = Xmin;
// 	offsetY = Ymin;
// 	offsetZ = Zmin;
  }
  NVoxelZ += nz;

  G4double prob;
  for( G4int ii = 0; ii < NVoxelXY; ii++, nVoxels++ )
  {
    fin >> prob;
    if( fin.eof() && ii != NVoxelXY-1)
    {
	ErrorFileTooShort(fname);
    }
    if(prob > 0.0)
    {
	theProbSum += prob;
    	probAccum[theProbSum] = nVoxels;
    }
  }
  fin.close();
}

void VHDPrimaryGeneratorAction::SetSourceProbMapSparse(const G4String& dirname)
{
  G4cout << "VHDPrimaryGeneratorAction::SetSourceProbMapSparse is used!" << G4endl;
  G4String fname = dirname + "/SparseDoseMap.g4d";
  std::ifstream fin(fname.c_str(), std::ios_base::in);  //ios_base::in ==> open file for reading
  if(!fin.is_open()) ErrorFileNotFound(fname);

  fin >> NVoxelX >> NVoxelY >> NVoxelZ;
  fin >> Xmin >> Xmax;
  fin >> Ymin >> Ymax;
  fin >> Zmin >> Zmax;

  NVoxelXY = NVoxelX * NVoxelY;
  dX = (Xmax - Xmin)/NVoxelX;
  dY = (Ymax - Ymin)/NVoxelY;
  dZ = (Zmax - Zmin)/NVoxelZ;
  offsetX = Xmin + dX/2.;
  offsetY = Ymin + dY/2.;
  offsetZ = Zmin + dZ/2.;
  
  //offsetX = Xmin + dX/2.+0.001;
  //offsetY = Ymin + dY/2.+0.001;
  //offsetZ = Zmin + dZ/2.+0.001;

  G4double prob;
  G4int ivox;
  theProbSum = 0;
  while(!fin.eof())
  {
    fin >> ivox >> prob;
	theProbSum += prob;
    probAccum[theProbSum] = ivox;
  }
  fin.close();
  
  //-- Normalize probabilities
  std::map<G4double,G4int>::iterator ite;
  for(ite = probAccum.begin(); ite != probAccum.end(); ite++)
  {
    theProbAccum[ (*ite).first / theProbSum ] = (*ite).second;
  }
  
  G4cout << "NVoxelX = " << NVoxelX << ", NVoxelY = " << NVoxelY << ", NVoxelZ = " << NVoxelZ << G4endl;
  G4cout << "dX = " << dX << ", dY = " << dY << ", dZ = " << dZ << G4endl;
  G4cout << "offsetX = " << offsetX << ", offsetY = " << offsetY << ", offsetZ = " << offsetZ << G4endl;
  G4cout << "theProbSum = " << theProbSum << G4endl;
}

G4ThreeVector VHDPrimaryGeneratorAction::GeneratePosition()
{

  G4double rnd = CLHEP::RandFlat::shoot();

  std::map<G4double,G4int>::iterator ite = theProbAccum.upper_bound(rnd);  //upper_bound: return the iterator pointing to the first element that is GREATER than rnd

  G4int nVox = (*ite).second;
  //G4cout << "nVox =  " << nVox << G4endl;
//   G4int nx = size_t(nVox%NVoxelX);
//   G4int ny = size_t( (nVox/NVoxelX)%NVoxelY );
//   G4int nz = size_t(nVox/NVoxelXY);

  G4int nx = nVox%NVoxelX;
  G4int ny = (nVox/NVoxelX)%NVoxelY;
  G4int nz = nVox/NVoxelXY;

  G4ThreeVector pos( offsetX + dX*nx,
		     offsetY + dY*ny,
		     offsetZ + dZ*nz);

//   fprintf(fpt1,"%6.8f\t%6.8f\t%6.8f\n",static_cast<float>(pos.x()),static_cast<float>(pos.y()),static_cast<float>(pos.z()));
  //fprintf(fpt2,"%6.8f\t%d\t%d\t%d\t%d\n",static_cast<float>(rnd),static_cast<int>(nVox),static_cast<int>(nx),static_cast<int>(ny),static_cast<int>(nz));
  return pos;
}

G4ThreeVector VHDPrimaryGeneratorAction::GenerateIsotropicMomentum()
{
	G4double rnd1,rnd2,px,py,pz,phi;
	G4double sintheta,sinphi,costheta,cosphi;

	rnd1 = G4UniformRand();
	costheta = std::cos(MinTheta) - rnd1*(std::cos(MinTheta) - std::cos(MaxTheta));
	sintheta = std::sqrt(1. - costheta*costheta);
	
	rnd2 = G4UniformRand();
	phi = MinPhi + (MaxPhi - MinPhi)*rnd2;
	sinphi = std::sin(phi);
	cosphi = std::cos(phi);

	px = -sintheta*cosphi;
	py = -sintheta*sinphi;
	pz = -costheta;
	
	G4double mag = std::sqrt(px*px + py*py + pz*pz);

	G4ThreeVector mom(px/mag,py/mag,pz/mag);

	return mom;
}

 

void VHDPrimaryGeneratorAction::ErrorFileTooShort(const G4String& fname)
{
	G4Exception(G4String("VHDPrimaryGeneratorAction::SetSourceProbMap, the file, " + fname + " is too short!"),"Error in argument",FatalErrorInArgument,"end of file is true, file too short!");
}

void VHDPrimaryGeneratorAction::ErrorFileNotFound(const G4String& fname)
{
	G4Exception(G4String("VHDPrimaryGeneratorAction::ReadDoseMapFile, the file, " + fname + " is not found!"),"Error in argument",FatalErrorInArgument,"file not found!");
}


