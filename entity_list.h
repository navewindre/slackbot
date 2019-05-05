#pragma once

#include <array>
#include <mutex>

#include "entity.h"

namespace G {
class EntityList {
public:
  void Update( );

  template < typename t = Entity > t Get( size_t idx ) {
    m_mutex.lock( );
    auto ret = ( t )m_entities.at( idx );
    m_mutex.unlock( );

    return ret;
  }

private:
  struct CEntInfo {
    uintptr_t m_Entity;
    long      m_Serial;
    uintptr_t m_Next;
    uintptr_t m_Prev;
  };

  std::mutex m_mutex;
  std::array< Entity, 65 > m_entities;
};
}

extern G::EntityList g_entityList;
