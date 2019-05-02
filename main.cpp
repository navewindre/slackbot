/*
    Slackbot - csgo external linux cheat
    Copyright (C) 2019  navewindre

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <thread>
#include <unistd.h>

#include "process.h"
#include "util.h"

P::Process g_proc( "csgo_linux64" );

const char* logo =
    "                ::::::::::::        \n"
    "            :::::::::::::::::::             \n"
    "         :::::::::::::::::::::::::          \n"
    "       ::::::::\033[34mcllcccccllllllll\033[0m::::::\n"
    "    :::::::::\033[34mlc               dc\033[0m:::::::\n"
    "   ::::::::\033[34mcl   clllccllll    oc\033[0m:::::::::\n"
    "  :::::::::\033[34mo   lc\033[0m::::::::\033[34mco   oc\033[0m::::::::::\n"
    " ::::::::::\033[34mo    cccclc\033[0m:::::\033[34mclcc\033[0m::::::::::::\n"
    " :::::::::::\033[34mlc        cclccclc\033[0m:::::::::::::\n"
    "::::::::::::::\033[34mlcclcc          lc\033[0m::::::::::::	 %s\n"
    "::::::::::\033[34mcclcc\033[0m:::::\033[34mlccclc     oc\033[0m:::::::::::	 %s\n"
    "::::::::::\033[34mo    l\033[0m::::::::::\033[34ml    lc\033[0m:::::::::::	 %s\n"
    " :::::\033[34mcll\033[0m:\033[34mo     clcllcccll     o\033[0m:::::::::::\n"
    " :::::\033[34mocc\033[0m:\033[34mo                  clc\033[0m:::::::::::\n"
    "  ::::\033[34mocl\033[0m:\033[34mccslclccclclccclclc\033[0m:::::::::::::\n"
    "   :::\033[34moclcccccccccccccllllllllllllll\033[0m:::::\n"
    "    ::\033[34mlcc1lcccccccccccccccccccccccco\033[0m::::\n"
    "      ::::::::::::::::::::::::::::::::\n"
    "        ::::::::::::::::::::::::::::\n"
    "           ::::::::::::::::::::::\n"
    "                ::::::::::::        \n\n";

void PrintLogo( ) {
  std::string ver = std::string( __DATE__ ) + " " + std::string( __TIME__ );

  system( "clear" );
  for( size_t i = 0; i < 100; ++i )
    printf( "\n" );

  printf( logo, "slackbot", "by navewindre", ver.c_str( ) );
}

int main( int argc, char** argv ) {
  PrintLogo( );

  int  cur_wait = 0;
  char wait[] = {'\\', '|', '/', '-', '\\', '|', '/', '-'};

  do {
    U::PrintLine( "Waiting for csgo %c", wait[ ++cur_wait % sizeof( wait ) ] );
    sleep_for( 300ms );
  } while( !g_proc.Attach( ) );

  do {
    U::PrintLine( "Waiting for modules %c", wait[ ++cur_wait % sizeof( wait ) ] );

    if( !g_proc.m_client )
      g_proc.m_client = g_proc.FindModule( "client_panorama_client.so" );
    if( !g_proc.m_engine )
      g_proc.m_engine = g_proc.FindModule( "engine_client.so" );
  } while( !g_proc.m_client || !g_proc.m_engine );

  for( ;; ) {
    sleep_for( 300ms );
  }
};
