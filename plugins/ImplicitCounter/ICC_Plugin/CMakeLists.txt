cmake_minimum_required(VERSION 3.20)

project(ImplicitConversionsCounter)

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})

find_package(LLVM REQUIRED)
find_package(Clang REQUIRED)

include(AddLLVM)

add_llvm_library(ImplicitConversionsCounter MODULE ImplicitConversionsCounter.cpp PLUGIN_TOOL clang)
