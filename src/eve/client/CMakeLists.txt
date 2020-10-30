add_subdirectory(exe)

add_library(Eve-Client-Lib
  Eve.cpp
)

target_link_libraries(Eve-Client-Lib PUBLIC 
  ${Boost_LIBRARIES}
  Messaging
  Util
)

target_include_directories(Eve-Client-Lib PUBLIC 
  "${Boost_INCLUDE_DIR}"
)