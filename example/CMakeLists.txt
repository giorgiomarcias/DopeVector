cmake_minimum_required(VERSION 3.3)

project(DopeVector_example)

option(WITH_EIGEN "Build DopeVector with Index<Dimension> as Eigen Matrix (if present) or not." ON)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_BINARY_DIR}/lib_dope_vector)
include(${CMAKE_CURRENT_SOURCE_DIR}/../sources_properties.cmake)

set(src
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
add_executable(${PROJECT_NAME} ${src})

target_link_libraries(${PROJECT_NAME} DopeVector)
set_dope_vector_source_files_properties()

if (WITH_EIGEN)
	set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules ${CMAKE_MODULE_PATH})
	find_package(Eigen3)
	if (EIGEN3_FOUND)
		target_include_directories(${PROJECT_NAME} PUBLIC ${EIGEN3_INCLUDE_DIR})
		target_compile_definitions(${PROJECT_NAME} PUBLIC DOPE_USE_EIGEN_INDEX)
	endif()
endif()
