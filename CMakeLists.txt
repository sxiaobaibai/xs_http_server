cmake_minimum_required(VERSION 3.8)
project("http server")

set(CMAKE_CXX_FLAGS "-O3 -std=c++17 -Wall")


add_executable(server src/main.cc)

target_include_directories(server
	PRIVATE
	./inc/
)

#target_link_libraries(exec
#	pthread
#)
