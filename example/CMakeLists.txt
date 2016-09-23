cmake_minimum_required(VERSION 3.3)

project(DopeVector_example)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_BINARY_DIR}/lib_dope_vector)
include(${CMAKE_CURRENT_SOURCE_DIR}/../sources_properties.cmake)

set(src
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
add_executable(${PROJECT_NAME} ${src})

target_link_libraries(${PROJECT_NAME} DopeVector)
set_dope_vector_source_files_properties()
