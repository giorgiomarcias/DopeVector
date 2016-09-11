#include <DopeVector/DopeVectorExtent.hpp>

namespace Container {

    template < typename T, SizeType Dimesnion >
    inline DopeVectorExtent<T, Dimension>::DopeVectorExtent(const IndexD &size)
		: DopeVector<T, D>()
		, _arrayPtr(nullptr)
	{
		resize(size);
	}

    template < typename T, SizeType Dimesnion >
    inline DopeVectorExtent<T, D> & DopeVectorExtent<T, Dimension>::operator=(const DopeVectorExtent &other)
	{
		if (&other != this) {
            IndexD size;
			other.allSizes(size);
			resize(size);
			std::memcpy(_arrayPtr.get(), other._arrayPtr.get(), DopeVector<T, D>::size() * sizeof(T));
		}
		return *this;
	}

    template < typename T, SizeType Dimesnion >
    inline void DopeVectorExtent<T, Dimension>::import(const DopeVector<T, Dimension> &o)
	{
		if (&o == this)
			return;
		try {
			const DopeVectorExtent<T, D> &oo = dynamic_cast<const DopeVectorExtent<T, D> &>(o);
            for (SizeType d = 0; d < D; ++d)
			if (DopeVector<T, D>::sizeAt(d) != oo.sizeAt(d))
				throw std::out_of_range("Matrixes do not have same size.");
			std::memcpy(_arrayPtr.get(), oo._arrayPtr.get(), DopeVector<T, D>::size() * sizeof(T));
		} catch(std::bad_cast &bc) {
			DopeVector<T, D>::import(o);
		}
	}



    template < typename T, SizeType Dimesnion >
    inline void DopeVectorExtent<T, Dimension>::resize(const IndexD &size)
	{
        SizeType total = size[0];
        for (SizeType i = 1; i < D; ++i)
			total *= size[i];
		if (total > 0) {
			if (total != DopeVector<T, D>::size())
				_arrayPtr.reset(new T[total]);		// Be aware: data is LOST!
			DopeVector<T, D>::operator=(DopeVector<T, D>(_arrayPtr.get(), 0, size));
		} else {
			clear();
		}
	}

    template < typename T, SizeType Dimesnion >
    inline void DopeVectorExtent<T, Dimension>::clear()
	{
		_arrayPtr.reset(nullptr);
		DopeVector<T, D>::operator=(DopeVector<T, D>());
	}

}
