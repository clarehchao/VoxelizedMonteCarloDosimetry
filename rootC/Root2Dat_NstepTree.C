
void Root2Dat_NstepTree(char *fdir='',int run1,int run2,int mode=1)
{
  

  char filename1[400],filename2[400];
  int n,k,ix,iy,iz;
  float nstep;
  FILE *myfile;
  TFile *f;
  TTree *stree;
  Int_t myentries;

  for (n = run1; n <= run2; n++)
  {
	printf("Run#: %d\n",n);

	gROOT->SetStyle("Plain");
  	//GateRootCoincBuffer mybuffer;

  	sprintf(filename1,"%s/Run%d/RootData/NStep.root",fdir,n);

  	f = new TFile(filename1, "READ");
  	f->ReadKeys();

  	if (f->IsZombie()) {
        	printf("Couldn't open file.\n");
        	return;
    	}
	else {
        	printf("Opened file: %s\n", filename1);
	}

    	printf("Getting  Singles tree...");

    	stree = (TTree *) f->Get("NStepTree");
    	printf("Got the tree.\n");

    	//stree->Print();
    	//stree->Scan();
    
    	myentries = (Int_t) stree->GetEntries();
    	printf("# of entries: %d\n",myentries);

    
    	stree->SetBranchAddress("posX",&ix);
    	stree->SetBranchAddress("posY",&iy);
    	stree->SetBranchAddress("posZ",&iz);
    	stree->SetBranchAddress("nstep",&nstep);
    
    
    	sprintf(filename2,"%s/Run%d/BustOutRoot/NStep.dat",fdir,n);
    	myfile = fopen(filename2,"w");  //this is the file where the data will be written

    	for (k=0;k < myentries; k++){
		stree->GetEntry(k);
       		//printf("%7d\t%7d\t%7d\t%9.3f\n",ix,iy,iz,edep);
       		fprintf(myfile,"%7d\t%7d\t%7d\t%6.8f\n",ix,iy,iz,nstep);
    	}
    	fclose(myfile);
  }   

}

