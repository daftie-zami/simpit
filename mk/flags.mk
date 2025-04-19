ARCH_FLAGS = -mthumb -mcpu=cortex-m3 -msoft-float
LDLIBS = -lm -lc -lnosys -L$(MODULE_DIR)/libopencm3/lib $(LIBOPENCM3)

ifeq ($(DEBUG), TRUE)
    DEBUG_FLAG = -g3
    OPTIMIZE_FLAG = -Og
    DEBUG_MACRO = -D__DEBUG__
else
    DEBUG_FLAG = -g0
    OPTIMIZE_FLAG = -O0
    DEBUG_MACRO = -D__NDEBUG__
endif

LDFLAGS = $(LDLIBS)\
    -static \
    -nostartfiles \
    -specs=nano.specs \
    $(DEBUG_FLAG) \
    -Wl,--gc-sections \
    -Wl,--print-memory-usage \
    $(ARCH_FLAGS)

CFLAGS = $(ARCH_FLAGS)\
    $(addprefix -I,$(INC_DIR)) \
    -D$(PLATFORM) \
    -std=gnu17 \
    -ffreestanding \
    $(DEBUG_MACRO) \
    -Wall -Wextra -Werror \
    -fdata-sections \
    -ffunction-sections \
    $(DEBUG_FLAG) $(OPTIMIZE_FLAG) \
    -MMD -MP
