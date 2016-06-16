/***************************************************************************/
/*(C) Copyright [2016] Hewlett Packard Enterprise Development Company, L.P.*/

#define CURL_STATICLIB

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include "config.h"
#include "ilo-credit-store.h"
#include "credit_save.h"
#include "base64.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME    "credit_save"
#define PROGRAM_VERSION VERSION 
#define MAX_RETRY_TIMES 3

#define ECHOFLAGS (ECHO | ECHOE | ECHOK | ECHONL)

// If option=0, set display mode close
int set_disp_mode(int fd,int option)
{
   int err;
   struct termios term;
   if(tcgetattr(fd,&term)==-1){
     perror("Cannot get the attribution of the terminal");
     return 1;
   }
   if(option)
        term.c_lflag|=ECHOFLAGS;
   else
        term.c_lflag &=~ECHOFLAGS;
   err=tcsetattr(fd,TCSAFLUSH,&term);
   if(err==-1 && err==EINTR){
        perror("Cannot set the attribution of the terminal");
        return 1;
   }
   return 0;
}

int getpasswd(char* passwd, int size)
{
   int c;
   int n = 0;
   if (!passwd) 
     return 1;

   do{
      c=getchar();
      if (c != '\n'|c!='\r'){
         passwd[n++] = c;
      }
   }while(c != '\n' && c !='\r' && n < (size - 1));
   passwd[n] = '\0';
   return n;
}

static void
print_version (void)
{
  printf("%s version - %s\n", PROGRAM_NAME, PROGRAM_VERSION);
  return ;
}

/* Print the usage information */

