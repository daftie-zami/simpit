# ---------------------------------------------------------
# Cross Compiler
# ---------------------------------------------------------

TOOLCHAIN_ROOT ?=
PREFIX  := $(TOOLCHAIN_ROOT)arm-none-eabi-
CC      := $(PREFIX)gcc
AS 		:= $(PREFIX)as
CXX     := $(PREFIX)g++
OBJCOPY := $(PREFIX)objcopy
OBJDUMP := $(PREFIX)objdump
SIZE    := $(PREFIX)size
GDB 	:= gdb-multiarch

# ---------------------------------------------------------