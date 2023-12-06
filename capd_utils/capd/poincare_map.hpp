///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "basic_types.hpp"
#include "map.hpp"
#include "ode_solver.hpp"
#include "section.hpp"
#include <capd/poincare/PoincareMap.hpp>

namespace Carina
{

template<typename MapT, typename SectionT, bool is_interval>
struct PoincareMapSwitch
{};

template<typename MapT, typename SectionT>
struct PoincareMapSwitch<MapT, SectionT, false>
{
    using Type = capd::poincare::BasicPoincareMap<OdeSolver<MapT>, SectionT>;
};

template<typename MapT, typename SectionT>
struct PoincareMapSwitch<MapT, SectionT, true>
{
    using Type = capd::poincare::PoincareMap<OdeSolver<MapT>, SectionT>;
};

template<typename MapT, typename SectionT>
struct PoincareMapInternal
{
    using ScalarType = typename MapT::ScalarType;
    static constexpr bool is_interval = capd::TypeTraits<ScalarType>::isInterval;
    using Type = typename PoincareMapSwitch<MapT, SectionT, is_interval>::Type;
};

template<typename MapT, typename SectionT>
using PoincareMap = typename PoincareMapInternal<MapT, SectionT>::Type;

}

namespace capd
{
namespace poincare
{

extern template class BasicPoincareMap<Carina::OdeSolver<Carina::RMap>, Carina::CoordinateSection<Carina::RMap>>;
extern template class PoincareMap<Carina::OdeSolver<Carina::IMap>, Carina::CoordinateSection<Carina::IMap>>;

extern template class BasicPoincareMap<Carina::OdeSolver<Carina::LRMap>, Carina::CoordinateSection<Carina::LRMap>>;
extern template class PoincareMap<Carina::OdeSolver<Carina::LIMap>, Carina::CoordinateSection<Carina::LIMap>>;

extern template class BasicPoincareMap<Carina::OdeSolver<Carina::RMap>, Carina::AffineSection<Carina::RMap>>;
extern template class PoincareMap<Carina::OdeSolver<Carina::IMap>, Carina::AffineSection<Carina::IMap>>;

extern template class BasicPoincareMap<Carina::OdeSolver<Carina::LRMap>, Carina::AffineSection<Carina::LRMap>>;
extern template class PoincareMap<Carina::OdeSolver<Carina::LIMap>, Carina::AffineSection<Carina::LIMap>>;

#ifdef __HAVE_MPFR__

extern template class BasicPoincareMap<Carina::OdeSolver<Carina::MpRMap>, Carina::CoordinateSection<Carina::MpRMap>>;
extern template class PoincareMap<Carina::OdeSolver<Carina::MpIMap>, Carina::CoordinateSection<Carina::MpIMap>>;

extern template class BasicPoincareMap<Carina::OdeSolver<Carina::MpRMap>, Carina::AffineSection<Carina::MpRMap>>;
extern template class PoincareMap<Carina::OdeSolver<Carina::MpIMap>, Carina::AffineSection<Carina::MpIMap>>;

#endif

}
}
