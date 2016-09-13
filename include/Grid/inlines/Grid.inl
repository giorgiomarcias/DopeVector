#include <Grid/Grid.hpp>

namespace container {

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
    for(SizeType D = Dimension; D > static_cast<SizeType>(0); --D) {
        constexpr SizeType d = D - static_cast<SizeType>(1);
        result[d] = i % range[d];
        i = i / range[d];
    }
    return result;
}



#ifdef GRID_TEMPLATE
#undef GRID_TEMPLATE
#endif
#ifdef GRID_DOMAIN
#undef GRID_DOMAIN
#endif
#define GRID_TEMPLATE template < typename T, SizeType Dimension, class Allocator, SizeType ... Args >
#define GRID_DOMAIN Grid< T, Dimension, Allocator, Args... >



    GRID_TEMPLATE
    GRID_DOMAIN::Grid() :
        DopeVector< T, Dimension, Args...>(nullptr, 0, IndexD::Zero()) {
        _array = _data.data();
    }

    GRID_TEMPLATE
    GRID_DOMAIN::Grid(const IndexD &size, const T & default_value) :
        DopeVector< T, Dimension, Args...>(nullptr, 0, size) {
        _data.resize(size(), default_value);
        _array = _data.data();
    }



    GRID_TEMPLATE
    inline const T * GRID_DOMAIN::data() const {
        return _data.data();
    }

    GRID_TEMPLATE
    inline T * GRID_DOMAIN::data() {
        return _data.data();
    }



    GRID_TEMPLATE
    inline typename GRID_DOMAIN::iterator GRID_DOMAIN::begin() const {
        return GRID_DOMAIN::iterator( data(), _size, 0 );
    }

    GRID_TEMPLATE
    inline typename GRID_DOMAIN::iterator GRID_DOMAIN::end() const {
        return GRID_DOMAIN::iterator( data(), _size, size() );
    }

    GRID_TEMPLATE
    inline typename GRID_DOMAIN::const_iterator GRID_DOMAIN::cbegin() const {
        return GRID_DOMAIN::const_iterator( data(), _size, 0 );
    }

    GRID_TEMPLATE
    inline typename GRID_DOMAIN::const_iterator GRID_DOMAIN::cend() const {
        return GRID_DOMAIN::const_iterator( data(), _size, size() );
    }

    GRID_TEMPLATE
    inline typename GRID_DOMAIN::iterator GRID_DOMAIN::to_iterator(const SizeType i) const {
        return (begin() + i);
    }

    GRID_TEMPLATE
    inline typename GRID_DOMAIN::iterator GRID_DOMAIN::to_iterator(const IndexD& i) const {
        return (begin() + position(i));
    }

    GRID_TEMPLATE
    inline typename GRID_DOMAIN::const_iterator GRID_DOMAIN::to_const_iterator(const SizeType i) const {
        return (cbegin() + i);
    }

    GRID_TEMPLATE
    inline typename GRID_DOMAIN::const_iterator GRID_DOMAIN::to_const_iterator(const IndexD& i) const {
        return (cbegin() + position(i));
    }

    GRID_TEMPLATE
    inline const typename GRID_DOMAIN::Data& GRID_DOMAIN::to_stdvector() const {
        return _data;
    }



    GRID_TEMPLATE
    inline bool GRID_DOMAIN::empty() const {
        return _data.empty();
    }

    GRID_TEMPLATE
    inline void GRID_DOMAIN::reset(const T & default_value) {
        _data.clear();
        _data.resize(size(), default_value);
        _array = _data.data();
    }



    GRID_TEMPLATE
    inline void GRID_DOMAIN::swap(Grid &o) {
        DopeVector< T, Dimension, Args ... >::swap(o);
        _data.swap(o._data);
    }

#undef GRID_TEMPLATE
#undef GRID_DOMAIN

}

#endif // Grid_hpp
