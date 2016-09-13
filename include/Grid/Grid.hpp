#ifndef Grid_hpp
#define Grid_hpp

#include <vector>
#include <DopeVector/DopeVector.hpp>
#include <Grid/GridIterator.hpp>

namespace container {

template < SizeType Dimension >
static SizeType to_position(const Index<Dimension> &index, const Index<Dimension> &range);

template < SizeType Dimension >
static SizeType to_index(const SizeType position, const Index<Dimension> &range);


template < typename T, SizeType Dimension, class Allocator = std::allocator< T >, SizeType ... Args >
class Grid : public DopeVector< T, Dimension, Args ... > {
public:
    typedef std::vector< T, Allocator >      Data;
    typedef GridIterator<T, Dimension>       iterator;
    typedef GridIterator<const T, Dimension> const_iterator;

    inline Grid();
    inline Grid(const IndexD &size = IndexD::Zero(), const T & default_value = T());
    inline Grid(const Grid &other) = default;
    inline Grid(Grid &&other) = default;

    virtual inline ~Grid();

    inline const T * data() const;
    inline T * data();


    inline iterator begin() const;
    inline iterator end() const;
    inline const_iterator cbegin() const;
    inline const_iterator cend() const;

    inline iterator to_iterator(const SizeType i) const;
    inline iterator to_iterator(const IndexD& i) const;
    inline const_iterator to_const_iterator(const SizeType i) const;
    inline const_iterator to_const_iterator(const IndexD& i) const;

    inline const Data& to_vector() const;

    inline bool empty() const;

    inline void reset(const T & default_value = T());

    virtual inline void swap(Grid &o);

    inline Grid & operator=(const Grid &o) = default;
    inline bool operator==(const Grid &o) const;
    inline operator Data() const;

protected:
    Data _data;
};



template < typename T, SizeType Dimension >
using StandardGrid = Gird<T, Dimension>;

template < typename T, SizeType Dimension >
using StandardGrid2D = Gird<T, static_cast<SizeType>(2)>;

template < typename T, SizeType Dimension >
using StandardGrid3D = Gird<T, static_cast<SizeType>(3)>;



#include <Grid/inlines/Grid.inl>

}

#endif // Grid_hpp
