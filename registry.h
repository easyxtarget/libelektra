/***************************************************************************
                registry.h  -  Methods for accessing the Linux Registry
                             -------------------
    begin                : Mon Dec 29 2003
    copyright            : (C) 2003 by Avi Alkalay
    email                : avi@unix.sh
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


/* Subversion stuff

$Id$
$LastChangedBy$

*/

#ifndef REGISTRY_H
#define REGISTRY_H

#include <sys/types.h>
#include <errno.h>
#include <stdio.h>

#define RG_KEY_DELIM            '/'


/* When FORMAT_VERSION changes, FORMAT must change also. */
#define RG_KEY_FORMAT_VERSION   2
#define RG_KEY_FORMAT           "RG002"


/****************************************************/
/* Key data types */
#define RG_KEY_TYPE_UNDEFINED 0
#define RG_KEY_TYPE_DIR       1
#define RG_KEY_TYPE_LINK      2
/* this gap is for special key meta types, that can't go into regular files */
#define RG_KEY_TYPE_BINARY    20
/* This gap is for binary data types that have some semantics 
   that somebody can invent in the future */
#define RG_KEY_TYPE_STRING    40
/* Data types bigger than RG_KEY_TYPE_STRING are still strings, but may have
   some semantics that will be handled by other livraries */





/****************************************************/
/* Key name space types */
#define RG_NS_SYSTEM        1  /* The 'system/ *' keys */
#define RG_NS_USER          2  /* The   'user/ *' keys */




/* Numerical types (deprecated) */
#define RG_DWORD  long long int
#define RG_DOUBLE double


/*
The key flags bit array. The '.' indicator means unknown:

7654 3210 7654 3210 7654 3210 7654 3210
...1 0... .0.. .1.. ..1. ..0. ...0 1... 0x10042008 Initialized
...1 1... .1.. .1.. ..1. ..1. ...1 1... 0x18442218 Initialized mask
.... .... .... .... .... .... .... ...1 0x00000001 HASTYPE
.... .... .... .... .... .... .... ..1. 0x00000002 HASKEY
.... .... .... .... .... .... .... .1.. 0x00000004 HASDATA
.... .... .... .... .... .... ..1. .... 0x00000020 HASDOMAIN
.... .... .... .... .... .... .1.. .... 0x00000040 HASCOMMENT
.... .... .... .... .... .... 1... .... 0x00000060 HASUID
.... .... .... .... .... ...1 .... .... 0x00000080 HASGID
.... .... .... .... .... .1.. .... .... 0x00000400 HASPRM
.... .... .... .... .... 1... .... .... 0x00000800 HASTIME
.... .... .... .... ...1 .... .... .... 0x00001000 NEEDSYNC
.... .... .... .... .1.. .... .... .... 0x00004000 ACTIVE ***DEPRECATED***
1... .... .... .... .... .... .... .... 0x80000000 FLAG (general flag)
*/

/* Key flags */
#define RG_KEY_FLAG_INITIALIZED       0x10042008
#define RG_KEY_FLAG_INITMASK          0x18442218

#define RG_KEY_FLAG_HASTYPE           1      /* key has a type set */
#define RG_KEY_FLAG_HASKEY            1<<1   /* key has a name */
#define RG_KEY_FLAG_HASDATA           1<<2   /* key has data */
#define RG_KEY_FLAG_HASDOMAIN         1<<5   /* key has user domain set */
#define RG_KEY_FLAG_HASCOMMENT        1<<6   /* key has comment set */
#define RG_KEY_FLAG_HASUID            1<<7   /* key has UID set */
#define RG_KEY_FLAG_HASGID            1<<8   /* key has GID set */
#define RG_KEY_FLAG_HASPRM            1<<10  /* key has permissions set */
#define RG_KEY_FLAG_HASTIME           1<<11  /* key has time set */
#define RG_KEY_FLAG_NEEDSYNC          1<<12  /* key needs a sync with storage */
#define RG_KEY_FLAG_ACTIVE            1<<14  /* ****deprecated**** */
#define RG_KEY_FLAG_FLAG              1<<31  /* general purpose flag */


