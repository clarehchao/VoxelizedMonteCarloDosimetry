
void Root2Dat_TTree2(char *fname='', char *tag='', int mode=1)
{
    gROOT->SetStyle("Plain");
    //GateRootCoincBuffer mybuffer;


  char filename1[400];
  sprintf(filename1,"%s",fname);

  TFile *f = new TFile(filename1, "READ");
  f->ReadKeys();

  if (f->IsZombie()) {
        printf("Couldn't open file.\n");
        return;
    } else {
        printf("Opened file: %s\n", filename1);
    }

    printf("Getting  Singles tree...");
    //TTree *stree = (TTree *) f->Get("calotree"); //singles tree name goes here. likewise, you can replace this with the Coincidences tree etc

    TTree *stree = (TTree *) f->Get("fluenceTree");
    printf("Got the tree.\n");

    //stree->Print();
    //stree->Scan();
    
    Int_t myentries = (Int_t) stree->GetEntries();
    printf("# of entries: %d\n",myentries);

    int ix,iy,iz,k,m,Nbin;
    float fluence[28];
    char tmp[50];

    stree->SetBranchAddress("posX",&ix);
    stree->SetBranchAddress("posY",&iy);
    stree->SetBranchAddress("posZ",&iz);

    Nbin = 28;
    for(k=0; k<Nbin; k++){
	sprintf(tmp,"fluence%0.2d",k+1);
    	stree->SetBranchAddress(tmp,&fluence[k]);
    }

    FILE *myfile;
    

    char filename2[400];
    sprintf(filename2,"BustOutRoot_%s.dat",tag);
    myfile= fopen(filename2,"w");  //this is the file where the data will be written

    for (k=0;k < myentries; k++){
       stree->GetEntry(k);
       //printf("%7d\t%7d\t%7d\t",ix,iy,iz);
       fprintf(myfile,"%7d\t%7d\t%7d\t",ix,iy,iz);
       for (m=0; m<Nbin; m++){
       	if(m < Nbin - 1){
       		printf("%6.8f\t",fluence[m]);
       		fprintf(myfile,"%6.8f\t",fluence[m]);
	}
	else{
		printf("%6.8f\n",fluence[m]);
       		fprintf(myfile,"%6.8f\n",fluence[m]);
	}
       }
    }
    fclose(myfile);
}

