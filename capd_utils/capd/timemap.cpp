///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "timemap.hpp"

namespace capd
{
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

}
}
