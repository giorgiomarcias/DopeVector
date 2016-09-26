// Copyright (c) 2016 Giorgio Marcias & Maurizio Kovacic
//
// This source code is part of DopeVector header library
// and it is subject to Apache 2.0 License.
//
// Author: Giorgio Marcias
// email: marcias.giorgio@gmail.com
// Author: Maurizio Kovacic
// email: maurizio.kovacic@gmail.com

#ifndef Index_hpp
#define Index_hpp

#ifdef USE_EIGEN

#include <DopeVector/internal/Common.hpp>
#include <Eigen/Core>

namespace dope {

	template < SizeType Dimension >
	using Index = Eigen::Matrix<SizeType, Dimension, 1>;

}

#else

#include <DopeVector/internal/Expression.hpp>
#include <array>

namespace dope {

	template < SizeType Dimension >
	class Index : public std::array<SizeType, Dimension>, public internal::StaticArrayExpression<Index<Dimension>, SizeType, Dimension> {
	public:
		explicit Index() = default;
		explicit Index(const Index &) = default;
		explicit Index(Index &&) = default;

		template < typename ... Sizes >
		explicit inline Index(const SizeType size0, Sizes &&...sizes);

		inline Index(const std::initializer_list<SizeType> &il);

		template < class E >
		inline Index(const internal::StaticArrayExpression<E, SizeType, Dimension> &e);

		Index & operator= (const Index &) = default;
		Index & operator= (Index &&) = default;

		template < class E >
		inline Index& operator=(const internal::StaticArrayExpression<E, SizeType, Dimension> &e);

		template < class E >
		inline Index& operator+=(const internal::StaticArrayExpression<E, SizeType, Dimension> &e);

		template < class E >
		inline Index& operator-=(const internal::StaticArrayExpression<E, SizeType, Dimension> &e);

		template < class E >
		inline Index& operator*=(const internal::StaticArrayExpression<E, SizeType, Dimension> &e);

		template < class E >
		inline Index& operator/=(const internal::StaticArrayExpression<E, SizeType, Dimension> &e);

		inline SizeType sum() const;

		inline SizeType prod() const;

		static inline constexpr Index Zero();
		static inline constexpr Index Ones();
		static inline constexpr Index Constant(const SizeType value);



		static inline SizeType to_position(const Index &index, const Index &range);
		static inline SizeType to_position(const Index &index, const Index &range, const Index &offset);
		static inline Index<Dimension> to_index(const SizeType position, const Index &range);
	};



	typedef Index<1>	    Index1;
	typedef Index<2>	    Index2;
	typedef Index<3>	    Index3;
	typedef Index<4>	    Index4;

}

#include <DopeVector/internal/inlines/Index.inl>

#endif

#endif // Index_hpp
