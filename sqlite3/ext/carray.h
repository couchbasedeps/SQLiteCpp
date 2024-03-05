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

// Adapted from carray.h in the SQLite source tree.
// Full docs: https://www.sqlite.org/carray.html
// clang-format off

/*
** 2020-11-17
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
**
** Interface definitions for the CARRAY table-valued function
** extension.
**
** "carray" is a table-valued-function that returns the values in a C-language array.
** Examples:
**
**      SELECT * FROM carray($ptr,5)
**
** The query above returns 5 integers contained in a C-language array
** at the address $ptr.  $ptr is a pointer to the array of integers.
** The pointer value must be assigned to $ptr using the
** sqlite3_bind_pointer() interface with a pointer type of "carray".
** For example:
**
**    static int aX[] = { 53, 9, 17, 2231, 4, 99 };
**    int i = sqlite3_bind_parameter_index(pStmt, "$ptr");
**    sqlite3_bind_pointer(pStmt, i, aX, "carray", 0);
**
** There is an optional third parameter to determine the datatype of
** the C-language array.  Allowed values of the third parameter are
** 'int32', 'int64', 'double', 'char*', 'struct iovec'.  Example:
**
**      SELECT * FROM carray($ptr,10,'char*');
**
** The default value of the third parameter is 'int32'.
**
*/

#ifndef _CARRAY_H
#define _CARRAY_H

#include "sqlite3.h"              /* Required for error code definitions */
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct sqlite3_iovec {
    const void * base;
    size_t len;
} sqlite3_iovec;


/* Use this interface to bind an array to the single-argument version
** of CARRAY().
*/
SQLITE_API int sqlite3_carray_bind(
  sqlite3_stmt *pStmt,        /* Statement to be bound */
  int i,                      /* Parameter index */
  void *aData,                /* Pointer to array data */
  int nData,                  /* Number of data elements */
  int mFlags,                 /* CARRAY flags */
  void (*xDel)(void*)         /* Destructgor for aData*/
);

/* Allowed values for the mFlags parameter to sqlite3_carray_bind().
*/
#define CARRAY_INT32     0    /* Data is 32-bit signed integers */
#define CARRAY_INT64     1    /* Data is 64-bit signed integers */
#define CARRAY_DOUBLE    2    /* Data is doubles */
#define CARRAY_TEXT      3    /* Data is char* */
#define CARRAY_BLOB      4    /* Data is struct sqlite3_iovec */
#define CARRAY_TEXT_LEN  5    /* Data is struct sqlite3_iovec, but type is TEXT not BLOB */

/* Initializes the extension, if linked statically. */
SQLITE_API int sqlite3_carray_init(sqlite3 *db,
                                   char **pzErrMsg,
                                   const sqlite3_api_routines *pApi);

#ifdef __cplusplus
}  /* end of the 'extern "C"' block */
#endif

#endif /* ifndef _CARRAY_H */
