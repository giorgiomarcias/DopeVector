cmake_minimum_required(VERSION 3.1)

project(DopeVector)

set(hdr_dir ${CMAKE_CURRENT_SOURCE_DIR}/include)

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

set(all_hdr ${hdr_internal_files} ${hdr_internal_inline_files} ${hdr_main_files})


add_library(${PROJECT_NAME} INTERFACE)

set(required_cxx_features
	cxx_alias_templates
	cxx_auto_type
	cxx_constexpr
	cxx_decltype
	cxx_default_function_template_args
	cxx_defaulted_functions
	cxx_defaulted_move_initializers
	cxx_deleted_functions
	cxx_explicit_conversions
	cxx_generalized_initializers
	cxx_lambdas
	cxx_noexcept
	cxx_nonstatic_member_init
	cxx_nullptr
	cxx_override
	cxx_range_for
	cxx_right_angle_brackets
	cxx_rvalue_references
	cxx_static_assert
	cxx_variadic_templates
)

target_compile_features(${PROJECT_NAME} INTERFACE ${required_cxx_features})
target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${hdr_dir}>
)
target_sources(${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE:${all_hdr}>)
