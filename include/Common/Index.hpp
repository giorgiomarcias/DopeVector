#ifndef Index_hpp
#define Index_hpp

#include <Common/Common.hpp>
#include <array>

namespace container {

template < SizeType Dimension >
class Index : public std::array<SizeType, Dimension> {
public:
    inline Index();
    inline Index(const SizeType value);

    inline Index(const Index &o) = default;
    inline Index(Index &&o) = default;

    virtual ~Index();

    static constexpr Index Zero();
    static constexpr Index Ones();
    static constexpr Index Constant(const SizeType value);

    template < SizeType Value >
    static constexpr Index Constant();

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

#include <Common/inlines/Index.inl>

}

#endif // Index_hpp
