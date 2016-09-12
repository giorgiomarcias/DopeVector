#ifndef Grid_hpp
#define Grid_hpp

#include <vector>
#include <DopeVector/DopeVector.hpp>

namespace Container {

template < SizeType Dimension >
static SizeType to_position(const Index<Dimension> &index, const Index<Dimension> &range) {
    SizeType pos = static_cast<SizeType>(0);
    SizeType dimProd = static_cast<SizeType>(1);
    for(SizeType i = static_cast<SizeType>(0); i < Dimension; ++i) {
        pos += index[i] * dimProd;
        dimProd *= range[i];
    }
    return pos;
}

template < SizeType Dimension >
static SizeType to_index(const SizeType position, const Index<Dimension> &range) {
    Index<Dimension> result(static_cast<SizeType>(0));
    SizeType i = position;
    for(SizeType d = 0; d < Dimension ; ++d) {
        result[d] = i % range[d];
        linIdx = i / range[d];
    }
    return result;
}


template < typename T, SizeType Dimension, class Allocator = std::allocator< T >, SizeType ... Args >
class Grid : public DopeVector< T, Dimension, Args ... > {
public:
    typedef std::vector< T, Allocator >  Data;
    typedef class iterator_base<T>             iterator;
    typedef class iterator_base<const T>       const_iterator;

    template < typename Type >
    class iterator_base {
    public:
        typedef iterator_base                   self_type;
        typedef Type                            value_type;
        typedef Type&                           reference;
        typedef Type*                           pointer;
        typedef std::random_access_iterator_tag iterator_category;
        typedef int                             difference_type;

        inline iterator_base() : _ptr(nullptr), _range(IndexD::Zero()), _i(0) {}
        inline iterator_base(pointer ptr, const IndexD &range, const SizeType i) : _ptr(ptr), _range(range), _i(i) {}
        inline iterator_base(pointer ptr, const IndexD &range, const IndexD &index) : _ptr(ptr), _range(range), _i(to_position(index, range)) {}
        inline iterator_base(const self_type &o) = default;
        inline iterator_base(self_type &&o) = default;
        inline ~iterator_base() {}

        inline SizeType to_position() const { return _i; }

        inline IndexD to_index() const {
            IndexD id(static_cast<SizeType>(0));
            SizeType i = _i;
            for(SizeType d = static_cast<SizeType>(0); d < Dimension; ++d) {
                id[d] = i % _size[d];
                i = i / _size[d];
            }
            return id;
        }

        inline bool operator==(const self_type &o) const { return (((*this - o) == 0) && ((_range == o._range) && (_i == o._i)) ); }
        inline bool operator!=(const self_type &o) const { return !(*this == o); }
        inline self_type& operator=(const self_type &o) = default;

        inline reference operator*() { return *_ptr; }
        inline pointer operator->() { return _ptr; }

        inline self_type& operator++() { ++_ptr; ++_i; return *this; }
        inline self_type operator++(int) { self_type copy(*this); ++*this; return copy; }

        inline self_type& operator--() { --_ptr; --_i; return *this; }
        inline self_type operator--(int) { self_type copy(*this); --*this; return copy; }

        inline self_type operator+(const SizeType n) const { self_type copy(*this); copy += n; return copy; }
        friend inline self_type operator+(const SizeType n, const self_type &it) { return (it + n); }
        inline self_type operator+(const IndexD &n) const { return (*this + Container::to_position(n, it._range)); }
        friend inline self_type operator+(const IndexD &n, const self_type &it) { return (it + n); }
        inline self_type operator-(const SizeType n) const { self_type copy(*this); copy -= n; return copy; }
        inline self_type operator-(const IndexD &n) const { return (*this - Container::to_position(n, _range)); }
        inline difference_type operator-(const self_type &o) const { if(_range == o._range) return (_ptr - o._ptr); }

        inline bool operator<(const self_type &o) const { if(_range == o._range) return ((*this - o) > 0); return false; }
        inline bool operator<=(const self_type &o) const { return ((*this == o) || (*this < 0)); }
        inline bool operator>(const self_type &o) const { return (!(*this == o) && !(*this < 0)); }
        inline bool operator>=(const self_type &o) const { return ((*this == o) || !(*this < 0)); }

        inline self_type& operator+=(const SizeType n) { _ptr += n; _i += n; return *this; }
        inline self_type& operator-=(const SizeType n) { _ptr -= n; _i -= n; return *this; }

        inline self_type& operator+=(const IndexD &n) { return (*this += Container::to_position(n, _size)); }
        inline self_type& operator-=(const IndexD &n) { return (*this -= Container::to_position(n, _size)); }

        inline reference operator[](const SizeType n) const { return *(*this + n); }
        inline reference operator[](const IndexD &n) const { return *(*this + n); }

        inline void swap(self_type &o) { std::swap(_ptr, o._ptr); std::swap(_range, o._range); std::swap(_i, o._i); }
        friend inline void swap(self_type &it0, self_type &it1) { it0.swap(it1); }

    protected:
        pointer  _ptr;
        IndexD   _range;
        SizeType _i;
    };



    Grid() : DopeVector< T, Dimension, Args...>(nullptr, 0, IndexD::Zero()) {
        _array = _data.data();
    }

    Grid(const IndexD &size = IndexD::Zero(), const T & default_value = T()) : DopeVector< T, Dimension, Args...>(nullptr, 0, size) {
        _data.resize(size(), default_value);
        _array = _data.data();
    }

    Grid(const Grid &other) = default;
    Grid(Grid &&other) = default;

    inline const T * data() const { return _data.data(); }
    inline T * data() { return _data.data(); }


    inline iterator begin() const { return iterator( data(), _size, 0 ); }
    inline iterator end() const { return iterator( data(), _size, size() ); }

    inline const_iterator cbegin() const { return const_iterator( data(), _size, 0 ); }
    inline const_iterator cend() const { return const_iterator( data(), _size, size() ); }

    inline iterator to_iterator(const SizeType i) const { return (begin() + i); }
    inline iterator to_iterator(const IndexD& i) const { return (begin() + position(i)); }

    inline const_iterator to_const_iterator(const SizeType i) const { return (cbegin() + i); }
    inline const_iterator to_const_iterator(const IndexD& i) const { return (cbegin() + position(i)); }

    inline bool empty() const { return _data.empty(); }

    inline void reset(const T & default_value = T()) {
        _data.clear();
        _data.resize(size(), default_value);
        _array = _data.data();
    }

    virtual inline void swap(Grid &o) {
        DopeVector< T, Dimension, Args ... >::swap(o);
        _data.swap(o._data);
    }

protected:
    Data _data;
};

}

#endif // Grid_hpp
