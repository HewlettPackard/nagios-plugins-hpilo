/* nagios_hpilo_engine -- retrieve an OID info from the SNMP agent of HP iLO
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

#include "config.h"
#include "nagios_hpilo_engine.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME	"nagios_hpilo_engine"
#define PROGRAM_VERSION VERSION

struct inst_list *list = NULL;

/* Array of HP iLO OID table. */
static struct ilo_oid_info ilo_oid_table[] = 
{
	{{"System Health Array",	{1,3,6,1,4,1,232,11,2,10,7},	11,	
		 "NULL",		{0},				0},	
	health_status_array,	ILO_SYS_OVERALL_COND},
	{{"System Health",	{1,3,6,1,4,1,232,11,2,10,7},	11,	
		 "NULL",		{0},				0},	
	parse_status_array,	ILO_SYS_OVERALL_COND},
	{{"Fans",		{1,3,6,1,4,1,232,6,2,6,4},	11, 
		 "Fan",		{1,3,6,1,4,1,232,6,2,6,7,1,9},	13},
	parse_status,	ILO_HLTH_TYPE},
	{{"Memory",		{1,3,6,1,4,1,232,6,2,14,4},	11, 
		 "NULL",		{0},				0},
	parse_status,	ILO_HLTH_TYPE},
	{{"Network",		{1,3,6,1,4,1,232,11,2,10,7},	11, 
		 "NULL",		{0},				0},
	parse_status_array, ILO_NIC_MIB_COND},
	{{"Power Supplies",	{1,3,6,1,4,1,232,6,2,9,1},	11, 
		 "Power Supply",	{1,3,6,1,4,1,232,6,2,9,3,1,4},	13}, 
	parse_status,ILO_HLTH_TYPE},
	{{"Processors",	{1,3,6,1,4,1,232,1,2,2,4},	11, 
		 "NULL",		{0},				0},
	parse_status,	ILO_HLTH_TYPE},
	{{"Storage",		{1,3,6,1,4,1,232,3,1,3},	10, 
		 "NULL",		{0},				0},
	parse_status,	ILO_HLTH_TYPE},
	{{"Temperatures",	{1,3,6,1,4,1,232,6,2,6,3},	11, 
		 "Sensor",		{1,3,6,1,4,1,232,6,2,6,8,1,6},	13},
	parse_status,	ILO_HLTH_TYPE},
	{{"Fan Redundancy",	{1,3,6,1,4,1,232,6,2,6,7,1,7},	13, 
		 "NULL",		{0},				0},
	parse_status,	ILO_REDUNDANT_TYPE},
	{{"Power Redundancy",	{1,3,6,1,4,1,232,6,2,9,3,1,9},	13, 
		 "NULL",		{0},				0},
	parse_status,   ILO_REDUNDANT_TYPE},
	{{"Host OS",	{1,3,6,1,4,1,232,11,2,2},	10, 
		 "OS Name",{1,3,6,1,4,1,232,11,2,2,1},		11},
	parse_status,   ILO_HLTH_STR_TYPE},
};
/* Array of HP iLO OID table. */
static struct ilo_oid_info ilo_oid_info_table[] = 
{
	{{"Power Supply Table",	{1,3,6,1,4,1,232,6,2,9,3},	11, 
		"NULL",		{0},		0},
		"NULL",		0},
	{{"Fan Table",		{1,3,6,1,4,1,232,6,2,6,7},	11, 
		"NULL",		{0},		0},
		"NULL",		0},
	{{"Temperature Table",	{1,3,6,1,4,1,232,6,2,6,8},	11, 
		"NULL",		{0},		0},
		"NULL",		0},
	{{"Storage Table",	{1,3,6,1,4,1,232,3,2,2,1},	11, 
		"NULL",		{0},		0},
		"NULL",		0},
	{{"Memory Table",       {1,3,6,1,4,1,232,6,2,14,12},    11,
                "NULL",         {0},            0},
                "NULL",         0},
	{{"Processor Table",    {1,3,6,1,4,1,232,1,2,2,1},      11,
                "NULL",         {0},            0},
                "NULL",         0},
	{{"Network Table",    	{1,3,6,1,4,1,232,18,2,3},      	10,
                "NULL",         {0},            0},
                "NULL",         0},
};
//static struct ilo_oid_info *ilo_oid_table = &ilo_old_info_table;

