# ---------------------------------------------------------
# Project
# ---------------------------------------------------------
# make debug-st DEBUG=TRUE

PROJECT ?= simpit

V ?= 1

DEBUG ?= FALSE

# ---------------------------------------------------------
# Folder structure
# ---------------------------------------------------------

BUILD_DIR	 := build
DOC_DIR      := doc
MODULE_DIR   := modules
SRC_DIR      := src
TEST_DIR	 := test
MAKE_DIR	 := mk

include $(MAKE_DIR)/arm-none-eabi.mk
include $(MAKE_DIR)/utils.mk
include $(MAKE_DIR)/verbose.mk
include $(MAKE_DIR)/sysdiag.mk

SRC = $(SRC_DIR)/lib/hid/hid.c \
	$(SRC_DIR)/lib/led/led.c \
	$(SRC_DIR)/lib/systick/systick.c \
	$(SRC_DIR)/lib/mpu6050/mpu6050.c \
	$(SRC_DIR)/lib/console/console.c \
	$(SRC_DIR)/app/simpit.c

INC += $(MODULE_DIR)/libopencm3/include  \
	$(SRC_DIR) $(SRC_DIR)/board $(SRC_DIR)/common

VPATH += $(INC)

TARGET_OBJ_DIR := $(BUILD_DIR)/objs
TARGET_DEP_DIR := $(BUILD_DIR)/deps
TARGET_OBJS  = $(addprefix $(TARGET_OBJ_DIR)/, $(SRC:.c=.o))
TARGET_DEPS  = $(addprefix $(TARGET_DEP_DIR)/, $(SRC:.c=.d))

TARGET_OBJ := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SRC))
TARGET_DEP := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.d,$(C_SRC))

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

