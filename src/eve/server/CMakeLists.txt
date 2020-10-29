add_subdirectory(api)
add_subdirectory(data)
add_subdirectory(smtp)
add_subdirectory(exe)

add_library(Eve-Lib
  Eve.cpp 
  Connection.cpp
)

target_link_libraries(Eve-Lib PUBLIC 
  ${Boost_LIBRARIES}
  Messaging
  Api
)

target_include_directories(Eve-Lib PUBLIC 
  "${Boost_INCLUDE_DIR}"
)