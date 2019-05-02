#include "process.h"

namespace U {
template < typename t > class Address {
  uintptr_t m_ptr;
  t         m_val;

public:
  Address( uintptr_t ptr ) : m_ptr( ptr ) {}
  Address( void* ptr ) : m_ptr( ( uintptr_t )( ptr ) ) {}

  template < typename other > Address( Address< other >&& other ) m_ptr( other.Ptr( ) ) {}

  uintptr_t Ptr( ) { return m_ptr; }

  t& Val( ) {
    m_val = g_proc.Read< t >( m_ptr );
    return m_val;
  }

  t& operator*( ) { return Val( ); }
  t* operator->( ) { return &Val( ); }
  operator uintptr_t( ) { return m_ptr; }

  t& operator=( const t& other ) {
    g_proc.Write< t >( m_ptr, other );
    return Val( );
  }

  void Set( const t& val ) { g_proc.Write< t >( m_ptr, val ); }

  Address< t > operator+( int offset ) { return Address< t >( m_ptr + offset ); }

  Address< t > operator-( int offset ) { return Address< t >( m_ptr - offset ); }
};
}
