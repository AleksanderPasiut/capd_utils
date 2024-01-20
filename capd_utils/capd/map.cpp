///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "map.hpp"

namespace capd
{
namespace map
{

template class Map<CapdUtils::RMatrix>;
template class Map<CapdUtils::IMatrix>;

#ifdef __FENV_ENABLED__

template class Map<CapdUtils::LRMatrix>;
template class Map<CapdUtils::LIMatrix>;

#endif

#ifdef __HAVE_MPFR__

template class Map<CapdUtils::MpRMatrix>;
template class Map<CapdUtils::MpIMatrix>;

#endif

}
}
