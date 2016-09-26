// Copyright (c) 2016 Giorgio Marcias & Maurizio Kovacic
//
// This source code is part of DopeVector header library
// and it is subject to Apache 2.0 License.
//
// Author: Giorgio Marcias
// email: marcias.giorgio@gmail.com
// Author: Maurizio Kovacic
// email: maurizio.kovacic@gmail.com

#ifndef Iterator_hpp
#define Iterator_hpp

#include <functional>
#include <iterator>
#include <type_traits>
#include <DopeVector/Index.hpp>

namespace dope {

	template < typename T, SizeType Dimension > class DopeVector;

	namespace internal {

		struct output_random_access_iterator_tag : public std:: output_iterator_tag, public std:: random_access_iterator_tag { };

		/**
		 * @brief The Iterator class
		 */
		template < typename T, SizeType Dimension, bool Const >
		class Iterator {
		public:

			////////////////////////////////////////////////////////////////////////
			// TYPEDEFS
			////////////////////////////////////////////////////////////////////////

			using difference_type   = std::make_signed<SizeType>::type;
			using value_type        = typename std::conditional<Const, typename std::add_const<T>::type, typename std::remove_const<T>::type>::type;
			using pointer           = typename std::add_pointer<typename std::conditional<Const, typename std::add_const<value_type>::type, typename std::remove_const<value_type>::type>::type>::type;
			using reference         = typename std::add_lvalue_reference<value_type>::type;
			using iterator_category = typename std::conditional<Const, std::random_access_iterator_tag, output_random_access_iterator_tag>::type;

			using DopeVectorType    = typename std::conditional<Const, typename std::add_const<DopeVector<T, Dimension>>::type, typename std::remove_const<DopeVector<T, Dimension>>::type>::type;
			using DopeVectorRef     = std::reference_wrapper<DopeVectorType>;
			using IndexD            = Index< Dimension >;
			using self_type         = Iterator;

			////////////////////////////////////////////////////////////////////////



			////////////////////////////////////////////////////////////////////////
			// CONSTRUCTORS
			////////////////////////////////////////////////////////////////////////

			/**
			 * @brief Default constructor.
			 */
			explicit inline Iterator();

			/**
			 * @brief Constructor.
			 */
			explicit inline Iterator(DopeVectorType &dope_vector, const SizeType i = static_cast<SizeType>(0));

			/**
			 * @brief Constructor.
			 */
			explicit inline Iterator(DopeVectorType &dope_vector, const IndexD &index);


			/**
			 * @brief Copy constructor.
			 */
			explicit inline Iterator(const Iterator& other ) = default;

			/**
			 * @brief Move constructor.
			 */
			explicit inline Iterator(Iterator&& other ) = default;

			////////////////////////////////////////////////////////////////////////



			////////////////////////////////////////////////////////////////////////
			// INFORMATION
			////////////////////////////////////////////////////////////////////////

			inline SizeType         to_original() const;
			inline SizeType         to_position() const;
			inline const IndexD &   to_index()    const;

			////////////////////////////////////////////////////////////////////////



			////////////////////////////////////////////////////////////////////////
			// ASSIGNMENT
			////////////////////////////////////////////////////////////////////////

			inline self_type& operator=(const self_type &o) = default;
			inline self_type& operator=(self_type &&o) = default;

			////////////////////////////////////////////////////////////////////////



			////////////////////////////////////////////////////////////////////////
			// DATA ACCESS METHODS
			////////////////////////////////////////////////////////////////////////

			inline reference operator* ();
			inline pointer   operator->();
			inline reference operator[](const SizeType n) const;
			inline reference operator[](const IndexD  &n) const;

			////////////////////////////////////////////////////////////////////////



			////////////////////////////////////////////////////////////////////////
			// INCREMENT OPERATIONS
			////////////////////////////////////////////////////////////////////////

			inline self_type & operator++();
			inline self_type   operator++(int);
			inline self_type & operator+=(const SizeType n);
			inline self_type   operator+ (const SizeType n) const;
			template < class E >
			inline self_type & operator+=(const internal::StaticArrayExpression<E, SizeType, Dimension> &n);
			template < class E >
			inline self_type   operator+ (const internal::StaticArrayExpression<E, SizeType, Dimension> &n) const;

			////////////////////////////////////////////////////////////////////////



			////////////////////////////////////////////////////////////////////////
			// DECREMENT OPERATIONS
			////////////////////////////////////////////////////////////////////////

			inline self_type &      operator--();
			inline self_type        operator--(int);
			inline self_type &      operator-=(const SizeType n);
			inline self_type        operator- (const SizeType n) const;
			template < class E >
			inline self_type &      operator-=(const internal::StaticArrayExpression<E, SizeType, Dimension> &n);
			template < class E >
			inline self_type        operator- (const internal::StaticArrayExpression<E, SizeType, Dimension> &n) const;
			inline difference_type  operator- (const self_type &o) const;

			////////////////////////////////////////////////////////////////////////



			////////////////////////////////////////////////////////////////////////
			// BOOLEAN OPERATIONS
			////////////////////////////////////////////////////////////////////////

			inline bool operator==(const self_type &o) const;
			inline bool operator!=(const self_type &o) const;
			inline bool operator< (const self_type &o) const;
			inline bool operator<=(const self_type &o) const;
			inline bool operator> (const self_type &o) const;
			inline bool operator>=(const self_type &o) const;

			////////////////////////////////////////////////////////////////////////



		private:
			DopeVectorRef       _data;          ///< A reference to the pointed DopeVector.
			IndexD              _currentIndex;  ///< The current position.
		};
	}

}

#include <DopeVector/internal/inlines/Iterator.inl>

#endif // Iterator_hpp
