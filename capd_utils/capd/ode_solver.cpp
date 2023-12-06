///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ode_solver.hpp"

namespace capd
{
namespace dynsys
{

template class BasicOdeSolver<Carina::RMap>;
template class OdeSolver<Carina::IMap>;

template class BasicOdeSolver<Carina::LRMap>;
template class OdeSolver<Carina::LIMap>;

#ifdef __HAVE_MPFR__

template class BasicOdeSolver<Carina::MpRMap>;
template class OdeSolver<Carina::MpIMap>;

#endif

}
}
