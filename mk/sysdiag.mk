# ----------------------------------------------
# System Diagnostic
# ----------------------------------------------
# Diagnostic tools for system information
# and system configuration.
# ----------------------------------------------

# https://stackoverflow.com/a/51149133
UNAME = $(shell uname)
# Linux / Unix
ifeq ($(UNAME), Linux)
	NPROCS = $(shell grep -c 'processor' /proc/cpuinfo)
	MAKEFLAGS += -j$(NPROCS)
	OSFAMILY := Linux
# MacOS
else ifeq ($(UNAME), Darwin)
	NPROCS = $(shell sysctl hw.ncpu  | grep -o '[0-9]\+')
	MAKEFLAGS += -j$(NPROCS)
	OSFAMILY := Darwin
# Windows
# FIXME - This is get the number of processors instead of cores
else ifeq ($(UNAME), Windows_NT) 
	NPROCS = $(shell WMIC CPU Get NumberOfCores | findstr [0-9])  
	MAKEFLAGS += -j$(NPROCS)
	OSFAMILY := Windows
endif