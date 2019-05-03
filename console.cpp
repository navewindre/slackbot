#include "console.h"

static const U::ConsoleMgr::HelpCmdStruct CMD_TIP_LIST[] = {
    {"help", "shows available commands"},
    {"var [set/get] [variable] [value]", "sets/prints a variable value"},
    {"print_vars", "shows a list of all available variables"}};

U::ConsoleMgr g_conMgr( &D::Holder );

namespace U {
void Con::ReadLineCallback( char* line ) {
  g_conMgr.OnReadLine( line );
}

/****************************************/

void ConsoleMgr::InitHandler( ) {
  m_prompt = strdup( "slackbot\033[34m>\033[0m " );
  rl_callback_handler_install( m_prompt, ( rl_vcpfunc_t* )( &Con::ReadLineCallback ) );
}

void ConsoleMgr::OnMainLoop( ) {
  rl_callback_read_char( );
}

void ConsoleMgr::OnReadLine( char* line ) {
  if( !line || strlen( line ) <= 1 )
    return;

  if( strstr( line, "print_vars" ) ) {
    PrintVars( );
  } else if( strstr( line, "var set" ) ) {
    char  var_name[ 256 ];
    float value;
    sscanf( line, "%*s %*s %s %f", var_name, &value );

    auto var = FindVar( var_name );
    if( var )
      SetVar( var, value );
    else
      printf( " Variable %s not found\n" );
  } else if( strstr( line, "var get" ) ) {
    char var_name[ 256 ];
    sscanf( line, "%*s %*s %s", var_name );

    auto var = FindVar( var_name );
    if( var )
      PrintVar( var );
    else
      printf( " Variable %s not found\n" );
  } else if( strstr( line, "help" ) ) {
    PrintHelp( );
  } else {
    printf( " Command not found\n" );
  }
}

void ConsoleMgr::PrintVar( D::ISetting* node ) {
  if( node->IsIntegral( ) ) {
    auto setting = static_cast< D::Setting< int >* >( node );
    printf( " %s %d (int)\n", setting->GetName( ), ( *setting )( ) );
  } else if( node->IsDecimal( ) ) {
    auto setting = static_cast< D::Setting< float >* >( node );
    printf( " %s %0.3f (float)\n", setting->GetName( ), ( *setting )( ) );
  } else if( node->IsBoolean( ) ) {
    auto setting = static_cast< D::Setting< bool >* >( node );
    printf( " %s %d (bool)\n", setting->GetName( ), ( *setting )( ) );
  }
}

void ConsoleMgr::PrintVars( ) {
  printf( "\033[34mList of available settings:\033[0m\n" );

  for( auto it : m_holder->GetNodes( ) ) {
    auto node = dynamic_cast< D::ISetting* >( it );

    PrintVar( node );
  }
}

D::ISetting* ConsoleMgr::FindVar( const char* varname ) {
  for( auto it : m_holder->GetNodes( ) ) {
    auto node = dynamic_cast< D::ISetting* >( it );

    if( strstr( node->GetName( ), varname ) )
      return node;
  }

  return nullptr;
}

void ConsoleMgr::SetVar( D::ISetting* var, float value ) {
  if( var->IsIntegral( ) ) {
    auto setting = static_cast< D::Setting< int >* >( var );
    ( *setting )( ) = ( int )( value );
  } else if( var->IsDecimal( ) ) {
    auto setting = static_cast< D::Setting< float >* >( var );
    ( *setting )( ) = value;
  } else if( var->IsBoolean( ) ) {
    auto setting = static_cast< D::Setting< bool >* >( var );
    ( *setting )( ) = !!value;
  }
}

void ConsoleMgr::PrintHelp( ) {
  size_t tip_count = sizeof( CMD_TIP_LIST ) / sizeof( HelpCmdStruct );

  printf( "\033[34mList of available commands:\033[0m\n" );
  for( size_t i = 0; i < tip_count; ++i ) {
    auto& tip = CMD_TIP_LIST[ i ];
    printf( " \033[34m%s\033[0m - %s\n", tip.cmd_name, tip.cmd_tip );
  }
}
}
