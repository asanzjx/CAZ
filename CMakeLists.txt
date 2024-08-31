cmake_minimum_required(VERSION 3.10)

SET(CMAKE_C_COMPILER "/usr/bin/clang")
SET(CMAKE_CXX_COMPILER "/usr/bin/clang++")

project(CAZ)

find_package(LLVM REQUIRED CONFIG)
set(llvm_config /usr/bin/llvm-config)

execute_process(
  COMMAND ${llvm_config} --cxxflags
  OUTPUT_VARIABLE llvm_cxxflags
)
execute_process(
  COMMAND ${llvm_config} --ldflags
  OUTPUT_VARIABLE llvm_ldflags
)
execute_process(
  COMMAND ${llvm_config} --libs
  OUTPUT_VARIABLE llvm_libs
)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${llvm_cxxflags}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${llvm_ldflags}")

# 添加源文件
add_executable(CAZ main.cpp Lexer.cpp Parser.cpp CodeGeneration.cpp)

# Link against LLVM libraries
target_link_libraries(CAZ ${llvm_libs})
