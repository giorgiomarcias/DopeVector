// Copyright (c) 2016 Giorgio Marcias & Maurizio Kovacic
//
// This source code is part of DopeVector header library
// and it is subject to Apache 2.0 License.
//
// Author: Giorgio Marcias
// email: marcias.giorgio@gmail.com
// Author: Maurizio Kovacic
// email: maurizio.kovacic@gmail.com

#include <DopeVector/internal/Iterator.hpp>
#include <limits>

namespace dope {

	namespace internal {

        ////////////////////////////////////////////////////////////////////////
        // CONSTRUCTORS
        ////////////////////////////////////////////////////////////////////////

        template < typename T, SizeType Dimension, bool Const >
        Iterator<T, Dimension, Const>::Iterator() : _data(), _i(std::numeric_limits<SizeType>::max()) {}

        template < typename T, SizeType Dimension, bool Const >
        Iterator<T, Dimension, Const>::Iterator(const DopeVector<T, Dimension> &dope_vector,
                                                const SizeType i) : _data(dope_vector), _i(i) {}

        template < typename T, SizeType Dimension, bool Const >
        Iterator<T, Dimension, Const>::Iterator(const DopeVector<T, Dimension> &dope_vector,
                                                const IndexD &index) : _data(dope_vector), _i(0) {
            _i = to_position(index, _data._range);
        }

        ////////////////////////////////////////////////////////////////////////



        ////////////////////////////////////////////////////////////////////////
        // DESTRUCTORS
        ////////////////////////////////////////////////////////////////////////

        template < typename T, SizeType Dimension, bool Const >
        Iterator<T, Dimension, Const>::~Iterator() {}

        ////////////////////////////////////////////////////////////////////////



        ////////////////////////////////////////////////////////////////////////
        // INFORMATION
        ////////////////////////////////////////////////////////////////////////

        template < typename T, SizeType Dimension, bool Const >
        inline SizeType Iterator<T, Dimension, Const>::to_original() const {
            const IndexD index = to_index();
            SizeType result = _data._accumulatedOffset;
            result += dope::to_position(index, _data._range, _data._offset);
            return result;
        }

        template < typename T, SizeType Dimension, bool Const >
        inline SizeType Iterator<T, Dimension, Const>::to_position() const {
            return _i;
        }

        template < typename T, SizeType Dimension, bool Const >
        inline typename Iterator<T, Dimension, Const>::IndexD Iterator<T, Dimension, Const>::to_index() const {
            return dope::to_index(_i, _data._range);
        }

        ////////////////////////////////////////////////////////////////////////



        ////////////////////////////////////////////////////////////////////////
        // DATA ACCESS METHODS
        ////////////////////////////////////////////////////////////////////////

        template < typename T, SizeType Dimension, bool Const >
        inline typename Iterator<T, Dimension, Const>::reference Iterator<T, Dimension, Const>::operator*() {
            return *operator ->();
        }

        template < typename T, SizeType Dimension, bool Const >
        inline typename Iterator<T, Dimension, Const>::pointer Iterator<T, Dimension, Const>::operator->() {

            return (_data._array + to_original());
        }

        template < typename T, SizeType Dimension, bool Const >
        inline typename Iterator<T, Dimension, Const>::reference Iterator<T, Dimension, Const>::operator[](const SizeType n) const {
            return *(*this + n);
        }

        template < typename T, SizeType Dimension, bool Const >
        inline typename Iterator<T, Dimension, Const>::reference Iterator<T, Dimension, Const>::operator[](const IndexD &n) const {
            return *(*this + n);
        }

        ////////////////////////////////////////////////////////////////////////



        ////////////////////////////////////////////////////////////////////////
        // INCREMENT OPERATIONS
        ////////////////////////////////////////////////////////////////////////

        template < typename T, SizeType Dimension, bool Const >
        inline typename Iterator<T, Dimension, Const>::self_type& Iterator<T, Dimension, Const>::operator++() {
            ++_i;
            return *this;
        }

        template < typename T, SizeType Dimension, bool Const >
        inline typename Iterator<T, Dimension, Const>::self_type Iterator<T, Dimension, Const>::operator++(int) {
            self_type copy(*this);
            ++*this;
            return copy;
        }

        template < typename T, SizeType Dimension, bool Const >
        inline typename Iterator<T, Dimension, Const>::self_type& Iterator<T, Dimension, Const>::operator+=(const SizeType n) {
            _i += n;
            return *this;
        }

        template < typename T, SizeType Dimension, bool Const >
        inline typename Iterator<T, Dimension, Const>::self_type& Iterator<T, Dimension, Const>::operator+=(const IndexD &n) {
            return (*this += to_position(n, _data._range));
        }

        template < typename T, SizeType Dimension, bool Const >
        inline typename Iterator<T, Dimension, Const>::self_type Iterator<T, Dimension, Const>::operator+ (const SizeType n) const {
            self_type copy(*this);
            copy += n;
            return copy;
        }

