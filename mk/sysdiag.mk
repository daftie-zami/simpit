# https://stackoverflow.com/a/51149133
UNAME = $(shell uname)
# Linux
ifeq ($(UNAME), Linux)
	NPROCS = $(shell grep -c 'processor' /proc/cpuinfo)
	MAKEFLAGS += -j$(NPROCS)
# MacOS
else ifeq ($(UNAME), Darwin)
	NPROCS = $(shell sysctl hw.ncpu  | grep -o '[0-9]\+')
	MAKEFLAGS += -j$(NPROCS)
endif