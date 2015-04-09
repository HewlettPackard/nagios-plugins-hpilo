
SCRIPTS	:= nagios_hpilo_cfg_generator nagios_hpilo_traps hpiLO_nagios_config

ifeq "$(RPM_BUILD_ROOT)" ""
PREFIX ?= \/usr\/local
else
PREFIX ?= \/usr\/local\/nagios
endif

.PHONY: all clean

all: $(SCRIPTS)

%: %.in
	sed "s/\@prefix\@/$(PREFIX)/g" < $< > $<.tmp
	mv $<.tmp $@

clean:
	rm -f $(SCRIPTS)
