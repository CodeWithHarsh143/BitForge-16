cmake_minimum_required(VERSION 3.28)
project(BitForge-16 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.5.0
)
FetchContent_MakeAvailable(Catch2)

add_executable(gates_test tests/hardware/gates_test.cpp src/hardware/gates.cpp)
target_link_libraries(gates_test PRIVATE Catch2::Catch2WithMain)
target_include_directories(gates_test PRIVATE include)

add_executable(arithmetic_test tests/hardware/arithmetic_test.cpp src/hardware/arithmetic.cpp src/hardware/gates.cpp)
target_link_libraries(arithmetic_test PRIVATE Catch2::Catch2WithMain)
target_include_directories(arithmetic_test PRIVATE include)

add_executable(mux_test tests/hardware/mux_test.cpp src/hardware/mux.cpp src/hardware/gates.cpp)
target_link_libraries(mux_test PRIVATE Catch2::Catch2WithMain)
target_include_directories(mux_test PRIVATE include)

add_executable(alu_test tests/hardware/alu_test.cpp src/hardware/alu.cpp src/hardware/gates.cpp src/hardware/arithmetic.cpp src/hardware/mux.cpp)
target_link_libraries(alu_test PRIVATE Catch2::Catch2WithMain)
target_include_directories(alu_test PRIVATE include)

add_test(NAME gates_test COMMAND gates_test)
add_test(NAME arithmetic_test COMMAND arithmetic_test)
add_test(NAME mux_test COMMAND mux_test)
add_test(NAME alu_test COMMAND alu_test)
