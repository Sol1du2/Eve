add_library(Api
  Request_handler.cpp
  Index_handler.cpp
  Json_handler.cpp
)

target_link_libraries(Api PUBLIC 
  Messaging
  Data
  Smtp
  Util
  nlohmann_json::nlohmann_json
)

target_include_directories(Api PUBLIC 
  "${JSON_INCLUDE_DIR}"
)