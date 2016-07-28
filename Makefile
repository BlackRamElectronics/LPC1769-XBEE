include BuildSystem/makefile.conf
include BuildSystem/Sources.mk
include BuildSystem/Includes.mk

NAME=LPC1769-XBEE
STARTUP_DEFS=-D__STARTUP_CLEAR_BSS -D__START=main

LDSCRIPTS=-L. -LLPC17XX/Linker -T gcc.ld
LFLAGS=$(USE_NANO) $(USE_NOHOST) $(LDSCRIPTS) $(GC) $(MAP)

all: $(NAME)-$(CORE).axf $(NAME)-$(CORE).bin

$(NAME)-$(CORE).axf: $(SOURCES) $(STARTUP)
	$(CC) $^ $(CFLAGS) $(LFLAGS) -o $@
	$(SIZE) $@
	
$(NAME)-$(CORE).bin:$(NAME)-$(CORE).axf
	$(OBJCOPY) -O binary $(NAME)-$(CORE).axf $(NAME)-$(CORE).bin

clean: 
	rm -f $(NAME)*.axf *.map $(NAME)*.bin
