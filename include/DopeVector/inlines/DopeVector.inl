#include <DopeVector/DopeVector.hpp>

namespace container {

    ////////////////////////////////////////////////////////////////////////
    // CONSTRUCTORS
    ////////////////////////////////////////////////////////////////////////

    template < typename T, SizeType Dimension, SizeType ... Args >
    inline DopeVector<T, Dimension, Args...>::DopeVector()
		: _array(nullptr)
		, _accumulatedOffset(0)
		, _size(IndexD::Zero())
		, _offset(IndexD::Zero())
	{ }

    template < typename T, SizeType Dimension, SizeType ... Args >
    inline DopeVector<T, Dimension, Args...>::DopeVector(T *array, const SizeType accumulatedOffset, const IndexD &size)
		: _array(array)
		, _accumulatedOffset(array ? accumulatedOffset : 0)
		, _size(array ? size : static_cast<const IndexD &>(IndexD::Zero()))
		, _offset(IndexD::Zero())
	{
		if (array) {
			_offset[D-1] = 1;
            for (SizeType j = D-1; j > 0; --j)
				_offset[j-1] = _size[j] * _offset[j];
		}
	}

    template < typename T, SizeType Dimension, SizeType ... Args >
    inline DopeVector<T, Dimension, Args...>::DopeVector(T *array, const SizeType accumulatedOffset, const IndexD &size, const IndexD &offset)
		: _array(array)
		, _accumulatedOffset(array ? accumulatedOffset : 0)
        , _size(array ? size : static_cast<const IndexD &>(IndexD::Zero()))
        , _offset(array ? offset : static_cast<const IndexD &>(IndexD::Zero()))
	{ }

    ////////////////////////////////////////////////////////////////////////



    ////////////////////////////////////////////////////////////////////////
    // ASSIGNMENT OPERATORS
    ////////////////////////////////////////////////////////////////////////

	template < typename T, SizeType Dimension, SizeType ... Args >
	inline void DopeVector<T, Dimension, Args...>::reset(T *array, const SizeType accumulatedOffset, const IndexD &size)
	{
		_array = array;
		_accumulatedOffset = _array ? accumulatedOffset : 0;
		_size = _array ? size : static_cast<const IndexD &>(IndexD::Zero());
		_offset = IndexD::Zero();
		if (_array) {
			_offset[D-1] = 1;
			for (SizeType j = D-1; j > 0; --j)
			_offset[j-1] = _size[j] * _offset[j];
		}
	}

	template < typename T, SizeType Dimension, SizeType ... Args >
	inline void DopeVector<T, Dimension, Args...>::reset(T *array, const SizeType accumulatedOffset, const IndexD &size, const IndexD &offset)
	{
		_array = array;
		_accumulatedOffset = _array ? accumulatedOffset : 0;
		_size = _array ? size : static_cast<const IndexD &>(IndexD::Zero());
		_offset = _array ? offset : static_cast<const IndexD &>(IndexD::Zero());
	}

    template < typename T, SizeType Dimension, SizeType ... Args >
    inline void DopeVector<T, Dimension, Args...>::import(const DopeVector &o)
	{
		if (&o == this)
			return;
		if (_size[0] != o._size[0])
			throw std::out_of_range("Matrixes do not have same size.");
        for (SizeType i = 0; i < _size[0]; ++i)
            operator[](i).import(o[i]);
	}

    ////////////////////////////////////////////////////////////////////////



    ////////////////////////////////////////////////////////////////////////
    // ACCESS METHODS
    ////////////////////////////////////////////////////////////////////////

    template < typename T, SizeType Dimension, SizeType ... Args >
    inline void DopeVector<T, Dimension, Args...>::at(const SizeType i, DopeVector<T, Dimension-1, Args...> &s) const
	{
		if (i >= _size[0]) {
			std::stringstream stream;
			stream << "Index " << i << " is out of range [0, " << _size[0]-1 << ']';
			throw std::out_of_range(stream.str());
		}
		s._array = _array + _offset[0] * i;
		s._accumulatedOffset = accumulatedOffset(i);
        for (SizeType j = 1; j < D; ++j) {
			s._size[j-1] = _size[j];
			s._offset[j-1] = _offset[j];
		}
	}

    template < typename T, SizeType Dimension, SizeType ... Args >
    inline DopeVector<T, Dimension-1, Args...> DopeVector<T, Dimension, Args...>::at(const SizeType i) const
    {
        DopeVector<T, D-1> s;
        at(i, s);
        return s;
    }