/* The table for getting the status string.  */
struct ilo_get_status_str_entry get_status_str_tbl[] = {
	{ILO_HLTH_TYPE,	get_hlth_status_str}, 
	{ILO_REDUNDANT_TYPE,	get_redundant_status_str},
	{ILO_HLTH_STR_TYPE,	get_oid_str},
	{0, NULL},
};


static void copy_str (char **dest_str, int dest_len, char *src_str);

/* Print the usage information */

static void 
usage (void) 
{
  struct option	*opt_ptr = NULL;

  printf("Usage: %s ", PROGRAM_NAME);

  for (opt_ptr=long_options;opt_ptr->name != NULL;opt_ptr++)
    {
      if (opt_ptr->has_arg == no_argument)
	  printf("[-%c] ", opt_ptr->val);
      else if (opt_ptr->has_arg == required_argument)
	  printf("-%c <%s> ", opt_ptr->val, opt_ptr->name);
    }

  printf("\n");

  return ;
}

static void
print_version (void)
{
  printf("%s version - %s\n", PROGRAM_NAME, PROGRAM_VERSION);
  return ;
}

static void 
print_help (void)
{
  int	i, j;
 
  usage(); 

  printf("\n");

  for (i=0;i<sizeof(help_string)/sizeof(help_string[0]);i++)
    {
      printf("%s\n", help_string[i]);

      /* Check if the string contains "oid" string.  */
      if (strstr(help_string[i], "oid")) 
        {
	  printf("\n\toid index\tOID info\n");
	  printf("\t------------------------------------\n");

	  for (j = 0; j < NUM_ELEMENTS(ilo_oid_table); j++) 
	    printf("\t%4d\t\t%s\n", j, 
		ilo_oid_table[j].oid_pool[HLTH_GROUP_OID].oid_name);

	  printf("\t------------------------------------\n");
	}
    }

  return ;
}

static void 
check_option_instruction (int instruction)
{
  switch (instruction)
    {
    case NAGIOS_OPTION_PRINT_VERSION:
      print_version();
      break;
    case NAGIOS_OPTION_PRINT_HELP:
      print_help();
      break;
    }

  exit(NAGIOS_UNKNOWN);
}

/* Processe the argument list from the command line.  */

int 
process_options (int argc, char **argv, struct ilo_snmp_options *options,
		 int *option_inst,int *json_flag)
{
  char option;
  int ret = NAGIOS_ILO_SUCCESS_STATUS;
  int num_argc_proceed = 0;
  int option_index = 0;

  /* Ignore the error message reported by unistd library. */
  opterr = 0;

  while((option = getopt_long(argc, argv, SHORT_OPTIONS, long_options, 
				&option_index)) != -1)
    {
      switch(option) 
	{
	case 'H':
	  options->host = optarg;
	  num_argc_proceed++;
	  break;
	case 'C':
	  options->community = optarg;
	  num_argc_proceed++;
	  break;
	case 'o':
	  options->oid_idx = atoi(optarg);
	  num_argc_proceed++;
	  break;
	case 'J':
	  *json_flag  = 1;	
           break;
	case 'V':
	  *option_inst = NAGIOS_OPTION_PRINT_VERSION;	
	  break;
	case 'h':
	  *option_inst = NAGIOS_OPTION_PRINT_HELP;
	  break;
	default:
	  ret = NAGIOS_ILO_FAIL_STATUS;
	}
    }

  if(num_argc_proceed != NAGIOS_ILO_REQUIRE_ARGC && 
					*option_inst == NAGIOS_OPTION_NOP)
    {
      ret = NAGIOS_ILO_FAIL_STATUS;
    }

  return ret;
}
static enum Nagios_status get_oid_str(struct ilo_oid_list **oid_list_ptr, void *data)
{
	enum Nagios_status    n_status = NAGIOS_WARNING;
	struct ilo_oid_list *oid_list = *oid_list_ptr;
	struct ilo_oid_info   *oid_info_ptr = (struct ilo_oid_info *) data;
	char osName_str[128];

