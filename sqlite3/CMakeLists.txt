# CMake file for compiling the sqlite3 static library under Windows (for ease of use)
#
# Copyright (c) 2012-2016 Sebastien Rombauts (sebastien.rombauts@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT) 

# add sources of the "sqlite3" static library
add_library(sqlite3
 sqlite3.c
 sqlite3.h
)

target_compile_definitions(sqlite3 PRIVATE 
  -DSQLITE_DEFAULT_WAL_SYNCHRONOUS=1
  -DSQLITE_LIKE_DOESNT_MATCH_BLOBS
  -DSQLITE_OMIT_SHARED_CACHE
  -DSQLITE_OMIT_DECLTYPE
  -DSQLITE_OMIT_DATETIME_FUNCS
  -DSQLITE_ENABLE_EXPLAIN_COMMENTS
  -DSQLITE_DISABLE_FTS3_UNICODE
  -DSQLITE_ENABLE_MEMORY_MANAGEMENT
  -DSQLITE_ENABLE_STAT4
  -DSQLITE_HAVE_ISNAN
  -DHAVE_LOCALTIME_R
  -DHAVE_USLEEP
  -DHAVE_UTIME
  -DSQLITE_OMIT_LOAD_EXTENSION 
  -DSQLITE_ENABLE_FTS4
  -DDSQLITE_ENABLE_FTS3_PARENTHESIS
  -DSQLITE_ENABLE_FTS3_TOKENIZER
)
