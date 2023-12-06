///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "basic_types.hpp"
#include "map.hpp"
#include "ode_solver.hpp"
#include "section.hpp"
#include <capd/poincare/PoincareMap.hpp>

namespace CapdUtils
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

extern template class BasicPoincareMap<CapdUtils::OdeSolver<CapdUtils::RMap>, CapdUtils::CoordinateSection<CapdUtils::RMap>>;
extern template class PoincareMap<CapdUtils::OdeSolver<CapdUtils::IMap>, CapdUtils::CoordinateSection<CapdUtils::IMap>>;

extern template class BasicPoincareMap<CapdUtils::OdeSolver<CapdUtils::LRMap>, CapdUtils::CoordinateSection<CapdUtils::LRMap>>;
extern template class PoincareMap<CapdUtils::OdeSolver<CapdUtils::LIMap>, CapdUtils::CoordinateSection<CapdUtils::LIMap>>;

extern template class BasicPoincareMap<CapdUtils::OdeSolver<CapdUtils::RMap>, CapdUtils::AffineSection<CapdUtils::RMap>>;
extern template class PoincareMap<CapdUtils::OdeSolver<CapdUtils::IMap>, CapdUtils::AffineSection<CapdUtils::IMap>>;

extern template class BasicPoincareMap<CapdUtils::OdeSolver<CapdUtils::LRMap>, CapdUtils::AffineSection<CapdUtils::LRMap>>;
extern template class PoincareMap<CapdUtils::OdeSolver<CapdUtils::LIMap>, CapdUtils::AffineSection<CapdUtils::LIMap>>;

#ifdef __HAVE_MPFR__

extern template class BasicPoincareMap<CapdUtils::OdeSolver<CapdUtils::MpRMap>, CapdUtils::CoordinateSection<CapdUtils::MpRMap>>;
extern template class PoincareMap<CapdUtils::OdeSolver<CapdUtils::MpIMap>, CapdUtils::CoordinateSection<CapdUtils::MpIMap>>;

extern template class BasicPoincareMap<CapdUtils::OdeSolver<CapdUtils::MpRMap>, CapdUtils::AffineSection<CapdUtils::MpRMap>>;
extern template class PoincareMap<CapdUtils::OdeSolver<CapdUtils::MpIMap>, CapdUtils::AffineSection<CapdUtils::MpIMap>>;

#endif

}
}
