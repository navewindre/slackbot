#pragma once

#include <memory>
#include <type_traits>
#include <vector>

#include "util.h"

namespace D {
constexpr const char* SETTINGS_FILENAME = ".slack_settings.dat";

extern const char* GetPath( );
extern void Save( const char* name, const void* src, size_t size );
extern void Load( const char* name, void* dest, size_t size );

class SettingNode {
public:
  virtual ~SettingNode( ) = default;

  virtual void Load( ) = 0;
  virtual void Save( ) = 0;

  virtual void Register( SettingNode* ptr ) = 0;
};

class SettingHolder : public SettingNode {
public:
  inline SettingHolder( ) {}
  inline SettingHolder( SettingHolder* other ) { other->Register( this ); }

  void Register( SettingNode* node ) override { m_nodes.push_back( node ); }

  void Load( ) {
    for( auto& it : m_nodes )
      it->Load( );
  }

  void Save( ) {
    if( U::FileExists( GetPath( ) ) )
      remove( GetPath( ) );

    for( auto& it : m_nodes )
      it->Save( );
  }

  auto GetNodes( ) { return m_nodes; }

private:
  std::vector< SettingNode* > m_nodes;
};

class ISetting : public SettingNode {
  void Register( SettingNode* ptr ) override {}

public:
  virtual bool IsIntegral( ) = 0;
  virtual bool IsDecimal( ) = 0;
  virtual bool IsBoolean( ) = 0;

  virtual const char* GetName( ) = 0;
};

template < typename t > class Setting : public ISetting {
public:
  inline Setting( SettingHolder* holder, const char* name, t default_ = t{} )
      : m_name( name ), m_value( default_ ) {
    holder->Register( this );
  }

  void Load( ) override { ::D::Load( m_name, &m_value, sizeof( t ) ); }
  void Save( ) override { ::D::Save( m_name, &m_value, sizeof( t ) ); }

  bool IsIntegral( ) override { return std::is_integral< t >::value && !IsBoolean( ); }
  bool IsDecimal( ) override { return std::is_floating_point< t >::value; }
  bool IsBoolean( ) override { return ( t )( 1 ) == ( t )( 2 ); }

  virtual const char* GetName( ) { return m_name; }

  inline operator t&( ) { return m_value; }
  inline t* operator&( ) { return &m_value; }
  inline t& operator( )( ) { return m_value; }

private:
  t           m_value;
  const char* m_name;
};

extern SettingHolder Holder;
}
