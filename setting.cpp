#include "setting.h"
#include "util.h"

const char* GetPath( ) {
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
  AutoAlloc< char > buffer( size * 2 + 1 );
  const uint8_t*    data = ( const uint8_t* )( src );

  for( size_t i = 0; i < size; ++i ) {
    sprintf( &( buffer.Get( )[ 2 * i ] ), "%02x", data[ i ] );
  }

  FILE* f = fopen( GetPath( ), "w+" );
  if( !f )
    return;

  fprintf( f, "%s=%s\n", name, buffer.Get( ) );
  fflush( f );

  fclose( f );
}

void D::Load( const char* name, void* dest, size_t size ) {
  AutoAlloc< char > buffer( size * 2 + 1 );
  uint8_t*          data = ( uint8_t* )( dest );

  FILE* f = fopen( GetPath( ), "rb" );
  if( !f )
    return;

  std::string key = std::string( name ) + "=%s";
  fscanf( f, key.c_str( ), buffer.Get( ) );

  fclose( f );

  if( !buffer.At( 0 ) )
    return;

  for( size_t i = 0; i < size; ++i ) {
    unsigned temp;
    sscanf( &( buffer.Get( )[ 2 * i ] ), "%02X", &temp );
    data[ i ] = temp;
  }
}
