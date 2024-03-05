//
// carray.h
//
// Copyright 2024-Present Couchbase, Inc.
//
// Use of this software is governed by the Business Source License included
// in the file licenses/BSL-Couchbase.txt.  As of the Change Date specified
// in that file, in accordance with the Business Source License, use of this
// software will be governed by the Apache License, Version 2.0, included in
// the file licenses/APL2.txt.
//

// Adapted from carray.c in the SQLite source tree:
// clang-format off

/*
** 2016-06-29
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
*/

#ifdef SQLITECPP_BUILDING_EXTENSION
#include <sqlite3ext.h>
SQLITE_EXTENSION_INIT3
#else
#include <sqlite3.h>
#endif

#include "carray.h"
#include "carray_internal.h"


/*
** Destructor for the carray_bind object
*/
static void carrayBindDel(void *pPtr){
  carray_bind *p = (carray_bind*)pPtr;
  if( p->xDel!=SQLITE_STATIC ){
     p->xDel(p->aData);
  }
  sqlite3_free(p);
}

/*
** Invoke this interface in order to bind to the single-argument
** version of CARRAY().
*/
SQLITE_API int sqlite3_carray_bind(
  sqlite3_stmt *pStmt,
  int idx,
  void *aData,
  int nData,
  int mFlags,
  void (*xDestroy)(void*)
){
  carray_bind *pNew;
  int i;
  pNew = sqlite3_malloc64(sizeof(*pNew));
  if( pNew==0 ){
    if( xDestroy!=SQLITE_STATIC && xDestroy!=SQLITE_TRANSIENT ){
      xDestroy(aData);
    }
    return SQLITE_NOMEM;
  }
  pNew->nData = nData;
  pNew->mFlags = mFlags;
  if( xDestroy==SQLITE_TRANSIENT ){
    sqlite3_int64 sz = nData;
    switch( mFlags & 0x07 ){
      case CARRAY_INT32:   sz *= 4;                     break;
      case CARRAY_INT64:   sz *= 8;                     break;
      case CARRAY_DOUBLE:  sz *= 8;                     break;
      case CARRAY_TEXT:    sz *= sizeof(char*);         break;
      case CARRAY_BLOB:    sz *= sizeof(sqlite3_iovec);  break;
      case CARRAY_TEXT_LEN:sz *= sizeof(sqlite3_iovec);  break;
      default: return SQLITE_MISUSE;
    }
    if( (mFlags & 0x07)==CARRAY_TEXT ){
      for(i=0; i<nData; i++){
        const char *z = ((char**)aData)[i];
        if( z ) sz += strlen(z) + 1;
      }
    }else if( (mFlags & 0x07)==CARRAY_BLOB || (mFlags & 0x07)==CARRAY_TEXT_LEN ){
      for(i=0; i<nData; i++){
        sz += ((sqlite3_iovec*)aData)[i].len;
      }
    } 
    pNew->aData = sqlite3_malloc64( sz );
    if( pNew->aData==0 ){
      sqlite3_free(pNew);
      return SQLITE_NOMEM;
    }
    if( (mFlags & 0x07)==CARRAY_TEXT ){
      char **az = (char**)pNew->aData;
      char *z = (char*)&az[nData];
      for(i=0; i<nData; i++){
        const char *zData = ((char**)aData)[i];
        sqlite3_int64 n;
        if( zData==0 ){
          az[i] = 0;
          continue;
        }
        az[i] = z;
        n = strlen(zData);
        memcpy(z, zData, n+1);
        z += n+1;
      }
    }else if( (mFlags & 0x07)==CARRAY_BLOB || (mFlags & 0x07)==CARRAY_TEXT_LEN ){
      sqlite3_iovec *p = (sqlite3_iovec*)pNew->aData;
      unsigned char *z = (unsigned char*)&p[nData];
      for(i=0; i<nData; i++){
        size_t n = ((sqlite3_iovec*)aData)[i].len;
        p[i].len = n;
        p[i].base = z;
        z += n;
        memcpy((void*)p[i].base, ((sqlite3_iovec*)aData)[i].base, n);
      }
    }else{
      memcpy(pNew->aData, aData, sz);
    }
    pNew->xDel = sqlite3_free;
  }else{
    pNew->aData = aData;
    pNew->xDel = xDestroy;
  }
  return sqlite3_bind_pointer(pStmt, idx, pNew, "carray-bind", carrayBindDel);
}
