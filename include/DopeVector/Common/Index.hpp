#ifndef Index_hpp
#define Index_hpp

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
	};



	typedef Index<1>	    Index1;
	typedef Index<2>	    Index2;
	typedef Index<3>	    Index3;
	typedef Index<4>	    Index4;
	
}

#include <DopeVector/Common/inlines/Index.inl>

#endif // Index_hpp