    template < typename T, SizeType Dimension, SizeType ... Args >
    inline const T & DopeVector<T, Dimension, Args...>::at(const IndexD &i) const
    {
        SizeType offset = _accumulatedOffset;
        for(SizeType d = 0; d < D; ++d) {
            offset += i[d] * _offset[d];
        }
        return *(_array + offset);
    }

    template < typename T, SizeType Dimension, SizeType ... Args >
    inline T & DopeVector<T, Dimension, Args...>::at(const IndexD &i)
    {
        SizeType offset = _accumulatedOffset;
        for(SizeType d = 0; d < D; ++d) {
            offset += i[d] * _offset[d];
        }
        return *(_array + offset);
    }

    template < typename T, SizeType Dimension, SizeType ... Args >
    inline DopeVector<T, Dimension-1, Args...> DopeVector<T, Dimension, Args...>::operator[](const SizeType i) const
	{
		DopeVector<T, D-1> s;
		at(i, s);
		return s;
	}

    template < typename T, SizeType Dimension, SizeType ... Args >
    inline const T & DopeVector<T, Dimension, Args...>::operator[](const IndexD &i) const
    {
        return at(i);
    }

    template < typename T, SizeType Dimension, SizeType ... Args >
    inline T & DopeVector<T, Dimension, Args...>::operator[](const IndexD &i)
    {
        return at(i);
    }

    ////////////////////////////////////////////////////////////////////////



    ////////////////////////////////////////////////////////////////////////
    // REDUCTION METHODS
    ////////////////////////////////////////////////////////////////////////

    template < typename T, SizeType Dimension, SizeType ... Args >
    inline void DopeVector<T, Dimension, Args...>::slice(const SizeType d, const SizeType i, DopeVector<T, Dimension-1, Args...> &s) const
	{
		if (d >= D) {
			std::stringstream stream;
			stream << "Index " << d << " is out of range [0, " << D-1 << ']';
			throw std::out_of_range(stream.str());
		}
		if (i >= _size[d]) {
			std::stringstream stream;
			stream << "Index " << i << " is out of range [0, " << _size[d]-1 << ']';
			throw std::out_of_range(stream.str());
		}
		s._array = _array + _offset[d] * i;
		s._accumulatedOffset = accumulatedOffset(i, d);
        SizeType k = 0;
        for (SizeType j = 0; j < d; ++j, ++k) {
			s._size[k] = _size[j];
			s._offset[k] = _offset[j];
		}
        for (SizeType j = d+1; j < D; ++j, ++k) {
			s._size[k] = _size[j];
			s._offset[k] = _offset[j];
		}
	}

    template < typename T, SizeType Dimension, SizeType ... Args >
    inline DopeVector<T, Dimension-1, Args...> DopeVector<T, Dimension, Args...>::slice(const SizeType d, const SizeType i) const
	{
        DopeVector<T, D-1, Args...> s;
		slice(d, i, s);
		return s;
	}

    template < typename T, SizeType Dimension, SizeType ... Args >
    inline void DopeVector<T, Dimension, Args...>::permute(const IndexD &order, DopeVector &p) const
	{
		std::array<bool, D> included = {{false}};
        for (SizeType d = 0; d < D; ++d) {
			if (order[d] >= D) {
				std::stringstream stream;
				stream << "Index " << order[d] << " is out of range [0, " << D-1 << ']';
				throw std::out_of_range(stream.str());
			}
			if (included[order[d]]) {
				std::stringstream stream;
				stream << "Dimension " << order[d] << " duplicated.";
				throw std::invalid_argument(stream.str());
			}
			included[order[d]] = true;
		}
		p._array = _array;
		p._accumulatedOffset = _accumulatedOffset;
        SizeType newSize[D], newOffset[D];
        std::memcpy(newSize, _size.data(), D * sizeof(SizeType));
        std::memcpy(newOffset, _offset.data(), D * sizeof(SizeType));
        for (SizeType d = 0; d < D; ++d) {
			p._size[d] = newSize[order[d]];
			p._offset[d] = newOffset[order[d]];
		}
	}

    template < typename T, SizeType Dimension, SizeType ... Args >
    inline DopeVector<T, Dimension, Args...> DopeVector<T, Dimension, Args...>::permute(const IndexD &order) const
	{
        DopeVector<T, Dimension, Args...> p;
		permute(order, p);
		return p;
	}

