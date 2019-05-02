#pragma once

#include <array>
#include <mutex>

#include "entity.h"

namespace G {
class EntityList {
public:
  void Update( );

  auto Get( size_t idx ) {
    m_mutex.lock( );
    auto ret = m_entities.at( idx );
    m_mutex.unlock( );

    return ret;
  }

private:
  std::mutex m_mutex;
  std::array< Entity, 65 > m_entities;
};
}

extern G::EntityList g_entityList;
