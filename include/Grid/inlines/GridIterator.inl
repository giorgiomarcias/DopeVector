#include <Grid/GridIterator.hpp>

#include <limits>

namespace container {

    template < typename T, SizeType Dimension >
    inline GridIterator<T, Dimension>::GridIterator() :
        _ptr(nullptr),
        _range(IndexD::Zero()),
        _i(0) {}

    template < typename T, SizeType Dimension >
    inline GridIterator<T, Dimension>::GridIterator(pointer        ptr,
                                                    const IndexD  &range,
                                                    const SizeType i) :
        _ptr(ptr),
        _range(range),
        _i(i) {}

    template < typename T, SizeType Dimension >
    inline GridIterator<T, Dimension>::GridIterator(pointer       ptr,
                                                    const IndexD &range,
                                                    const IndexD &index) :
        _ptr(ptr),
        _range(range),
        _i(to_position(index, range)) {}



    template < typename T, SizeType Dimension >
    inline GridIterator<T, Dimension>::~GridIterator() {}



    template < typename T, SizeType Dimension >
    inline SizeType GridIterator<T, Dimension>::to_position() const {
        return _i;
    }

    template < typename T, SizeType Dimension >
    inline typename GridIterator<T, Dimension>::IndexD GridIterator<T, Dimension>::to_index() const {
        IndexD id(static_cast<SizeType>(0));
        SizeType i = _i;
        for(SizeType d = static_cast<SizeType>(0); d < Dimension; ++d) {
            id[d] = i % _range[d];
            i = i / _range[d];
        }
        return id;
    }



    template < typename T, SizeType Dimension >
    inline bool GridIterator<T, Dimension>::operator==(const self_type &o) const {
        return (((*this - o) == 0) && ((_range == o._range) && (_i == o._i)) );
    }

    template < typename T, SizeType Dimension >
    inline bool GridIterator<T, Dimension>::operator!=(const self_type &o) const {
        return !(*this == o);
    }



    template < typename T, SizeType Dimension >
    inline typename GridIterator<T, Dimension>::reference GridIterator<T, Dimension>::operator*() {
        return *_ptr;
    }

    template < typename T, SizeType Dimension >
    inline typename GridIterator<T, Dimension>::pointer GridIterator<T, Dimension>::operator->() {
        return _ptr;
    }



    template < typename T, SizeType Dimension >
    inline GridIterator<T, Dimension>& GridIterator<T, Dimension>::operator++() {
        ++_ptr;
        ++_i;
        return *this;
    }

    template < typename T, SizeType Dimension >
    inline GridIterator<T, Dimension> GridIterator<T, Dimension>::operator++(int) {
        self_type copy(*this);
        ++*this;
        return copy;
    }

    template < typename T, SizeType Dimension >
    inline GridIterator<T, Dimension>& GridIterator<T, Dimension>::operator--() {
        --_ptr;
        --_i;
        return *this;
    }

    template < typename T, SizeType Dimension >
    inline GridIterator<T, Dimension> GridIterator<T, Dimension>::operator--(int) {
        self_type copy(*this);
        --*this;
        return copy;
    }



    template < typename T, SizeType Dimension >
    inline GridIterator<T, Dimension> GridIterator<T, Dimension>::operator+(const SizeType n) const {
        self_type copy(*this);
        copy += n;
        return copy;
    }

    template < typename T, SizeType Dimension >
    inline GridIterator<T, Dimension> operator+(const SizeType n, const typename GridIterator<T, Dimension>::self_type &it) {
        return (it + n);
    }

    template < typename T, SizeType Dimension >
    inline GridIterator<T, Dimension> GridIterator<T, Dimension>::operator+(const IndexD &n) const {
        return (*this + to_position(n, _range));
    }

    template < typename T, SizeType Dimension >
    inline GridIterator<T, Dimension> operator+(const typename GridIterator<T, Dimension>::IndexD &n, const typename GridIterator<T, Dimension>::self_type &it) {
        return (it + n);
    }

    template < typename T, SizeType Dimension >
    inline GridIterator<T, Dimension> GridIterator<T, Dimension>::operator-(const SizeType n) const {
        self_type copy(*this);
        copy -= n;
        return copy;
    }

    template < typename T, SizeType Dimension >
    inline GridIterator<T, Dimension> GridIterator<T, Dimension>::operator-(const IndexD &n) const {
        return (*this - to_position(n, _range));
    }



    template < typename T, SizeType Dimension >
    inline typename GridIterator<T, Dimension>::difference_type GridIterator<T, Dimension>::operator-(const self_type &o) const {
        if(_range == o._range) {
            return (_ptr - o._ptr);
        }
        return std::numeric_limits<difference_type>::infinity();
    }

    template < typename T, SizeType Dimension >
    inline bool GridIterator<T, Dimension>::operator<(const self_type &o) const {
        if(_range == o._range) {
            return ((*this - o) > 0);
        }
        return false;
    }

    template < typename T, SizeType Dimension >
    inline bool GridIterator<T, Dimension>::operator<=(const self_type &o) const {
        return ((*this == o) || (*this < 0));
    }

    template < typename T, SizeType Dimension >
    inline bool GridIterator<T, Dimension>::operator>(const self_type &o) const {
        return (!(*this == o) && !(*this < 0));
    }

    template < typename T, SizeType Dimension >
    inline bool GridIterator<T, Dimension>::operator>=(const self_type &o) const {
        return ((*this == o) || !(*this < 0));
    }



    template < typename T, SizeType Dimension >
    inline GridIterator<T, Dimension>& GridIterator<T, Dimension>::operator+=(const SizeType n) {
        _ptr += n;
        _i += n;
        return *this;
    }

    template < typename T, SizeType Dimension >
    inline GridIterator<T, Dimension>& GridIterator<T, Dimension>::operator-=(const SizeType n) {
        _ptr -= n;
        _i -= n;
        return *this;
    }

    template < typename T, SizeType Dimension >
    inline GridIterator<T, Dimension>& GridIterator<T, Dimension>::operator+=(const IndexD &n) {
        return (*this += to_position(n, _range));
    }

    template < typename T, SizeType Dimension >
    inline GridIterator<T, Dimension>& GridIterator<T, Dimension>::operator-=(const IndexD &n) {
        return (*this -= to_position(n, _range));
    }



    template < typename T, SizeType Dimension >
    inline typename GridIterator<T, Dimension>::reference GridIterator<T, Dimension>::operator[](const SizeType n) const {
        return *(*this + n);
    }

    template < typename T, SizeType Dimension >
    inline typename GridIterator<T, Dimension>::reference GridIterator<T, Dimension>::operator[](const IndexD &n) const {
        return *(*this + n);
    }



    template < typename T, SizeType Dimension >
    inline void GridIterator<T, Dimension>::swap(self_type &o) {
        std::swap(_ptr, o._ptr);
        std::swap(_range, o._range);
        std::swap(_i, o._i);
    }

    template < typename T, SizeType Dimension >
    inline void swap(GridIterator<T, Dimension> &it0, GridIterator<T, Dimension> &it1) {
        it0.swap(it1);
    }

}
