#include "TString.h"
#include "TROOT.h"

void runEdepTree(char *fdir='',char *sfile='',char *srcpname='',int run1,int run2){
    gROOT->ProcessLine(TString::Format(".x rootC/Root2Dat_EdepTree.C (\"%s\",\"%s\",\"%s\",%d,%d)",fdir,sfile,srcpname,run1,run2));
}
