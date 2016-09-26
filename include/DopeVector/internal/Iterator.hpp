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
#include <DopeVector/DopeVector.hpp>
#include <DopeVector/Index.hpp>

namespace dope {

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

            using Data              = std::reference_wrapper<DopeVector<T, Dimension>>;
            using IndexD            = Index< Dimension >;
            using self_type         = Iterator;
            using difference_type   = std::make_signed<SizeType>;
            using value_type        = std::conditional<Const, std::add_const<T>::type, std::remove_const<T>::type>::type;
            using pointer           = std::conditional<Const, std::add_const<value_type>::type, std::remove_const<value_type>::type>::type;
            using reference         = value_type&;
            using iterator_category = std::conditional<Const, std::random_access_iterator_tag, output_random_access_iterator_tag>::type;

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
            explicit inline Iterator(const DopeVector<T, Dimension> &dope_vector, const SizeType i = static_cast<SizeType>(0));

            /**
             * @brief Constructor.
             */
            explicit inline Iterator(const DopeVector<T, Dimension> &dope_vector, const IndexD &index);


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
            // DESTRUCTORS
            ////////////////////////////////////////////////////////////////////////

            /**
             * @brief Destructor.
             */
            ~Iterator();

            ////////////////////////////////////////////////////////////////////////



            ////////////////////////////////////////////////////////////////////////
            // INFORMATION
            ////////////////////////////////////////////////////////////////////////

            inline SizeType to_original() const;
            inline SizeType to_position() const;
            inline IndexD   to_index()    const;

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

            inline self_type& operator++();
            inline self_type  operator++(int);
            inline self_type& operator+=(const SizeType n);
            inline self_type& operator+=(const IndexD  &n);
            inline self_type  operator+ (const SizeType n) const;
            inline self_type  operator+ (const IndexD  &n) const;
            friend inline self_type operator+(const SizeType n, const self_type &it);
            friend inline self_type operator+(const IndexD  &n, const self_type &it);

            ////////////////////////////////////////////////////////////////////////



            ////////////////////////////////////////////////////////////////////////
            // DECREMENT OPERATIONS
            ////////////////////////////////////////////////////////////////////////

            inline self_type&      operator--();
            inline self_type       operator--(int);
            inline self_type&      operator-=(const SizeType n);
            inline self_type&      operator-=(const IndexD  &n);
            inline self_type       operator- (const SizeType n) const;
            inline self_type       operator- (const IndexD  &n) const;
            inline difference_type operator- (const self_type &o) const;

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



            ////////////////////////////////////////////////////////////////////////
            // SWAPS
            ////////////////////////////////////////////////////////////////////////

            inline void swap(self_type &o);
            friend inline void swap(self_type &a, self_type &b);

            ////////////////////////////////////////////////////////////////////////



        private:
            Data     _data;     ///< A reference to the pointed DopeVector.
            SizeType _i;        ///< The position of the pointed data in the pointed DopeVector.
        };
	}

}

#include <DopeVector/internal/inlines/Iterator.inl>

#endif // Iterator_hpp