/* Return codes generated by Key class' methods */
#define RG_KEY_RET_OK                       0
#define RG_KEY_RET_NULLKEY                  EINVAL
#define RG_KEY_RET_UNINITIALIZED            EINVAL
#define RG_KEY_RET_NOKEY                    ENODATA
#define RG_KEY_RET_NODATA                   ENODATA
#define RG_KEY_RET_NOGROUP                  ENODATA
#define RG_KEY_RET_NODESC                   ENODATA
#define RG_KEY_RET_NODOMAIN                 ENODATA
#define RG_KEY_RET_NOCRED                   EACCES
#define RG_KEY_RET_NOTIME                   ENODATA
#define RG_KEY_RET_TRUNC                    ENOBUFS
#define RG_KEY_RET_TYPEMISMATCH             EILSEQ
#define RG_KEY_RET_INVALIDKEY               EAFNOSUPPORT
#define RG_KEY_RET_NOTFOUND                 ENOENT



/* Options to be ORed in some methods (registryGetChildKeys) */
#define RG_O_RECURSIVE          1      /* act recursively */
#define RG_O_DIR                1<<1   /* Include dirs */
#define RG_O_NOVALUE            1<<2   /* get also nonvalue keys */
#define RG_O_NOEMPTY            1<<3   /* get no empty keys */
#define RG_O_STATONLY           1<<4   /* only stat instead of getting */
#define RG_O_INACTIVE           1<<5   /* get incative keys also */
#define RG_O_SORT               1<<6   /* sort keys */
#define RG_O_NFOLLOWLINK        1<<7   /* dont follow symlinks */

/* XML exporting options for keytoStrem() */
#define RG_O_CONDENSED          1<<8   /* dont show in fancy XML formating */
#define RG_O_NUMBERS            1<<9   /* use UID and GID intead of names */
#define RG_O_XMLHEADERS         1<<10  /* print also XML headers */



typedef struct _Key {
	 u_int8_t      type;        /* data type */
	    uid_t      uid;         /* owner user ID */
	    uid_t      gid;         /* owner group ID */
	   mode_t      access;      /* access control */
	   time_t      atime;       /* time for last access */
	   time_t      mtime;       /* time for last modidification */
	   time_t      ctime;       /* time for last change (meta info) */
	   size_t      commentSize; /* size of the description string */
	   size_t      dataSize;    /* size of the value */
	   size_t      recordSize;  /* dataSize + commentSize + control */
	u_int32_t      flags;       /* Some control flags */
	   char *      key;         /* The name of the key */
	   char *      comment;     /* A comment about this key-value pair */
	   char *      userDomain;  /* User domain */
	   void *      data;        /* The value */
	struct _Key *  next;
} Key;



/* Key Name Anatomy

Key::key is the key name. It is a unique identifier for a registry key.
An exmaple of a complex key name is:

	user:some\.user.My Environment.SOME\.VAR

From this example:
  Root name = "user:some\.user"
      Owner = "some.user"
  Base name = "SOME\.VAR"
Parent name = "user:some\.user.My Environment"

*/





typedef struct _KeySet {
	Key *          start;
	Key *          end;
	Key *          cursor;
	size_t         size;
} KeySet;





/**************************************

Registry methods

***************************************/

/** All registry operations must be done on an opened registry object.
This method initializes a connection to the Registry server */
int registryOpen();


/** Closes the connection to the Registry server, and frees some internal memory */
int registryClose();


int registryGetValue(char *keyname, char *returned,size_t maxSize);
int registryGetKeyByParent(char *parentName, char *baseName, Key *returned);
int registryGetKeyByParentKey(Key *parent, char *baseName, Key *returned);
int registryGetValueByParent(char *parentName, char *baseName, char *returned, size_t maxSize);

int registrySetValue(char *keyname, char *value);
int registrySetValueByParent(char *parentName, char *baseName, char *value);

int registryRemove(char *keyName);
int registryLink(char *oldPath,char *newKeyName);

int registryGetKeyByParent(char *parentName, char *baseName, Key *returned);
int registryGetKeyByParentKey(Key *parent, char *basename, Key *returned);
int registryGetValueByParent(char *parentName, char *baseName, char *returned, size_t maxSize);

// int registryGetLink(char *keyname, char *returned,int maxSize);
// int registrySetLink(char *keyname, char *value);

int registryGetComment(char *keyname, char *returned,size_t maxSize);
size_t registrySetComment(char *keyname, char *comment);

int registryStatKey(Key *key);
int registryGetKey(Key *key);
int registrySetKey(Key *key);

//int registryGetGroupOfKeys(char *root, char *groupName, KeySet *returned);
int registryGetChildKeys(char *parent, KeySet *returned,unsigned long options);
int registryGetRootKeys(KeySet *returned);
/* int registryStatChildKeys(char *parentName, KeySet *returned, int recursive); */

