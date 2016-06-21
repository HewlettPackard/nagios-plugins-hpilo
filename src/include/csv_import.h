/* csv_import.h -- types and prototypes used by nagios iLO engine 
   (C) Copyright [2016] Hewlett Packard Enterprise Development Company, L.P.

   This program is free software; you can redistribute it and/or modify 
   it under the terms of version 2 of the GNU General Public License as 
   published by the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 

   See the GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License 
   along with this program; if not, write to:
    Free Software Foundation, Inc.
    51 Franklin Street, Fifth Floor
    Boston, MA 02110-1301, USA.  */

#ifndef CSV_IMPORT_H
#define CSV_IMPORT_H

#include "nagios_hpilo_common.h"

/* Macro definition */
#define USAGE	"-F <csv file name> [-V] [-h]"

/* Available options for this plugin.  */ 
#define SHORT_OPTIONS "hVF:"

/* The required the number of arguments for this plugin.  */
#define NAGIOS_ILO_REQUIRE_ARGC	1

/* Strings for help menu.  */
static char help_string[][128] =
  {"-F, --file=csv file name\n\t",
   "-V, --version\n\tprint the version number",
   "-h, --help\n\tprint this help menu"
  };

/* options for arguments.  */
static struct option long_options[] =
  {
    {"file",		required_argument,	0,	'F'}, 
    {"version",		no_argument,		0,	'V'},
    {"help",		no_argument,		0,	'h'},
    {0,			0,			0,	  0}
  };

#endif
