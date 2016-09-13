#include <DopeVector/Common/Index.hpp>

namespace container {

	template < SizeType Dimension > template < typename ... Sizes >
	inline Index<Dimension>::Index(const SizeType size0,  Sizes &&...sizes)
		: std::array<SizeType, Dimension>({{size0, static_cast<SizeType>(sizes)...}})
	{ }

	template < SizeType Dimension > template < class E >
	inline Index<Dimension>::Index(const internal::StaticArrayExpression<E, SizeType, Dimension> &e)
	{
		for (SizeType i = 0; i < Dimension; ++i)
		std::array<SizeType, Dimension>::operator[](i) = e.getAt(i);
	}

	template < SizeType Dimension > template < class E >
	inline Index<Dimension>& Index<Dimension>::operator=(const internal::StaticArrayExpression<E, SizeType, Dimension> &e)
	{
		for (SizeType i = 0; i < Dimension; ++i)
		std::array<SizeType, Dimension>::operator[](i) = e.getAt(i);
	}

	template < SizeType Dimension > template < class E >
	inline Index<Dimension>& Index<Dimension>::operator+=(const internal::StaticArrayExpression<E, SizeType, Dimension> &e)
	{
		for (SizeType i = 0; i < Dimension; ++i)
		std::array<SizeType, Dimension>::operator[](i) += e.getAt(i);
	}

	template < SizeType Dimension > template < class E >
	inline Index<Dimension>& Index<Dimension>::operator-=(const internal::StaticArrayExpression<E, SizeType, Dimension> &e)
	{
		for (SizeType i = 0; i < Dimension; ++i)
		std::array<SizeType, Dimension>::operator[](i) -= e.getAt(i);
	}

	template < SizeType Dimension > template < class E >
	inline Index<Dimension>& Index<Dimension>::operator*=(const internal::StaticArrayExpression<E, SizeType, Dimension> &e)
	{
		for (SizeType i = 0; i < Dimension; ++i)
		std::array<SizeType, Dimension>::operator[](i) *= e.getAt(i);
	}

	template < SizeType Dimension > template < class E >
	inline Index<Dimension>& Index<Dimension>::operator/=(const internal::StaticArrayExpression<E, SizeType, Dimension> &e)
	{
		for (SizeType i = 0; i < Dimension; ++i)
		std::array<SizeType, Dimension>::operator[](i) /= e.getAt(i);
	}

}
