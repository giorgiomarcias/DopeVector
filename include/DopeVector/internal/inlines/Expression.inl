// Copyright (c) 2016 Giorgio Marcias & Maurizio Kovacic
//
// This source code is part of DopeVector header library
// and it is subject to Apache 2.0 License.
//
// Author: Giorgio Marcias
// email: marcias.giorgio@gmail.com
// Author: Maurizio Kovacic
// email: maurizio.kovacic@gmail.com

#ifndef DOPE_USE_EIGEN_INDEX
#include <DopeVector/internal/Expression.hpp>

namespace dope {

	namespace internal {

		template < class E, typename T, SizeType Dimension >
		inline T StaticArrayExpression<E, T, Dimension>::getAt(const SizeType i) const
		{
			return static_cast<E const&>(*this)[i];
		}

		template < class E, typename T, SizeType Dimension >
		inline StaticArrayExpression<E, T, Dimension>::operator E const&() const
		{
			return static_cast<const E &>(*this);
		}

		template < class E, typename T, SizeType Dimension >
		inline StaticArrayExpression<E, T, Dimension>::operator E &()
		{
			return static_cast<E &>(*this);
		}



		template < typename T, SizeType Dimension >
		const T& ConstantExpression<T, Dimension>::operator[](const SizeType) const
		{
			return _t;
		}



		template < class El, class Er, typename T, SizeType Dimension >
		inline StaticArraySum<El, Er, T, Dimension>::StaticArraySum(const El &el, const Er &er)
		    : _el(el), _er(er)
		{ }

		template < class El, class Er, typename T, SizeType Dimension >
		inline T StaticArraySum<El, Er, T, Dimension>::operator[](const SizeType i) const
		{
			if (!_values[i]) {
				T t = _el.getAt(i) + _er.getAt(i);
				_values[i] = [t]()->T{ return t; };
			}
			return _values[i]();
		}



		template < class El, class Er, typename T, SizeType Dimension >
		inline StaticArraySub<El, Er, T, Dimension>::StaticArraySub(const El &el, const Er &er)
		    : _el(el), _er(er)
		{ }

		template < class El, class Er, typename T, SizeType Dimension >
		inline T StaticArraySub<El, Er, T, Dimension>::operator[](const SizeType i) const
		{
			if (!_values[i]) {
				T t = _el.getAt(i) - _er.getAt(i);
				_values[i] = [t]()->T{ return t; };
			}
			return _values[i]();
		}



		template < class El, class Er, typename T, SizeType Dimension >
		inline StaticArrayProd<El, Er, T, Dimension>::StaticArrayProd(const El &el, const Er &er)
		    : _el(el), _er(er)
		{ }

		template < class El, class Er, typename T, SizeType Dimension >
		inline T StaticArrayProd<El, Er, T, Dimension>::operator[](const SizeType i) const
		{
			if (!_values[i]) {
				T t = _el.getAt(i) * _er.getAt(i);
				_values[i] = [t]()->T{ return t; };
			}
			return _values[i]();
		}



		template < class El, class Er, typename T, SizeType Dimension >
		inline StaticArrayQuot<El, Er, T, Dimension>::StaticArrayQuot(const El &el, const Er &er)
		    : _el(el), _er(er)
		{ }

		template < class El, class Er, typename T, SizeType Dimension >
		inline T StaticArrayQuot<El, Er, T, Dimension>::operator[](const SizeType i) const
		{
			if (!_values[i]) {
				T t = _el.getAt(i) / _er.getAt(i);
				_values[i] = [t]()->T{ return t; };
			}
			return _values[i]();
		}

	}


	template < class El, class Er, typename T, SizeType Dimension >
	inline internal::StaticArraySum<El, Er, T, Dimension> operator+ (const internal::StaticArrayExpression<El, T, Dimension> &el, const internal::StaticArrayExpression<Er, T, Dimension> &er)
	{
		return internal::StaticArraySum<El, Er, T, Dimension>(el, er);
	}


	template < class El, class Er, typename T, SizeType Dimension >
	inline internal::StaticArraySub<El, Er, T, Dimension> operator- (const internal::StaticArrayExpression<El, T, Dimension> &el, const internal::StaticArrayExpression<Er, T, Dimension> &er)
	{
		return internal::StaticArraySub<El, Er, T, Dimension>(el, er);
	}


	template < class El, class Er, typename T, SizeType Dimension >
	inline internal::StaticArrayProd<El, Er, T, Dimension> operator* (const internal::StaticArrayExpression<El, T, Dimension> &el, const internal::StaticArrayExpression<Er, T, Dimension> &er)
	{
		return internal::StaticArrayProd<El, Er, T, Dimension>(el, er);
	}


	template < class El, class Er, typename T, SizeType Dimension >
	inline internal::StaticArrayQuot<El, Er, T, Dimension> operator/ (const internal::StaticArrayExpression<El, T, Dimension> &el, const internal::StaticArrayExpression<Er, T, Dimension> &er)
	{
		return internal::StaticArrayQuot<El, Er, T, Dimension>(el, er);
	}
	
}

#endif
