cmake_minimum_required(VERSION 2.8)

project(DopeVector_example)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_BINARY_DIR}/lib_dope_vector)

set(src
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
add_executable(${PROJECT_NAME} ${src})

target_link_libraries(${PROJECT_NAME} INTERFACE DopeVector)
