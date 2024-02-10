///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "basic_types.hpp"

namespace CapdUtils
{

using RMap = capd::map::Map<RMatrix>;
using IMap = capd::map::Map<IMatrix>;

#ifdef __HAVE_LONG__

using LRMap = capd::map::Map<LRMatrix>;
using LIMap = capd::map::Map<LIMatrix>;

#endif

#ifdef __HAVE_MPFR__

using MpRMap = capd::map::Map<MpRMatrix>;
using MpIMap = capd::map::Map<MpIMatrix>;

#endif

}

namespace capd
{
namespace map
{

extern template class BasicFunction<CapdUtils::Real>;
extern template class BasicFunction<CapdUtils::Interval>;

#ifdef __HAVE_LONG__

extern template class BasicFunction<CapdUtils::LReal>;
extern template class BasicFunction<CapdUtils::LInterval>;

#endif

#ifdef __HAVE_MPFR__

extern template class BasicFunction<CapdUtils::MpReal>;
extern template class BasicFunction<CapdUtils::MpInterval>;

#endif

extern template class Map<CapdUtils::RMatrix>;
extern template class Map<CapdUtils::IMatrix>;

#ifdef __HAVE_LONG__

extern template class Map<CapdUtils::LRMatrix>;
extern template class Map<CapdUtils::LIMatrix>;

#endif

#ifdef __HAVE_MPFR__

extern template class Map<CapdUtils::MpRMatrix>;
extern template class Map<CapdUtils::MpIMatrix>;

#endif

}
}
