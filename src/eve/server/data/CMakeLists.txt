add_library(Data
  Email_storage.cpp
  Data_definition.cpp
)

target_link_libraries(Data PUBLIC
  Util
  nlohmann_json::nlohmann_json
)

target_include_directories(Data PUBLIC 
  "${Boost_INCLUDE_DIR}"
  "${JSON_INCLUDE_DIR}"
)