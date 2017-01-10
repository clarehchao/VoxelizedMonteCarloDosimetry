
void Root2Dat_FluenceAllTree(char *fdir='',char *sfile='',char *parname='',int run1,int run2,int mode=1)
//void Root2Dat_FluenceTree(char *fdir='',char *parname='',char *sfile='',int isReg,int Nbin,int run1,int run2,int mode=1)
{

  int ix,iy,iz,m,n,k;
  float fluence;
  FILE *myfile1,*myfile2;
  char fname1[400],fname2[400],fluencename[100],treename[100],fname3[400],organ[50],partag[20];
  TFile *f;
  TTree *tree;
  Int_t myentries;
  int Ntree = 2;

  sprintf(fname3,"%s/%s",fdir,sfile);
  myfile2= fopen(fname3,"r");
  if(myfile2 == NULL)
  {
	printf("cannot open the file %s\n",fname3);
  }
  else{
	while(fscanf(myfile2,"%s\n",organ) == 1)
	{
  	  printf("Source organ: %s\n",organ);
	  for(m=run1; m <= run2; m++){
		printf("Run#: %d\n",m);
	  	for(n=1; n<=Ntree; n++){
	  		gROOT->SetStyle("Plain");
	  		//GateRootCoincBuffer mybuffer;
			
			if(n == 1)
			    sprintf(partag,"Electron");
			else
			    sprintf(partag,"Photon");			
			sprintf(fname1,"%s/%s/%s/Run%d/RootData/%sFluenceAll.root",fdir,organ,parname,m,partag);
	  		sprintf(treename,"%sCellFluxAllTree",partag);
	  		printf("fname1: %s\n",fname1);
	  		printf("treename: %s\n",treename);

	  		f = new TFile(fname1, "READ");
	  		f->ReadKeys();

	  		if (f->IsZombie()) {
				printf("Couldn't open file.\n");
				return;
	    	}
			else {
				printf("Opened file: %s\n", fname1);
	    	}

    		printf("Getting the tree weeeee!...");

    		tree = (TTree *) f->Get(treename);
    		printf("Got the tree.\n");

    		tree->Print();
    		tree->Scan();
    
    		myentries = (Int_t) tree->GetEntries();
    		printf("# of entries: %d\n",myentries);

    		sprintf(fluencename,"fluence");
    		tree->SetBranchAddress("posX",&ix);
    		tree->SetBranchAddress("posY",&iy);
    		tree->SetBranchAddress("posZ",&iz);
    		tree->SetBranchAddress(fluencename,&fluence);


    		if(myentries > 0){
			    sprintf(fname2,"%s/%s/%s/Run%d/BustOutRoot/%sFluenceAll.dat",fdir,organ,parname,m,partag);
    			myfile1 = fopen(fname2,"w");  //this is the file where the data will be written
    			for (k=0;k < myentries; k++){
       				tree->GetEntry(k);
       				printf("%7d\t%7d\t%7d\t%6.8f\n",ix,iy,iz,fluence);
       				fprintf(myfile1,"%d\t%d\t%d\t%6.8f\n",ix,iy,iz,fluence);
    			}
    			fclose(myfile1);
			    printf("particle: %s: %d entries in the ROOT file; BUSTED OUT!\n",partag,myentries);
    		}
    		else{
			    printf("particle: %s, no fluence entries in the ROOT file, no busting out!\n",partag);
    		}
		    f->Close();  //close the ROOT file!!!
		
	  	}
		printf("Run #%d ::Fluence Data out of ROOT file:: MISSION COMPLETED!\n",m);
	  }
	}
   }
   fclose(myfile2);
}

