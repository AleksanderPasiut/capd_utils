///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "map.hpp"
#include "ode_solver.hpp"
#include "timemap.hpp"
#include "poincare_map.hpp"


#include <capd/map/Map.hpp>
#include <capd/dynsys/BasicOdeSolver.hpp>
#include <capd/dynsys/OdeSolver.hpp>
#include <capd/poincare/TimeMap.hpp>
#include <capd/poincare/PoincareMap.hpp>

namespace capd
{
namespace map
{

template class BasicFunction<CapdUtils::Real>;
template class BasicFunction<CapdUtils::Interval>;

template class BasicFunction<CapdUtils::LReal>;
template class BasicFunction<CapdUtils::LInterval>;

#ifdef __HAVE_MPFR__

template class BasicFunction<CapdUtils::MpReal>;
template class BasicFunction<CapdUtils::MpInterval>;

#endif

template class Map<CapdUtils::RMatrix>;
template class Map<CapdUtils::IMatrix>;

template class Map<CapdUtils::LRMatrix>;
template class Map<CapdUtils::LIMatrix>;

#ifdef __HAVE_MPFR__

template class Map<CapdUtils::MpRMatrix>;
template class Map<CapdUtils::MpIMatrix>;

#endif

}

namespace dynsys
{

template class BasicOdeSolver<CapdUtils::RMap>;
template class OdeSolver<CapdUtils::IMap>;

template class BasicOdeSolver<CapdUtils::LRMap>;
template class OdeSolver<CapdUtils::LIMap>;

#ifdef __HAVE_MPFR__

template class BasicOdeSolver<CapdUtils::MpRMap>;
template class OdeSolver<CapdUtils::MpIMap>;

#endif

}

namespace poincare
{

template class TimeMap<CapdUtils::OdeSolver<CapdUtils::RMap>>;
template class TimeMap<CapdUtils::OdeSolver<CapdUtils::IMap>>;

template class TimeMap<CapdUtils::OdeSolver<CapdUtils::LRMap>>;
template class TimeMap<CapdUtils::OdeSolver<CapdUtils::LIMap>>;

#ifdef __HAVE_MPFR__

template class TimeMap<CapdUtils::OdeSolver<CapdUtils::MpRMap>>;
template class TimeMap<CapdUtils::OdeSolver<CapdUtils::MpIMap>>;

#endif

template class BasicPoincareMap<CapdUtils::OdeSolver<CapdUtils::RMap>, CapdUtils::CoordinateSection<CapdUtils::RMap>>;
template class PoincareMap<CapdUtils::OdeSolver<CapdUtils::IMap>, CapdUtils::CoordinateSection<CapdUtils::IMap>>;

template class BasicPoincareMap<CapdUtils::OdeSolver<CapdUtils::LRMap>, CapdUtils::CoordinateSection<CapdUtils::LRMap>>;
template class PoincareMap<CapdUtils::OdeSolver<CapdUtils::LIMap>, CapdUtils::CoordinateSection<CapdUtils::LIMap>>;

template class BasicPoincareMap<CapdUtils::OdeSolver<CapdUtils::RMap>, CapdUtils::AffineSection<CapdUtils::RMap>>;
template class PoincareMap<CapdUtils::OdeSolver<CapdUtils::IMap>, CapdUtils::AffineSection<CapdUtils::IMap>>;

template class BasicPoincareMap<CapdUtils::OdeSolver<CapdUtils::LRMap>, CapdUtils::AffineSection<CapdUtils::LRMap>>;
template class PoincareMap<CapdUtils::OdeSolver<CapdUtils::LIMap>, CapdUtils::AffineSection<CapdUtils::LIMap>>;

#ifdef __HAVE_MPFR__

template class BasicPoincareMap<CapdUtils::OdeSolver<CapdUtils::MpRMap>, CapdUtils::CoordinateSection<CapdUtils::MpRMap>>;
template class PoincareMap<CapdUtils::OdeSolver<CapdUtils::MpIMap>, CapdUtils::CoordinateSection<CapdUtils::MpIMap>>;

template class BasicPoincareMap<CapdUtils::OdeSolver<CapdUtils::MpRMap>, CapdUtils::AffineSection<CapdUtils::MpRMap>>;
template class PoincareMap<CapdUtils::OdeSolver<CapdUtils::MpIMap>, CapdUtils::AffineSection<CapdUtils::MpIMap>>;

#endif

}
}

