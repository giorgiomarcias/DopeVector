#ifndef Index_hpp
#define Index_hpp

#ifdef USE_EIGEN

#include <DopeVector/Common/Common.hpp>
#include <Eigen/Core>

namespace container {

template < SizeType Dimension >
using Index = Eigen::Matrix<SizeType, Dimension, 1>;

}

#else

#include <DopeVector/Common/Expression.hpp>
#include <array>

namespace container {

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

        static constexpr Index Zero();
        static constexpr Index Ones();
        static constexpr Index Constant(const SizeType value);
	};



	typedef Index<1>	    Index1;
	typedef Index<2>	    Index2;
	typedef Index<3>	    Index3;
	typedef Index<4>	    Index4;
	
}

#include <DopeVector/Common/inlines/Index.inl>

#endif

#endif // Index_hpp
