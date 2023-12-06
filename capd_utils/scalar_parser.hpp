///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>

#include "capd/basic_types.hpp"

namespace CapdUtils
{

template<typename ScalarOut>
struct ScalarParser
{
    template<typename T>
    struct always_false
    {
        static constexpr bool value = false;
    };

    static_assert(always_false<ScalarOut>::value, "Unsupported scalar conversion!");
};

template<>
struct ScalarParser<Real>
{
    Real operator() (const std::string& arg) const
    {
        return capd::basicalg::inferFromString<Real, false>(arg);
    }
};

template<>
struct ScalarParser<Interval>
{
    Interval operator() (const std::string& arg) const
    {
        ScalarParser<Real> internal_parser;
        const Real value = internal_parser(arg);
        return Interval(value, value);
    }
};

#ifdef __HAVE_MPFR__

template<>
struct ScalarParser<MpReal>
{
    MpReal operator() (const std::string& arg) const
    {
        return MpReal(arg);
    }
};

template<>
struct ScalarParser<MpInterval>
{
    MpInterval operator() (const std::string& arg) const
    {
        const MpReal value( arg );
        return MpInterval(value, value);
    }
};

#endif

template<typename ScalarOut>
inline ScalarOut parse(const std::string& arg)
{
    ScalarParser<ScalarOut> parser {};
    return parser(arg);
}

}
