add_executable(slackbot
  main.cpp
  process.cpp
  entity_list.cpp
  setting.cpp
  console.cpp
  )

target_include_directories(slackbot PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}
  )

add_compile_options( -lreadline )

set( CMAKE_CXX_FLAGS "-lreadline -ltermcap -lpthread  -std=gnu++17" )

message( STATUS, "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
