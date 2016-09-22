cmake_minimum_required(VERSION 2.8)

project(DopeVector_example)

set(hdr_dir ${CMAKE_CURRENT_SOURCE_DIR}/../include)

set(hdr_internal_files
	${hdr_dir}/DopeVector/internal/Common.hpp
	${hdr_dir}/DopeVector/internal/Expression.hpp
	${hdr_dir}/DopeVector/internal/GridIterator.hpp
)
source_group(headers\\internal FILES ${hdr_internal_files})

set(hdr_internal_inline_files
	${hdr_dir}/DopeVector/internal/inlines/Index.inl
	${hdr_dir}/DopeVector/internal/inlines/Expression.inl
	${hdr_dir}/DopeVector/internal/inlines/GridIterator.inl
	${hdr_dir}/DopeVector/internal/inlines/DopeVector.inl
	${hdr_dir}/DopeVector/internal/inlines/Grid.inl
)
set_source_files_properties(${hdr_internal_inline_files} PROPERTIES XCODE_EXPLICIT_FILE_TYPE "sourcecode.cpp.h")
source_group(headers\\internal\\inlines FILES ${hdr_internal_inline_files})

set(hdr_main_files
	${hdr_dir}/DopeVector/DopeVector.hpp
	${hdr_dir}/DopeVector/Grid.hpp
	${hdr_dir}/DopeVector/Index.hpp
)
source_group(headers FILES ${hdr_main_files})

set(src ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

set(all_src ${hdr_internal_files} ${hdr_internal_inline_files} ${hdr_main_files} ${src})



add_executable(${PROJECT_NAME} ${all_src})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(${PROJECT_NAME} PUBLIC ${hdr_dir})
