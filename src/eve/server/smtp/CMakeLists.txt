add_library(Smtp
  Smtp_client.cpp
)

target_link_libraries(Smtp PUBLIC 
  ${Boost_LIBRARIES}
)

target_include_directories(Smtp PUBLIC 
  "${Boost_INCLUDE_DIR}"
)