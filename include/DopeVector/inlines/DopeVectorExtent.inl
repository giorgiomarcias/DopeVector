namespace dp {

	template < typename T, std::size_t D >
	inline DopeVectorExtent<T, D>::DopeVectorExtent(const std::array<std::size_t, D> &size)
	: DopeVector<T, D>()
	, _arrayPtr(nullptr)
	{
		resize(size);
	}

	template < typename T, std::size_t D >
	inline DopeVectorExtent<T, D> & DopeVectorExtent<T, D>::operator=(const DopeVectorExtent &other)
	{
		if (&other != this) {
			std::array<std::size_t, D> size;
			other.size(size);
			resize(size);
			std::memcpy(_arrayPtr.get(), other._arrayPtr.get(), DopeVector<T, D>::totalSize() * sizeof(T));
		}
		return *this;
	}

	template < typename T, std::size_t D >
	inline void DopeVectorExtent<T, D>::import(const DopeVector<T, D> &o)
	{
		if (&o == this)
			return;
		try {
			const DopeVectorExtent<T, D> &oo = dynamic_cast<const DopeVectorExtent<T, D> &>(o);
			for (std::size_t d = 0; d < D; ++d)
				if (DopeVector<T, D>::sizeAt(d) != oo.sizeAt(d))
					throw std::out_of_range("Matrixes do not have same size.");
			std::memcpy(_arrayPtr.get(), oo._arrayPtr.get(), DopeVector<T, D>::size() * sizeof(T));
		} catch(std::bad_cast &bc) {
			DopeVector<T, D>::import(o);
		}
	}



	template < typename T, std::size_t D >
	inline void DopeVectorExtent<T, D>::resize(const std::array<std::size_t, D> &size)
	{
		std::size_t total = size[0];
		for (std::size_t i = 1; i < D; ++i)
			total *= size[i];
		if (total > 0) {
			if (total != DopeVector<T, D>::size())
				_arrayPtr.reset(new T[total]);		// Be aware: data is LOST!
			DopeVector<T, D>::operator=(DopeVector<T, D>(_arrayPtr.get(), 0, size));
		} else {
			clear();
		}
	}

	template < typename T, std::size_t D >
	inline void DopeVectorExtent<T, D>::clear()
	{
		_arrayPtr.reset(nullptr);
		DopeVector<T, D>::operator=(DopeVector<T, D>());
	}
	
}