    template < typename T, SizeType Dimension, SizeType ... Args >
    inline void DopeVector<T, Dimension, Args...>::window(const IndexD &start, IndexD &size, DopeVector<T, Dimension, Args...> &w) const
	{
        for (SizeType d = 0; d < D; ++d) {
			if (start[d] >= _size[d]) {
				std::stringstream stream;
				stream << "Index " << start[d] << " is out of range [0, " << _size[d] << ']';
				throw std::out_of_range(stream.str());
			}
			if (start[d] + size[d] > _size[d]) {
				std::stringstream stream;
				stream << "Window size " << size[d] << " is out of range [" << 0 << ", " << _size[d] - start[d] << ']';
				throw std::out_of_range(stream.str());
			}
		}
        SizeType newAccumulatedOffset = _accumulatedOffset;
        for (SizeType d = 0; d < D; ++d)
			newAccumulatedOffset += _offset[d] * start[d];
		w._array = _array + (newAccumulatedOffset - _accumulatedOffset);
		w._accumulatedOffset = newAccumulatedOffset;
		w._size = size;
		w._offset = _offset;
	}

    template < typename T, SizeType Dimension, SizeType ... Args >
    inline DopeVector<T, Dimension, Args...> DopeVector<T, Dimension, Args...>::window(const IndexD &start, const IndexD &size) const
	{
        DopeVector<T, Dimension, Args...> w;
		window(start, size, w);
		return w;
	}

    ////////////////////////////////////////////////////////////////////////



    ////////////////////////////////////////////////////////////////////////
    // INFORMATION
    ////////////////////////////////////////////////////////////////////////

    template < typename T, SizeType Dimension, SizeType ... Args >
    inline SizeType DopeVector<T, Dimension, Args...>::sizeAt(const SizeType d) const
	{
		if (d >= D) {
			std::stringstream stream;
			stream << "Index " << d << " is out of range [0, " << D-1 << ']';
			throw std::out_of_range(stream.str());
		}
		return _size[d];
	}

    template < typename T, SizeType Dimension, SizeType ... Args >
    inline const typename DopeVector<T, Dimension, Args...>::IndexD & DopeVector<T, Dimension, Args...>::allSizes() const
	{
		return _size;
	}

    template < typename T, SizeType Dimension, SizeType ... Args >
    inline SizeType DopeVector<T, Dimension, Args...>::size() const
	{
        SizeType total = _size[0];
        for (SizeType i = 1; i < D; ++i)
			total *= _size[i];
		return total;
	}

    template < typename T, SizeType Dimension, SizeType ... Args >
    inline SizeType DopeVector<T, Dimension, Args...>::accumulatedOffset(const SizeType i, const SizeType d) const
	{
		if (d >= D) {
			std::stringstream stream;
			stream << "Index " << d << " is out of range [0, " << D-1 << ']';
			throw std::out_of_range(stream.str());
		}
		if (i >= _size[d]) {
			std::stringstream stream;
			stream << "Index " << i << " is out of range [0, " << _size[d]-1 << ']';
			throw std::out_of_range(stream.str());
		}
		return  _accumulatedOffset + _offset[d] * i;
	}

    ////////////////////////////////////////////////////////////////////////






    ////////////////////////////////////////////////////////////////////////
    // CONSTRUCTORS
    ////////////////////////////////////////////////////////////////////////

    template < typename T, SizeType ... Args >
    inline DopeVector<T, 1, Args...>::DopeVector()
		: _array(nullptr)
		, _accumulatedOffset(0)
		, _size(Index1::Zero())
		, _offset(Index1::Zero())
	{ }

    template < typename T, SizeType ... Args >
    inline DopeVector<T, 1, Args...>::DopeVector(T *array, const SizeType accumulatedOffset, const SizeType size)
		: _array(array)
		, _accumulatedOffset(array ? accumulatedOffset : 0)
		, _size(array ? Index1::Constant(size) : Index1::Zero())
		, _offset(array ? Index1::Ones() : Index1::Zero())
	{ }

    template < typename T, SizeType ... Args >
    inline DopeVector<T, 1, Args...>::DopeVector(T *array, const SizeType accumulatedOffset, const SizeType size, const SizeType offset)
		: _array(array)
		, _accumulatedOffset(array ? accumulatedOffset : 0)
		, _size(array ? Index1::Constant(size) : Index1::Zero())
		, _offset(array ? Index1::Constant(offset) : Index1::Zero())
	{ }

    template < typename T, SizeType ... Args >
    inline DopeVector<T, 1, Args...>::DopeVector(T *array, const SizeType accumulatedOffset, const Index1 &size)
		: _array(array)
		, _accumulatedOffset(array ? accumulatedOffset : 0)
		, _size(array ? size : static_cast<const Index1 &>(Index1::Zero()))
		, _offset(array ? Index1::Ones() : Index1::Zero())
	{ }

