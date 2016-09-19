#include <Grid/Grid.hpp>

namespace container {

	template < SizeType Dimension >
	inline SizeType total_size(const Index<Dimension> &size)
	{
		SizeType total = static_cast<SizeType>(0);
		for (SizeType i = static_cast<SizeType>(0); i < Dimension; ++i)
		total *= size[i];
		return total;
	}

template < SizeType Dimension >
SizeType to_position(const Index<Dimension> &index, const Index<Dimension> &range) {
    SizeType pos = static_cast<SizeType>(0);
    SizeType dimProd = static_cast<SizeType>(1);
    for(SizeType D = Dimension; D > static_cast<SizeType>(0); --D) {
        constexpr SizeType d = D - static_cast<SizeType>(1);
        pos += index[d] * dimProd;
        dimProd *= range[d];
    }
    return pos;
}

template < SizeType Dimension >
SizeType to_index(const SizeType position, const Index<Dimension> &range) {
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



    ////////////////////////////////////////////////////////////////////////
    // CONSTRUCTORS
    ////////////////////////////////////////////////////////////////////////

    GRID_TEMPLATE
    inline GRID_DOMAIN::Grid(const IndexD &size, const T & default_value)
		: _data(total_size(size), default_value)
	{
		DopeVector<T, Dimension, Args...>::reset(_data.data(), static_cast<SizeType>(0), size);
    }

    GRID_TEMPLATE
    inline GRID_DOMAIN::Grid(const SizeType size, const T & default_value)
		: _data(total_size(Index<Dimension>::Constant(size)), default_value)
	{
        DopeVector<T, Dimension, Args...>::reset(_data.data(), static_cast<SizeType>(0), Index<Dimension>::Constant(size));
    }

    ////////////////////////////////////////////////////////////////////////



    ////////////////////////////////////////////////////////////////////////
    // DESTRUCTOR
    ////////////////////////////////////////////////////////////////////////

    GRID_TEMPLATE
    inline GRID_DOMAIN::~Grid() {}

    ////////////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////////////
    // DATA
    ////////////////////////////////////////////////////////////////////////

    GRID_TEMPLATE
    inline const T * GRID_DOMAIN::data() const {
        return this->_data.data();
    }

    GRID_TEMPLATE
    inline T * GRID_DOMAIN::data() {
        return this->_data.data();
    }

    ////////////////////////////////////////////////////////////////////////



    ////////////////////////////////////////////////////////////////////////
    // ITERATORS
    ////////////////////////////////////////////////////////////////////////

    GRID_TEMPLATE
    inline typename GRID_DOMAIN::iterator GRID_DOMAIN::begin() {
        return GRID_DOMAIN::iterator( this->data(), this->_size, 0 );
    }

    GRID_TEMPLATE
    inline typename GRID_DOMAIN::iterator GRID_DOMAIN::end() {
        return GRID_DOMAIN::iterator( this->data(), this->_size, this->size() );
    }

    GRID_TEMPLATE
    inline typename GRID_DOMAIN::const_iterator GRID_DOMAIN::cbegin() const {
        return GRID_DOMAIN::const_iterator( this->data(), this->_size, 0 );
    }

    GRID_TEMPLATE
    inline typename GRID_DOMAIN::const_iterator GRID_DOMAIN::cend() const {
        return GRID_DOMAIN::const_iterator( this->data(), this->_size, this->size() );
    }

    ////////////////////////////////////////////////////////////////////////



    ////////////////////////////////////////////////////////////////////////
    // CONVERSIONS
    ////////////////////////////////////////////////////////////////////////

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
        return this->_data;
    }

    GRID_TEMPLATE
    inline typename GRID_DOMAIN::Data& GRID_DOMAIN::to_stdvector() {
        return this->_data;
    }

    GRID_TEMPLATE
    inline GRID_DOMAIN::operator Data() const {
        return this->_data;
    }

    ////////////////////////////////////////////////////////////////////////



    ////////////////////////////////////////////////////////////////////////
    // INFORMATION
    ////////////////////////////////////////////////////////////////////////

    GRID_TEMPLATE
    inline bool GRID_DOMAIN::empty() const {
        return this->_data.empty();
    }

    GRID_TEMPLATE
    inline bool GRID_DOMAIN::operator==(const Grid &o) const {
		if (&o == this)
			return true;
		for (SizeType i = static_cast<SizeType>(0); i < Dimension; ++i)
			if (DopeVector<T, Dimension, Args...>::sizeAt(i) != o.sizeAt(i))
				return false;
		return _data == o._data;
    }

    ////////////////////////////////////////////////////////////////////////



    ////////////////////////////////////////////////////////////////////////
    // RESET
    ////////////////////////////////////////////////////////////////////////

    GRID_TEMPLATE
    inline void GRID_DOMAIN::clear() {
        _data.clear();
		DopeVector<T, Dimension, Args...>::reset(nullptr, static_cast<SizeType>(0), IndexD::Zero());
    }

    GRID_TEMPLATE
    inline void GRID_DOMAIN::reset(const T & default_value) {
		IndexD size = DopeVector<T, Dimension, Args...>::allSizes();
        _data.assign(total_size(size), default_value);
		DopeVector<T, Dimension, Args...>::reset(_data.data(), static_cast<SizeType>(0), size);
    }

    ////////////////////////////////////////////////////////////////////////



    ////////////////////////////////////////////////////////////////////////
    // ASSIGNMENTS
    ////////////////////////////////////////////////////////////////////////

	GRID_TEMPLATE
	inline void GRID_DOMAIN::import(const DopeVector<T, Dimension, Args...> &o)
	{
		if (&o == this)
			return;
		try {
			const GRID_DOMAIN &oo = dynamic_cast<const GRID_DOMAIN &>(o);
			IndexD size = DopeVector<T, Dimension, Args...>::allSizes();
			for (SizeType d = 0; d < Dimension; ++d)
				if (size[d] != oo.sizeAt(d))
					throw std::out_of_range("Matrixes do not have same size.");
			_data = oo._data;
			DopeVector<T, Dimension, Args...>::reset(_data.data(), static_cast<SizeType>(0), size); // could be unuseful
		} catch(std::bad_cast &bc) {
			DopeVector<T, Dimension, Args...>::import(o);
		}
	}

    GRID_TEMPLATE
    inline void GRID_DOMAIN::swap(Grid &o) {
        this->_data.swap(o._data);
		IndexD size = DopeVector<T, Dimension, Args...>::allSizes();
		DopeVector<T, Dimension, Args...>::reset(_data.data(), static_cast<SizeType>(0), size);
    }

    ////////////////////////////////////////////////////////////////////////



#undef GRID_TEMPLATE
#undef GRID_DOMAIN

}
