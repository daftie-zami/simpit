CC_PREFIX  ?= arm-none-eabi-
CC      := $(CC_PREFIX)gcc
CXX     := $(CC_PREFIX)g++
LD      := $(CC_PREFIX)gcc
AS      := $(CC_PREFIX)as
OBJCOPY := $(CC_PREFIX)objcopy
OBJDUMP := $(CC_PREFIX)objdump
SIZE    := $(CC_PREFIX)size
GDB     := gdb-multiarch