    template < typename T, SizeType ... Args >
    inline DopeVector<T, 1, Args...>::DopeVector(T *array, const SizeType accumulatedOffset, const Index1 &size, const Index1 &offset)
		: _array(array)
		, _accumulatedOffset(array ? accumulatedOffset : 0)
		, _size(array ? size : static_cast<const Index1 &>(Index1::Zero()))
		, _offset(array ? offset : static_cast<const Index1 &>(Index1::Zero()))
	{ }

    ////////////////////////////////////////////////////////////////////////



    ////////////////////////////////////////////////////////////////////////
    // ASSIGNMENT OPERATORS
    ////////////////////////////////////////////////////////////////////////

	template < typename T, SizeType ... Args >
	inline void DopeVector<T, 1, Args...>::reset(T *array, const SizeType accumulatedOffset, const SizeType size)
	{
		_array = array;
		_accumulatedOffset = _array ? accumulatedOffset : 0;
		_size = _array ? Index1::Constant(size) : Index1::Zero();
		_offset = _array ? Index1::Ones() : Index1::Zero();
	}

	template < typename T, SizeType ... Args >
	inline void DopeVector<T, 1, Args...>::reset(T *array, const SizeType accumulatedOffset, const SizeType size, const SizeType offset)
	{
		_array = array;
		_accumulatedOffset = _array ? accumulatedOffset : 0;
		_size = _array ? Index1::Constant(size) : Index1::Zero();
		_offset = _array ? Index1::Constant(offset) : Index1::Zero();
	}

	template < typename T, SizeType ... Args >
	inline void DopeVector<T, 1, Args...>::reset(T *array, const SizeType accumulatedOffset, const Index1 &size)
	{
		_array = array;
		_accumulatedOffset = _array ? accumulatedOffset : 0;
		_size = _array ? size : static_cast<const Index1 &>(Index1::Zero());
		_offset = _array ? Index1::Ones() : Index1::Zero();
	}

	template < typename T, SizeType ... Args >
	inline void DopeVector<T, 1, Args...>::reset(T *array, const SizeType accumulatedOffset, const Index1 &size, const Index1 &offset)
	{
		_array = array;
		_accumulatedOffset = _array ? accumulatedOffset : 0;
		_size = _array ? size : static_cast<const Index1 &>(Index1::Zero());
		_offset = _array ? offset : static_cast<const Index1 &>(Index1::Zero());
	}

    template < typename T, SizeType ... Args >
    inline void DopeVector<T, 1, Args...>::import(const DopeVector<T, 1, Args...> &o)
	{
		if (&o == this)
			return;
		if (_size[0] != o._size[0])
			throw std::out_of_range("Matrixes do not have same size.");
        for (SizeType i = 0; i < _size[0]; ++i)
			operator[](i) = o[i];
	}

    ////////////////////////////////////////////////////////////////////////



    ////////////////////////////////////////////////////////////////////////
    // ACCESS METHODS
    ////////////////////////////////////////////////////////////////////////

    template < typename T, SizeType ... Args >
    inline const T & DopeVector<T, 1, Args...>::at(const SizeType i) const
    {
        if (i >= _size[0]) {
            std::stringstream stream;
            stream << "Index " << i << " is out of range [0, " << _size[0]-1 << ']';
            throw std::out_of_range(stream.str());
        }
        return *(_array + i * _offset[0]);
    }

    template < typename T, SizeType ... Args >
    inline T & DopeVector<T, 1, Args...>::at(const SizeType i)
    {
        if (i >= _size[0]) {
            std::stringstream stream;
            stream << "Index " << i << " is out of range [0, " << _size[0]-1 << ']';
            throw std::out_of_range(stream.str());
        }
        return *(_array + i * _offset[0]);
    }

    template < typename T, SizeType ... Args >
    inline const T & DopeVector<T, 1, Args...>::at(const Index1 i) const
    {
        return at(i[0]);
    }

    template < typename T, SizeType ... Args >
    inline T & DopeVector<T, 1, Args...>::at(const Index1 i)
    {
        return at(i[0]);
    }

    template < typename T, SizeType ... Args >
    inline const T & DopeVector<T, 1, Args...>::operator[](const SizeType i) const
	{
        return at(i);
	}

    template < typename T, SizeType ... Args >
    inline T & DopeVector<T, 1, Args...>::operator[](const SizeType i)
	{
        return at(i);
	}

    template < typename T, SizeType ... Args >
    inline const T & DopeVector<T, 1, Args...>::operator[](const Index1 i) const
    {
        return at(i[0]);
    }

    template < typename T, SizeType ... Args >
    inline T & DopeVector<T, 1, Args...>::operator[](const Index1 i)
    {
        return at(i[0]);
    }

