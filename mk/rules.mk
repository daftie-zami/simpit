# ---------------------------------------------------------
# Compiler, Architecture and linker flags
# ---------------------------------------------------------
ifeq ($(DEBUG), TRUE)
	DEBUG_FLAG = -g3
	OPTIMIZE_FLAG = -Og
	DEBUG_MACRO = -DDEBUG
else
	DEBUG_FLAG = -g0
	OPTIMIZE_FLAG = -O3
	DEBUG_MACRO = -DNDEBUG
endif

ARCH_FLAGS := -mthumb -mcpu=cortex-m3 -msoft-float

CFLAGS = $(ARCH_FLAGS) \
	$(addprefix -I,$(INC)) \
	-DSTM32F1 \
	-std=gnu17 \
	-ffreestanding \
	$(DEBUG_MACRO) \
	-Wall -Wextra -Werror \
	-fdata-sections \
	-ffunction-sections \
	$(DEBUG_FLAG) $(OPTIMIZE_FLAG) \
	-MMD -MP -MF$(TARGET_DEP_DIR)/$*.d

LDSCRIPT = $(SRC_DIR)/board/stm32_f103.ld
LDFLAGS = -lc -lm -lnosys -L$(MODULE_DIR)/libopencm3/lib -lopencm3_stm32f1 \
	--static \
	-nostartfiles \
	-specs=nano.specs \
	-L$(MODULE_DIR)/libopencm3/lib -T$(LDSCRIPT) \
	$(ARCH_FLAGS) $(DEBUG_FLAG) \
	-Wl,-Map=$(TARGET_MAP),--cref \
	-Wl,--gc-sections \
	-Wl,--print-memory-usage

# ---------------------------------------------------------
# Things we will build
# ---------------------------------------------------------

TARGET_ELF := $(BUILD_DIR)/$(PROJECT).elf
TARGET_BIN := $(BUILD_DIR)/$(PROJECT).bin
TARGET_HEX := $(BUILD_DIR)/$(PROJECT).hex
TARGET_MAP := $(BUILD_DIR)/$(PROJECT).map
TARGET_LSS := $(BUILD_DIR)/$(PROJECT).lss
TARGET_LIST := $(BUILD_DIR)/$(PROJECT).list

# ---------------------------------------------------------
# Rules ("Ordular, ilk hedefiniz Akdeniz'dir, ileri!")
# ---------------------------------------------------------

.PHONY: all _dirs clean flash-st debug-st debug-gdb

all: $(TARGET_BIN) $(TARGET_HEX) $(TARGET_LSS) $(TARGET_LIST)

_dirs:
	$(Q) mkdir -p $(TARGET_OBJ_DIR)
	$(Q) mkdir -p $(TARGET_DEP_DIR)

$(TARGET_OBJ_DIR)/%.o: %.c _dirs
	@echo "$(GREEN)[CC] Compiling\t$(notdir $<)$(NO_COLOR)"
	$(Q) $(CC) $(CFLAGS) -c $< -o $@

$(TARGET_OBJ_DIR)/%.o: %.s _dirs
	@echo "$(GREEN)[AS] Assembling\t$(notdir $<)$(NO_COLOR)"
	$(Q) $(CC) $(ASFLAGS) -c $< -o $@

$(TARGET_ELF): $(TARGET_OBJS)
	@echo "$(YELLOW)[LD] Linking\t$(notdir $@)$(NO_COLOR)"
	$(Q) $(CC) $(TARGET_OBJS) $(LDFLAGS) -o $(TARGET_ELF)
	@echo "\e[36m[SIZE] Firmware size:$(NO_COLOR)"
	$(Q) $(SIZE) -A -x $@

$(TARGET_BIN): $(TARGET_ELF)
	@echo "$(BLUE)[OBJCOPY] Creating bin\t$(notdir $@)$(NO_COLOR)"
	$(Q) $(OBJCOPY) -O binary $< $@

$(TARGET_HEX): $(TARGET_ELF)
	@echo "$(BLUE)[OBJCOPY] Creating hex\t$(notdir $@)$(NO_COLOR)"
	$(Q) $(OBJCOPY) -O ihex --set-start 0x08000000 $< $@

$(TARGET_LSS): $(TARGET_ELF)
	@echo "$(MAGENTA)[OBJDUMP] Creating lss\t$(notdir $@)$(NO_COLOR)"
	$(Q) $(OBJDUMP) -h -S $< > $@

$(TARGET_LIST): $(TARGET_ELF)
	@echo "$(MAGENTA)[OBJDUMP] Creating list\t$(notdir $@)$(NO_COLOR)"
	$(Q) $(OBJDUMP) -S $< > $@

clean:
	@echo "$(BOLD)[CLEAN] clean objects and binaries$(NO_COLOR)"
	$(Q)rm -rf $(TARGET_OBJS) $(TARGET_DEPS) $(TARGET_ELF) $(TARGET_BIN) $(TARGET_HEX) $(TARGET_MAP) $(TARGET_LSS) $(TARGET_LIST)

flash-st: all
	@echo "$(WARNING) This command requires sudo privileges!$(NO_COLOR)"
	@echo "$(CYAN)[FLASH] Flashing target via STLink$(NO_COLOR)"
	$(Q) sudo openocd -f interface/stlink.cfg -f target/stm32f1x.cfg -c "program $(TARGET_BIN) 0x08000000 reset exit"

debug-st: flash-st
	@echo "$(WARNING) This command requires sudo privileges!$(NO_COLOR)"
	@echo "$(CYAN)[DEBUG] Starting GDB server via STLink$(NO_COLOR)"
	$(Q) sudo openocd -f interface/stlink.cfg -f target/stm32f1x.cfg

debug-gdb:
	$(Q) $(GDB) \
			-ex "target extended-remote :3333" \
			-ex "monitor reset halt" \
			-ex "break main" \
			-ex "continue" \
			-ex "layout split" \
			$(TARGET_ELF)

# -include $(TARGET_DEPS)

# ---------------------------------------------------------