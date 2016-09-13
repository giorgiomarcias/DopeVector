#include <Common/Index.hpp>

namespace container {

    template < SizeType Dimension >
    Index<Dimension>::Index() :
        std::array<SizeType, Dimension>() {}

    template < SizeType Dimension >
    Index<Dimension>::Index(const std::array<SizeType, Dimension> &array) :
        std::array<SizeType, Dimension>(array) {}

    template < SizeType Dimension >
    Index<Dimension>::Index(const SizeType value) :
        std::array<SizeType, Dimension>() {
        reset(value);
    }



    template < SizeType Dimension >
    Index<Dimension>::~Index() { }



    template < SizeType Dimension >
    constexpr Index<Dimension> Index<Dimension>::Zero() {
        return Index(static_cast<SizeType>(0));
    }

    template < SizeType Dimension >
    constexpr Index<Dimension> Index<Dimension>::Ones() {
        return Index(static_cast<SizeType>(1));
    }

    template < SizeType Dimension >
    constexpr Index<Dimension> Index<Dimension>::Constant(const SizeType value) {
        return Index(value);
    }



    template < SizeType Dimension >
    inline void Index<Dimension>::reset(const SizeType value) {
        this->fill(value);
    }



    template < SizeType Dimension >
    inline Index<Dimension>& Index<Dimension>::operator=(const Index &o) {
        std::copy(o.begin(), o.end(), this->begin());
        return *this;
    }

    template < SizeType Dimension >
    inline Index<Dimension> Index<Dimension>::operator+(const Index &o) const {
        Index<Dimension> res;
        for(SizeType i = static_cast<SizeType>(0); i < Dimension; ++i) {
            res[i] = *this[i] + o[i];
        }
        return res;
    }

    template < SizeType Dimension >
    inline Index<Dimension> Index<Dimension>::operator-(const Index &o) const {
        Index<Dimension> res;
        for(SizeType i = static_cast<SizeType>(0); i < Dimension; ++i) {
            res[i] = *this[i] - o[i];
        }
        return res;
    }

    template < SizeType Dimension >
    inline Index<Dimension> Index<Dimension>::operator*(const Index &o) const {
        Index<Dimension> res;
        for(SizeType i = static_cast<SizeType>(0); i < Dimension; ++i) {
            res[i] = *this[i] * o[i];
        }
        return res;
    }

    template < SizeType Dimension >
    inline Index<Dimension> Index<Dimension>::operator/(const Index &o) const {
        Index<Dimension> res;
        for(SizeType i = static_cast<SizeType>(0); i < Dimension; ++i) {
            res[i] = *this[i] / o[i];
        }
        return res;
    }

    template < SizeType Dimension >
    inline Index<Dimension>& Index<Dimension>::operator+=(const Index &o) {
        for(SizeType i = static_cast<SizeType>(0); i < Dimension; ++i) {
            *this[i] += o[i];
        }
        return *this;
    }

    template < SizeType Dimension >
    inline Index<Dimension>& Index<Dimension>::operator-=(const Index &o) {
        for(SizeType i = static_cast<SizeType>(0); i < Dimension; ++i) {
            *this[i] -= o[i];
        }
        return *this;
    }

    template < SizeType Dimension >
    inline Index<Dimension>& Index<Dimension>::operator*=(const Index &o) {
        for(SizeType i = static_cast<SizeType>(0); i < Dimension; ++i) {
            *this[i] *= o[i];
        }
        return *this;
    }

    template < SizeType Dimension >
    inline Index<Dimension>& Index<Dimension>::operator/=(const Index &o) {
        for(SizeType i = static_cast<SizeType>(0); i < Dimension; ++i) {
            *this[i] /= o[i];
        }
        return *this;
    }

    template < SizeType Dimension >
    inline Index<Dimension> Index<Dimension>::operator*(const SizeType n) const {
        Index<Dimension> res;
        for(SizeType i = static_cast<SizeType>(0); i < Dimension; ++i) {
            res[i] = *this[i] * n;
        }
        return res;
    }

    template < SizeType Dimension >
    inline Index<Dimension> Index<Dimension>::operator/(const SizeType n) const {
        Index<Dimension> res;
        for(SizeType i = static_cast<SizeType>(0); i < Dimension; ++i) {
            res[i] = *this[i] / n;
        }
        return res;
    }

}
