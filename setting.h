#pragma once

#include <memory>
#include <vector>

namespace D {
constexpr const char* SETTINGS_FILENAME = ".slack_settings.dat";

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
  inline SettingHolder( const char* name ) : m_name( name ) {}
  inline SettingHolder( SettingHolder* other, const char* name ) : m_name( name ) {
    other->Register( this );
  }

  void Register( SettingNode* node ) override { m_nodes.push_back( node ); }

  void Load( ) {
    for( auto& it : m_nodes )
      it->Load( );
  }

  void Save( ) {
    for( auto& it : m_nodes )
      it->Save( );
  }

private:
  const char*                 m_name;
  std::vector< SettingNode* > m_nodes;
};

class ISetting : public SettingNode {
  void Register( SettingNode* ptr ) override {}
};

template < typename t > class Setting : public ISetting {
  inline Setting( SettingHolder* holder, const char* name, t default_ = t{} )
      : m_name( name ), m_value( default_ ) {
    holder->Register( this );
  }

  void Load( ) override { ::D::Load( m_name, &m_value, sizeof( t ) ); }

  void Save( ) override { ::D::Save( m_name, &m_name, sizeof( t ) ); }

  inline operator t&( ) { return m_value; }
  inline t* operator&( ) { return &m_value; }
  inline t& operator( )( ) { return m_value; }

private:
  t           m_value;
  const char* m_name;
};
}
