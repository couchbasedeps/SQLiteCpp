/**
 * @file    Exception.cpp
 * @ingroup SQLiteCpp
 * @brief   Encapsulation of the error message from SQLite3 on a std::runtime_error.
 *
 * Copyright (c) 2012-2016 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#include <SQLiteCpp/Exception.h>

#include <sqlite3.h>


namespace SQLite
{

void (*Exception::logger)(const Exception&);

Exception::Exception(const std::string& aErrorMessage, int ret, int extended) :
    std::runtime_error(aErrorMessage),
    mErrcode(ret),
    mExtendedErrcode(extended)
{
    if (logger)
        logger(*this);
}

Exception::Exception(sqlite3* apSQLite) :
    Exception(sqlite3_errmsg(apSQLite),
              sqlite3_errcode(apSQLite),
              sqlite3_extended_errcode(apSQLite))
{
}

Exception::Exception(sqlite3* apSQLite, int ret) :
    Exception(sqlite3_errmsg(apSQLite),
              ret,
              sqlite3_extended_errcode(apSQLite))
{
}

// Return a string, solely based on the error code
const char* Exception::getErrorStr() const noexcept // nothrow
{
    return sqlite3_errstr(mErrcode);
}


}  // namespace SQLite
