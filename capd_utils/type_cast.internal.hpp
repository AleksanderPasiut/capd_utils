///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "capd/basic_tools.hpp"

namespace CapdUtils
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Internal implementation of scalar converter
//! @details Takes into account if coverted types are intervals or not.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ScalarOut, bool is_interval_out, typename ScalarIn, bool is_interval_in>
struct ScalarConverterInternal
{
    template<typename T>
    struct always_false
    {
        static constexpr bool value = false;
    };

    static_assert(always_false<ScalarOut>::value, "Unsupported scalar conversion!");
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Internal implementation of scalar converter specialized for: non-interval type to itself
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ScalarType>
struct ScalarConverterInternal<ScalarType, false, ScalarType, false>
{
    ScalarType operator() (const ScalarType& arg) const
    {
        return arg;
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Internal implementation of scalar converter specialized for: non-interval to non-interval
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ScalarOut, typename ScalarIn>
struct ScalarConverterInternal<ScalarOut, false, ScalarIn, false>
{
    template<typename T>
    struct always_false
    {
        static constexpr bool value = false;
    };

    static_assert(always_false<ScalarOut>::value, "Unsupported scalar conversion!");
};

#ifdef __HAVE_LONG__

template<>
struct ScalarConverterInternal<Real, false, LReal, false>
{
    Real operator() (const LReal& arg) const
    {
        return static_cast<Real>(arg);
    }
};

template<>
struct ScalarConverterInternal<LReal, false, Real, false>
{
    LReal operator() (const Real& arg) const
    {
        return static_cast<LReal>(arg);
    }
};

#endif

#ifdef __HAVE_MPFR__

template<>
struct ScalarConverterInternal<Real, false, MpReal, false>
{
    Real operator() (const MpReal& arg) const
    {
        return toDouble(arg);
    }
};

#ifdef __HAVE_LONG__

template<>
struct ScalarConverterInternal<LReal, false, MpReal, false>
{
    LReal operator() (const MpReal& arg) const
    {
        return toLongDouble(arg);
    }
};

#endif

template<>
struct ScalarConverterInternal<MpReal, false, Real, false>
{
    MpReal operator() (const Real& arg) const
    {
        return MpReal(arg);
    }
};

#ifdef __HAVE_LONG__

template<>
struct ScalarConverterInternal<MpReal, false, LReal, false>
{
    MpReal operator() (const LReal& arg) const
    {
        return MpReal(arg);
    }
};

#endif

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Internal implementation of scalar converter specialized for: interval to non-interval
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ScalarOut, typename ScalarIn>
struct ScalarConverterInternal<ScalarOut, false, ScalarIn, true>
{
    ScalarOut operator() (const ScalarIn& arg) const
    {
        using BoundType = typename BasicTools::BoundType<ScalarIn>::ScalarType;
        ScalarConverterInternal<ScalarOut, false, BoundType, false> internal {};

        const ScalarIn mid = middle(arg);
        return internal( mid.leftBound() );
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Internal implementation of scalar converter specialized for: non-interval to interval
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ScalarOut, typename ScalarIn>
struct ScalarConverterInternal<ScalarOut, true, ScalarIn, false>
{
    ScalarOut operator() (const ScalarIn& arg) const
    {
        return ScalarOut(arg, arg);
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Internal implementation of scalar converter specialized for: interval type to itself
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ScalarType>
struct ScalarConverterInternal<ScalarType, true, ScalarType, true>
{
    ScalarType operator() (const ScalarType& arg) const
    {
        return arg;
    }
};

// template<typename ScalarOut, typename ScalarIn>
// struct ScalarConverterInternal<ScalarOut, true, ScalarIn, true>
// {
//     ScalarOut operator() (const ScalarIn& arg) const
//     {
//         using BoundTypeIn  = typename BasicTools::BoundType<ScalarIn>::ScalarType;
//         using BoundTypeOut = typename BasicTools::BoundType<ScalarOut>::ScalarType;

//         using RoundingPolicyIn  = typename ScalarIn::RoundingPolicy;
//         using RoundingPolicyOut = typename ScalarOut::RoundingPolicy;

//         ScalarConverterInternal<BoundTypeOut, false, BoundTypeIn, false> internal {};

//         RoundingPolicyIn::roundDown();
//         RoundingPolicyOut::roundDown();
//         const BoundTypeOut left = internal( arg.leftBound() );

//         RoundingPolicyIn::roundUp();
//         RoundingPolicyOut::roundUp();
//         const BoundTypeOut right = internal( arg.rightBound() );

//         RoundingPolicyIn::roundCut();
//         RoundingPolicyOut::roundCut();

//         return ScalarOut(left, right);
//     }
// };

}
