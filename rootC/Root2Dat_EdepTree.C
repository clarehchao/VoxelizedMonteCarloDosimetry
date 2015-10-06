
void Root2Dat_EdepTree(char *fdir='',char *sfile='',char *srcpname='',int run1,int run2,int mode=1)
{
  char filename1[400],filename2[400],sfname[400],srcname[100];
  int n,k,ix,iy,iz;
  float edep;
  FILE *myfile1,*myfile2;
  TFile *f;
  TTree *stree;
  Int_t myentries;
  
  sprintf(sfname,"%s/%s",fdir,sfile);
  myfile1 = fopen(sfname,"r");
  if(myfile1 == NULL)
  {
	printf("cannot open the file %s\n",sfname);
  }
  else
  {
    while(fscanf(myfile1,"%s\n",srcname) == 1)
	{
	  printf("Source setup: %s\n",srcname);
      for (n = run1; n <= run2; n++)
      {
	    printf("Run#: %d\n",n);

	    gROOT->SetStyle("Plain");
      	//GateRootCoincBuffer mybuffer;

      	sprintf(filename1,"%s/%s/%s/Run%d/RootData/Edep.root",fdir,srcname,srcpname,n);

      	f = new TFile(filename1, "READ");
      	f->ReadKeys();

      	if (f->IsZombie()) {
        	printf("Couldn't open file.\n");
        	return;
        }
	    else    printf("Opened file: %s\n", filename1);

    	printf("Getting  Singles tree...");

    	stree = (TTree *) f->Get("EdepTree");
    	printf("Got the tree.\n");

    	//stree->Print();
    	//stree->Scan();
    
    	myentries = (Int_t) stree->GetEntries();
    	printf("# of entries: %d\n",myentries);

    	stree->SetBranchAddress("posX",&ix);
    	stree->SetBranchAddress("posY",&iy);
    	stree->SetBranchAddress("posZ",&iz);
    	stree->SetBranchAddress("eng",&edep);
    	
    	sprintf(filename2,"%s/%s/%s/Run%d/BustOutRoot/Edep.dat",fdir,srcname,srcpname,n);
    	myfile2 = fopen(filename2,"w");  //this is the file where the data will be written
    	for (k=0;k < myentries; k++){
	        stree->GetEntry(k);
       		//printf("%7d\t%7d\t%7d\t%9.3f\n",ix,iy,iz,edep);
       		fprintf(myfile2,"%7d\t%7d\t%7d\t%6.8f\n",ix,iy,iz,edep);
    	}
    	fclose(myfile2);
    	printf("Write the file: %s\n",filename2);
      }
    }
  }
  fclose(myfile1);
}

