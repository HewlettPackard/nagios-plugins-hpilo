# (C) Copyright [2013-2014] Hewlett-Packard Development Company, L.P.

# Overridden during automated builds
COMP_NAME    := nagios-plugins-hpilo
COMP_VER     := 1.4.0
COMP_PKG_REV := 700

NAME         := $(COMP_NAME)
VERSION      := $(COMP_VER)
BUILD_NUMBER := $(COMP_PKG_REV)

CONFIG_HEADER := config.h

ARCH ?= $(shell uname -m)
MAKE := make ARCH=${ARCH}

RPMSOURCES=$(shell rpm --eval %{_sourcedir})
RPMRPMS=$(shell rpm --eval %{_rpmdir})

INSTALL=install
DIRINSTALL=install -d

SHELL = /bin/sh

OS=$(shell uname)

RM  = /bin/rm -f
LD = gcc 
AS = gcc
CC = gcc
CXX = g++

LN = /bin/ln -sf

ifneq ($(ARCH),x86_64)
C32FLAGS = -m32
LD32FLAGS = -melf_i386
endif

ifeq "$(RPM_BUILD_ROOT)" ""
PREFIX ?= /usr/local
else
PREFIX ?= /usr/local/nagios
DESTDIR = $(RPM_BUILD_ROOT)
endif

LIBEXECDIR=$(DESTDIR)$(PREFIX)/libexec
LIBEXE_LINKDIR=/usr/local/sbin/

TARFILE=$(NAME)-$(VERSION).tar.gz

SUBDIRS := src scripts 

INSTSUBDIRS := src scripts

.PHONY: subdirs $(SUBDIRS)
.PHONY: all clean install source_tarball


all: create_config_header subdirs 

create_config_header:
	echo "#define VERSION	\"$(COMP_VER)\"" > $(CONFIG_HEADER)

clean:
	rm -rf tmp
	rm -f config.h
	rm -f $(TARFILE) $(NAME).spec 
	for d in $(SUBDIRS); do ${MAKE} -C $$d clean; done
	# Editor backup files
	find . -name '*~' -exec rm {} \;
	find . -name '#*#' -exec rm {} \;
	find . -name '.*.swp' -exec rm {} \;
	find . -name '.*.orig' -exec rm {} \;

distclean: clean

subdirs:
	for d in $(SUBDIRS); do set -e; ${MAKE} -C $$d VERSION=$(COMP_VER) RPM_BUILD_ROOT=$(RPM_BUILD_ROOT); done

$(SUBDIRS):
	cd $@ && ${MAKE} -f all;  

$(NAME).spec:	$(NAME).spec.in
	sed "s/\%VERSION\%/$(VERSION)/g" < $< > $<.tmp
	sed -i "s/\%RELEASE\%/$(subst %,\%,$(BUILD_NUMBER)$(MX_DIST)$(MX_REL))/g" $<.tmp
	mv $<.tmp $@

install: all
	$(DIRINSTALL) -m 755 $(LIBEXECDIR)
	$(INSTALL) -m 755 ./src/nagios_hpilo_engine $(LIBEXECDIR)
	$(INSTALL) -m 755 ./scripts/nagios_hpilo_cfg_generator $(LIBEXECDIR)
	$(INSTALL) -m 755 ./scripts/nagios_hpilo_traps $(LIBEXECDIR)
	$(INSTALL) -m 755 ./scripts/hpiLO_nagios_config $(LIBEXECDIR)
	

source_tarball:	$(TARFILE)
$(TARFILE): $(NAME).spec
	mkdir -p tmp/$(NAME)-$(VERSION)
	tar -c * --exclude '*/.svn' \
	         --exclude tmp \
                 --exclude '*~' \
                 --exclude '#*#' \
                 --exclude $(NAME).spec \
                 --exclude '.*.swp' | (cd tmp/$(NAME)-$(VERSION) && tar x)
	$(MAKE) -C tmp/$(NAME)-$(VERSION) distclean
	$(MAKE) -C tmp/$(NAME)-$(VERSION) \
          COMP_NAME=$(COMP_NAME) COMP_VER=$(COMP_VER) \
          COMP_PKG_REV=$(COMP_PKG_REV) $(NAME).spec
	tar -C tmp -cz $(NAME)-$(VERSION) > $@

rpm: $(TARFILE)
	@test -d $(RPMSOURCES) || \
	  (echo "RPM SOURCE directory does not exist [$(RPMSOURCES)]" && exit 1)
	rpmbuild -ta --target $(ARCH) $(NAME)-$(VERSION).tar.gz
