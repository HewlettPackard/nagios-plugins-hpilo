AUTOMAKE_OPTIONS=foreign

AM_LDFLAGS = -L$(top_srcdir)/src/ilo_credit/.libs/ -lilocrst -lm -ldl -lcrypto

AM_CPPFLAGS = -I$(top_srcdir)/src/include

AM_CFLAGS =

libexec_PROGRAMS = nagios_hpeilo_engine

nagios_hpeilo_engine_SOURCES= nagios_hpilo_engine.c nagios_hpilo_snmp.c base64.c

