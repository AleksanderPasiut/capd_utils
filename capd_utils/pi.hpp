///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "capd/map.hpp"

namespace CapdUtils
{

template<typename ScalarType>
inline ScalarType get_pi();

template<>
inline double get_pi()
{
    return M_PI;
}

template<>
inline Interval get_pi()
{
    return Interval::pi();
}

#ifdef __HAVE_MPFR__

template<>
inline MpReal get_pi()
{
    return MpReal::pi();
}

template<>
inline MpInterval get_pi()
{
    return MpInterval::pi();
}

#endif

}
