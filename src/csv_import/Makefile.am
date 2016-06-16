AUTOMAKE_OPTIONS=foreign

AM_LDFLAGS = -L$(top_srcdir)/src/ilo_credit/.libs/ -lm -lilocrst -ldl -lcrypto

AM_CPPFLAGS = -I$(top_srcdir)/src/include

AM_CFLAGS =

libexec_PROGRAMS = csv_import

csv_import_SOURCES = csv_import.c
