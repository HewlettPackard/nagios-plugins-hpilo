/* nagios_hpilo_common.h -- types and prototypes used by nagios iLO plug-in 
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

#ifndef NAGIOS_HPILO_COMMON_H
#define NAGIOS_HPILO_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

#define TRUE	1
#define FALSE	0

/* Internal status.  */
#define NAGIOS_ILO_SUCCESS_STATUS	0
#define NAGIOS_ILO_FAIL_STATUS		-1

/* Cast a member of a structure out to the containing structure.  */
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define container_of(ptr, type, member) ({            \
 const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
  (type *)( (char *)__mptr - offsetof(type,member) );})


#define NUM_ELEMENTS(array) \
	(sizeof(array) / sizeof(typeof(array[0])))

/* Return status defined by Nagios.  */
enum Nagios_status
{
  NAGIOS_OK, 
  NAGIOS_WARNING, 
  NAGIOS_CRITICAL,
  NAGIOS_UNKNOWN 
};

/* Error Debug Macro.  */
#define ILO_ERR_DEBUG(ppError, fmt, args...) \
	if (*ppError == NULL) { \
		asprintf(ppError, "[Error]: " fmt, ## args); \
	} else { \
		char	buf[strlen(*ppError) + 1]; \
		sprintf(buf, "%s", *ppError); \
		free(*ppError); \
		asprintf(ppError, "%s[Error]: " fmt, buf, ##args); \
	}

/* Data structure to store services detailed  OID inforamtion retrieved from a specific OID. */
/* Data structure to create object for each instance */
struct inst {
	char *json_doc;		/* 1024 bytes allocation */
	int  bytes_cons;	/*track the pos in json_doc after updating each element */
	struct inst *next;
 };

/* Data structure to create instances */
struct inst_list {
	int inst_count; /*number of instances*/
	struct inst *obj; /*Instance List*/
}*glist;

/* Data structure to store OID information retrieved from a specific OID.  */
struct ilo_oid_list 
{
  /* The OID name.  */
  oid *name;		

  /* Length of a specific OID.  */
  size_t name_len;	

  /* Length of the content for an OID.  */
  size_t value_len;	

  int index;

  /* Type of a specific OID.  */
  u_char type;		

  /* The content of the specific OID if it is a string OID.  */
  char *string;	

  /* The content of a integer OID.  */
  int integer;	

  /* Pointer to the next entry.  */
  struct ilo_oid_list *next;
};

/* Data structure to store options via the argument list of the command 
   line.  */
struct ilo_snmp_options 
{
  /* Pointer to host name (IP address).  */
  char *host;	

  /* Pointer to communuity.  */
  char *community;	

  /* Port number of the SNMP agent.  */
  int port;		

  /* SNMP version. */
  int version;	

  /* SNMP timeout (microsecond).  */
  int timeout;	

  /* SNMP retries.  */ 
  int retries;	

  /* Index to the iLO OID table.  */
  int oid_idx;	
};

/* Private data for this plugin.  */
struct ilo_snmp_priv 
{
  /* Options information.  */
  struct ilo_snmp_options options;	

  /* Pointer to a SNMP session.	 */
  netsnmp_session *session;	

  /* Pointer to an error string.  */	
  char *err_str;	

  /* Pointer to an OID list.  */
  struct ilo_oid_list	*oid_list, *oid_list_info;		

  
};

#endif
