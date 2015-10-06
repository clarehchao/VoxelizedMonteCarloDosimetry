void Root2Dat_SrcEngHIST(char *fdir='',int run1, int run2,int mode=1)
{
  double tmp;
  int m,i,Ni;
  Ni = 1000;
  TFile *f1, *f2;
  char fname1[500],fname2[500],outfilename[500];
  FILE *myfile;
  TH1F *hist1, *hist2;

  
  for(m = run1; m <= run2; m++)
  {
	printf("Run#: %d\n",m);
	
  	gROOT->SetStyle("Plain");
  	//GateRootCoincBuffer mybuffer;

  	//read the energy histogram for electrons
  	sprintf(fname1,"%s/Run%d/RootData/hE_electron.root",fdir,m);
	printf("fname1 = %s\n",fname1);
  	f1 = new TFile(fname1, "READ");
  	f1->ReadKeys();

  	if (f1->IsZombie()) {
        	printf("Couldn't open file.\n");
        	return;
  	} else {
        	printf("Opened file: %s\n", fname1);
  	}

  	printf("Getting  the 1D histogram...\n");
  	hist1 = (TH1F *) f1->Get("KineticEnergy_electron"); //singles tree name goes here. likewise, you can replace this with the Coincidences tree etc
  	printf("Finished getting the 1D float histogram.\n");
    
  
  	sprintf(outfilename,"%s/Run%d/BustOutRoot/EHIST_electron.dat",fdir,m);
  	myfile= fopen(outfilename,"w"); //this is the file where the data will be written
  	for (i=0;i < Ni; i++){
      	tmp = hist1->GetBinContent(i);
      		//printf("%d\t%f\n",i,tmp);
      		fprintf(myfile,"%d\t%f\n",i,tmp);
  	}
  	fclose(myfile);

  	//read the energy histogram for photons
  	sprintf(fname2,"%s/Run%d/RootData/hE_photon.root",fdir,m);
  	f2 = new TFile(fname2, "READ");
  	f2->ReadKeys();

  	if (f2->IsZombie()) {
        	printf("Couldn't open file.\n");
        	return;
  	} else {
        	printf("Opened file: %s\n", fname2);
  	}

  	printf("Getting  the 1D histogram...");
  	hist2 = (TH1F *) f2->Get("KineticEnergy_photon"); //singles tree name goes here. likewise, you can replace this with the Coincidences tree etc
  	printf("Finished getting the 1D float histogram.\n");
    
  	sprintf(outfilename,"%s/Run%d/BustOutRoot/EHIST_photon.dat",fdir,m);
  	myfile= fopen(outfilename,"w"); //this is the file where the data will be written
  	for (i=0;i < Ni; i++){
      		tmp = hist2->GetBinContent(i);
      		//printf("%d\t%f\n",i,tmp);
      		fprintf(myfile,"%d\t%f\n",i,tmp);
  	}
  	fclose(myfile);

  	printf("::Data busted out of ROOT file:: MISSION COMPLETED! ^_____^\n");
  }
}

