cmake_minimum_required(VERSION 2.8)

project(DopeVector)

# source files
set(hdr_dir ${CMAKE_CURRENT_SOURCE_DIR}/../include)
set(src
	${hdr_dir}/DopeVector/internal/Common.hpp
	${hdr_dir}/DopeVector/internal/Index.hpp
	${hdr_dir}/DopeVector/internal/inlines/Index.inl
	${hdr_dir}/DopeVector/internal/Expression.hpp
	${hdr_dir}/DopeVector/internal/inlines/Expression.inl
	${hdr_dir}/DopeVector/internal/GridIterator.hpp
	${hdr_dir}/DopeVector/internal/inlines/GridIterator.inl
	${hdr_dir}/DopeVector/internal/inlines/DopeVector.inl
	${hdr_dir}/DopeVector/internal/inlines/Grid.inl
	${hdr_dir}/DopeVector/DopeVector.hpp
	${hdr_dir}/DopeVector/Grid.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

# create the executable
add_executable(${PROJECT_NAME} ${src})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(${PROJECT_NAME} PUBLIC ${hdr_dir})
