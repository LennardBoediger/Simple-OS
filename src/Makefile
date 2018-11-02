# Makefile zum Toolchain bauen
# ============================
#
# Alles bauen (Toolchain (binutils, GCC) + QEMU + GDB):
#	make all
#
# Einzelne Dinge bauen:
# 	make toolchain
# 	make qemu
# 	make gdb
#
# Aufräumen:
# 	make clean		-- Build-Verzeichnisse löschen
# 	make mrproper		-- wie clean, zusätzlich entpackte Quellen löschen
# 	make distclean		-- wie mrproper, zusätzlich die heruntergeladenen Archive löschen
#
# Um die Toolchain woanders zu installieren (anstatt $HOME/arm), muss die
# Umgebungsvariable PREFIX entsprechend gesetzt sein. Beispielsweise:
# 	PREFIX=/arm make all
#
# Um sich die ganzen Kommandos anzuschauen (quasi das HOWTO in Kompaktform),
# funktioniert z. B. sowas wie:
# 	 make -n -B all | grep -v "^touch"
#


# Standardmäßig kommt die Toolchain von unserem Server. Und zwar von:
#MIRROR = https://svn.kbs.tu-berlin.de/svn/BSPrak/2014w/public/toolchain
#FETCH_FROM = MIRROR

# Um die einzelnen Pakete stattdessen von den offiziellen Seiten zu beziehen, einfach
# das Kommentarzeichen der folgenden Zeile entfernen:
FETCH_FROM = $(X-$*)_URL

# Standardinstallationspunkt der Toolchain
PREFIX ?= $(HOME)/arm

# Default-Target
all:

#
# Bestandteile der Toolchain
#

BINUTILS = binutils-2.31.1
BINUTILS_ARCHIVE = $(BINUTILS).tar.bz2
BINUTILS_URL = http://ftp.gnu.org/gnu/binutils
BINUTILS_CONFIGURE = ../$(BINUTILS)/configure --target=arm-none-eabi --prefix=$(PREFIX) --enable-interwork --disable-nls

#GCC = gcc-5.4.0
GCC = gcc-4.9.4
GCC_ARCHIVE = $(GCC).tar.bz2
GCC_URL = http://ftp.gnu.org/gnu/gcc/$(GCC)
GCC_CONFIGURE = ln -snf ../$(GMP) gmp && ln -snf ../$(MPFR) mpfr && ln -snf ../$(MPC) mpc && ../$(GCC)/configure --target=arm-none-eabi --prefix=$(PREFIX) --enable-interwork --enable-languages="c" --with-newlib --without-headers --disable-nls

GMP = gmp-6.0.0a
GMP2 = gmp-6.0.0
GMP_ARCHIVE = $(GMP).tar.bz2
GMP_URL = http://ftp.gnu.org/gnu/gmp

MPFR = mpfr-3.1.4
MPFR_ARCHIVE = $(MPFR).tar.bz2
MPFR_URL = http://www.mpfr.org/$(MPFR)

MPC = mpc-1.0.2
MPC_ARCHIVE = $(MPC).tar.gz
MPC_URL = https://ftp.gnu.org/gnu/mpc/

NEWLIB = newlib-2.4.0
NEWLIB_ARCHIVE = $(NEWLIB).tar.gz
NEWLIB_URL = ftp://sourceware.org/pub/newlib
NEWLIB_CONFIGURE = ../$(NEWLIB)/configure --target=arm-none-eabi --prefix=$(PREFIX) --enable-interwork --disable-nls



MKIMAGE = u-boot-2016.03
MKIMAGE_ARCHIVE = $(MKIMAGE).tar.bz2
MKIMAGE_URL = ftp://ftp.denx.de/pub/u-boot
MKIMAGE_CONFIGURE = true

QEMU = qemu-2.7.0
QEMU_ARCHIVE = $(QEMU).tar.bz2
QEMU_URL = http://wiki.qemu-project.org/download
QEMU_CONFIGURE = ../$(QEMU)/configure --target-list="arm-softmmu" --prefix=$(PREFIX) --python="/usr/bin/python2.7"
qemu-pool: QEMU_CONFIGURE += --disable-sdl --disable-bluez --disable-vnc --disable-smartcard-nss --disable-gtk

#AT91 = at91-v2.1.2.patch
#AT91_ARCHIVE = $(AT91).gz
#AT91_URL = $(MIRROR)

GDB = gdb-7.11.1
GDB_ARCHIVE = $(GDB).tar.gz
GDB_URL = http://ftp.gnu.org/gnu/gdb
GDB_CONFIGURE = ../$(GDB)/configure --target=arm-none-eabi --prefix=$(PREFIX) --enable-interwork --disable-nls

# Einzelteile zusammensammeln
PACKAGES = $(PACKAGES_BUILD) $(PACKAGES_EXTRA)
PACKAGES_BUILD = BINUTILS GCC NEWLIB MKIMAGE QEMU GDB
PACKAGES_EXTRA = GMP MPFR MPC AT91

