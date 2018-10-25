#
# Kurzanleitung
# =============
#
# make		-- Baut den Kernel.
# make all
#
# make install	-- Baut den Kernel und transferiert ihn auf den Server.
# 		   Das Board holt sich diesen Kernel beim nächsten Reset.
#
# make clean	-- Löscht alle erzeugten Dateien.
#


#
# Quellen
#
LSCRIPT = kernel.lds
OBJ = start.o
OBJ += uart.o

SUBMISSION_FILES = $(shell find . -name "*")
TUBIT_NAMES = $(shell awk 'NR > 1  {ORS="+"; print prev} {prev=$$1} END { ORS=""; print $$1 }' namen.txt )
#TUBIT_NAMES = $(shell cut -d' ' -f1  namen.txt | tr '\n' '+')

ifeq ($(TUBIT_NAMES), )
    TUBIT_NAMES = "invalid"
    $(error "namen.txt ist fehlerhaft oder leer!")
    INVAL = 1
endif

#
# Konfiguration
#
CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump

CFLAGS = -Wall -Wextra -ffreestanding -mcpu=cortex-a7 -O2
CPPFLAGS = -Iinclude

DEP = $(OBJ:.o=.d)

#
# Regeln
#
.PHONY: all
all: kernel kernel.bin dump

-include $(DEP)

%.o: %.S
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -o $@ -c $<

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -o $@ -c $<

kernel: $(LSCRIPT) $(OBJ)
	$(LD) -T$(LSCRIPT) -o $@ $(OBJ) $(LIBGCC)

kernel.bin: kernel
	$(OBJCOPY) -Obinary --set-section-flags .bss=contents,alloc,load,data $< $@

kernel.img: kernel.bin
	mkimage -A arm -T standalone -C none -a 0x8000 -d $< $@
	
dump:
	$(OBJDUMP) -D kernel > kernel_dump.s

.PHONY: install
install: kernel.img
#	cp -v kernel.img /srv/tftp
	arm-install-image $<

.PHONY: clean
clean:
	rm -f kernel kernel.bin kernel.img kernel_dump.s
	rm -f $(OBJ)
	rm -f $(DEP)

submission:	clean 
	tar -czf "$(TUBIT_NAMES).tar.gz" $(SUBMISSION_FILES)

