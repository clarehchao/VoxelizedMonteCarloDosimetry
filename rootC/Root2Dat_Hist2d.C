


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
    TH2F *hist = (TH2F *) f->Get("Ehist"); //singles tree name goes here. likewise, you can replace this with the Coincidences tree etc

    printf("Got the 2d histogram.\n");
    
    FILE *myfile;

    myfile= fopen("RootData.dat","w"); % this is the file where the data will be written

	double tmp;
	int i,j,Ni,Nj

    for (i=0;i < Ni; i++){
		for(j=0; j< Nj; j++){
			tmp = hist->GetBinConten(i,j);
			printf("%d\t%d\t%f\n",i,j,tmp);
			fprintf(myfile,"%d\t%d\t%f\n",i,j,tmp);
		}
    }

    fclose(myfile);   

}

