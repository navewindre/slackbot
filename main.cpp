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
    " :::::::::::\033[34mlc        cclccclc\033[0m:::::::::::::	 %s\n"
    "::::::::::::::\033[34mlcclcc          lc\033[0m::::::::::::	 %s\n"
    "::::::::::\033[34mcclcc\033[0m:::::\033[34mlccclc     oc\033[0m:::::::::::	 %s\n"
    "::::::::::\033[34mo    l\033[0m::::::::::\033[34ml    lc\033[0m:::::::::::\n"
    " :::::\033[34mcll\033[0m:\033[34mo     clcllcccll     o\033[0m:::::::::::\n"
    " :::::\033[34mocc\033[0m:\033[34mo                  clc\033[0m:::::::::::\n"
    "  ::::\033[34mocl\033[0m:\033[34mccslclccclclccclclc\033[0m:::::::::::::\n"
    "   :::\033[34moclcccccccccccccllllllllllllll\033[0m:::::\n"
    "    ::\033[34mlcc1lcccccccccccccccccccccccco\033[0m::::\n"
    "      ::::::::::::::::::::::::::::::::\n"
    "        ::::::::::::::::::::::::::::\n"
    "           ::::::::::::::::::::::\n"
    "                ::::::::::::        \n";

void PrintLogo( ) {
  std::string ver = std::string( __DATE__ ) + " " + std::string( __TIME__ );

  printf( logo, "slackbot", "by navewindre", ver.c_str( ) );
}

int main( int argc, char** argv ) {
  PrintLogo( );

  int cur_wait = 0;
  do {
    char wait[] = {'\\', '|', '/', '-', '\\', '|', '/', '-'};

    U::PrintLine( "Waiting for csgo %c", wait[ ++cur_wait % sizeof( wait ) ] );
    sleep_for( 300ms );
  } while( !g_proc.Attach( ) );
  U::PrintLine( "csgo found\n" );

  g_proc.m_client = g_proc.FindModule( "client_panorama_client.so" );
  g_proc.m_engine = g_proc.FindModule( "engine_client.so" );

  U::Log(
      "client_panorama_client.so: [ %08x : %08x ]\n", g_proc.m_client.start, g_proc.m_client.end );
  U::Log(
      "engine_client.so:          [ %08x : %08x ]\n", g_proc.m_engine.start, g_proc.m_engine.end );

  for( ;; ) {
    sleep_for( 300ms );
  }
};
