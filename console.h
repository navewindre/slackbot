#pragma once

#include "setting.h"
#include "util.h"

#include <readline/history.h>
#include <readline/readline.h>

namespace U {
class ConsoleMgr {
public:
  ConsoleMgr( D::SettingHolder* holder_ptr ) : m_holder( holder_ptr ) {}

  void InitHandler( );
  void OnMainLoop( );
  void OnReadLine( char* line );

private:
  void PrintVars( );

  void PrintVar( D::ISetting* node );
  void SetVar( D::ISetting* var, float value );
  D::ISetting* FindVar( const char* varname );

  void PrintHelp( );

public:
  struct HelpCmdStruct {
    const char* cmd_name;
    const char* cmd_tip;
  };

private:
  char*             m_prompt;
  D::SettingHolder* m_holder;
};

namespace Con {
extern void ReadLineCallback( char* line );
}
}

extern U::ConsoleMgr g_conMgr;
