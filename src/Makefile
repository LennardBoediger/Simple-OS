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
LSCRIPT = ./bs-logic/kernel.lds
OBJ = ./bs-logic/inputtest.o
OBJ += ./bs-logic/start.o
OBJ += ./bs-logic/Interrupt_Handler.o
OBJ += ./bs-logic/Interrupt_Vector_Table.o
OBJ += ./bs-logic/Stack.o
OBJ += ./bs-logic/dataab_helper.o
OBJ += ./bs-logic/mode_reg_helper.o
OBJ += ./driver/interrupt_regs_driver.o
OBJ += ./driver/uart_driver.o
OBJ += ./lib/kprintf.o
OBJ += ./lib/regcheck.o
OBJ += ./lib/regcheck_asm.o



SUBMISSION_FILES = $(shell find . -name '*' -not -name '*.tar.gz' -not -name '.*' -not -type d )
TUBIT_NAMES = $(shell awk 'NR > 1  {ORS="+"; print prev} {prev=$$1} END { ORS=""; print $$1 }' ../Abgabe/namen.txt )
#TUBIT_NAMES = $(shell cut -d' ' -f1  ../Abgabe/namen.txt | tr '\n' '+')

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
CPPFLAGS = -Iinclude -I$(PWD)/include/

DEP = $(OBJ:.o=.d)

#
# Regeln
#
.PHONY: all
all: ./bs-logic/kernel ./bs-logic/kernel.bin dump

-include $(DEP)

%.o: %.S
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -o $@ -c $<

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -o $@ -c $<

./bs-logic/kernel: $(LSCRIPT) $(OBJ)
	$(LD) -T$(LSCRIPT) -o $@ $(OBJ) $(LIBGCC)

./bs-logic/kernel.bin: ./bs-logic/kernel
	$(OBJCOPY) -Obinary --set-section-flags .bss=contents,alloc,load,data $< $@

./bs-logic/kernel.img: ./bs-logic/kernel.bin
	mkimage -A arm -T standalone -C none -a 0x8000 -d $< $@
	
dump:
	$(OBJDUMP) -D ./bs-logic/kernel > ./bs-logic/kernel_dump.s

.PHONY: install
install: ./bs-logic/kernel.img
	cp -v ./bs-logic/kernel.img /srv/tftp
#	arm-install-image $<

.PHONY: clean
clean:
	rm -f ./bs-logic/kernel ./bs-logic/kernel.bin ./bs-logic/kernel.img ./bs-logic/kernel_dump.s
	rm -f $(OBJ)
	rm -f $(DEP)

submission:	clean 
	tar -czf "$(TUBIT_NAMES).tar.gz" $(SUBMISSION_FILES)

