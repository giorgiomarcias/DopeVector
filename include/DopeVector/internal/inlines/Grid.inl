// Copyright (c) 2016 Giorgio Marcias & Maurizio Kovacic
//
// This source code is part of DopeVector header library
// and it is subject to Apache 2.0 License.
//
// Author: Giorgio Marcias
// email: marcias.giorgio@gmail.com
// Author: Maurizio Kovacic
// email: maurizio.kovacic@gmail.com

#include <DopeVector/Grid.hpp>

namespace dope {

	template < SizeType Dimension >
	inline SizeType total_size(const Index<Dimension> &size)
	{
		SizeType total = static_cast<SizeType>(1);
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



	////////////////////////////////////////////////////////////////////////////
	// CONSTRUCTORS
	////////////////////////////////////////////////////////////////////////////

	template < typename T, SizeType Dimension, class Allocator >
	inline Grid<T, Dimension, Allocator>::Grid(const IndexD &size, const T & default_value)
		: _data(total_size(size), default_value)
	{
		DopeVector<T, Dimension>::reset(_data.data(), static_cast<SizeType>(0), size);
	}

	template < typename T, SizeType Dimension, class Allocator >
	inline Grid<T, Dimension, Allocator>::Grid(const SizeType size, const T & default_value)
		: _data(total_size(Index<Dimension>::Constant(size)), default_value)
	{
		DopeVector<T, Dimension>::reset(_data.data(), static_cast<SizeType>(0), Index<Dimension>::Constant(size));
	}

	////////////////////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////////////////////
	// DESTRUCTOR
	////////////////////////////////////////////////////////////////////////////

	template < typename T, SizeType Dimension, class Allocator >
	inline Grid<T, Dimension, Allocator>::~Grid()
	{ }

	////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////////
	// DATA
	////////////////////////////////////////////////////////////////////////////

	template < typename T, SizeType Dimension, class Allocator >
	inline const T * Grid<T, Dimension, Allocator>::data() const
	{
		return _data.data();
	}

	template < typename T, SizeType Dimension, class Allocator >
	inline T * Grid<T, Dimension, Allocator>::data()
	{
		return _data.data();
	}

	////////////////////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////////////////////
	// ITERATORS
	////////////////////////////////////////////////////////////////////////////

	template < typename T, SizeType Dimension, class Allocator >
	inline typename Grid<T, Dimension, Allocator>::iterator Grid<T, Dimension, Allocator>::begin()
	{
		return Grid<T, Dimension, Allocator>::iterator(data(), DopeVector<T, Dimension>::allSizes(), 0);
	}

	template < typename T, SizeType Dimension, class Allocator >
	inline typename Grid<T, Dimension, Allocator>::iterator Grid<T, Dimension, Allocator>::end()
	{
		return Grid<T, Dimension, Allocator>::iterator(data(), DopeVector<T, Dimension>::allSizes(), DopeVector<T, Dimension>::size());
	}

	template < typename T, SizeType Dimension, class Allocator >
	inline typename Grid<T, Dimension, Allocator>::const_iterator Grid<T, Dimension, Allocator>::begin() const
	{
		return Grid<T, Dimension, Allocator>::const_iterator(data(), DopeVector<T, Dimension>::_size, 0);
	}

	template < typename T, SizeType Dimension, class Allocator >
	inline typename Grid<T, Dimension, Allocator>::const_iterator Grid<T, Dimension, Allocator>::end() const
	{
		return Grid<T, Dimension, Allocator>::const_iterator(data(), DopeVector<T, Dimension>::allSizes(), DopeVector<T, Dimension>::size());
	}

	template < typename T, SizeType Dimension, class Allocator >
	inline typename Grid<T, Dimension, Allocator>::const_iterator Grid<T, Dimension, Allocator>::cbegin() const
	{
		return Grid<T, Dimension, Allocator>::const_iterator(data(), DopeVector<T, Dimension>::allSizes(), 0);
	}

	template < typename T, SizeType Dimension, class Allocator >
	inline typename Grid<T, Dimension, Allocator>::const_iterator Grid<T, Dimension, Allocator>::cend() const
	{
		return Grid<T, Dimension, Allocator>::const_iterator(data(), DopeVector<T, Dimension>::allSizes(), DopeVector<T, Dimension>::size());
	}

	////////////////////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////////////////////
	// CONVERSIONS
	////////////////////////////////////////////////////////////////////////////

	template < typename T, SizeType Dimension, class Allocator >
	inline typename Grid<T, Dimension, Allocator>::iterator Grid<T, Dimension, Allocator>::to_iterator(const SizeType i) const
	{
		return (begin() + i);
	}

	template < typename T, SizeType Dimension, class Allocator >
	inline typename Grid<T, Dimension, Allocator>::iterator Grid<T, Dimension, Allocator>::to_iterator(const IndexD& i) const
	{
		return (begin() + position(i));
	}

	template < typename T, SizeType Dimension, class Allocator >
	inline typename Grid<T, Dimension, Allocator>::const_iterator Grid<T, Dimension, Allocator>::to_const_iterator(const SizeType i) const
	{
		return (cbegin() + i);
	}

	template < typename T, SizeType Dimension, class Allocator >
	inline typename Grid<T, Dimension, Allocator>::const_iterator Grid<T, Dimension, Allocator>::to_const_iterator(const IndexD& i) const
	{
		return (cbegin() + position(i));
	}

	template < typename T, SizeType Dimension, class Allocator >
	inline const typename Grid<T, Dimension, Allocator>::Data& Grid<T, Dimension, Allocator>::to_stdvector() const
	{
		return _data;
	}

	template < typename T, SizeType Dimension, class Allocator >
	inline typename Grid<T, Dimension, Allocator>::Data& Grid<T, Dimension, Allocator>::to_stdvector()
	{
		return _data;
	}

	template < typename T, SizeType Dimension, class Allocator >
	inline Grid<T, Dimension, Allocator>::operator Data() const
	{
		return _data;
	}

	////////////////////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////////////////////
	// INFORMATION
	////////////////////////////////////////////////////////////////////////////

	template < typename T, SizeType Dimension, class Allocator >
	inline bool Grid<T, Dimension, Allocator>::empty() const
	{
		return _data.empty();
	}

	template < typename T, SizeType Dimension, class Allocator >
	inline bool Grid<T, Dimension, Allocator>::operator==(const Grid &o) const
	{
		if (&o == this)
			return true;
		for (SizeType i = static_cast<SizeType>(0); i < Dimension; ++i)
			if (DopeVector<T, Dimension>::sizeAt(i) != o.sizeAt(i))
				return false;
		return _data == o._data;
	}

	////////////////////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////////////////////
	// RESET
	////////////////////////////////////////////////////////////////////////////

	template < typename T, SizeType Dimension, class Allocator >
	inline void Grid<T, Dimension, Allocator>::clear()
	{
		_data.clear();
		DopeVector<T, Dimension>::reset(nullptr, static_cast<SizeType>(0), IndexD::Zero());
	}

	template < typename T, SizeType Dimension, class Allocator >
	inline void Grid<T, Dimension, Allocator>::reset(const T & default_value)
	{
		IndexD size = DopeVector<T, Dimension>::allSizes();
		_data.assign(total_size(size), default_value);
		DopeVector<T, Dimension>::reset(_data.data(), static_cast<SizeType>(0), size);
	}

	////////////////////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////////////////////
	// ASSIGNMENTS
	////////////////////////////////////////////////////////////////////////////

	template < typename T, SizeType Dimension, class Allocator >
	inline void Grid<T, Dimension, Allocator>::import(const DopeVector<T, Dimension> &o)
	{
		if (&o == this)
			return;
		try {
			const Grid<T, Dimension, Allocator> &oo = dynamic_cast<const Grid<T, Dimension, Allocator> &>(o);
			IndexD size = DopeVector<T, Dimension>::allSizes();
			for (SizeType d = 0; d < Dimension; ++d)
				if (size[d] != oo.sizeAt(d))
					throw std::out_of_range("Matrixes do not have same size.");
			_data = oo._data;
			DopeVector<T, Dimension>::reset(_data.data(), static_cast<SizeType>(0), size); // could be unuseful
		} catch(std::bad_cast &bc) {
			DopeVector<T, Dimension>::import(o);
		}
	}

	template < typename T, SizeType Dimension, class Allocator >
	inline void Grid<T, Dimension, Allocator>::swap(Grid &o)
	{
		_data.swap(o._data);
		IndexD size = DopeVector<T, Dimension>::allSizes();
		DopeVector<T, Dimension>::reset(_data.data(), static_cast<SizeType>(0), size);
	}

	////////////////////////////////////////////////////////////////////////////
	
}
