#include "setting.h"
#include "settings.h"
#include "util.h"

D::SettingHolder D::Holder;
D::Settings      g_cfg;

const char* D::GetPath( ) {
  static char out_dir[ PATH_MAX ];
  static bool dir_set = false;

  if( !dir_set ) {
    strcat( out_dir, getenv( "HOME" ) );
    strcat( out_dir, "/" );
    strcat( out_dir, D::SETTINGS_FILENAME );

    dir_set = true;
  }

  return out_dir;
}

void D::Save( const char* name, const void* src, size_t size ) {
  char*          buffer = ( char* )alloca( size * 2 + 1 );
  const uint8_t* data = ( const uint8_t* )( src );

  memset( buffer, 0, size * 2 + 1 );

  for( size_t i = 0; i < size; ++i ) {
    sprintf( &buffer[ 2 * i ], "%02x", data[ i ] );
  }

  FILE* f = fopen( GetPath( ), "a" );
  if( !f )
    return;

  fprintf( f, "%s = %s\n", name, buffer );
  fflush( f );

  fclose( f );
}

void D::Load( const char* name, void* dest, size_t size ) {
  char*    buffer = ( char* )alloca( size * 2 + 1 );
  uint8_t* data = ( uint8_t* )( dest );

  memset( buffer, 0, size * 2 + 1 );

  FILE* f = fopen( GetPath( ), "r+" );
  if( !f )
    return;

  char   read_name[ 64 ]{};
  char*  line = nullptr;
  size_t len = 128;

  /*
   * probably not the proper way
   * don't wanna fuck with an entire ini parser
  */
  while( getline( &line, &len, f ) != -1 ) {
    if( line ) {
      sscanf( line, "%s = %s", read_name, buffer );
      if( !strcmp( name, read_name ) )
        break;

      free( line );
      line = nullptr;
    }
  }

  fclose( f );

  if( !buffer[ 0 ] )
    return;

  for( size_t i = 0; i < size; ++i ) {
    unsigned temp;
    sscanf( &buffer[ 2 * i ], "%02x", &temp );
    data[ i ] = temp;
  }
}
