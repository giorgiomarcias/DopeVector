template < typename T, std::size_t D >
inline DopeVector<T, D>::DopeVector()
	: _array(nullptr)
	, _accumulatedOffset(0)
	, _size({{0}})
	, _offset({{0}})
{ }

template < typename T, std::size_t D >
inline DopeVector<T, D>::DopeVector(const T *array, const std::size_t accumulatedOffset, const std::array<std::size_t, D> &size)
	: _array(const_cast<T*>(array))
	, _accumulatedOffset(accumulatedOffset)
	, _size(size)
	, _offset({{0}})
{ }

template < typename T, std::size_t D >
inline DopeVector<T, D>::DopeVector(const T *array, const std::size_t accumulatedOffset, const std::array<std::size_t, D> &size, const std::array<std::size_t, D> &offset)
	: _array(const_cast<T*>(array))
	, _accumulatedOffset(accumulatedOffset)
	, _size(size)
	, _offset(offset)
{ }

template < typename T, std::size_t D >
inline void DopeVector<T, D>::at(const std::size_t i, DopeVector<T, D-1> &s) const
{
	if (i >= _size[0]) {
		std::stringstream stream;
		stream << "Index " << i << " is out of range [0, " << _size[0]-1 << ']';
		throw std::out_of_range(stream.str());
	}
	s._array = _array + _offset[0] * i;
	s._accumulatedOffset = accumulatedOffset(i);
	for (std::size_t j = 1; j < D; ++j) {
		s._size[j-1] = _size[j];
		s._offset[j-1] = _offset[j];
	}
}

template < typename T, std::size_t D >
inline DopeVector<T, D-1> DopeVector<T, D>::operator[](const std::size_t i) const
{
	DopeVector<T, D-1> s;
	at(i, s);
	return s;
}

template < typename T, std::size_t D >
inline void DopeVector<T, D>::slice(const std::size_t d, const std::size_t i, DopeVector<T, D-1> &s) const
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
	std::size_t k = 0;
	for (std::size_t j = 0; j < d; ++j, ++k) {
		s._size[k] = _size[j];
		s._offset[k] = _offset[j];
	}
	for (std::size_t j = d+1; j < D; ++j, ++k) {
		s._size[k] = _size[j];
		s._offset[k] = _offset[j];
	}
}

template < typename T, std::size_t D >
inline DopeVector<T, D-1> DopeVector<T, D>::slice(const std::size_t d, const std::size_t i) const
{
	DopeVector<T, D-1> s;
	slice(d, i, s);
	return s;
}

