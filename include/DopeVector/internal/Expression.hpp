// Copyright (c) 2016 Giorgio Marcias & Maurizio Kovacic
//
// This source code is part of DopeVector header library
// and it is subject to Apache 2.0 License.
//
// Author: Giorgio Marcias
// email: marcias.giorgio@gmail.com
// Author: Maurizio Kovacic
// email: maurizio.kovacic@gmail.com

#ifndef Expression_hpp
#define Expression_hpp

#ifndef DOPE_USE_EIGEN_INDEX
#include <DopeVector/internal/Common.hpp>
#include <array>
#include <functional>

namespace dope {

	namespace internal {

		template < class E, typename T, SizeType Dimension >
		class StaticArrayExpression {
		public:
			inline T getAt(const SizeType i) const;

			inline operator E const&() const;

			inline operator E&();
		};



		template < class E, typename T, typename Op, SizeType Dimension >
		class StaticArrayUnaryExpression : public StaticArrayExpression<StaticArrayUnaryExpression<E, T, Op, Dimension>, T, Dimension> {
		private:
			const E                                           &_e;
			mutable std::array<std::function<T()>, Dimension>  _values;
			static const Op                                    _op;

		public:
			inline StaticArrayUnaryExpression(const E &e);

			inline T operator[](const SizeType i) const;
		};



		template < class El, class Er, typename T, typename Op, SizeType Dimension >
		class StaticArrayBinaryExpression : public StaticArrayExpression<StaticArrayBinaryExpression<El, Er, T, Op, Dimension>, T, Dimension> {
		private:
			const El                                          &_el;
			const Er	                                          &_er;
			mutable std::array<std::function<T()>, Dimension>  _values;
			static const Op                                    _op;

		public:
			inline StaticArrayBinaryExpression(const El &el, const Er &er);

			inline T operator[](const SizeType i) const;
		};



		template < class E, typename T, SizeType Dimension >
		inline internal::StaticArrayUnaryExpression<E, T, std::negate<T>, Dimension> operator- (const internal::StaticArrayExpression<E, T, Dimension> &e);



		template < class El, class Er, typename T, SizeType Dimension >
		inline internal::StaticArrayBinaryExpression<El, Er, T, std::plus<T>, Dimension> operator+ (const internal::StaticArrayExpression<El, T, Dimension> &el, const internal::StaticArrayExpression<Er, T, Dimension> &er);



		template < class El, class Er, typename T, SizeType Dimension >
		inline internal::StaticArrayBinaryExpression<El, Er, T, std::minus<T>, Dimension> operator- (const internal::StaticArrayExpression<El, T, Dimension> &el, const internal::StaticArrayExpression<Er, T, Dimension> &er);


		template < class El, class Er, typename T, SizeType Dimension >
		inline internal::StaticArrayBinaryExpression<El, Er, T, std::multiplies<T>, Dimension> operator* (const internal::StaticArrayExpression<El, T, Dimension> &el, const internal::StaticArrayExpression<Er, T, Dimension> &er);



		template < class El, class Er, typename T, SizeType Dimension >
		inline internal::StaticArrayBinaryExpression<El, Er, T, std::divides<T>, Dimension> operator/ (const internal::StaticArrayExpression<El, T, Dimension> &el, const internal::StaticArrayExpression<Er, T, Dimension> &er);



		template < class El, class Er, typename T, SizeType Dimension >
		inline internal::StaticArrayBinaryExpression<El, Er, T, std::modulus<T>, Dimension> operator% (const internal::StaticArrayExpression<El, T, Dimension> &el, const internal::StaticArrayExpression<Er, T, Dimension> &er);

	}

}

#include <DopeVector/internal/inlines/Expression.inl>

#endif

#endif