	memset(osName_str,0,128);
	while (oid_list != NULL)
	{
		if(snmp_oid_ncompare(oid_list->name,oid_list->name_len,
					oid_info_ptr->oid_pool[HLTH_COMP_OID].oid,oid_info_ptr->oid_pool[HLTH_COMP_OID].oid_len,
					oid_info_ptr->oid_pool[HLTH_COMP_OID].oid_len) == 0 )
		{
			strcpy(osName_str,oid_list->string);
			n_status=NAGIOS_OK;
		}
		oid_list = oid_list->next;

	}

	oid_list = (*oid_list_ptr)->next;
	free_oid_list(oid_list);
	/* Only one element remaining */
	(*oid_list_ptr)->next = NULL;
	if(n_status != NAGIOS_OK)
	{
		sprintf(osName_str,"%s","Host is UNREACHABLE - Probable Cause might be AMS is not installed on the host OS.");
	}

	copy_str(&(*oid_list_ptr)->string, (*oid_list_ptr)->value_len,osName_str);
	(*oid_list_ptr)->value_len = strlen((*oid_list_ptr)->string);
	(*oid_list_ptr)->type = ASN_OCTET_STR;

	return n_status;

}
/* Parse the health_status_array returned by the SNMP agent of iLO.  */
enum Nagios_status health_status_array (void **data_ptr, int *type_ptr)
{
  	struct ilo_snmp_priv *priv_ptr = NULL;
	struct ilo_oid_list *oid_list = (struct ilo_oid_list *) *data_ptr;
  	struct ilo_oid_info  *oid_info_ptr = container_of((int *) type_ptr, 
						struct ilo_oid_info, type);

	char *status_str[] = {"NA", "Other", "OK", "Degraded", "Failed"};

	char status_array[256];

	int statusArrayIndex = 0;

	enum Nagios_status systemStatus = NAGIOS_OK;

	memset(status_array,0,sizeof(status_array));

	for (statusArrayIndex = 0; statusArrayIndex < 15; statusArrayIndex++)
 	{
 		oid_list->integer = oid_list->string[statusArrayIndex];

		if (!statusArrayIndex)
		{
  			switch (oid_list->integer)
    			{
    				case ILO_HLTH_STATUS_NA:
				case ILO_HLTH_STATUS_OTHER:
					systemStatus = NAGIOS_UNKNOWN;
      				break;
    				case ILO_HLTH_STATUS_DEGRADED:
      					systemStatus = NAGIOS_WARNING;
      				break;
    				case ILO_HLTH_STATUS_FAILED:
      					systemStatus = NAGIOS_CRITICAL;
      				break;
    				case ILO_HLTH_STATUS_OK:
      					systemStatus = NAGIOS_OK;
      				break;
				default:
					systemStatus = NAGIOS_UNKNOWN;
					break;
    			}
			sprintf(status_array,"%s",status_str[oid_list->integer]);
		}
		else
 		{
			sprintf(status_array,"%s;%s",status_array,status_str[oid_list->integer]);
 		}
  	}
	copy_str(&(oid_list->string),oid_list->value_len,status_array);

	oid_list->value_len = strlen(oid_list->string);
  	oid_list->type = ASN_OCTET_STR;
  	return systemStatus;
}

/* Parse the status array returned by the SNMP agent of iLO.  */

enum Nagios_status 
parse_status_array (void **data_ptr, int *type_ptr)
{
  struct ilo_snmp_priv *priv_ptr = NULL;
  struct ilo_oid_list *oid_list = (struct ilo_oid_list *) *data_ptr;
  struct ilo_oid_info	*oid_info_ptr = container_of((int *) type_ptr, 
						struct ilo_oid_info, type);

  oid_list->integer = oid_list->string[*type_ptr];

  return get_hlth_status_str((struct ilo_oid_list **) data_ptr, oid_info_ptr);
}

/* Parse the status returned by the SNMP agent of iLO.  */