    ////////////////////////////////////////////////////////////////////////



    ////////////////////////////////////////////////////////////////////////
    // REDUCTION METHODS
    ////////////////////////////////////////////////////////////////////////

    template < typename T, SizeType ... Args >
    inline const T & DopeVector<T, 1, Args...>::slice(const SizeType i) const
	{
		return *this[i];
	}

    template < typename T, SizeType ... Args >
    inline T & DopeVector<T, 1, Args...>::slice(const SizeType i)
	{
		return *this[i];
	}

    template < typename T, SizeType ... Args >
    inline void DopeVector<T, 1, Args...>::permute(const Index1 &order, DopeVector<T, 1, Args...> &p) const
	{
        for (SizeType d = 0; d < 1; ++d) {
			if (order[d] >= 1) {
				std::stringstream stream;
				stream << "Index " << order[d] << " is out of range [0, 0]";
				throw std::out_of_range(stream.str());
			}
		}
		if (&p == this)
			return;
		p._array = _array;
		p._accumulatedOffset = _accumulatedOffset;
        SizeType newSize[1] = {_size[0]};
        SizeType newOffset[1] = {_offset[0]};
        for (SizeType d = 0; d < 1; ++d) {
			p._size[d] = newSize[order[d]];
			p._offset[d] = newOffset[order[d]];
		}
	}

    template < typename T, SizeType ... Args >
    inline DopeVector<T, 1, Args...> DopeVector<T, 1, Args...>::permute(const Index1 &order) const
	{
        DopeVector<T, 1, Args...> p;
		permute(order, p);
		return p;
	}

    template < typename T, SizeType ... Args >
    inline void DopeVector<T, 1, Args...>::window(const Index1 &start, const Index1 &size, DopeVector<T, 1, Args...> &w) const
	{
        return window(start[0], size[0], w);
	}

    template < typename T, SizeType ... Args >
    inline DopeVector<T, 1, Args...> DopeVector<T, 1, Args...>::window(const Index1 &start, const Index1 &size) const
	{
        DopeVector<T, 1, Args...> w;
		window(start, size, w);
		return w;
	}

    template < typename T, SizeType ... Args >
    inline void DopeVector<T, 1, Args...>::window(const SizeType start, const SizeType size, DopeVector<T, 1, Args...> &w) const
	{
		if (start >= _size[0]) {
			std::stringstream stream;
			stream << "Index " << start << " is out of range [0, " << _size[0] << ']';
			throw std::out_of_range(stream.str());
		}
		if (start + size > _size[0]) {
			std::stringstream stream;
			stream << "Window size " << size << " is out of range [" << 0 << ", " << _size[0] - start << ']';
			throw std::out_of_range(stream.str());
		}
		w._accumulatedOffset = _accumulatedOffset + _offset[0] * start;
		w._array = _array + (w._accumulatedOffset - _accumulatedOffset);
		w._size[0] = size;
		w._offset[0] = _offset[0];
	}

    template < typename T, SizeType ... Args >
    inline DopeVector<T, 1, Args...> DopeVector<T, 1, Args...>::window(const SizeType start, const SizeType size) const
	{
        DopeVector<T, 1, Args...> w;
		window(start, size, w);
		return w;
	}

    ////////////////////////////////////////////////////////////////////////



    ////////////////////////////////////////////////////////////////////////
    // INFORMATION
    ////////////////////////////////////////////////////////////////////////

    template < typename T, SizeType ... Args >
    inline SizeType DopeVector<T, 1, Args...>::sizeAt(const SizeType d) const
	{
		if (d >= 1) {
			std::stringstream stream;
			stream << "Index " << d << " is out of range [0, " << 0 << ']';
			throw std::out_of_range(stream.str());
		}
		return _size[0];
	}

    template < typename T, SizeType ... Args >
    inline const Index1 & DopeVector<T, 1, Args...>::allSizes() const
	{
		return _size;
	}

    template < typename T, SizeType ... Args >
    inline SizeType DopeVector<T, 1, Args...>::size() const
	{
		return _size[0];
	}

    template < typename T, SizeType ... Args >
    inline SizeType DopeVector<T, 1, Args...>::accumulatedOffset(const SizeType i) const
	{
		if (i >= _size[0]) {
			std::stringstream stream;
			stream << "Index " << i << " is out of range [0, " << _size[0]-1 << ']';
			throw std::out_of_range(stream.str());
		}
		return _accumulatedOffset + _offset[0] * i;
	}

    ////////////////////////////////////////////////////////////////////////

}
