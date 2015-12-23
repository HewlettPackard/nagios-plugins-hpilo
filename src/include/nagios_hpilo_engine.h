/* nagios_hpilo_engine.h -- types and prototypes used by nagios iLO engine 
   (C) Copyright [2015] Hewlett-Packard Enterprise Development Company, L.P.

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

/* Written by Adrian Huang <adrian.huang@hp.com>.  */

#ifndef NAGIOS_HPILO_ENGINE_H
#define NAGIOS_HPILO_ENGINE_H

#include "nagios_hpilo_common.h"

/* Macro definition */
#define USAGE	"-H <IP Address> -C <SNMP Community> -o <oid_index> [-h] [-V] [-J]"

/* Available options for this plugin.  */ 
#define SHORT_OPTIONS "hJVC:H:o:"

/* The required the number of arguments for this plugin.  */
#define NAGIOS_ILO_REQUIRE_ARGC	3

#define IDX_OUT_OF_RANGE(idx, array) 	((idx < 0 || idx >= NUM_ELEMENTS(array)) ? TRUE : FALSE)

#define ILO_OID_NAME_LEN	24 

#define MAX_OID_INFO_ENTRIES	2

/* Health group OID.  
   For example: Fans, power supplies and so on.  */
#define HLTH_GROUP_OID	0

/* Health component OID info
   For example: Fan 1, Fan 2.....Fan n
		Sensor 1, Sensor 2.....Sensor n.  */
#define HLTH_COMP_OID	1

/* Strings for help menu.  */
static char help_string[][128] =
  {"-H, --hostname=ip_address\n\tHost name or IP address to connect with",
   "-C, --community=SNMP_COMMINUTY\n\tSNMP comminuty string. "
   "For example: -C public or --community=public",
   "-o, --oid=oid_index\n\tOID index",
   "-J, --json\nprint the oid data in json format",
   "-V, --version\n\tprint the version number",
   "-h, --help\n\tprint this help menu"
  };
   
/* options for arguments.  */
static struct option long_options[] =
  {
    {"hostname",	required_argument,	0,	'H'}, 
    {"community",	required_argument,	0,	'C'},
    {"oid",		required_argument,	0,	'o'},
    {"version",		no_argument,		0,	'V'},
    {"json",		no_argument,		0,	'J'},
    {"help",		no_argument,		0,	'h'},
    {0,			0,			0,	  0}
  };


/* iLO health status.  */
enum 
{
  ILO_HLTH_STATUS_NA,
  ILO_HLTH_STATUS_OTHER,
  ILO_HLTH_STATUS_OK,
  ILO_HLTH_STATUS_DEGRADED,
  ILO_HLTH_STATUS_FAILED,
};

/* iLO Redundant status.  */
enum 
{
  ILO_RED_STATUS_OTHER = 1,
  ILO_RED_STATUS_NOT_RED,
  ILO_RED_STATUS_RED,
};

/* A variety of OID types.  */
enum 
{
  /* The array index of the system overall condition.  */
  ILO_SYS_OVERALL_COND, 

  ILO_HLTH_TYPE,

  ILO_HLTH_STR_TYPE,

  ILO_REDUNDANT_TYPE,

  /* The array index of NIC condition (232.18.1.3:cpqNicMibCondition).  */
  ILO_NIC_MIB_COND = 14,
};

/* Option operation.  */
enum 
{
  NAGIOS_OPTION_NOP,
  NAGIOS_OPTION_PRINT_VERSION,
  NAGIOS_OPTION_PRINT_HELP
};

/* This data structure is used to store the different type associated with the
   different get_status_str_fn_ptr function.   */
struct ilo_get_status_str_entry 
{
  /* The type of different component.  */
  int type;

  /* Function pointer.  */
  enum Nagios_status (*get_status_str_fn_ptr) (struct ilo_oid_list **, void *);
};

/* Generic OID info.  */
struct generic_oid_info
{
  /* OID Name.  */
  char oid_name[ILO_OID_NAME_LEN]; 

  /* OID Number.  */
  oid oid[MAX_OID_LEN];

  /* OID Length.  */
  int oid_len;
};


/* The data structure for iLO OID. */
struct ilo_oid_info
{
  struct generic_oid_info oid_pool[MAX_OID_INFO_ENTRIES];

  /* Function Pointer.  */
  enum Nagios_status (*parse_status_fn_ptr)(void **, int *);

  /* Type of status.  */
  int type;			    
};

/* Callback functions.  */
static enum Nagios_status parse_status(void **, int *);
static enum Nagios_status parse_status_array(void **, int *);
static enum Nagios_status health_status_array(void **, int *);
static enum Nagios_status get_hlth_status_str(struct ilo_oid_list **, void *);
static enum Nagios_status get_redundant_status_str(struct ilo_oid_list **,void *);
static enum Nagios_status get_oid_str(struct ilo_oid_list **,void *);

#endif
