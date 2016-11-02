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



		template < class E, typename T, typename Op, SizeType Dimension >
		const Op StaticArrayUnaryExpression<E, T, Op, Dimension>::_op = Op();

		template < class E, typename T, typename Op, SizeType Dimension >
		inline StaticArrayUnaryExpression<E, T, Op, Dimension>::StaticArrayUnaryExpression(const E &e)
		    : _e(e)
		{ }

		template < class E, typename T, typename Op, SizeType Dimension >
		inline T StaticArrayUnaryExpression<E, T, Op, Dimension>::operator[](const SizeType i) const
		{
			if (!_values[i]) {
				T t = _op(_e.getAt(i));
				_values[i] = [t]()->T{ return t; };
			}
			return _values[i]();
		}



		template < class El, class Er, typename T, typename Op, SizeType Dimension >
		const Op StaticArrayBinaryExpression<El, Er, T, Op, Dimension>::_op = Op();

		template < class El, class Er, typename T, typename Op, SizeType Dimension >
		inline StaticArrayBinaryExpression<El, Er, T, Op, Dimension>::StaticArrayBinaryExpression(const El &el, const Er &er)
		    : _el(el), _er(er)
		{ }

		template < class El, class Er, typename T, typename Op, SizeType Dimension >
		inline T StaticArrayBinaryExpression<El, Er, T, Op, Dimension>::operator[](const SizeType i) const
		{
			if (!_values[i]) {
				T t = _op(_el.getAt(i), _er.getAt(i));
				_values[i] = [t]()->T{ return t; };
			}
			return _values[i]();
		}


		template < class E, typename T, SizeType Dimension >
		inline internal::StaticArrayUnaryExpression<E, T, std::negate<T>, Dimension> operator- (const internal::StaticArrayExpression<E, T, Dimension> &e)
		{
			return internal::StaticArrayUnaryExpression<E, T, std::negate<T>, Dimension>(e);
		}



		template < class El, class Er, typename T, SizeType Dimension >
		inline internal::StaticArrayBinaryExpression<El, Er, T, std::plus<T>, Dimension> operator+ (const internal::StaticArrayExpression<El, T, Dimension> &el, const internal::StaticArrayExpression<Er, T, Dimension> &er)
		{
			return internal::StaticArrayBinaryExpression<El, Er, T, std::plus<T>, Dimension>(el, er);
		}



		template < class El, class Er, typename T, SizeType Dimension >
		inline internal::StaticArrayBinaryExpression<El, Er, T, std::minus<T>, Dimension> operator- (const internal::StaticArrayExpression<El, T, Dimension> &el, const internal::StaticArrayExpression<Er, T, Dimension> &er)
		{
			return internal::StaticArrayBinaryExpression<El, Er, T, std::minus<T>, Dimension>(el, er);
		}



		template < class El, class Er, typename T, SizeType Dimension >
		inline internal::StaticArrayBinaryExpression<El, Er, T, std::multiplies<T>, Dimension> operator* (const internal::StaticArrayExpression<El, T, Dimension> &el, const internal::StaticArrayExpression<Er, T, Dimension> &er)
		{
			return internal::StaticArrayBinaryExpression<El, Er, T, std::multiplies<T>, Dimension>(el, er);
		}



		template < class El, class Er, typename T, SizeType Dimension >
		inline internal::StaticArrayBinaryExpression<El, Er, T, std::divides<T>, Dimension> operator/ (const internal::StaticArrayExpression<El, T, Dimension> &el, const internal::StaticArrayExpression<Er, T, Dimension> &er)
		{
			return internal::StaticArrayBinaryExpression<El, Er, T, std::divides<T>, Dimension>(el, er);
		}



		template < class El, class Er, typename T, SizeType Dimension >
		inline internal::StaticArrayBinaryExpression<El, Er, T, std::modulus<T>, Dimension> operator% (const internal::StaticArrayExpression<El, T, Dimension> &el, const internal::StaticArrayExpression<Er, T, Dimension> &er)
		{
			return internal::StaticArrayBinaryExpression<El, Er, T, std::modulus<T>, Dimension>(el, er);
		}

	}
	
}

#endif
