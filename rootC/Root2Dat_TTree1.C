
void Root2Dat_TTree1(char *fname='', char *tag='', int mode=1)
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

    TTree *stree = (TTree *) f->Get("EdepTree");
    printf("Got the tree.\n");

    stree->Print();
    //stree->Scan();
    
    Int_t myentries = (Int_t) stree->GetEntries();
    printf("# of entries: %d\n",myentries);

    int ix,iy,iz;
    float edep;
    stree->SetBranchAddress("posX",&ix);
    stree->SetBranchAddress("posY",&iy);
    stree->SetBranchAddress("posZ",&iz);
    stree->SetBranchAddress("eng",&edep);


    //int ilayer;
    //float eng;
    //stree->SetBranchAddress("ilayer",&ilayer);
    //stree->SetBranchAddress("eng",&eng);

    //stree->SetBranchAddress("CopyNo",&ilayer);
    //stree->SetBranchAddress("eng",&eng);

    //int* ilayer = 0;
    //int ilayer;
    //TBranch *b_ilayer = stree->GetBranch("ilayer");
    //b_ilayer->SetAddress(&ilayer);

    //float* eng=0;
    //float eng;
    //TBranch *b_eng = stree->GetBranch("eng");
    //b_eng->SetAddress(&eng);

    
    FILE *myfile;
    int i;

    char filename2[400];
    sprintf(filename2,"BustOutRoot_%s.dat",tag);
    myfile= fopen(filename2,"w");  //this is the file where the data will be written

    for (i=0;i < myentries; i++){
       stree->GetEntry(i);
       printf("%7d\t%7d\t%7d\t%9.3f\n",ix,iy,iz,edep);
       fprintf(myfile,"%7d\t%7d\t%7d\t%6.8f\n",ix,iy,iz,edep);
    }
    fclose(myfile);   

}