enum Nagios_status 
parse_status (void **data_ptr, int *type_ptr)
{
  struct ilo_snmp_priv *priv_ptr = NULL;
  struct ilo_oid_list *oid_list = (struct ilo_oid_list *) *data_ptr;
  struct ilo_get_status_str_entry *entry_ptr;
  struct ilo_oid_info	*oid_info_ptr = container_of((int *) type_ptr, 
						struct ilo_oid_info, type);
  
  enum Nagios_status n_status = NAGIOS_UNKNOWN;

  if (oid_list == NULL) 
    {
      if (priv_ptr) 
	{
	  priv_ptr = container_of((struct ilo_oid_list **) data_ptr, 
				   struct ilo_snmp_priv, 
				   oid_list);
	  ILO_ERR_DEBUG(&priv_ptr->err_str, "[%s] OidList is NULL", __func__);
	}
      return NAGIOS_CRITICAL;
    }	

	for (entry_ptr = get_status_str_tbl; entry_ptr->type; entry_ptr++) 
	{
		if (entry_ptr->type == *type_ptr && entry_ptr->get_status_str_fn_ptr)
		{
			n_status = entry_ptr->get_status_str_fn_ptr((struct ilo_oid_list **) data_ptr, oid_info_ptr);
		}
	}

  return n_status;
}

/* Copy string from source to destination.  */

static void 
copy_str (char **dest_str, int dest_len, char *src_str)
{

  /* If the dest_str was allocated before, the sprintf system call will
     be invoked to replace the original data. If not, the asprintf
     system call will be invoked to allocate a memory space for dest_str.  */
  if (*dest_str) 
    {
      if (dest_len >= strlen(src_str)) 
	{
	  sprintf(*dest_str, "%s",  src_str);
	} 
      else 
	{
	  /* Since the length of src_str is larger than dest_str, 
	     the dest_str must be freed to prevent the buffer overflow.  */
	  free(*dest_str);
	  asprintf(dest_str, "%s", src_str);
	}
    } 
  else 
    asprintf(dest_str, "%s", src_str);

  return ;
}

/* Get the failed component for specific group.
  (For example: Fan group, Temperature group or Power Supply group.  */
   
static void
get_failed_component_status(struct ilo_oid_list **oid_list_ptr, 
			    struct ilo_oid_info *oid_info_ptr, 
			    char **str_addr)
{
  struct ilo_snmp_priv	*priv_ptr = NULL;
  struct ilo_oid_list	*oid_list = *oid_list_ptr; 

  struct generic_oid_info *generic_oid_info_ptr;
  char	buf[1024];
  char  *toolong="string is too long.";
  int	slot_num = 1;
		
  priv_ptr = container_of((struct ilo_oid_list **) oid_list_ptr, 
		           struct ilo_snmp_priv, oid_list);

  /* Get the generic OID info. */
  generic_oid_info_ptr = &oid_info_ptr->oid_pool[HLTH_COMP_OID];

  get_ilo_oid_list(priv_ptr->session, generic_oid_info_ptr->oid, 
		   generic_oid_info_ptr->oid_len, &priv_ptr->err_str);

  sprintf(buf, "%s:", (*oid_list_ptr)->string);

  /* Skip the head entry. */
  while ((oid_list = oid_list->next)) 
    {
      if (oid_list->integer != ILO_HLTH_STATUS_OK) {
	if(1024>(strlen(buf)+strlen(toolong)+1)){
		sprintf(buf, "%s [%s %d - %s]", buf, generic_oid_info_ptr->oid_name, 
		slot_num, str_addr[oid_list->integer]); 
	}else{
		sprintf(buf, "%s ", toolong); 
		
	}
      }

      slot_num++;
    }
  
  copy_str(&(*oid_list_ptr)->string, (*oid_list_ptr)->value_len, buf);
  (*oid_list_ptr)->value_len = strlen((*oid_list_ptr)->string);

  /* Free the elements of the OID list in addition to the first 
     element (head).  */
  oid_list = (*oid_list_ptr)->next;
  free_oid_list(oid_list);

  /* Only one element remaining */
  (*oid_list_ptr)->next = NULL;

}

/* Get the health string and return the nagios status.  */