int registrySetKeys(KeySet *ks);





/**************************************

Key methods

***************************************/

int keyInit(Key *key);
int keyClose(Key *key);

int keyIsInitialized(Key *key);
int keyNeedsSync(Key *key);
int keyDup(Key *source,Key *dest);

int keySerialize(Key *key,void *buffer, size_t maxSize);
int keyUnserialize(Key *key,void *buffer);
size_t keyGetSerializedSize(Key *key);

u_int8_t keyGetType(Key *key);
u_int8_t keySetType(Key *key,u_int8_t type);

int keySetFlag(Key *key);
int keyGetFlag(Key *key);

size_t keyGetRecordSize(Key *key);
size_t keyGetNameSize(Key *key);
size_t keyGetFullNameSize(Key *key);

size_t keyGetName(Key *key, char *returnedName, size_t maxSize);
size_t keySetName(Key *key, char *newName);

size_t keyGetFullName(Key *key, char *returnedName, size_t maxSize);
size_t keyGetRootName(Key *key, char *returned, size_t maxSize);
size_t keyGetFullRootName(Key *key, char *returned, size_t maxSize);

size_t keyGetBaseName(Key *key, char *returned, size_t maxSize);
size_t keyNameGetBaseNameSize(char *keyName);
size_t keyGetBaseNameSize(Key *key);

size_t keyNameGetRootNameSize(char *keyName);
size_t keyGetRootNameSize(Key *key);
size_t keyGetFullRootNameSize(Key *key);


size_t keyGetCommentSize(Key *key);
size_t keyGetComment(Key *key, char *returnedDesc, size_t maxSize);
size_t keySetComment(Key *key, char *newDesc);

uid_t keyGetUID(Key *key);
int keySetUID(Key *key, uid_t uid);

gid_t keyGetGID(Key *key);
int keySetGID(Key *key, gid_t gid);

mode_t keyGetAccess(Key *key);
int keySetAccess(Key *key, mode_t mode);

size_t keyGetOwner(Key *key, char *returned, size_t maxSize);
size_t keySetOwner(Key *key, char *userDomain);


size_t keyGetDataSize(Key *key);

// int keyGetInteger(Key *key, RG_DWORD *returnedInt);
// size_t keySetInteger(Key *key, RG_DWORD newInt);
//
// int keyGetDouble(Key *key, RG_DOUBLE *returnedDouble);
// size_t keySetDouble(Key *key, RG_DOUBLE newDouble);

size_t keyGetString(Key *key, char *returnedString, size_t maxSize);
size_t keySetString(Key *key, char *newString);

size_t keyGetBinary(Key *key, void *returnedBinary, size_t maxSize);
size_t keySetBinary(Key *key, void *newBinary, size_t dataSize);

size_t keyGetLink(Key *key, char *returnedTarget, size_t maxSize);
size_t keySetLink(Key *key, char *target);

time_t keyGetMTime(Key *key);
time_t keyGetATime(Key *key);
time_t keyGetCTime(Key *key);

size_t keyGetParentName(Key *key, char *returned, size_t maxSize);

size_t keyToString(Key *key, char *returned, size_t maxSize);

int keyIsSystem(Key *key);
int keyNameIsSystem(char *keyName);

int keyIsUser(Key *key);
int keyNameIsUser(char *keyName);

int keyGetNameSpace(Key *key);
int keyNameGetNameSpace(char *keyName);

int keyIsDir(Key *key);
int keyIsLink(Key *key);

u_int32_t keyCompare(Key *key1, Key *key2);

size_t keyToStream(Key *key, FILE* stream, unsigned long options);


/**************************************

KeySet methods

***************************************/

int ksInit(KeySet *ks);
int ksClose(KeySet *ks);

size_t ksInsert(KeySet *ks, Key *toInsert);
size_t ksAppend(KeySet *ks, Key *toAppend);

size_t ksInsertKeys(KeySet *ks, KeySet *toInsert);
size_t ksAppendKeys(KeySet *ks, KeySet *toAppend);

size_t ksToStream(KeySet *ks, FILE* stream, unsigned long options);
int ksCompare(KeySet *ks1, KeySet *ks2, KeySet *removed);

int ksRewind(KeySet *ks);
Key *ksNext(KeySet *ks);


// Key *ksLookupByName(KeySet *ks,char *keyName);
// Key *ksLookupByRegex(KeySet *ks,regex_t *regex);




/***************************************

Helpers

***************************************/


size_t strblen(char *s);


#endif /* REGISTRY\_H */
