# ---------------------------------------------------------
# Project
# ---------------------------------------------------------
# make debug-st DEBUG=TRUE

PROJECT ?= simpit

V ?= 0

DEBUG ?= FALSE

# ---------------------------------------------------------
# Folder structure
# ---------------------------------------------------------

ROOT         := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
BUILD_DIR	 := build
DOC_DIR      := $(ROOT)/doc
MODULE_DIR   := $(ROOT)/modules
SRC_DIR      := $(ROOT)/src
TEST_DIR	 := $(ROOT)/test
MAKE_DIR	 := $(ROOT)/mk

include $(MAKE_DIR)/arm-none-eabi.mk
include $(MAKE_DIR)/utils.mk
include $(MAKE_DIR)/verbose.mk
include $(MAKE_DIR)/sysdiag.mk

SRC = hid.c \
	led.c \
	systick.c \
	simpit.c \
	mpu6050.c \
	console.c

INC += $(MODULE_DIR)/libopencm3/include  \
	$(shell find $(SRC_DIR) -type d)

VPATH += $(INC)

TARGET_OBJ_DIR := $(BUILD_DIR)/objs
TARGET_DEP_DIR := $(BUILD_DIR)/deps
TARGET_OBJS  = $(addprefix $(TARGET_OBJ_DIR)/, $(SRC:.c=.o))
TARGET_DEPS  = $(addprefix $(TARGET_DEP_DIR)/, $(SRC:.c=.d))

include $(MAKE_DIR)/rules.mk

# ---------------------------------------------------------
# Rules
# ---------------------------------------------------------
.DEFAULT_GOAL := help

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
	$(Q) mkdir -p build/doc
	$(Q) doxygen doc/Doxyfile