static enum Nagios_status
get_hlth_status_str (struct ilo_oid_list **oid_list_ptr, void *data) 
{
  enum Nagios_status	n_status = NAGIOS_UNKNOWN;
  int			hlth_status; 
  struct ilo_snmp_priv	*priv_ptr = NULL;
  struct ilo_oid_info   *oid_info_ptr = (struct ilo_oid_info *) data;

  char *status_str[] = {"N/A", "Other", "OK", "Degraded", "Failed"};

  hlth_status = (*oid_list_ptr)->integer;

  copy_str(&(*oid_list_ptr)->string, (*oid_list_ptr)->value_len,
	  status_str[hlth_status]);

  (*oid_list_ptr)->value_len = strlen((*oid_list_ptr)->string);
  (*oid_list_ptr)->type = ASN_OCTET_STR;

  switch (hlth_status) 
    {
    case ILO_HLTH_STATUS_NA:
    case ILO_HLTH_STATUS_OTHER:
      n_status = NAGIOS_UNKNOWN;
      break;
    case ILO_HLTH_STATUS_DEGRADED:
      if (oid_info_ptr->oid_pool[HLTH_COMP_OID].oid_len) 
		get_failed_component_status(oid_list_ptr, oid_info_ptr,
					    status_str);
      n_status = NAGIOS_WARNING;
      break;
    case ILO_HLTH_STATUS_FAILED: 
      priv_ptr = container_of((struct ilo_oid_list **) oid_list_ptr, 
			       struct ilo_snmp_priv, oid_list);

      if (oid_info_ptr->oid_pool[HLTH_COMP_OID].oid_len) 
		get_failed_component_status(oid_list_ptr, oid_info_ptr,
					    status_str);
      if(priv_ptr->err_str) 
	{ 
	  copy_str(&priv_ptr->err_str, strlen(priv_ptr->err_str),
		   (*oid_list_ptr)->string);
	} 
      else 
	{
	  asprintf(&priv_ptr->err_str, (*oid_list_ptr)->string);
	}
	  
      n_status = NAGIOS_CRITICAL;
      break;
    case ILO_HLTH_STATUS_OK:
      n_status = NAGIOS_OK;
      break;
    }

  return n_status;
}

/* Get the string of the redundantconfigure and return the nagios status.  */

static enum Nagios_status
get_redundant_status_str (struct ilo_oid_list **oid_list_ptr, void *data) 
{
  int status = ILO_RED_STATUS_OTHER;

  struct ilo_snmp_priv *priv_ptr = NULL;
  struct ilo_oid_list *oid_list = *oid_list_ptr;

  enum Nagios_status n_status = NAGIOS_UNKNOWN;

  char status_str[][16] = {"Other", "Non-Redundant", "Redundant"};

  int i;

  /* Find the redundant status by parsing each component. The paring 
     sequence of the redundant status is:
	ILO_RED_STATUS_RED --> ILO_RED_STATUS_NOT_RED.  */
  for (i=ILO_RED_STATUS_RED;i>=ILO_RED_STATUS_NOT_RED;i--)
    {	  
      while (oid_list) 
        {

	  /* Since one of all fans is in the redundant/non-redundant 
	     configuration, it's a redundant/non-redundant configuration.  */
          if (oid_list->integer == i) 
	    {
	      status = i;
	      break;
	    }

          oid_list = oid_list->next;

       }

       /* Get the redundant status. Exit the "for" loop.  */
       if (status == i)
	       break;

	/* Try again.  */
	oid_list = *oid_list_ptr; 
    }

  /* Free the elements of the OID list in addition to the first 
     element (head).  */
  oid_list = (*oid_list_ptr)->next;
  free_oid_list(oid_list);

  /* Only one element remaining */
  (*oid_list_ptr)->next = NULL;

  copy_str(&(*oid_list_ptr)->string, (*oid_list_ptr)->value_len, 
	  status_str[status-1]);

  (*oid_list_ptr)->value_len = strlen((*oid_list_ptr)->string);
  (*oid_list_ptr)->type = ASN_OCTET_STR;

  
  switch (status) 
    {
    case ILO_RED_STATUS_OTHER:
    case ILO_RED_STATUS_NOT_RED:
      n_status = NAGIOS_WARNING;
      break;
    case ILO_RED_STATUS_RED:
      n_status = NAGIOS_OK;
      break;
    default: 
      n_status = NAGIOS_UNKNOWN;
    }

  return n_status;
}


