#pragma once

#include "auto_alloc.h"

#include <cstdarg>
#include <cstring>
#include <dirent.h>
#include <stdio.h>
#include <thread>
#include <unistd.h>

using std::this_thread::sleep_for;
using namespace std::chrono_literals;

namespace U {
inline std::string ReadSymlink( const char* path ) {
  AutoAlloc< char > buf( PATH_MAX );
  std::string       ret{};

  int len = readlink( path, buf.Get( ), PATH_MAX );
  if( len != -1 ) {
    buf.At( len ) = 0;
    ret = buf.Get( );
  }

  return ret;
}

inline void PrintLine( const char* fmt, ... ) {
  va_list args;
  char    buf[ 64 ];

  va_start( args, fmt );
  vsprintf( buf, fmt, args );
  va_end( args );

  printf( "\r\33[2L" );
  printf( "%s", buf );

  fflush( stdout );
}

inline void Log( const char* fmt, ... ) {
  va_list args;

  va_start( args, fmt );
  vprintf( fmt, args );
  va_end( args );

  fflush( stdout );
}
}
