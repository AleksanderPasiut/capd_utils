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

#ifdef __FENV_ENABLED__

template class TimeMap<CapdUtils::OdeSolver<CapdUtils::LRMap>>;
template class TimeMap<CapdUtils::OdeSolver<CapdUtils::LIMap>>;

#endif

#ifdef __HAVE_MPFR__

template class TimeMap<CapdUtils::OdeSolver<CapdUtils::MpRMap>>;
template class TimeMap<CapdUtils::OdeSolver<CapdUtils::MpIMap>>;

#endif

}
}
