/* nagios_hpilo_snmp -- The base code for Linux to communicate with the SNMP
                        agent of the iLO via the SNMP APIs. 
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

#include "nagios_hpilo_snmp.h"
extern struct inst_list *list;
/* Return TRUE if the 'str' contains the hexadecimal character.  */

static int 
is_hex_str (char *str, int value_len)
{
  int i;
  int hex_format = FALSE;

  for (i = 0; i < value_len; i++) 
    {
      if (!isprint(str[i]) && !isspace(str[i])) 
        {
	  hex_format = TRUE;
	  break;
	}
    }

  return hex_format;
}

/* Print the string in the hexadecimal format.  */

static void 
print_hex_str (char *str, int value_len)
{
  int i;

  for (i = 0; i < value_len; i++)
    printf("%02X ", (unsigned char) str[i]);

  printf("\n");
}

/* Copy the string in the hexadecimal format.  */

static void 
copy_hex_str (char *str, int value_len, char *hex_buf, int len)
{
  int i, hex_len = len/3;

  for (i = 0; i < value_len && i < hex_len; i++)
   {
    sprintf(hex_buf,"%02X ", (unsigned char) str[i]);
    hex_buf = hex_buf + 3;
   }
}

/* Retrieve the variable list from the netsnmp component. Next, the retrieved 
   list will be placed in ilo_oid_list in order to process each valid OID.  */

static int 
get_netsnmp_var (struct ilo_oid_list *dest, netsnmp_variable_list *src)
{
  int ret = NAGIOS_ILO_SUCCESS_STATUS;

  switch (src->type) 
    {
    case ASN_INTEGER:
      dest->integer = *src->val.integer;
      break;
    case ASN_OCTET_STR:

      /* Make sure the length is identical */
      if (src->val_len != dest->value_len)
	dest->value_len = src->val_len;

      dest->string = (char *) malloc(dest->value_len + 1);
      
      if (dest->string == NULL) 
	ret = NAGIOS_ILO_FAIL_STATUS;

      memcpy(dest->string, src->val.string, dest->value_len);
      dest->string[dest->value_len] = '\0';

      break;
    case ASN_COUNTER:
	dest->integer = (int) &src->val.counter64;
	
      break;
    case ASN_GAUGE:
	dest->integer = (int) &src->val.counter64;

      break;
    case ASN_IPADDRESS:
      asprintf(&dest->string, "%d.%d.%d.%d", src->val.string[0],
					     src->val.string[1],
					     src->val.string[2],
	  				     src->val.string[3]);
	  
      break;
    default:
      ret = NAGIOS_ILO_FAIL_STATUS;
  }

  return ret;
}

/* Copy the oid information from oid_src to oid_dest.  */

static void 
oid_copy (oid *oid_dest, size_t *dest_len, oid *oid_src, size_t src_len)
{
  int	i;

  for (i = 0; i < src_len; i++) {
    oid_dest[i] = oid_src[i];
  }

  *dest_len = src_len;
}

/* Allocate an OID list and copy the data of netsnmp variable list to 
   the allocated OID list.  */

static struct ilo_oid_list *
alloc_oid_list (netsnmp_variable_list *var_src, char **error_ptr)
{
  struct ilo_oid_list *oid_list = NULL;

  oid_list = (struct ilo_oid_list *) malloc(sizeof(*oid_list));

  if (oid_list != NULL) 
    {
      memset(oid_list, 0, sizeof(struct ilo_oid_list));

      oid_list->type = var_src->type;
      oid_list->value_len = var_src->val_len;
      oid_list->index = var_src->index;

      if(get_netsnmp_var(oid_list, var_src) != NAGIOS_ILO_SUCCESS_STATUS) 
	{
	  ILO_ERR_DEBUG(error_ptr, "Unknown type '%d'\n", var_src->type);
	  return NULL;
	}

      oid_list->name = (oid *) malloc(MAX_OID_LEN);

      if (oid_list->name == NULL) 
	{
	  ILO_ERR_DEBUG(error_ptr, "malloc failed for the 'name' "
				    "member of the oid list\n");
	  return NULL;
	}

      oid_copy(oid_list->name, &oid_list->name_len, var_src->name, 
	       var_src->name_length);
      oid_list->next = NULL;
    } 
  else 
    {
      ILO_ERR_DEBUG(error_ptr, "malloc failed for entry of the oid list\n");
    }

  return oid_list;
}

/* Add an OID list to the tail of the head.  */