static void
usage (void)
{
  struct option *opt_ptr = NULL;

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
print_help (void)
{
  int   i, j;

  usage();

  printf("\n");

  for (i=0;i<sizeof(help_string)/sizeof(help_string[0]);i++)
    {
      printf("%s\n", help_string[i]);
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

int
process_options (int argc, char **argv, char **host,
                 int *option_inst)
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
          *host = optarg;
          num_argc_proceed++;
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

int check_y_n(char *con_char, int loop) {
    char buf;
    buf = getchar();
    setbuf(stdin,NULL);
    //if (strcmp(buf,"y") == 0) {
    if (buf == 'y') {
      //printf(" %s will be updated.\n",host);
      return OPT_Y;
    }
    //else if (strcmp(con_char,"n") == 0) {
    else if (buf == 'n') {
      //printf(" %s does not change.\n",host);
      return OPT_N;
    } else if (buf == '\n') {
      return OPT_DEFAULT;
    } else if (loop) {
      printf("ERROR: You must type 'y' or 'n'.\n");
      //printf("Do you wish to update or not(y/n) (Blank is n): ");
      return OPT_OTHER;
    } else
      return OVER_MAX_LOOP;
}

int main(int argc, char **argv) {
  int option_instruction = OPTION_NOP;
  int loop=MAX_RETRY_TIMES;
  char *host,*p,*p1,auth_pass[50],priv_pass[50],name[50],con_char[10];
  char auth_prot[4]="SHA";
  char priv_prot[4]="AES";
  char * delimiter=",";
  if (process_options(argc, argv, &host, &option_instruction)
                      != NAGIOS_ILO_SUCCESS_STATUS)
    {
      usage();
      exit(NAGIOS_UNKNOWN);
    }

  if (option_instruction != NAGIOS_OPTION_NOP)
    check_option_instruction(option_instruction);

  if (exist_cred(host) == 0) {
    printf(" %s is exists. Do you wish to update or not(y/n) (Blank is n): ",host);
    while (loop--) {
      int ret=check_y_n(con_char,loop);
      if (ret == OPT_Y) { 
        printf(" %s will be updated.\n",host);
        break;
      } else if (ret == OPT_N || ret == OPT_DEFAULT) {
        printf(" %s does not change.\n",host);
        return 0;
      } else if (ret == OPT_OTHER)
	printf("Do you wish to update or not(y/n) (Blank is y): ");
      else if (ret == OVER_MAX_LOOP)
        return 1;
    }

  }

  loop=MAX_RETRY_TIMES;
  do {
    int ret;
    printf("Enter iLO SNMPv3 user Security Name for %s: ", host);
    scanf("%s",name);
    getchar();
  
    while (1) {
      printf("Enter iLO SNMPv3 user Authentication Protocol (MD5/SHA) (Blank is SHA): ");
      char buf[1024]={};
      buf[0] = getchar();
      if (buf[0] != '\n') {
	scanf("%s",buf+1);
        getchar();
        if (strcmp(buf,"MD5") == 0 || strcmp(buf,"SHA") == 0) {
          strcpy(auth_prot,buf);
          break;
        }
      } else {
        break;
      }
      printf("\n\tError: Please enter \"MD5\" or \"SHA\".\n");
    }
 
    while (1) {
      char buf[1024]={};
      printf("Enter iLO SNMPv3 user Authentication Passphrase: ");
      scanf("%s",buf);
      getchar();
      if (strlen(buf) > 7 && strlen(buf) < 50) {
        strcpy(auth_pass,buf);
        break;
      } else
        printf("\n\tError: Authentication Passphrase must contain 8 to 49 characters.\n");
    }
  
    while (1) {
      printf("Enter iLO SNMPv3 user Privacy Protocol (DES/AES) (Blank is AES): ");
      char buf[1024]={};
      buf[0] = getchar();
      if (buf[0] != '\n') {
        scanf("%s",buf+1);
        getchar();
        if (strcmp(buf,"DES") == 0 || strcmp(buf,"AES") == 0) {
          strcpy(priv_prot,buf);
          break;
        }
      } else {
        break;
      }
      printf("\n\tError: Please enter \"DES\" or \"AES\"\n");
    }

    while (1) {
      printf("Enter iLO SNMPv3 user Privacy Passphrase (Blank is the same as Authentication Passphrase): ");
      char buf[1024]={};
      buf[0] = getchar();
      if (buf[0] != '\n') {
        scanf("%s",buf+1);
        getchar();
        if (strlen(buf) > 7 && strlen(buf) < 50) {
          strcpy(priv_pass,buf);
          break;
        } else
          printf("\n\tError: Privacy Passphrase must contain 8 to 49 characters.\n");
      } else {
        strcpy(priv_pass,auth_pass);
        break;
      }
    }
  
    printf("\t\t*********************************************\n");
    printf("\t\t iLO SNMPv3 user for %s\n", host);
    printf("\t\t Security Name\t\t\t :%s\n", name);
    printf("\t\t Authentication Protocol\t :%s\n", auth_prot);
    printf("\t\t Authentication Passphrase\t :%s\n",auth_pass);
    printf("\t\t Privacy Protocol\t\t :%s\n", priv_prot);
    printf("\t\t Privacy Passphrase\t\t :%s\n", priv_pass);
    printf("\t\t*********************************************\n");
    printf("\nDo you wish to save above configured data(y/n) (Blank is y):");
 
    int loop1=MAX_RETRY_TIMES;
    while (loop1--) {
      int ret=check_y_n(con_char,loop);
      if (ret == OPT_Y || ret == OPT_DEFAULT) {
        printf("\n %s will be saved.\n",host);
	char *cred = (char *)malloc(sizeof(char)*
                     (strlen(name)+strlen(auth_prot)+strlen(auth_pass)
                     +strlen(priv_prot)+strlen(priv_pass)+6));
        strcpy(cred, name);
  	strcat(cred, delimiter);
  	strcat(cred, auth_prot);
  	strcat(cred, delimiter);
  	strcat(cred, auth_pass);
  	strcat(cred, delimiter);
  	strcat(cred, priv_prot);
  	strcat(cred, delimiter);
  	strcat(cred, priv_pass);
        //printf("%s\n", cred);
        if (set_cred(host,cred) == 0) {
          printf("iLO Credential succeeds to encrypt and save.\n");
          free(cred);
          return 0;
        } else {
          free(cred);
          return 1;
        }

        /*printf("%s\n",get_cred(host));
	char *value;
	value=get_cred(host);
	char all_cred[1024];
        size_t all_len;
	int r = base64_decode(value, strlen(value), all_cred, &all_len);
        if (r == 0)
	printf("%s,%d\n",all_cred,all_len);
	printf("%c\n",all_cred[all_len]);*/
      } else if (ret == OPT_N) {
        break;
      } else if (ret == OPT_OTHER)
	printf("Do you wish to save(y/n) (Blank is y): ");
      else if (ret==OVER_MAX_LOOP)
        return 1;
    }

  } while (--loop);
  return 1;
}
