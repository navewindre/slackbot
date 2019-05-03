add_executable(slackbot
  main.cpp
  process.cpp
  entity_list.cpp
  setting.cpp
  )

target_include_directories(slackbot PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}
  )

set_property(TARGET slackbot PROPERTY CXX_STANDARD 14)