        template < typename T, SizeType Dimension, bool Const >
        inline typename Iterator<T, Dimension, Const>::self_type Iterator<T, Dimension, Const>::operator+ (const IndexD &n) const {
            self_type copy(*this);
            copy += n;
            return copy;
        }

        template < typename T, SizeType Dimension, bool Const >
        inline Iterator<T, Dimension, Const> operator+(const SizeType n, const Iterator<T, Dimension, Const> &it) {
            return it + n;
        }

        template < typename T, SizeType Dimension, bool Const >
        inline Iterator<T, Dimension, Const> operator+(const Index<Dimension> &n, const Iterator<T, Dimension, Const> &it) {
            return it + n;
        }

        ////////////////////////////////////////////////////////////////////////



        ////////////////////////////////////////////////////////////////////////
        // DECREMENT OPERATIONS
        ////////////////////////////////////////////////////////////////////////

        template < typename T, SizeType Dimension, bool Const >
        inline typename Iterator<T, Dimension, Const>::self_type& Iterator<T, Dimension, Const>::operator--() {
            --_i;
            return *this;
        }

        template < typename T, SizeType Dimension, bool Const >
        inline typename Iterator<T, Dimension, Const>::self_type Iterator<T, Dimension, Const>::operator--(int) {
            self_type copy(*this);
            --*this;
            return copy;
        }

        template < typename T, SizeType Dimension, bool Const >
        inline typename Iterator<T, Dimension, Const>::self_type& Iterator<T, Dimension, Const>::operator-=(const SizeType n) {
            _i -= n;
            return *this;
        }

        template < typename T, SizeType Dimension, bool Const >
        inline typename Iterator<T, Dimension, Const>::self_type& Iterator<T, Dimension, Const>::operator-=(const IndexD &n) {
            return (*this -= n);
        }

        template < typename T, SizeType Dimension, bool Const >
        inline typename Iterator<T, Dimension, Const>::self_type Iterator<T, Dimension, Const>::operator- (const SizeType n) const {
            self_type copy(*this);
            copy -= n;
            return copy;
        }

        template < typename T, SizeType Dimension, bool Const >
        inline typename Iterator<T, Dimension, Const>::self_type Iterator<T, Dimension, Const>::operator- (const IndexD &n) const {
            self_type copy(*this);
            copy -= n;
            return copy;
        }

        template < typename T, SizeType Dimension, bool Const >
        inline typename Iterator<T, Dimension, Const>::difference_type Iterator<T, Dimension, Const>::operator- (const self_type &o) const {
            if(_data == o._data)
                return (_i - o._i);
            return std::numeric_limits<difference_type>::max();
        }

        ////////////////////////////////////////////////////////////////////////



        ////////////////////////////////////////////////////////////////////////
        // BOOLEAN OPERATIONS
        ////////////////////////////////////////////////////////////////////////

        template < typename T, SizeType Dimension, bool Const >
        inline bool Iterator<T, Dimension, Const>::operator==(const self_type &o) const {
            return ((_data == o._data) && (_i == o._i));
        }

        template < typename T, SizeType Dimension, bool Const >
        inline bool Iterator<T, Dimension, Const>::operator!=(const self_type &o) const {
            return ((_data == o._data) && (_i != o._i));
        }

        template < typename T, SizeType Dimension, bool Const >
        inline bool Iterator<T, Dimension, Const>::operator< (const self_type &o) const {
            return ((_data == o._data) && (_i < o._i);
        }

        template < typename T, SizeType Dimension, bool Const >
        inline bool Iterator<T, Dimension, Const>::operator<=(const self_type &o) const {
            return ((_data == o._data) && (_i <= o._i);
        }

        template < typename T, SizeType Dimension, bool Const >
        inline bool Iterator<T, Dimension, Const>::operator> (const self_type &o) const {
            return ((_data == o._data) && (_i > o._i);
        }

        template < typename T, SizeType Dimension, bool Const >
        inline bool Iterator<T, Dimension, Const>::operator>=(const self_type &o) const {
            return ((_data == o._data) && (_i >= o._i);
        }

        ////////////////////////////////////////////////////////////////////////



        ////////////////////////////////////////////////////////////////////////
        // SWAPS
        ////////////////////////////////////////////////////////////////////////

        template < typename T, SizeType Dimension, bool Const >
        inline void Iterator<T, Dimension, Const>::swap(self_type &o) {
            std::swap(_data, o._data);
            std::swap(_i, o._i);
        }

        template < typename T, SizeType Dimension, bool Const >
        inline void swap(Iterator<T, Dimension, Const> &a, Iterator<T, Dimension, Const> &b) {
            a.swap(b);
        }

        ////////////////////////////////////////////////////////////////////////



    } // namespace internal

} // namespace dope
