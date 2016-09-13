#ifndef Index_hpp
#define Index_hpp

#include <Common/Common.hpp>
#include <array>

namespace container {

template < SizeType Dimension >
class Index : public std::array<SizeType, Dimension> {
public:
    inline Index();
    inline Index(const std::array<SizeType, Dimension> &array);
    inline Index(const SizeType value);

    inline Index(const Index &o) = default;
    inline Index(Index &&o) = default;

    virtual ~Index();

    static constexpr Index Zero();
    static constexpr Index Ones();
    static constexpr Index Constant(const SizeType value);

    inline void reset(const SizeType value = static_cast<SizeType>(0));

    inline Index& operator= (const Index &o);
    inline Index  operator+ (const Index &o) const;
    inline Index  operator- (const Index &o) const;
    inline Index  operator* (const Index &o) const;
    inline Index  operator/ (const Index &o) const;
    inline Index& operator+=(const Index &o);
    inline Index& operator-=(const Index &o);
    inline Index& operator*=(const Index &o);
    inline Index& operator/=(const Index &o);
    inline Index  operator* (const SizeType n) const;
    inline Index  operator/ (const SizeType n) const;
};

typedef Index<1> Index1;
typedef Index<2> Index2;
typedef Index<3> Index3;
typedef Index<4> Index4;

}

#include <Common/inlines/Index.inl>

#endif // Index_hpp
