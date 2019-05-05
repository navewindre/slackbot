#include "entity_list.h"
#include "address.h"
#include "process.h"

G::EntityList g_entityList;

namespace G {
void EntityList::Update( ) {
  static ptr< CEntInfo > entity_list_ptr = *ptr<>(
      g_proc.PatternSearch( g_proc.m_client,
                            "48 8d ? ? ? ? ? 48 8d ? ? ? ? ? e8 ? ? ? ? 5d 48 8d ? ? ? ? ? 48 8d ? "
                            "? ? ? ? 48 8d ? ? ? ? ? e9 ? ? ? ? ? ? ? ? 55 48 8d",
                            2 ) +
      8 );

  m_mutex.lock( );

  for( auto link = entity_list_ptr; !!link; link = ptr< CEntInfo >( link->m_Next ) ) {
    Entity ent = *ptr< Entity >( link->m_Entity );
    int    index = *ent.m_nIndex( );

    printf( "index: %d\n" );
    if( index > 0 && index < 65 )
      m_entities.at( index ) = ent;
  }

  m_mutex.unlock( );
}
}