static void 
oid_list_add (struct ilo_oid_list **head, struct ilo_oid_list *oid_list)
{
  struct ilo_oid_list *tmp;

  if (*head == NULL) 
    {
      /* No entry is in the head, so the oid_list is the head.  */
       *head = oid_list;
    } 
  else 
    {
      /* Add the oid_list to the tail of the head.  */
      tmp = *head;

      while (tmp->next != NULL)
	tmp = tmp->next;
	
      tmp->next = oid_list;
    }

  return ;
}

/* Print the value of each OID.  */

static void 
print_oid (struct ilo_oid_list *oid_list)
{
  int oid_num = 0;

  while (oid_list != NULL) 
    {
      switch (oid_list->type) 
	{
	case ASN_INTEGER:
	  printf("%d\n", oid_list->integer);
	  break;
	case ASN_OCTET_STR:
		
	  if (is_hex_str(oid_list->string, oid_list->value_len) == TRUE) 
	    print_hex_str(oid_list->string, oid_list->value_len);
	  else
	    printf("%s\n", oid_list->string);

	  break;
	case ASN_IPADDRESS:
	  printf("%s\n", oid_list->string);
	  break;
	default:
	  printf("[%s] no such type: %d\n", __func__, oid_list->type);
	}
	oid_list = oid_list->next;
    }
}

/* Initialize the options data structure.  */

static void 
init_options (struct ilo_snmp_options *options)
{
  options->host = NULL;
  options->community = NULL;

  options->port = NAGIOS_ILO_SNMP_PORT;
  options->version = SNMP_VERSION_2c;
  options->timeout = NAGIOS_ILO_SNMP_TIMEOUT;
  options->retries = NAGIOS_ILO_SNMP_RETRIES;
  options->oid_idx = 0;

  return ;
}

/* Free the allocated the elements of the OID list.  */
void 
free_oid_list (struct ilo_oid_list *oid_list)
{
  struct ilo_oid_list *prev;

  while (oid_list) 
    {
      prev = oid_list;

      free(oid_list->name);
      free(oid_list->string);
	  
      oid_list = oid_list->next;

      free(prev);
    }

  return ;
}

/* Initialize struct ilo_snmp_priv the data structure.  */

void 
init_priv_data (struct ilo_snmp_priv *priv_ptr)
{

  priv_ptr->session = NULL;
  priv_ptr->err_str = NULL;

  priv_ptr->oid_list = NULL;

  init_options(&priv_ptr->options);

  return ;
}

/* Establish a SNMP session with the SNMP agent of HP iLO by invoking 
   a SNMP API.  */

int init_ilo_snmp_session(struct ilo_snmp_priv * priv_ptr)
{
  int ret = NAGIOS_ILO_SUCCESS_STATUS;
  netsnmp_session session;

  init_snmp("Nagios_hpilo_snmp");

  snmp_sess_init(&session);
  session.peername = strdup(priv_ptr->options.host);
  session.community = strdup(priv_ptr->options.community);
  session.community_len = strlen((char *) session.community);
  session.version = priv_ptr->options.version;
  session.timeout = priv_ptr->options.timeout;
  session.retries = priv_ptr->options.retries;

  priv_ptr->session = snmp_open(&session);

  if (priv_ptr->session == NULL) 
    {
      snmp_error(&session, &session.s_errno, &session.s_snmp_errno,
		 &priv_ptr->err_str);
      ret = NAGIOS_ILO_FAIL_STATUS;
    }

  return ret;
}

/* Get the netsnmp variable list by establishing a SNMP session with the 
   SNMP agent of HP iLO. Once the list is retrieved, the list will be copied 
   to the ilo_oid_list. Note that the netsnmp variable list will not be 
   available after this function returns. That's why we need to copy the 
   variable list to the private data structure (ilo_oid_list).  */

