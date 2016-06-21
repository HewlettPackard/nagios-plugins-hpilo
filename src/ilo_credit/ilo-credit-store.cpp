/***************************************************************************/
/*(C) Copyright [2016] Hewlett Packard Enterprise Development Company, L.P.*/

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <tdb.h>

#include "base64.h"
#include "ilo-credit-store.h"

#define PROGRAM_NAME    "ilo-credit-store"
#define PROGRAM_VERSION 1.0
#define TDBDIR	 	"/etc/nagios"
#define TDBFILE 	"/etc/nagios/.snmpv3_credential.tdb"

using namespace std;

TDB_CONTEXT *_tdb;             // Handle on tdb protected by lock

int storeString(const string &key, const string &value)
{
    TDB_DATA k, v;

    k.dptr = (unsigned char *)key.c_str();
    k.dsize = key.length();

    v.dptr = (unsigned char *)value.c_str();
    v.dsize = value.length();

    int result = tdb_store(_tdb, k, v, TDB_REPLACE);

    return result;
}

string fetchString(const string &key)
{
    TDB_DATA k;

    k.dptr = (unsigned char *)key.c_str();
    k.dsize = key.length();

    TDB_DATA v;

    v = tdb_fetch(_tdb, k);

    if (v.dptr == NULL)
        return string();

    string result((char *)v.dptr, v.dsize);

    free(v.dptr);

    return result;
}

bool remove(const string &key)
{
    TDB_DATA k;

    k.dptr = (unsigned char *)key.c_str();
    k.dsize = key.length();

    bool result = (tdb_delete(_tdb, k) == 0);

    return result;
}

bool exists(const string &key)
{
    TDB_DATA k;

    k.dptr = (unsigned char *)key.c_str();
    k.dsize = key.length();

    bool result = tdb_exists(_tdb, k);

    return result;
}

extern "C"
char *
get_cred(char * host)
{
  const char * encode_credential;
  string key, value;
  size_t test;
  int   option_instruction = OPTION_NOP;

  if (host)
    key=host;
  else
    return NULL;

  _tdb = tdb_open_ex(TDBFILE, 0, 0, O_RDONLY, 0600, NULL, NULL);

  if (!_tdb) {
    cout<<"Could not open "<<TDBFILE<<endl;
    return NULL;
  }

  if (exists(key))
    value=fetchString(key);
  else
    value=fetchString("default");

//  cout<<key<<":"<<value<<endl;

//  base64_decode((char *)value.c_str(), (unsigned char **)&encode_credential, &test);
//  value = encode_credential;
//  cout<<value<<endl;

  tdb_close(_tdb); 
  encode_credential=value.c_str();
  return (char *)encode_credential;
}

extern "C"
int 
exist_cred(char * host) {
  string key;
  if (host)
    key=host;
  else
    return 1;

  _tdb = tdb_open_ex(TDBFILE, 0, 0, O_RDWR, 0600, NULL, NULL);

  if (!_tdb) 
    return 1;
  if (exists(key)) {
    tdb_close(_tdb);
    return 0;
  } else {
    tdb_close(_tdb);
    return 1;
  }
}

extern "C"
int 
set_cred(char * host, char * cred) {
  char encode_credential[1024]={0};
  string key = host, value = cred;
  FILE *_textf;
  int   option_instruction = OPTION_NOP, status;
  size_t encode_size=1024;

  if((_textf = fopen(TDBFILE, "r+" )) == NULL) {
    DIR* dir = opendir(TDBDIR);
    if (dir) 
      /* Directory exists. */
      closedir(dir);
    else {
      status = mkdir(TDBDIR, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);        
      if (status != 0) {
        cout<<"Create dir "<<TDBDIR<<" failed!"<<endl;
        return -1;
      }
    }

    _tdb = tdb_open_ex(TDBFILE,
                       0,    // hash_size
                       0,    // tdb_flags
                       O_CREAT|O_RDWR, // open_flags
                       0600,          // mode
                       NULL,   // log_fn
                       NULL);         // hash_fn
    struct passwd *nagios_pw=getpwnam("nagios");
    int nagios_uid=nagios_pw->pw_uid;
    struct group *nagcmd_gr=getgrnam("nagcmd");
    if (!nagcmd_gr) {
      cout<<"Group nagcmd does not exist"<<endl;
      cout<<"Use Group nagios instead, user should chown nagios:nagcmd "
          <<TDBFILE<<", after create Group nagcmd."<<endl;
      nagcmd_gr=getgrnam("nagios");
    }
    int nagios_gid=nagcmd_gr->gr_gid;
    status = chown(TDBFILE,nagios_uid,nagios_gid);
    if (status != 0) {
      cout<<"chown for the tdbfile error"<<endl;
      tdb_close(_tdb);
      return -1;
    }
    status = chmod(TDBFILE, S_IRUSR | S_IWUSR | S_IRGRP);
    if (status != 0) {
      cout<<"chmod for the tdbfile error"<<endl;
      tdb_close(_tdb);
      return -1;
    }
  } else {
    fclose(_textf);
    _tdb = tdb_open_ex(TDBFILE, 0, 0, O_RDWR, 0600, NULL, NULL);
  }

  if (!_tdb) {
    cout<<"Could not create "<<TDBFILE<<endl;
    return -1;
  } else {
    if (exists(key)) {
      if (!remove(key)) {
        cout<<"remove error."<<endl;
        return -2;
      }

    }

    status = base64_encode((const void*)value.c_str(), strlen(value.c_str()), encode_credential, encode_size);
    if (status == 0) {
      value=encode_credential;
      if (storeString(key, value) != 0) {
//         printf("iLO Credential succeeds to encrypt and save.\n");
//         value="";
//         value=fetchString(key);
//         cout<<key<<" "<<value<<endl;
        printf("iLO Credential failed.\n");
        return -1;
      }
    }

    tdb_close(_tdb);
    return 0;
  }
}
