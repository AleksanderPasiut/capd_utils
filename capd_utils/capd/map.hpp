///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "basic_types.hpp"
#include <capd/map/Map.hpp>

namespace CapdUtils
{

using RMap = capd::map::Map<RMatrix>;
using IMap = capd::map::Map<IMatrix>;

using LRMap = capd::map::Map<LRMatrix>;
using LIMap = capd::map::Map<LIMatrix>;

#ifdef __HAVE_MPFR__

using MpRMap = capd::map::Map<MpRMatrix>;
using MpIMap = capd::map::Map<MpIMatrix>;

#endif

}

namespace capd
{
namespace map
{

extern template class Map<CapdUtils::RMatrix>;
extern template class Map<CapdUtils::IMatrix>;

extern template class Map<CapdUtils::LRMatrix>;
extern template class Map<CapdUtils::LIMatrix>;

#ifdef __HAVE_MPFR__

extern template class Map<CapdUtils::MpRMatrix>;
extern template class Map<CapdUtils::MpIMatrix>;

#endif

}
}
