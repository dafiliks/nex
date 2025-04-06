add_executable(nex "main.cpp" "frontend/tokenizer.cpp" "frontend/parser.cpp" "backend/generator.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET nex PROPERTY CXX_STANDARD 20)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_DEBUG "-g")
