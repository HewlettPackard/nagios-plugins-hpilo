/* nagios_hpilo_snmp.h -- types and prototypes used by nagios iLO plug-in 
   (C) Copyright [2015] Hewlett-Packard Development Company, L.P.

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

#ifndef NAGIOS_HPILO_SNMP_H
#define NAGIOS_HPILO_SNMP_H

#include "nagios_hpilo_common.h"

/* Default SNMP port number of HP iLO.  */
#define NAGIOS_ILO_SNMP_PORT	161

/* 3-second timeout.  */
#define NAGIOS_ILO_SNMP_TIMEOUT		(3 * 1000000) 
#define NAGIOS_ILO_SNMP_RETRIES		3


#define SNMP_GETBULK_ERRINDEX	10

/* SNMP version defintion.  */
enum 
{
  SNMP_V1 = 1,
  SNMP_V2C,
  SNMP_V3_NOPRIV,
  SNMP_V3_PRIV
};

#endif
