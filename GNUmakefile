# $Id: GNUmakefile,v 1.2 2000-10-19 12:22:10 stanaka Exp $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := VoxelizedHumanDoseMultiSDv1
G4TARGET := $(name)
G4EXLIB := true

# Debug info
#CPPFLAGS += -g 

ifndef G4INSTALL
  G4INSTALL = ../..
endif

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/architecture.gmk

ifdef G4ANALYSIS_USE_ROOT   # If we have ROOT
CPPFLAGS += -DG4ANALYSIS_USE_ROOT
CPPFLAGS += $(shell root-config --cflags)
EXTRALIBS  += $(shell root-config --glibs)
endif

include $(G4INSTALL)/config/binmake.gmk

visclean:
	rm -f g4*.prim g4*.eps g4*.wrl
	rm -f .DAWN_*

