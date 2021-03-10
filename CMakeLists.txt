cmake_minimum_required(VERSION 3.8)

set(CMAKE_TOOLCHAIN_FILE "common/intros/cmake/config-attiny817xmini-gcc.cmake")
set(CMAKE_BUILD_TYPE     "Release")

project(test)

find_package(device REQUIRED)
find_package(intros REQUIRED)

add_executable(test
	src/main.cpp
)

target_include_directories(test
	PRIVATE
	include
)

target_link_libraries(test
	PRIVATE
	device::attiny817xmini
	intros::kernel
)

setup_target(test)
