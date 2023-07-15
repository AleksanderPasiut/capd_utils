///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "basic_types.hpp"
#include <capd/map/Map.hpp>

namespace Carina
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

extern template class Map<Carina::RMatrix>;
extern template class Map<Carina::IMatrix>;

extern template class Map<Carina::LRMatrix>;
extern template class Map<Carina::LIMatrix>;

#ifdef __HAVE_MPFR__

extern template class Map<Carina::MpRMatrix>;
extern template class Map<Carina::MpIMatrix>;

#endif

}
}
