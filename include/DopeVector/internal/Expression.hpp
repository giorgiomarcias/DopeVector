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



		template < typename T, SizeType Dimension >
		class ConstantExpression : public StaticArrayExpression<ConstantExpression<T, Dimension>, T, Dimension> {
		private:
			T _t;
		public:
			const T& operator[](const SizeType) const;
		};



		template < class El, class Er, typename T, SizeType Dimension >
		class StaticArraySum : public StaticArrayExpression<StaticArraySum<El, Er, T, Dimension>, T, Dimension> {
		private:
			const El                                          &_el;
			const Er	                                          &_er;
			mutable std::array<std::function<T()>, Dimension>  _values;

		public:
			inline StaticArraySum(const El &el, const Er &er);

			inline T operator[](const SizeType i) const;
		};



		template < class El, class Er, typename T, SizeType Dimension >
		class StaticArraySub : public StaticArrayExpression<StaticArraySub<El, Er, T, Dimension>, T, Dimension> {
		private:
			const El                                          &_el;
			const Er	                                          &_er;
			mutable std::array<std::function<T()>, Dimension>  _values;

		public:
			inline StaticArraySub(const El &el, const Er &er);

			inline T operator[](const SizeType i) const;
		};



		template < class El, class Er, typename T, SizeType Dimension >
		class StaticArrayProd : public StaticArrayExpression<StaticArrayProd<El, Er, T, Dimension>, T, Dimension> {
		private:
			const El                                          &_el;
			const Er	                                          &_er;
			mutable std::array<std::function<T()>, Dimension>  _values;

		public:
			inline StaticArrayProd(const El &el, const Er &er);

			inline T operator[](const SizeType i) const;
		};



		template < class El, class Er, typename T, SizeType Dimension >
		class StaticArrayQuot : public StaticArrayExpression<StaticArrayQuot<El, Er, T, Dimension>, T, Dimension> {
		private:
			const El                                          &_el;
			const Er	                                          &_er;
			mutable std::array<std::function<T()>, Dimension>  _values;

		public:
			inline StaticArrayQuot(const El &el, const Er &er);

			inline T operator[](const SizeType i) const;
		};

	}




	template < class El, class Er, typename T, SizeType Dimension >
	inline internal::StaticArraySum<El, Er, T, Dimension> operator+ (const internal::StaticArrayExpression<El, T, Dimension> &el, const internal::StaticArrayExpression<Er, T, Dimension> &er);


	template < class El, class Er, typename T, SizeType Dimension >
	inline internal::StaticArraySub<El, Er, T, Dimension> operator- (const internal::StaticArrayExpression<El, T, Dimension> &el, const internal::StaticArrayExpression<Er, T, Dimension> &er);


	template < class El, class Er, typename T, SizeType Dimension >
	inline internal::StaticArrayProd<El, Er, T, Dimension> operator* (const internal::StaticArrayExpression<El, T, Dimension> &el, const internal::StaticArrayExpression<Er, T, Dimension> &er);


	template < class El, class Er, typename T, SizeType Dimension >
	inline internal::StaticArrayQuot<El, Er, T, Dimension> operator/ (const internal::StaticArrayExpression<El, T, Dimension> &el, const internal::StaticArrayExpression<Er, T, Dimension> &er);
	
}

#include <DopeVector/internal/inlines/Expression.inl>

#endif

#endif
