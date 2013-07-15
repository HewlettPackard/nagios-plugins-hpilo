#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(nagios-plugins-hpilo, 1.2.0, adrian.huang@hp.com)
AC_CONFIG_SRCDIR([src/nagios_hpilo_engine.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE(nagios-plugins-hpilo, 1.2.0)

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
AC_CHECK_LIB(snmp, snmp_sess_init, [LIBS="$LIBS -lsnmp"], [AC_MSG_ERROR(Could not find snmp library.)])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset strdup])
AC_CONFIG_FILES([
	scripts/nagios_hpilo_cfg_generator
	scripts/nagios_hpilo_traps
])

AC_OUTPUT(Makefile src/Makefile scripts/Makefile)
