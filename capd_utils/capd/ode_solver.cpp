///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ode_solver.hpp"

namespace capd
{
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
}
