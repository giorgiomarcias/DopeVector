// Copyright (c) 2016 Giorgio Marcias
//
// This source code is subject to Apache 2.0 License.
//
// Author: Giorgio Marcias
// email: marcias.giorgio@gmail.com

#ifndef DopeVectorExtent_hpp
#define DopeVectorExtent_hpp

#include <DopeVector/DopeVector.hpp>
#include <memory>

namespace dp {

/// The DopeVectorExtent class is a wrapper of DopeVector providing a built-in
/// memory storage and management.
template < typename T, std::size_t D >
class DopeVectorExtent : public DopeVector<T, D> {
public:
    /**
     *    @brief Default constructor.
     */
	inline DopeVectorExtent() = default;

    /**
     *    @brief Initializer contructor.
     *    @param size               Sizes of the D-dimensional matrix.
     */
    inline DopeVectorExtent(const std::array<std::size_t, D> &size)
        : DopeVector<T, D>()
        , _arrayPtr(nullptr)
    {
        resize(size);
    }

    /**
     *    @brief Copy constructor.
     */
	DopeVectorExtent(const DopeVectorExtent<T, D> &other) = default;

    /**
     *    @brief Move constructor.
     */
	DopeVectorExtent(DopeVectorExtent &&other) = default;

    /**
     *    @brief Copy assignment operator.
     */
    inline DopeVectorExtent & operator=(const DopeVectorExtent &other)
    {
        if (&other != this) {
            std::array<std::size_t, D> size;
            other.size(size);
            resize(size);
            std::memcpy(_arrayPtr.get(), other._arrayPtr.get(), DopeVector<T, D>::totalSize() * sizeof(T));
        }
        return *this;
    }

    /**
     *    @brief Move assignment operator.
     */
	DopeVectorExtent & operator=(DopeVectorExtent &&other) = default;

    /**
     *    @brief Changes the sizes of this matrix.
     *    @param size               The new sizes.
     */
    inline void resize(const std::array<std::size_t, D> &size)
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

    /**
     *    @brief Empties this array, making its size 0.
     */
    inline void clear()
    {
        _arrayPtr.reset(nullptr);
        DopeVector<T, D>::operator=(DopeVector<T, D>());
    }

    /**
     *    @brief Copies all single elements from o to this matrix.
     *    @param o                  The matrix to copy from.
     */
    inline void import(const DopeVector<T, D> &o)
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

private:
    std::unique_ptr<T[]>    _arrayPtr;              ///< Sharp pointer in memory to this matrix, with automatic storage (de)allocation.
};

//#include <DopeVector/inlines/DopeVectorExtent.inl>

}

#endif /* DopeVectorExtent_hpp */