ARCHIVES = $(foreach target,$(PACKAGES),$($(target)_ARCHIVE))

# ermöglicht Reverse-Lookup (so dass z. B. $(X-gdb-7.6) nach "GDB" umgesetzt wird)
$(foreach target,$(PACKAGES),$(eval X-$($(target)) = $(target)))


#
# Globale Einstellungen
#

THREADS := $(shell grep "^processor" /proc/cpuinfo | wc -l)
#THREADS := 1
export PATH := $(PREFIX)/bin:$(PATH)


#
# Allgemeine Regeln
#

.PHONY: all all-pool toolchain gdb mkimage qemu qemu-pool
all: toolchain gdb qemu
all-pool: toolchain gdb mkimage qemu-pool
toolchain: $(GCC)-build/.installed
gdb: $(GDB)-build/.installed
#mkimage: $(MKIMAGE)-build/.installed
qemu: $(QEMU)-build/.installed
qemu-pool: qemu

.PHONY: clean mrproper distclean
clean:
	rm -Rf $(foreach target,$(PACKAGES_BUILD),$($(target))-build)
mrproper: clean
	rm -Rf $(foreach target,$(PACKAGES),$($(target)))
distclean: mrproper
	rm -Rf $(ARCHIVES)


#
# Generelle Regeln
#

.SECONDARY:

%-build/.installed: %-build/.built
	make -C $*-build install
	touch $@

%-build/.built: %-build/.configured
	make -C $*-build -j $(THREADS)
	touch $@

%-build/.configured: %/.unpacked
	mkdir -p $*-build
	cd $*-build && $($(X-$*)_CONFIGURE)
	touch $@

%/.unpacked: %.tar.gz
	tar xzf $<
	touch $@

%/.unpacked: %.tar.bz2
	tar xjf $<
	touch $@

%.patch: %.patch.gz
	gunzip -c $< > $@

$(filter %.tar.bz2,$(ARCHIVES)): %.tar.bz2:
	wget -c --no-check-certificate $($(FETCH_FROM))/$($(X-$*)_ARCHIVE) -O $@.part
	mv $@.part $@

$(filter %.tar.gz,$(ARCHIVES)): %.tar.gz:
	wget -c --no-check-certificate $($(FETCH_FROM))/$($(X-$*)_ARCHIVE) -O $@.part
	mv $@.part $@

$(filter %.patch.gz,$(ARCHIVES)): %.gz:
	wget -c --no-check-certificate $($(FETCH_FROM))/$($(X-$*)_ARCHIVE) -O $@.part
	mv $@.part $@


#
# Spezielle Regeln
#

# GCC braucht Binutils und besteht aus mehreren Quellpaketen
$(GCC)-build/.configured: $(BINUTILS)-build/.installed $(GMP)/.unpacked $(MPFR)/.unpacked $(MPC)/.unpacked

# minimale GCC (wird benötigt, um Newlib bauen zu können, um wiederum GCC fertig zu bauen)
$(GCC)-build/.built-gcc: $(GCC)-build/.configured
	make -C $(GCC)-build -j $(THREADS) all-gcc
	touch $@
$(GCC)-build/.installed-gcc: $(GCC)-build/.built-gcc
	make -C $(GCC)-build install-gcc
	touch $@

# Newlib braucht minimale GCC
$(NEWLIB)-build/.configured: $(GCC)-build/.installed-gcc

# vollständige GCC braucht Newlib
$(GCC)-build/.built: $(NEWLIB)-build/.installed

# mkimage hat kein "normales" Makefile
$(MKIMAGE)-build/.installed: %-build/.installed: %-build/.built
	cp $*-build/tools/mkimage $(PREFIX)/bin
	touch $@
$(MKIMAGE)-build/.built: %-build/.built: %-build/.configured
	make O=../$*-build -C $* -j $(THREADS) tools-all
	touch $@

# QEMU braucht Patch für AT91RM9200-Support
#$(QEMU)-build/.configured: $(QEMU)/.patched
#$(QEMU)/.patched: $(AT91) $(QEMU)/.unpacked
#	patch -d $(QEMU) -p1 < $<
#	touch $@

# bei QEMU installieren wir einen Wrapper zum Aufrufen mit
$(QEMU)-build/.installed: %-build/.installed: %-build/.built
	make -C $*-build install
#	echo '#!/bin/sh' > $(PREFIX)/bin/qemu-bsprak
#	echo 'exec $(PREFIX)/bin/qemu-system-arm -M portux920t -m 64M -nographic "$$@"' >> $(PREFIX)/bin/qemu-bsprak
#	chmod 0755 $(PREFIX)/bin/qemu-bsprak
	touch $@

$(GMP)/.unpacked: $(GMP).tar.bz2
	tar xjf $<
	mv $(GMP2) $(GMP)
	touch $@

