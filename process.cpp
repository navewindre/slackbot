#include "process.h"
#include "address.h"
#include "util.h"

#include <dirent.h>
#include <fcntl.h>
#include <fstream>
#include <iterator>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>

namespace P {
bool Process::Valid( ) {
  char path[ PATH_MAX ];

  sprintf( path, "/proc/%d", m_pid );

  struct stat st;
  if( stat( path, &st ) == 0 && S_ISDIR( st.st_mode ) )
    return true;

  return false;
}

bool Process::Attach( ) {
  char path[ PATH_MAX ];
  DIR* proc = opendir( "/proc" );

  m_pid = -1;

  if( !proc )
    return false;

  for( struct dirent* e = readdir( proc ); !!e; e = readdir( proc ) ) {
    int id = strtol( e->d_name, 0, 10 );
    if( id ) {
      sprintf( path, "/proc/%d/exe", id );

      auto   link = U::ReadSymlink( path );
      size_t s = link.find_last_of( "/" );
      if( s == std::string::npos )
        continue;

      if( !link.compare( s + 1, std::string::npos, m_name ) ) {
        m_pid = id;
        break;
      }
    }
  }

  closedir( proc );

  return m_pid != -1;
}

Module_t Process::FindModule( const char* name ) {
  char      path[ PATH_MAX ];
  uintptr_t start{};
  uintptr_t end{};
  Module_t  ret{};

  char        g1;
  std::string g2;

  sprintf( path, "/proc/%d/maps", m_pid );
  std::ifstream maps( path, std::fstream::in | std::fstream::binary );
  if( !maps.is_open( ) ) {
    return Module_t{};
  }

  std::string cur_line;
  while( std::getline( maps, cur_line ) ) {
    std::istringstream ss( cur_line );

    ss >> std::hex >> start >> g1 >> end;

    ss >> g2 >> g2 >> g2 >> g2;

    std::string module;
    std::getline( ss, module );

    if( module.find( name ) != std::string::npos ) {
      ret = {start, end};
      break;
    }
  }

  return ret;
}

uintptr_t Process::PatternSearch( Module_t mod, std::string pattern, size_t offset ) {
  auto bin_match = []( uint8_t* code, uint8_t* pattern, size_t size ) -> bool {
    for( size_t i = 0; i < size; ++i ) {
      if( pattern[ i ] && code[ i ] != pattern[ i ] )
        return false;
    }

    return true;
  };

  std::istringstream         iss( pattern );
  std::vector< std::string > tokens{std::istream_iterator< std::string >{iss},
                                    std::istream_iterator< std::string >{}};

  std::vector< uint8_t > sig_bytes;

  for( auto hex_byte : tokens ) {
    sig_bytes.push_back( ( uint8_t )( std::strtoul( hex_byte.c_str( ), nullptr, 16 ) ) );
  }

  if( sig_bytes.size( ) < 2 )
    return 0;  // sanity check

  AutoAlloc< uint8_t > buffer( 1024 );

  for( uintptr_t offset = 0; offset < mod.Size( ); offset += buffer.Size( ) ) {
    Read( mod.start + offset, buffer.Get( ), buffer.Size( ) );
    for( size_t i{}; i < buffer.Size( ); ++i ) {
      if( bin_match( buffer.Get( ), sig_bytes.data( ), sig_bytes.size( ) ) ) {
        return mod.start + offset;
      }
    }
  }

  return 0;
}
}
