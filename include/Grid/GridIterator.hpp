#ifndef GridIterator_hpp
#define GridIterator_hpp

#include <iterator>
#include <DopeVector/Common/Index.hpp>

namespace container {

template < typename T, SizeType Dimension >
class GridIterator : public std::iterator<std::random_access_iterator_tag, T>{
public:
    typedef Index<Dimension> IndexD;
    typedef GridIterator     self_type;

    inline GridIterator();
    inline GridIterator(pointer ptr, const IndexD &range, const SizeType i);
    inline GridIterator(pointer ptr, const IndexD &range, const IndexD &index);
    inline GridIterator(const self_type &o) = default;
    inline GridIterator(self_type &&o) = default;
    inline ~GridIterator();

    inline SizeType to_position() const;
    inline IndexD   to_index() const;

    inline bool operator==(const self_type &o) const;
    inline bool operator!=(const self_type &o) const;
    inline self_type& operator=(const self_type &o) = default;

    inline reference operator*();
    inline pointer operator->();

    inline self_type& operator++();
    inline self_type operator++(int);

    inline self_type& operator--();
    inline self_type operator--(int);

    inline self_type operator+(const SizeType n) const;
    friend inline self_type operator+(const SizeType n, const self_type &it) ;
    inline self_type operator+(const IndexD &n) const;
    friend inline self_type operator+(const IndexD &n, const self_type &it);
    inline self_type operator-(const SizeType n) const;
    inline self_type operator-(const IndexD &n) const;
    inline difference_type operator-(const self_type &o) const;

    inline bool operator<(const self_type &o) const;
    inline bool operator<=(const self_type &o) const;
    inline bool operator>(const self_type &o) const;
    inline bool operator>=(const self_type &o) const;

    inline self_type& operator+=(const SizeType n);
    inline self_type& operator-=(const SizeType n);

    inline self_type& operator+=(const IndexD &n);
    inline self_type& operator-=(const IndexD &n);

    inline reference operator[](const SizeType n) const;
    inline reference operator[](const IndexD &n) const;

    inline void swap(self_type &o);
    friend inline void swap(self_type &it0, self_type &it1);

protected:
    pointer  _ptr;
    IndexD   _range;
    SizeType _i;
};

#include <Grid/inlines/GridIterator.inl>

}

#endif // GridIterator_hpp
