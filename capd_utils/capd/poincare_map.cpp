///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "poincare_map.hpp"

namespace capd
{
namespace poincare
{

template class BasicPoincareMap<Carina::OdeSolver<Carina::RMap>, Carina::CoordinateSection<Carina::RMap>>;
template class PoincareMap<Carina::OdeSolver<Carina::IMap>, Carina::CoordinateSection<Carina::IMap>>;

template class BasicPoincareMap<Carina::OdeSolver<Carina::LRMap>, Carina::CoordinateSection<Carina::LRMap>>;
template class PoincareMap<Carina::OdeSolver<Carina::LIMap>, Carina::CoordinateSection<Carina::LIMap>>;

template class BasicPoincareMap<Carina::OdeSolver<Carina::RMap>, Carina::AffineSection<Carina::RMap>>;
template class PoincareMap<Carina::OdeSolver<Carina::IMap>, Carina::AffineSection<Carina::IMap>>;

template class BasicPoincareMap<Carina::OdeSolver<Carina::LRMap>, Carina::AffineSection<Carina::LRMap>>;
template class PoincareMap<Carina::OdeSolver<Carina::LIMap>, Carina::AffineSection<Carina::LIMap>>;

#ifdef __HAVE_MPFR__

template class BasicPoincareMap<Carina::OdeSolver<Carina::MpRMap>, Carina::CoordinateSection<Carina::MpRMap>>;
template class PoincareMap<Carina::OdeSolver<Carina::MpIMap>, Carina::CoordinateSection<Carina::MpIMap>>;

template class BasicPoincareMap<Carina::OdeSolver<Carina::MpRMap>, Carina::AffineSection<Carina::MpRMap>>;
template class PoincareMap<Carina::OdeSolver<Carina::MpIMap>, Carina::AffineSection<Carina::MpIMap>>;

#endif

}
}
