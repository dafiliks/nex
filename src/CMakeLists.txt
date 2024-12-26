find_package(LLVM REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(nex "main.cpp" "frontend/tokenizer.cpp" "frontend/parser.cpp" "frontend/ast.cpp" "backend/generator.cpp")

llvm_config(nex USE_SHARED support core irreader)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET nex PROPERTY CXX_STANDARD 20)
endif()