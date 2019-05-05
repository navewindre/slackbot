#include "visuals.h"
#include "process.h"

namespace F {
void Visuals::Glow( ) {
  static ptr<> glow_obj_manager = g_proc.PatternSearch(
      g_proc.m_client, "48 8d ? ? ? ? ? 48 8d ? ? ? ? ? e8 ? ? ? ? 48 8d ? ? ? ? ? 5d c3 90", 0x2 );
}
}