int 
get_ilo_oid_list (struct snmp_session *session, oid *target_oid, 
		  size_t target_oid_len, char **error_ptr)
{
  int status;
  int running = TRUE;
  oid cur_oid[MAX_OID_LEN];
  size_t cur_oid_len;

  struct snmp_pdu *pdu_ptr, *response_ptr;
  netsnmp_variable_list	*var_list,*var_list_prev=NULL;
  struct ilo_oid_list *oid_list;

  struct ilo_snmp_priv *priv_ptr = container_of(error_ptr, 
						struct ilo_snmp_priv, err_str);

  if (priv_ptr == NULL) 
    {
      ILO_ERR_DEBUG(error_ptr, "priv_ptr is NULL!\n");
      return NAGIOS_ILO_FAIL_STATUS;
    }

  oid_copy(cur_oid, &cur_oid_len, target_oid, target_oid_len);

  while (running) 
    {
      pdu_ptr = snmp_pdu_create(SNMP_MSG_GETBULK);

      /* Max-repetitions: Tell get-bulk to attemp up to 'errindex' 
	   get-next operations to get the remaining objects.  */
      pdu_ptr->errindex = SNMP_GETBULK_ERRINDEX;

      /* Non-repeater.  */
      pdu_ptr->errstat = 0;

      snmp_add_null_var(pdu_ptr, cur_oid, cur_oid_len);

      status = snmp_synch_response(session, pdu_ptr, &response_ptr);
 
      if (status == STAT_SUCCESS && response_ptr->errstat == SNMP_ERR_NOERROR) 
	{

	  var_list = var_list_prev =response_ptr->variables;
		
	  /* Add each element of the netsnmp variable list to 
	     struct ilo_oid_list.  */
	  while (var_list) 
	    {
	      if (netsnmp_oid_is_subtree(target_oid, 
	  				 target_oid_len,
					 var_list->name, 
					 var_list->name_length) != 0) 
		{
		  running = FALSE;
		  break;
		}
		
	      oid_list = alloc_oid_list(var_list, error_ptr);
			  
	      if (oid_list == NULL) 
		{
		  ILO_ERR_DEBUG(error_ptr, "oid_list is NULL!\n");
		  return NAGIOS_ILO_FAIL_STATUS;
		}

	      oid_list_add(&priv_ptr->oid_list, oid_list);
	      var_list_prev = var_list;
	      var_list = var_list->next_variable;
	    }
	    var_list=var_list_prev;

	  if ((var_list->type == SNMP_ENDOFMIBVIEW) ||
	      (var_list->type == SNMP_NOSUCHOBJECT) ||
	      (var_list->type == SNMP_NOSUCHINSTANCE)) 
	    {
	      running = FALSE;
	    } 
	  else 
	    {
		oid_copy(cur_oid, &cur_oid_len, var_list->name,var_list->name_length);
	    }
	} 
      else 
	{ /* Cannot get the response */
	  if (status == STAT_SUCCESS) 
	    {
	      ILO_ERR_DEBUG(error_ptr, "Cannot get the response: %s\n",
			    snmp_errstring(response_ptr->errstat));
	    } 
	  else 
	    {
	      snmp_error(session, &session->s_errno, &session->s_snmp_errno, 
		 	 error_ptr);
	    }

	   return NAGIOS_ILO_FAIL_STATUS;

	}
	  
      if (response_ptr)
	snmp_free_pdu(response_ptr);	

    }

  return NAGIOS_ILO_SUCCESS_STATUS;
}

/* Free the allocated private data structure (struct ilo_snmp_priv *).  */ 

void 
free_priv_data (struct ilo_snmp_priv * priv_ptr)
{
	
  free_oid_list(priv_ptr->oid_list);

  priv_ptr->oid_list = NULL;

  if (priv_ptr->err_str)
    free(priv_ptr->err_str);

  return ;
}


