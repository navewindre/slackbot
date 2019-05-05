#pragma once

#include <address.h>

#define OFFSET( name, offset, type ) \
  auto name( ) { return Get< type >( offset ); }

class Entity {
  uintptr_t m_ptr;

public:
  template < typename t > auto Get( std::ptrdiff_t offset ) { return ptr< t >( m_ptr + offset ); }

public:
  OFFSET( m_nIndex, 0x94, int );
};
