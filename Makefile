# ---------------------------------------------------------
# Target
# ---------------------------------------------------------
# make debug-st DEBUG=TRUE

PROJECT ?= simpit

V ?= 0

# ---------------------------------------------------------
# Debug and Optimization Flags
# ---------------------------------------------------------
DEBUG ?= FALSE

ifeq ($(DEBUG), TRUE)
	DEBUG_FLAG = -g3
	OPTIMIZE_FLAG = -Og
	DEBUG_MACRO = -DDEBUG
else
	DEBUG_FLAG = -g0
	OPTIMIZE_FLAG = -O3
	DEBUG_MACRO = -DNDEBUG
endif

# ---------------------------------------------------------
# Folder structure
# ---------------------------------------------------------

ROOT         := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
BUILD_DIR    := $(ROOT)/build
DOC_DIR      := $(ROOT)/doc
MODULE_DIR   := $(ROOT)/modules
SRC_DIR      := $(ROOT)/src
TEST_DIR	 := $(ROOT)/test
MAKE_DIR	 := $(ROOT)/mk
PLATFORM_DIR := $(ROOT)/platform

# ---------------------------------------------------------
# Includes
# ---------------------------------------------------------

include $(MAKE_DIR)/arm-none-eabi.mk
include $(MAKE_DIR)/utils.mk
include $(MAKE_DIR)/verbose.mk
include $(MAKE_DIR)/sysdiag.mk

# ---------------------------------------------------------
# Sources, Includes and Libraries
# ---------------------------------------------------------
SRC = $(notdir $(shell find $(SRC_DIR) -name '*.c'))

INC += $(shell find $(SRC_DIR) -type d) \
	$(MODULE_DIR)/libopencm3/include

LDSCRIPT = $(SRC_DIR)/board/stm32_f103.ld

VPATH += $(INC)

# ---------------------------------------------------------
# Things we will build
# ---------------------------------------------------------

TARGET_ELF := $(BUILD_DIR)/$(PROJECT).elf
TARGET_BIN := $(BUILD_DIR)/$(PROJECT).bin
TARGET_HEX := $(BUILD_DIR)/$(PROJECT).hex
TARGET_MAP := $(BUILD_DIR)/$(PROJECT).map
TARGET_LSS := $(BUILD_DIR)/$(PROJECT).lss
TARGET_LIST := $(BUILD_DIR)/$(PROJECT).list
TARGET_OBJ_DIR := $(BUILD_DIR)/objs
TARGET_DEP_DIR := $(BUILD_DIR)/deps
TARGET_DOC_DIR := $(BUILD_DIR)/doc
TARGET_OBJS  = $(addprefix $(TARGET_OBJ_DIR)/, $(SRC:.c=.o))
TARGET_DEPS  = $(addprefix $(TARGET_DEP_DIR)/, $(SRC:.c=.d))

# ---------------------------------------------------------
# Rules
# ---------------------------------------------------------
.DEFAULT_GOAL := help

include $(MAKE_DIR)/rules.mk

.PHONY: help doxygen

help:
	@echo ""
	@echo "\t\tSimPit"
	@echo ""
	@echo "------------------------------ [Usage] ------------------------------"
	@echo "make help\tShow this help message"
	@echo "---------------------------------------------------------------------"

# FIXME Doxygen
doxygen:
	$(Q) mkdir -p $(TARGET_DOC_DIR)
	$(Q) doxygen doc/Doxyfile

-include $(DEPS)