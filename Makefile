######################################################################
# User Makefile sample for the Source Navigator / Introl Compiler Env.
#
# Copyright (C) 2000 K-TEAM S.A.
#
######################################################################

# Packages needed by the compiler (must be on the network)
# (DO NOT forget the / at the end).
prefix = $(KPRJ)/
sft_pack = $(KPRJ)/kteam/
hdw_pack = $(KPRJ)/kh2pack601/

# Hardware Debug Settings
port	= COM1
speed	= 38400

# Hierachical structure for separating source/object/binary
# (DO NOT forget the / at the end).
srcdir	= ./
asmdir = ./
includedir = ./
objdir = ./
libdir = ./
bindir = ./

# Include the master makefile
# (PLEASE do not modify)
include $(sft_pack)etc/Makefile

OBJ = $(addsuffix .o,$(basename $(shell ls  *.c *.s *.S *.cc 2>null)))
INC = $(addsuffix .o,$(basename $(shell ls  *.h 2>null)))
EXE = $(addsuffix .s37,$(basename $(shell grep -wl main *.c *.s *.S *.cc 2>null)))
 
all: $(EXE)
	@echo Compilation done
	@rm null

$(EXE): $(OBJ)

clean:
	@echo Cleaning...
	@rm $(EXE)

