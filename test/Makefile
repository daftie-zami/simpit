PROJECT ?= test

PLATFORM ?= STM32F1

DEBUG ?= FALSE

DEBUG_PROBE ?= STLINK

V ?= 1

BUILD_DIR := build
MK_DIR := ../mk
MODULE_DIR := ../modules
SRC_DIR := ../src
TOOLS_DIR := ../tools

LDSCRIPT := $(SRC_DIR)/board/stm32_f103.ld
LIBOPENCM3 := -lopencm3_stm32f1

# Source files
C_SRC = $(SRC_DIR)/lib/hid/hid.c /test_hid/test_hid.c 

# Include directories
INC_DIR = $(shell find $(SRC_DIR) -type d) $(MODULE_DIR)/libopencm3/include

include $(MK_DIR)/verbosity.mk

all: $(PROJECT)

clean : 
	@echo "$(RED)[CLEAN] Cleaning up...$(NO_COLOR)"
	@rm -rf $(BUILD_DIR)

include $(MK_DIR)/utils.mk
include $(MK_DIR)/doxygen.mk
include $(MK_DIR)/flags.mk
include $(MK_DIR)/openocd.mk
include $(MK_DIR)/rules.mk
include $(MK_DIR)/sysdiag.mk
include $(MK_DIR)/toolchain.mk

help:
	@echo "Makefile for $(PROJECT)"
	@echo "Usage: make [target]"
	@echo ""
	@echo "Targets:"
	@echo "$(PROJECT) --- Build the project"

.DEFAULT_GOAL := help
.PHONY: all clean help test