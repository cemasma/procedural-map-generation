cmake_minimum_required(VERSION 3.24)
project(procedural-map-generation)

add_compile_options(-std=c++14)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

set(MY_HEADER_FILES
	# put all your .h here. Right now we have none.
	# Example : 
	# ../common/shader.hpp
	generator.h
)

set(MY_SOURCE_FILES
	# put all your .c/.cpp here.
	generator.cpp
	main.cpp
	# ../common/shader.cpp
)

add_executable(procedural-map-generation
	${MY_HEADER_FILES}
	${MY_SOURCE_FILES}
)
target_link_libraries(procedural-map-generation
	${SDL2_LIBRARIES}
)