///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "poincare_map.hpp"

namespace capd
{
namespace poincare
{

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
