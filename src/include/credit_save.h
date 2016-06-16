/* credit_setting.h -- types and prototypes used by nagios iLO engine 
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

/* Written by Maple Yang <maple.yang@hpe.com>.               */

#ifndef CREDIT_SETTING_H
#define CREDIT_SETTING_H

#include "nagios_hpilo_common.h"

/* Macro definition */
#define USAGE	"-H <IP Address> [-V] [-h]"

/* Available options for this plugin.  */ 
#define SHORT_OPTIONS "hVH:"

/* The required the number of arguments for this plugin.  */
#define NAGIOS_ILO_REQUIRE_ARGC	1

/* Strings for help menu.  */
static char help_string[][128] =
  {"-H, --hostip=ip_address\n\tHP iLO IP address or \"default\" for all servers",
   "-V, --version\n\tprint the version number",
   "-h, --help\n\tprint this help menu"
  };

/* options for arguments.  */
static struct option long_options[] =
  {
    {"hostname",	required_argument,	0,	'H'}, 
    {"version",		no_argument,		0,	'V'},
    {"help",		no_argument,		0,	'h'},
    {0,			0,			0,	  0}
  };

enum
{
  OPT_Y,
  OPT_N,
  OPT_DEFAULT,
  OPT_OTHER,
  OVER_MAX_LOOP 
};

#endif
