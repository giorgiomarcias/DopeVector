#include <DopeVector/internal/Index.hpp>

namespace container {

	template < SizeType Dimension > template < typename ... Sizes >
	inline Index<Dimension>::Index(const SizeType size0,  Sizes &&...sizes)
	: std::array<SizeType, Dimension>({{size0, static_cast<SizeType>(std::forward<SizeType>(sizes))...}})
	{ }

	template < SizeType Dimension >
	inline Index<Dimension>::Index(const std::initializer_list<SizeType> &il)
	{
		std::copy(il.begin(), il.end(), std::array<SizeType, Dimension>::begin());
		std::fill(std::array<SizeType, Dimension>::begin() + il.size(), std::array<SizeType, Dimension>::end(), std::array<SizeType, Dimension>::operator[](std::min(Dimension, il.size()) - 1));
	}

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

    template < SizeType Dimension >
    constexpr Index<Dimension> Index<Dimension>::Zero() {
        return Index({{0}});
    }

    template < SizeType Dimension >
    constexpr Index<Dimension> Index<Dimension>::Ones() {
        return Index({{1}});
    }

    template < SizeType Dimension >
    constexpr Index<Dimension> Index<Dimension>::Constant( const SizeType value ) {
        return Index({{value}});
    }

}
