ELF := $(BUILD_DIR)/bin/$(PROJECT).elf
BIN := $(BUILD_DIR)/bin/$(PROJECT).bin
HEX := $(BUILD_DIR)/bin/$(PROJECT).hex
MAP := $(BUILD_DIR)/bin/$(PROJECT).map
LSS := $(BUILD_DIR)/bin/$(PROJECT).lss
LIST := $(BUILD_DIR)/bin/$(PROJECT).list

OBJ := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SRC))
DEP := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.d,$(C_SRC))

$(PROJECT): $(BIN) $(HEX) $(LIST) $(LSS)

# TODO Test this
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "$(GREEN)[CC] Compiling\t$(notdir $<)$(NO_COLOR)"
	@mkdir -p $(dir $@)
	$(Q)$(CC) $(CFLAGS) -o $@ -c $<

#TODO add cxx and s compiler rules
$(ELF): $(OBJ)
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)[LD] Linking\t$@$(NO_COLOR)"
	$(Q)$(LD) $(OBJ) $(LDFLAGS) -Wl,-Map=$(MAP),--cref -L$(MODULE_DIR)/libopencm3/lib -T$(LDSCRIPT) -o $@
	@echo "$(WHITE)[SIZE] Firmware size:$(NO_COLOR)"
	$(Q)$(SIZE) -A -x $@

$(BIN): $(ELF)
	@echo "$(BLUE)[OBJCOPY] Creating bin\t$(notdir $@)$(NO_COLOR)"
	$(Q)$(OBJCOPY) -Obinary $< $@

$(HEX): $(ELF)
	@echo "$(BLUE)[OBJCOPY] Creating hex\t$(notdir $@)$(NO_COLOR)"
	$(Q)$(OBJCOPY) -Oihex $< $@

$(LIST): $(ELF)
	@echo "$(MAGENTA)[OBJDUMP] Creating list\t$(notdir $@)$(NO_COLOR)"
	$(Q)$(OBJDUMP) -S $< > $@

$(LSS): $(ELF)
	@echo "$(MAGENTA)[OBJDUMP] Creating lss\t$(notdir $@)$(NO_COLOR)"
	$(Q)$(OBJDUMP) -h -S $< > $@

-include $(DEP)