cmake_minimum_required(VERSION 2.8)

project(DopeVector)

# source files
set(hdr_dir ${CMAKE_CURRENT_SOURCE_DIR}/../include)
set(src
    ${hdr_dir}/DopeVector/DopeVector.hpp
    ${hdr_dir}/DopeVector/inlines/DopeVector.inl
    ${hdr_dir}/DopeVector/DopeVectorExtent.hpp
    ${hdr_dir}/DopeVector/inlines/DopeVectorExtent.inl
    ${hdr_dir}/DopeVector/DopeVectors.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

# create the executable
add_executable(${PROJECT_NAME} ${src})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(${PROJECT_NAME} PUBLIC ${hdr_dir})
