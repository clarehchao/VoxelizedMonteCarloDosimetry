
void show(char *fname='', int mode=1)
{
  TFile* file = TFile::Open(fname);
  TCanvas* c =new TCanvas();
  //c->Divide(1,2);
  //c->cd(1);
  //file->Get("LongitudinalProfile")->Draw();
  //c->cd(2);
  file->Get("VertexKineticEnergy")->Draw();
}
