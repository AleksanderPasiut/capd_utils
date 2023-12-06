///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "basic_types.hpp"
#include "ode_solver.hpp"
#include <capd/poincare/TimeMap.hpp>

namespace CapdUtils
{

template<typename MapT>
using Timemap = capd::poincare::TimeMap<OdeSolver<MapT>>;

}

namespace capd
{
namespace poincare
{

extern template class TimeMap<CapdUtils::OdeSolver<CapdUtils::RMap>>;
extern template class TimeMap<CapdUtils::OdeSolver<CapdUtils::IMap>>;

extern template class TimeMap<CapdUtils::OdeSolver<CapdUtils::LRMap>>;
extern template class TimeMap<CapdUtils::OdeSolver<CapdUtils::LIMap>>;

#ifdef __HAVE_MPFR__

extern template class TimeMap<CapdUtils::OdeSolver<CapdUtils::MpRMap>>;
extern template class TimeMap<CapdUtils::OdeSolver<CapdUtils::MpIMap>>;

#endif

}
}
