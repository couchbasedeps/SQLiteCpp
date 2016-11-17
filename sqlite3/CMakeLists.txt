# CMake file for compiling the sqlite3 static library under Windows (for ease of use)
#
# Copyright (c) 2012-2016 Sebastien Rombauts (sebastien.rombauts@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT) 

# add sources of the "sqlite3" static library
add_definitions(-DSQLITE_ENABLE_FTS4_UNICODE61 -DSQLITE_OMIT_LOAD_EXTENSION -DSQLITE_ENABLE_FTS4)
add_library(sqlite3 SHARED
 sqlite3.c
 sqlite3.h
)

if(MSVC)
    set_target_properties(sqlite3 PROPERTIES LINK_FLAGS
                "/def:${CMAKE_CURRENT_LIST_DIR}/sqlite3.def")	
endif()