template < typename T, std::size_t D >
inline void DopeVector<T, D>::permute(const std::array<std::size_t, D> &order, DopeVector<T, D> &p) const
{
	std::array<bool, D> included = {{false}};
	for (std::size_t d = 0; d < D; ++d) {
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
	std::size_t newSize[D], newOffset[D];
	std::memcpy(newSize, _size.data(), D * sizeof(std::size_t));
	std::memcpy(newOffset, _offset.data(), D * sizeof(std::size_t));
	for (std::size_t d = 0; d < D; ++d) {
		p._size[d] = newSize[order[d]];
		p._offset[d] = newOffset[order[d]];
	}
}

template < typename T, std::size_t D >
inline DopeVector<T, D> DopeVector<T, D>::permute(const std::array<std::size_t, D> &order) const
{
	DopeVector<T, D> p;
	permute(order, p);
	return p;
}

template < typename T, std::size_t D >
inline void DopeVector<T, D>::window(const std::array<std::size_t, D> &start, std::array<std::size_t, D> &size, DopeVector<T, D> &w) const
{
	for (std::size_t d = 0; d < D; ++d) {
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
	std::size_t newAccumulatedOffset = _accumulatedOffset;
	for (std::size_t d = 0; d < D; ++d)
	newAccumulatedOffset += _offset[d] * start[d];
	w._array = _array + (newAccumulatedOffset - _accumulatedOffset);
	w._accumulatedOffset = newAccumulatedOffset;
	w._size = size;
	w._offset = _offset;
}

template < typename T, std::size_t D >
inline DopeVector<T, D> DopeVector<T, D>::window(const std::array<std::size_t, D> &start, const std::array<std::size_t, D> &size) const
{
	DopeVector<T, D> w;
	window(start, size, w);
	return w;
}

template < typename T, std::size_t D >
inline void DopeVector<T, D>::import(const DopeVector<T, D> &o)
{
	if (&o == this)
	return;
	if (_size[0] != o._size[0])
	throw std::out_of_range("Matrixes do not have same size.");
	for (std::size_t i = 0; i < _size[0]; ++i)
	operator[](i).import(o[i]);
}

template < typename T, std::size_t D >
inline std::size_t DopeVector<T, D>::sizeAt(const std::size_t d) const
{
	if (d >= D) {
		std::stringstream stream;
		stream << "Index " << d << " is out of range [0, " << D-1 << ']';
		throw std::out_of_range(stream.str());
	}
	return _size[d];
}

template < typename T, std::size_t D >
inline void DopeVector<T, D>::allSizes(std::array<std::size_t, D> &s) const
{
	s = _size;
}

template < typename T, std::size_t D >
inline std::size_t DopeVector<T, D>::size() const
{
	std::size_t total = _size[0];
	for (std::size_t i = 1; i < D; ++i)
	total *= _size[i];
	return total;
}

template < typename T, std::size_t D >
inline std::size_t DopeVector<T, D>::accumulatedOffset(const std::size_t i, const std::size_t d) const
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



template < typename T >
inline DopeVector<T, 1>::DopeVector()
	: _array(nullptr)
	, _accumulatedOffset(0)
	, _size({{0}})
	, _offset({{0}})
{ }

template < typename T >
inline DopeVector<T, 1>::DopeVector(const T *array, const std::size_t accumulatedOffset, const std::size_t size)
	: _array(const_cast<T*>(array))
	, _accumulatedOffset(accumulatedOffset)
	, _size({{size}})
	, _offset({{0}})
{ }

template < typename T >
inline DopeVector<T, 1>::DopeVector(const T *array, const std::size_t accumulatedOffset, const std::size_t size, const std::size_t offset)
	: _array(const_cast<T*>(array))
	, _accumulatedOffset(accumulatedOffset)
	, _size({{size}})
	, _offset({{offset}})
{ }

template < typename T >
inline DopeVector<T, 1>::DopeVector(const T *array, const std::size_t accumulatedOffset, const std::array<std::size_t, 1> &size)
	: _array(const_cast<T*>(array))
	, _accumulatedOffset(accumulatedOffset)
	, _size(size)
	, _offset({{0}})
{ }

template < typename T >
inline DopeVector<T, 1>::DopeVector(const T *array, const std::size_t accumulatedOffset, const std::array<std::size_t, 1> &size, const std::array<std::size_t, 1> &offset)
	: _array(const_cast<T*>(array))
	, _accumulatedOffset(accumulatedOffset)
	, _size(size)
	, _offset(offset)
{ }

template < typename T >
inline const T & DopeVector<T, 1>::operator[](const std::size_t i) const
{
	if (i >= _size[0]) {
		std::stringstream stream;
		stream << "Index " << i << " is out of range [0, " << _size[0]-1 << ']';
		throw std::out_of_range(stream.str());
	}
	return *(_array + i * _offset[0]);
}

template < typename T >
inline T & DopeVector<T, 1>::operator[](const std::size_t i)
{
	if (i >= _size[0]) {
		std::stringstream stream;
		stream << "Index " << i << " is out of range [0, " << _size[0]-1 << ']';
		throw std::out_of_range(stream.str());
	}
	return *(_array + i * _offset[0]);
}

template < typename T >
inline const T & DopeVector<T, 1>::slice(const std::size_t i) const
{
	return *this[i];
}

template < typename T >
inline T & DopeVector<T, 1>::slice(const std::size_t i)
{
	return *this[i];
}

template < typename T >
inline void DopeVector<T, 1>::permute(const std::array<std::size_t, 1> &order, DopeVector<T, 1> &p) const
{
	for (std::size_t d = 0; d < 1; ++d) {
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
	std::size_t newSize[1] = {_size[0]};
	std::size_t newOffset[1] = {_offset[0]};
	for (std::size_t d = 0; d < 1; ++d) {
		p._size[d] = newSize[order[d]];
		p._offset[d] = newOffset[order[d]];
	}
}

template < typename T >
inline DopeVector<T, 1> DopeVector<T, 1>::permute(const std::array<std::size_t, 1> &order) const
{
	DopeVector<T, 1> p;
	permute(order, p);
	return p;
}

template < typename T >
inline void DopeVector<T, 1>::window(const std::array<std::size_t, 1> &start, const std::array<std::size_t, 1> &size, DopeVector<T, 1> &w) const
{
	if (start[0] >= _size[0]) {
		std::stringstream stream;
		stream << "Index " << start[0] << " is out of range [0, " << _size[0] << ']';
		throw std::out_of_range(stream.str());
	}
	if (start[0] + size[0] > _size[0]) {
		std::stringstream stream;
		stream << "Window size " << size[0] << " is out of range [" << 0 << ", " << _size[0] - start[0] << ']';
		throw std::out_of_range(stream.str());
	}
	std::size_t newAccumulatedOffset = _accumulatedOffset + _offset[0] * start[0];
	w._array = _array + (newAccumulatedOffset - _accumulatedOffset);
	w._accumulatedOffset = newAccumulatedOffset;
	w._size[0] = size[0];
	w._offset[0] = _offset[0];
}

template < typename T >
inline DopeVector<T, 1> DopeVector<T, 1>::window(const std::array<std::size_t, 1> &start, const std::array<std::size_t, 1> &size) const
{
	DopeVector<T, 1> w;
	window(start, size, w);
	return w;
}

template < typename T >
inline void DopeVector<T, 1>::window(const std::size_t start, const std::size_t size, DopeVector<T, 1> &w) const
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

template < typename T >
inline DopeVector<T, 1> DopeVector<T, 1>::window(const std::size_t start, const std::size_t size) const
{
	DopeVector<T, 1> w;
	window(start, size, w);
	return w;
}

template < typename T >
inline void DopeVector<T, 1>::import(const DopeVector<T, 1> &o)
{
	if (&o == this)
	return;
	if (_size[0] != o._size[0])
	throw std::out_of_range("Matrixes do not have same size.");
	for (std::size_t i = 0; i < _size[0]; ++i)
	operator[](i) = o[i];
}

template < typename T >
inline std::size_t DopeVector<T, 1>::sizeAt(const std::size_t d) const
{
	if (d >= 1) {
		std::stringstream stream;
		stream << "Index " << d << " is out of range [0, " << 0 << ']';
		throw std::out_of_range(stream.str());
	}
	return _size[0];
}

template < typename T >
inline void DopeVector<T, 1>::allSizes(std::array<std::size_t, 1> &s) const
{
	s = _size;
}

template < typename T >
inline std::size_t DopeVector<T, 1>::size() const
{
	return _size[0];
}

template < typename T >
inline std::size_t DopeVector<T, 1>::accumulatedOffset(const std::size_t i) const
{
	if (i >= _size[0]) {
		std::stringstream stream;
		stream << "Index " << i << " is out of range [0, " << _size[0]-1 << ']';
		throw std::out_of_range(stream.str());
	}
	return _accumulatedOffset + _offset[0] * i;
}
