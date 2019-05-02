#pragma once

#include <cinttypes>
#include <string>
#include <sys/uio.h>

namespace P {
struct Module_t {
  uintptr_t start, end;
};

class Process {
public:
  Process( const char* proc ) : m_name( proc ), m_pid( -1 ){};

  bool Valid( );
  bool Attach( );

  inline bool Read( uintptr_t at, void* out, size_t len ) {
    struct iovec l = {out, len};
    struct iovec r = {( void* )at, len};

    return ( size_t )( process_vm_readv( m_pid, &l, 1, &r, 1, 0 ) ) == len;
  }

  template < typename t > inline t Read( uintptr_t at ) {
    t ret = t{};

    if( Read( at, &ret, sizeof( t ) ) )
      return ret;

    return t{};
  }

  template < typename t > inline bool Write( uintptr_t at, t val, size_t len = sizeof( t ) ) {
    struct iovec l = {( void* )( &val ), len};
    struct iovec r = {( void* )( at ), len};

    return ( size_t )( process_vm_writev( m_pid, &l, 1, &r, 1, 0 ) ) == len;
  }

  inline int Pid( ) { return m_pid; }

public:
  Module_t FindModule( const char* module );
  uintptr_t PatternSearch( uintptr_t, std::string pattern, size_t offset );

public:
  Module_t m_client;
  Module_t m_engine;

private:
  const char* m_name;
  long        m_pid;
};
}

extern P::Process g_proc;
