add_executable(Eve-Client
  main.cpp
)

target_link_libraries(Eve-Client PUBLIC 
  Eve-Client-Lib
)

target_include_directories(Eve-Client PUBLIC 
  "${PROJECT_BINARY_DIR}"
)