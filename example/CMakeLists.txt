cmake_minimum_required(VERSION 2.8)

project(DopeVector)

# source files
set(hdr_dir ${CMAKE_CURRENT_SOURCE_DIR}/../include)
set(src
	${hdr_dir}/DopeVector/Common/Common.hpp
	${hdr_dir}/DopeVector/Common/Index.hpp
	${hdr_dir}/DopeVector/Common/inlines/Index.inl
	${hdr_dir}/DopeVector/Common/Expression.hpp
	${hdr_dir}/DopeVector/Common/inlines/Expression.inl
	${hdr_dir}/DopeVector/DopeVector.hpp
	${hdr_dir}/DopeVector/inlines/DopeVector.inl
	${hdr_dir}/DopeVector/DopeVectorExtent.hpp
	${hdr_dir}/DopeVector/inlines/DopeVectorExtent.inl
	${hdr_dir}/DopeVector/DopeVectors.hpp
	${hdr_dir}/Grid/Grid.hpp
	${hdr_dir}/Grid/inlines/Grid.inl
	${hdr_dir}/Grid/GridIterator.hpp
	${hdr_dir}/Grid/inlines/GridIterator.inl
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

# create the executable
add_executable(${PROJECT_NAME} ${src})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(${PROJECT_NAME} PUBLIC ${hdr_dir})
