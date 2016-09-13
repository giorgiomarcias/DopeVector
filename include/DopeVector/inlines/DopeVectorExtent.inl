#include <DopeVector/DopeVectorExtent.hpp>

namespace container {

    template < typename T, SizeType Dimension >
    inline DopeVectorExtent<T, Dimension>::DopeVectorExtent(const Index<Dimension> &size)
        : DopeVector<T, Dimension>()
		, _arrayPtr(nullptr)
	{
		resize(size);
	}

    template < typename T, SizeType Dimension >
    inline DopeVectorExtent<T, Dimension> & DopeVectorExtent<T, Dimension>::operator=(const DopeVectorExtent &other)
	{
		if (&other != this) {
            Index<Dimension> size;
			other.allSizes(size);
			resize(size);
			std::copy(_arrayPtr.get(), _arrayPtr.get() + DopeVector<T, Dimension>::size(), other._arrayPtr.get());
		}
		return *this;
	}

    template < typename T, SizeType Dimension >
    inline void DopeVectorExtent<T, Dimension>::import(const DopeVector<T, Dimension> &o)
	{
		if (&o == this)
			return;
		try {
            const DopeVectorExtent<T, Dimension> &oo = dynamic_cast<const DopeVectorExtent<T, Dimension> &>(o);
            for (SizeType d = 0; d < Dimension; ++d)
            if (DopeVector<T, Dimension>::sizeAt(d) != oo.sizeAt(d))
				throw std::out_of_range("Matrixes do not have same size.");
            std::copy(_arrayPtr.get(), _arrayPtr.get() + DopeVector<T, Dimension>::size(), oo._arrayPtr.get());
		} catch(std::bad_cast &bc) {
            DopeVector<T, Dimension>::import(o);
		}
	}



    template < typename T, SizeType Dimension >
    inline void DopeVectorExtent<T, Dimension>::resize(const Index<Dimension> &size)
	{
        SizeType total = size[0];
        for (SizeType i = 1; i < Dimension; ++i)
			total *= size[i];
		if (total > 0) {
            if (total != DopeVector<T, Dimension>::size())
				_arrayPtr.reset(new T[total]);		// Be aware: data is LOST!
            DopeVector<T, Dimension>::operator=(DopeVector<T, Dimension>(_arrayPtr.get(), 0, size));
		} else {
			clear();
		}
	}

    template < typename T, SizeType Dimension >
    inline void DopeVectorExtent<T, Dimension>::clear()
	{
		_arrayPtr.reset(nullptr);
        DopeVector<T, Dimension>::operator=(DopeVector<T, Dimension>());
	}

}
