


void Root2Dat(char *fname='', int mode=1)
{
    gROOT->SetStyle("Plain");
    //GateRootCoincBuffer mybuffer;


  char filename[400];
  sprintf(filename,"%s",fname);

  TFile *f = new TFile(filename, "READ");
  f->ReadKeys();

  if (f->IsZombie()) {
        printf("Couldn't open file.\n");
        return;
    } else {
        printf("Opened file: %s\n", filename);
    }

    printf("Getting  Singles tree...");
    //TTree *stree = (TTree *) f->Get("calotree"); //singles tree name goes here. likewise, you can replace this with the Coincidences tree etc

    TTree *stree = (TTree *) f->Get("leETree");
    printf("Got the tree.\n");

    stree->Print();
    //stree->Scan();
    
    Int_t myentries = (Int_t) stree->GetEntries();
    printf("# of entries: %d\n",myentries);


    int ilayer;
    float eng;
    //stree->SetBranchAddress("ilayer",&ilayer);
    //stree->SetBranchAddress("eng",&eng);

    stree->SetBranchAddress("CopyNo",&ilayer);
    stree->SetBranchAddress("eng",&eng);


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

    myfile= fopen("RootData.dat","w");  //this is the file where the data will be written

    for (i=0;i < myentries; i++){
       stree->GetEntry(i);
       //printf("%d\t%d\t%f\n",i,ilayer,eng);
       fprintf(myfile,"%7d\t%7d\t%9.3f\n",i,ilayer,eng);
    }

    fclose(myfile);   

}

