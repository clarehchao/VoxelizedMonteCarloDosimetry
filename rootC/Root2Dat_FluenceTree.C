
void Root2Dat_FluenceTree(char *fdir='',int Nbin,int run1,int run2,int mode=1)
{

  int ix,iy,iz,m,n,k;
  float fluence;
  FILE *myfile;
  char fname1[400],fname2[400],fluencename[100],treename[100];
  TFile *f;
  TTree *tree;
  Int_t myentries;
  
  for(m=run1; m <= run2; m++){
	printf("Run#: %d\n",m);
  	for(n=1; n<=Nbin; n++){
  		gROOT->SetStyle("Plain");
  		//GateRootCoincBuffer mybuffer;

  		sprintf(fname1,"%s/Run%d/RootData/Fluence%0.2d.root",fdir,m,n);
  		sprintf(treename,"fluenceTree%0.2d",n);

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

    		//tree->Print();
    		//tree->Scan();
    
    		myentries = (Int_t) tree->GetEntries();
    		printf("# of entries: %d\n",myentries);

    		sprintf(fluencename,"fluence%0.2d",n);
    		tree->SetBranchAddress("posX",&ix);
    		tree->SetBranchAddress("posY",&iy);
    		tree->SetBranchAddress("posZ",&iz);
    		tree->SetBranchAddress(fluencename,&fluence);


    		if(myentries > 0){
    			sprintf(fname2,"%s/Run%d/BustOutRoot/fluence%0.2d.dat",fdir,m,n);
    			myfile = fopen(fname2,"w");  //this is the file where the data will be written
    			for (k=0;k < myentries; k++){
       				tree->GetEntry(k);
       				//printf("%7d\t%7d\t%7d\t%6.8f\n",ix,iy,iz,fluence);
       				fprintf(myfile,"%d\t%d\t%d\t%6.8f\n",ix,iy,iz,fluence);
    			}
    			fclose(myfile);
    		}
    		else{
			printf("no entries in .root, no busting out needed ^___^\n");
    		}
  	}
  }
}

