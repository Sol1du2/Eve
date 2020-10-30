add_subdirectory(api)
add_subdirectory(data)
add_subdirectory(smtp)
add_subdirectory(exe)

add_library(Eve-Server-Lib
  Eve.cpp 
  Connection.cpp
)

target_link_libraries(Eve-Server-Lib PUBLIC 
  ${Boost_LIBRARIES}
  Messaging
  Api
)

target_include_directories(Eve-Server-Lib PUBLIC 
  "${Boost_INCLUDE_DIR}"
)