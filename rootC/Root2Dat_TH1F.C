void Root2Dat_TH1F(char *fname='', char *tag='', int mode=1)
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

    printf("Getting  the 1D histogram...");
    TH1F *hist = (TH1F *) f->Get("VertexKineticEnergy"); //singles tree name goes here. likewise, you can replace this with the Coincidences tree etc
    printf("Finished getting the 1D float histogram.\n");
    
    FILE *myfile;
    char outfilename[200];
    sprintf(outfilename,"BustOutRoot_%s.dat",tag);

    myfile= fopen(outfilename,"w"); //this is the file where the data will be written

    double tmp;
    int i,Ni;
    Ni = 1000;

    for (i=0;i < Ni; i++){
	tmp = hist->GetBinContent(i);
	//printf("%d\t%f\n",i,tmp);
	fprintf(myfile,"%d\t%f\n",i,tmp);
    }
    fclose(myfile);
    printf("::Data busted out of ROOT file:: MISSION COMPLETED!\n");
}

