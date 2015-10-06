


void RootOut(char *fname='', int mode=1)
{
    gROOT->SetStyle("Plain");
    //GateRootCoincBuffer mybuffer;

  char filename[100];
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
    TTree *stree = (TTree *) f->Get("Singles"); //singles tree name goes here. likewise, you can replace this with the Coincidences tree etc

    printf("Got the tree.\n");

    stree->Print();
    Int_t myentries = (Int_t) stree->GetEntries();

    float zz1,yy1;
    int id1;

    TBranch *b_zz1 = stree->GetBranch("globalPosZ");
    b_zz1->SetAddress(&zz1);
    TBranch *b_yy1 = stree->GetBranch("globalPosY");
    b_yy1->SetAddress(&yy1);
    TBranch *b_ID1 = stree->GetBranch("eventID");
    b_ID1->SetAddress(&id1);
    
    FILE *myfile;

    myfile= fopen("RootData.dat","w"); % this is the file where the data will be written

    for (int i=0;i < myentries; i++){
       fprintf(myfile,"%7d %9.3f %9.3f %9.3f\n",i,id1,zz1,yy1);
    }

    fclose(myfile);   

}

