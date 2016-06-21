AUTOMAKE_OPTIONS=foreign

AM_LDFLAGS = -L$(top_srcdir)/src/ilo_credit/.libs/ -lilocrst -lm -ldl -lcrypto

AM_CPPFLAGS = -I$(top_srcdir)/src/include

AM_CFLAGS =

libexec_PROGRAMS = credit_save

credit_save_SOURCES = credit_save.c
