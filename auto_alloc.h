#pragma once

#include <cstdlib>

template < class t > class AutoAlloc {
  t*     m_mem;
  size_t m_count;

public:
  AutoAlloc( size_t count ) : m_count( count ) {
    m_mem = ( t* )( malloc( count * sizeof( t ) ) );
    if( !m_mem )
      throw "memory allocation error";
  }

  ~AutoAlloc( ) {
    if( m_mem )
      free( m_mem );
  }

  operator t*( ) { return m_mem; }

  auto Get( ) { return m_mem; }

  t& At( size_t i ) { return m_mem[ i ]; }

  operator uintptr_t( ) { return ( uintptr_t )( m_mem ); }

  size_t Size( ) { return sizeof( t ) * m_count; }

  void Realloc( size_t newcount = 0 ) {
    if( newcount != 0 )
      m_count = newcount;

    Free( );

    m_mem = ( t* )( malloc( m_count * sizeof( t ) ) );
  }

private:
  void Free( ) {
    if( m_mem )
      free( m_mem );

    m_mem = nullptr;
  }
};
