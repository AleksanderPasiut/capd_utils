///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "timemap.hpp"

namespace capd
{
namespace poincare
{

template class TimeMap<Carina::OdeSolver<Carina::RMap>>;
template class TimeMap<Carina::OdeSolver<Carina::IMap>>;

template class TimeMap<Carina::OdeSolver<Carina::LRMap>>;
template class TimeMap<Carina::OdeSolver<Carina::LIMap>>;

#ifdef __HAVE_MPFR__

template class TimeMap<Carina::OdeSolver<Carina::MpRMap>>;
template class TimeMap<Carina::OdeSolver<Carina::MpIMap>>;

#endif

}
}
