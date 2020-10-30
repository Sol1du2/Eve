add_library(Messaging 
  Http_parser.cpp
  Request.cpp
  Reply.cpp
)

target_link_libraries(Messaging PUBLIC 
  ${Boost_LIBRARIES}
)

target_include_directories(Messaging PUBLIC 
  "${Boost_INCLUDE_DIR}"
)