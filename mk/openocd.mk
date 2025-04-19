DEBUG_PROBE ?= STLINK

OPENOCD_DIR := $(BUILD_DIR)/$(TOOLS_DIR)/openocd
OPENOCD_CFG := $(OPENOCD_DIR)/openocd.cfg

ocd-test: $(OPENOCD_CFG)

$(OPENOCD_CFG):
	$(Q)python3 $(TOOLS_DIR)/openocd_cfg_gen.py --interface $(DEBUG_PROBE) --target $(PLATFORM) --build-dir $(OPENOCD_DIR)

# FIXME
debug: flash
	@echo "$(YELLOW)[DEBUG] Starting OpenOCD...$(NO_COLOR)"
	@$(Q)openocd -f $(OPENOCD_CFG)
	@echo "$(GREEN)[DEBUG] OpenOCD started!$(NO_COLOR)"

debug-attach:
	@echo "$(YELLOW)[DEBUG] Attaching to OpenOCD...$(NO_COLOR)"
	@$(Q)openocd -f $(OPENOCD_CFG) -c "init; halt"
	@echo "$(GREEN)[DEBUG] OpenOCD attached!$(NO_COLOR)"

debug-gdb:
	$(Q) $(GDB) \
			-ex "target extended-remote :3333" \
			-ex "monitor reset halt" \
			-ex "break main" \
			-ex "continue" \
			-ex "layout split" \
			$(ELF)

flash: $(OPENOCD_CFG) $(PROJECT)
	@echo "$(YELLOW)[OPENOCD] Flashing $(ELF)$(NO_COLOR)"
	@$(Q)openocd -f $(OPENOCD_CFG) -c "program $(ELF) verify reset exit"
	@echo "$(GREEN)[OPENOCD] Flashing done!$(NO_COLOR)"

ocd-view-log:
	@tail -n 50 $(OPENOCD_LOG)

.PHONY: flash clean-openocd view-log