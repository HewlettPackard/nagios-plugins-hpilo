/***************************************************************************/
/*(C) Copyright [2016] Hewlett Packard Enterprise Development Company, L.P.*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "ilo-credit-store.h"
#include "csv_import.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME    "csv_import"
#define PROGRAM_VERSION VERSION

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
process_options (int argc, char **argv, char **file,
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
        case 'F':
          *file= optarg;
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

int main(int argc, char **argv) {
  char *filename;
  int option_instruction = OPTION_NOP;
  if (process_options(argc, argv, &filename, &option_instruction)
                     != NAGIOS_ILO_SUCCESS_STATUS) {
    usage();
    exit(NAGIOS_UNKNOWN);
  }

  if (option_instruction != NAGIOS_OPTION_NOP)
    check_option_instruction(option_instruction);

  long pos;
  FILE *fp; 
  char StrLine[1024];             //one line max
  if((fp = fopen(filename,"r")) == NULL)
  { 
     printf("\n\tError! File%s does not exists...", filename); 
     return -1; 
  } 

   int icount=0;
   int isuc=0, ifail=0;
   while (fgets(StrLine,1024,fp)) {
     /*pos=ftell(fp);
     printf("pos is %ld bytes\n", pos);

     printf("%d,%s\n", ++icount,StrLine);*/
     const char s[2] = ",";
     char * delimiter=",";
     char *token;

     /* get the first token */
     token = strtok(StrLine, s);
     //printf( "%s\n", token );
     char value[1024]={0}, key[1024]={0};
     strcpy(value, token);
     
   
     /* walk through other tokens */
     if ( token != NULL ) 
     {
        token = strtok(NULL, s);
        strcpy(key, token);
        //printf( "%s\n", key);
     }

     token = strtok(NULL, s);
     while( token != NULL ) 
     {
        //printf("%d:", ++ic);
        strcat(key, delimiter);
        strcat(key, token);
        //printf( "%s\n", key);
        token = strtok(NULL, s);
     }

     if (set_cred(value,key) == 0)
       ++isuc;
     else
       ++ifail;
     ++icount;
     
   }
   
  fclose(fp);

  printf("Total %d iLO Credential records:\n", icount);
  printf(" %d records succeed to encrypt and save,\n", isuc);
  printf(" %d records fail to encrypt and save.\n", ifail);
  if (ifail > 0)
    return 1;
  else
    return 0;
}