int 
main (int argc, char **argv)
{
  struct ilo_snmp_priv	snmp_priv;
  struct ilo_oid_info	*oid_info_ptr = NULL;

  int	func_ret,next=0;
  int	oid_idx;
  int	option_instruction = NAGIOS_OPTION_NOP;
  int   jflag = 0;

  struct inst *instance, *tmp = NULL;

  enum Nagios_status status = NAGIOS_OK;

  init_priv_data(&snmp_priv);

  if (process_options(argc, argv, &snmp_priv.options, &option_instruction, &jflag) 
		      != NAGIOS_ILO_SUCCESS_STATUS) 
    {
      usage();
      exit(NAGIOS_UNKNOWN);
    }

  if (option_instruction != NAGIOS_OPTION_NOP)
    check_option_instruction(option_instruction);

  oid_idx = snmp_priv.options.oid_idx;

/* Verify the json flag status */
   if (!jflag)
   {
	/* Check if the OID index is out of range in the iLO OID table.  */
	if (IDX_OUT_OF_RANGE(oid_idx, ilo_oid_table) == TRUE) 
  	{
    		asprintf(&snmp_priv.err_str, "OID index '%d' is out-of-range "
				   "in iLO OID table. The OID index "
				   "should be 0-%d", 
				   oid_idx, NUM_ELEMENTS(ilo_oid_table)-1);

    		status = NAGIOS_ILO_FAIL_STATUS;
    		goto output_n_cleanup;
  	}
    	oid_info_ptr = &ilo_oid_table[oid_idx];
   }
   else {
        oid_info_ptr = &ilo_oid_info_table[oid_idx-1];
   }

  /* Establish a SNMP session with the SNMP agent of iLO.  */
  if((init_ilo_snmp_session(&snmp_priv)) != NAGIOS_ILO_SUCCESS_STATUS) 
    {
      printf("Error for initializing SNMP session: %s\n", snmp_priv.err_str);
      free(snmp_priv.err_str);
      return NAGIOS_UNKNOWN;
	  
    }

  /* Retrieve the OID information and store in oid_list of 
     struct ilo_snmp_priv.  */
  func_ret = get_ilo_oid_list(snmp_priv.session, 
			      oid_info_ptr->oid_pool[HLTH_GROUP_OID].oid, 
			      oid_info_ptr->oid_pool[HLTH_GROUP_OID].oid_len,
			      &snmp_priv.err_str);

   if(jflag)
   {
      list = (struct inst_list *) malloc(sizeof (struct inst_list *));
      if (list != NULL)
      {
		list->inst_count = 0;
		list->obj = NULL;
      }
      /* Retrive the OID information from oid_list and store 
         in list of struct inst_list. */
      service_details_print_oid_info(&snmp_priv.oid_list, &oid_idx);

      /* Get the OID information in json format */
      if (list->obj != NULL)
      {
		printf("var jData = \'{");
		instance = list->obj;
		while(instance != NULL)
		{
			if(next) {
        			printf(", %s",instance->json_doc);
			}
			else {
				printf(" %s ",instance->json_doc);
			}
        		tmp = instance;
        		instance = instance->next;
        		free(tmp->json_doc);
        		free(tmp);
			next = 1;
   		}
   		printf ("\}\';\n\nvar serviceID = %d;\n", oid_idx);
   		free(list);
      }else {
   		printf ("\n\nvar serviceID = %d;\n", oid_idx);
		printf ("\nvar jData = \"\";\n");
      }
   }
	
   if (func_ret != NAGIOS_ILO_SUCCESS_STATUS) 
    {
      status = NAGIOS_ILO_FAIL_STATUS;
      goto output_n_cleanup;
    }

output_n_cleanup:

  if (status == NAGIOS_OK && !jflag) 
    {
      /* The additional status parsing process is required if the function
         pointer and the retrieved OID list are valid.  */
      if (oid_info_ptr->parse_status_fn_ptr && snmp_priv.oid_list) 
	{
	  status = 
	    oid_info_ptr->parse_status_fn_ptr((void **) &snmp_priv.oid_list,
					       &oid_info_ptr->type);
	}
    }
  if(!jflag)
    status = print_oid_info(status, &snmp_priv.oid_list);

  free_priv_data(&snmp_priv);

  if(snmp_priv.session)
    snmp_close(snmp_priv.session);	

  return status;
}