/* Get the service detailed information. */
service_details_print_oid_info (struct ilo_oid_list **oid_list_ptr, int *oid) {
	struct ilo_oid_list *oid_list = *oid_list_ptr;
	struct inst *instance,*tmp = NULL;
	char **table_ele;
	int tbl_ele, inst_id, inst_last = -1;
	int count = 0, newinst = 0, inst_store[512] = {0};
	char i, str_len = 0;
	char buf[1024] = {0}, *oid_str, *json_c = NULL, pBuf[128] = {0};

	switch (*oid) {
		case 1:	
			table_ele = &ps_tbl_ele;
			break;
		case 2: 
			table_ele = &fan_tbl_ele;
			break;
		case 3: 
			table_ele = &temp_tbl_ele;
			break;
		case 4: 
			table_ele = &st_tbl_ele;
			break;
		case 5: 
			table_ele = &mem_tbl_ele;
			break;
		case 6: 
			table_ele = &pro_tbl_ele;
			break;
		case 7: 
			table_ele = &net_tbl_ele;
			break;
		default:
			break;
	}

  	while (oid_list != NULL ) {
		switch (*oid) {
		case 4: 
			inst_id = oid_list->name[oid_list->name_len-4];
			tbl_ele = oid_list->name[oid_list->name_len-2];
			break;
		case 5: 
		case 6: 
		case 7: 
			inst_id = oid_list->name[oid_list->name_len-1];
			tbl_ele = oid_list->name[oid_list->name_len-2];
			break;
		default: 
			inst_id = oid_list->name[oid_list->name_len-1];
			tbl_ele = oid_list->name[oid_list->name_len-3];
			break;
		}
		/* Create instance */
		if ( inst_last < inst_id ) {
			instance = (struct inst *) malloc(sizeof (struct inst *));

			/* Create object for each instance */
			/* Store each inst_id in inst_store[] and record the last inst_id for inst_last */
			if (instance != NULL)
			{
				inst_store[list->inst_count] = inst_id;
				list->inst_count++;
				inst_last = inst_id;
				instance->json_doc = (char *) malloc(sizeof(char)*1024);
				sprintf(buf, "\"%d\": \{", inst_id);
				strncpy(instance->json_doc,buf,strlen(buf));
				instance->bytes_cons = strlen(buf);
				instance->next=NULL;
				newinst=1;

				if(list->obj == NULL)
				{
					list->obj = instance;
				} else
				{
					tmp = list->obj;
					while(tmp->next != NULL)
						tmp = tmp->next;
					tmp->next = instance;	
				}
			}
		}
		/* Add object into each instance*/ 
		else
		{
			newinst = 0;
			tmp = list->obj;
			count = 0;
			/* Match instance by inst_id*/
			while(inst_store[count] != inst_id)
			{
				tmp = tmp->next;
				count++;
			}
			instance = tmp;
			tmp = NULL;
		}
		/* Store each instances OID information in respective objects */
		if (instance != NULL  && instance->json_doc != NULL){
			switch (oid_list->type) 
			{
			case ASN_INTEGER:
	 			sprintf(buf, "\"%s\":%d",table_ele[tbl_ele-1],oid_list->integer);
	  			break;
			case ASN_OCTET_STR:
	  			if (is_hex_str(oid_list->string, oid_list->value_len) == TRUE) {
					 copy_hex_str(oid_list->string, oid_list->value_len, pBuf, 128);
					 sprintf(buf, "\"%s\":\"%s\"", table_ele[tbl_ele-1], pBuf);
				}
	  			else { 
					oid_str = oid_list->string;
					if ( *oid_str == ' ')
					{
						sprintf (oid_str, "%s", "");
						sprintf(buf, "\"%s\":\"%s\"", table_ele[tbl_ele-1], oid_str);
					}
					else {
						sprintf(buf, "\"%s\":\"%s\"", table_ele[tbl_ele-1], oid_list->string);
					}
				}
				break;
			case ASN_IPADDRESS:
				sprintf(buf, "\"%s\":\"%s\"", table_ele[tbl_ele-1], oid_list->string);
				break;
			case ASN_COUNTER:
	 			sprintf(buf, "\"%s\":%d",table_ele[tbl_ele-1],oid_list->integer);
				break;
			case ASN_GAUGE:
	 			sprintf(buf, "\"%s\":%d",table_ele[tbl_ele-1],oid_list->integer);
				break;
			default:
				sprintf(buf, "\"%d\":\"%s\"",tbl_ele,"null");
				break;
			}
			if(!newinst)
			{
				json_c = &(instance->json_doc[instance->bytes_cons]);
				strcpy(json_c,",");
				instance->bytes_cons++;
			}
			json_c = &(instance->json_doc[instance->bytes_cons]);
			strncpy(json_c,buf,strlen(buf));
			instance->bytes_cons = instance->bytes_cons+strlen(buf);
		}

		instance = tmp = NULL;
		memset(buf,0,sizeof(buf));
		oid_list = oid_list->next;
	}
	tmp = list->obj;
	while(tmp != NULL){
		json_c = &(tmp->json_doc[tmp->bytes_cons]);
		strcpy(json_c,"}");
		tmp->bytes_cons++;
		tmp = tmp->next;
	}
}

int
print_oid_info (int status, struct ilo_oid_list **oid_list_ptr)
{

  //if (status != NAGIOS_CRITICAL) 
  if (status != NAGIOS_ILO_FAIL_STATUS) 
    {
      print_oid(*oid_list_ptr);
    } 
  else 
    {
      struct ilo_snmp_priv * priv_ptr = NULL;

      priv_ptr = container_of(oid_list_ptr, struct ilo_snmp_priv, oid_list);

		if (priv_ptr != NULL)  
			if (priv_ptr->err_str)
			{
				if(strcmp(priv_ptr->err_str,"Timeout") == 0)
				{
					status = NAGIOS_UNKNOWN;
					printf("%s - Probable Cause might be SNMP Community string is incorrect or not configured.\n", priv_ptr->err_str);
				}
				else
				{	
					printf("%s\n", priv_ptr->err_str);
				}
			}
	}

  return status;
}
