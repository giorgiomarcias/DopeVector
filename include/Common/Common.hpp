#ifndef Common_hpp
#define Common_hpp

#include <array>

namespace Container {

typedef std::size_t SizeType;

template < SizeType Dimension >
class Index : public std::array<SizeType, Dimension> {
public:
    Index() : std::array<SizeType, Dimension>() {}
    Index(const SizeType value) : std::array<SizeType, Dimension>() { reset(value); }

    Index(const Index &o) = default;
    Index(Index &&o) = default;

    static Index Zero() { return Index(static_cast<SizeType>(0)); }
    static Index Ones() { return Index(static_cast<SizeType>(1)); }
    static Index Constant(const SizeType value) { return Index(value); }

    template < SizeType V >
    static Index Constant() { return Index(V); }

    inline void reset(const SizeType value = static_cast<SizeType>(0)) {
        for(SizeType i = static_cast<SizeType>(0); i < Dimension; ++i) {
            *this[i] = value;
        }
    }

    inline Index& operator=(const Index &o) {
        for(SizeType i = static_cast<SizeType>(0); i < Dimension; ++i) {
            *this[i] = o[i];
        }
        return *this;
    }

    inline Index& operator+(const Index &o) {
        for(SizeType i = static_cast<SizeType>(0); i < Dimension; ++i) {
            *this[i] += o[i];
        }
        return *this;
    }

    inline Index& operator-(const Index &o) {
        for(SizeType i = static_cast<SizeType>(0); i < Dimension; ++i) {
            *this[i] -= o[i];
        }
        return *this;
    }

    inline Index& operator*(const Index &o) {
        for(SizeType i = static_cast<SizeType>(0); i < Dimension; ++i) {
            *this[i] *= o[i];
        }
        return *this;
    }

    inline Index& operator/(const Index &o) {
        for(SizeType i = static_cast<SizeType>(0); i < Dimension; ++i) {
            *this[i] /= o[i];
        }
        return *this;
    }

    inline bool operator==(const Index &o) const {
        for(SizeType i = static_cast<SizeType>(0); i < Dimension; ++i) {
            if(*this[i] != o[i]) return false;
        }
        return true;
    }

    inline bool operator<(const Index &o) const {
        for(SizeType i = static_cast<SizeType>(0); i < Dimension; ++i) {
            if(*this[i] < o[i]) return true;
            if(*this[i] > o[i]) return false;
        }
        return false;
    }
};

}

#endif // Common_hpp
