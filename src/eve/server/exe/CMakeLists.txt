add_executable(Eve-Server 
  main.cpp
)

target_link_libraries(Eve-Server PUBLIC 
  Eve-Lib
)

target_include_directories(Eve-Server PUBLIC 
  "${PROJECT_BINARY_DIR}"
)