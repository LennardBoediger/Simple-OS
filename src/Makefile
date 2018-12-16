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
# make submission
#		-- packt alles im Verzeichnis für die Abgabe zusammen
# make home
#		-- kopiert das fertige image nach $TFTP_PATH, für die
#		   Arbeit zuhause einfach den Pfad eintragen


#
# Quellen
#
LSCRIPT = kernel.lds
OBJ += ./bs-logic/start.o
OBJ += ./bs-logic/threads/init_thread.o
OBJ += ./bs-logic/threads/threads_handler.o
OBJ += ./bs-logic/Boot.o
OBJ += ./bs-logic/dataab_helper.o
OBJ += ./bs-logic/Interrupt_Handler.o
OBJ += ./bs-logic/Interrupt_Vector_Table.o
OBJ += ./bs-logic/kprintf.o
OBJ += ./bs-logic/mode_reg_helper.o
OBJ += ./bs-logic/Stack.o
OBJ += ./driver/interrupt_regs_driver.o
OBJ += ./driver/uart_driver.o
OBJ += ./lib/Interrupt_printer.o
OBJ += ./lib/printf_lib.o
OBJ += ./lib/regcheck.o
OBJ += ./lib/regcheck_asm.o
OBJ += ./lib/systemfunctions.o
OBJ += ./lib/Rick.o
OBJ += ./syscalls/syscalls.o
OBJ += ./user/interactive_test.o
OBJ += ./user/uart_listen.o
OBJ += ./user/uprintf.o




SUBMISSION_FILES = $(shell find . -name '*' -not -name '*.tar.gz' -not -name '.*' -not -type d -not -name '*.pdf' -not -path './.*')
TUBIT_NAMES = $(shell awk 'NR > 1  {ORS="+"; print prev} {prev=$$1} END { ORS=""; print $$1 }' namen.txt )


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
.PHONY: all install clean
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
	mkimage -A arm -T standalone -C none -a 0x100000 -d $< $@
	
dump:
	$(OBJDUMP) -D kernel > kernel_dump.s

install: kernel.img
	arm-install-image $<

clean:
	rm -f kernel kernel.bin kernel.img kernel_dump.s
	rm -f $(OBJ)
	rm -f $(DEP)

submission:	clean 
	tar -czf "$(TUBIT_NAMES).tar.gz" $(SUBMISSION_FILES)

home: kernel.img
	cp -v kernel.img /srv/tftp

