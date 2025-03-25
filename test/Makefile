PROJECT ?= test_

V ?= 0

DEBUG ?= FALSE

ROOT         := ..
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

TEST_LED_SRC = 	led.c \
		led_test.c

TARGET_OBJ_DIR := $(BUILD_DIR)/objs
TARGET_DEP_DIR := $(BUILD_DIR)/deps
TARGET_OBJS  = $(addprefix $(TARGET_OBJ_DIR)/, $(SRC:.c=.o))
TARGET_DEPS  = $(addprefix $(TARGET_DEP_DIR)/, $(SRC:.c=.d))

INC += $(MODULE_DIR)/libopencm3/include  \
	$(shell find $(SRC_DIR) -type d)

VPATH += $(INC)

include $(MAKE_DIR)/rules.mk