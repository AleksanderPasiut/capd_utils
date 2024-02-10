///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "basic_types.hpp"
#include "map.hpp"

namespace CapdUtils
{

template<typename MapT, bool is_interval>
struct OdeSolverSwitch
{};

template<typename MapT>
struct OdeSolverSwitch<MapT, false>
{
    using Type = capd::dynsys::BasicOdeSolver<MapT>;
};

template<typename MapT>
struct OdeSolverSwitch<MapT, true>
{
    using Type = capd::dynsys::OdeSolver<MapT>;
};

template<typename MapT>
struct OdeSolverInternal
{
    using ScalarType = typename MapT::ScalarType;
    static constexpr bool is_interval = capd::TypeTraits<ScalarType>::isInterval;
    using Type = typename OdeSolverSwitch<MapT, is_interval>::Type;
};

template<typename MapT>
using OdeSolver = typename OdeSolverInternal<MapT>::Type;

}

namespace capd
{
namespace dynsys
{

extern template class BasicOdeSolver<CapdUtils::RMap>;
extern template class OdeSolver<CapdUtils::IMap>;

extern template class BasicOdeSolver<CapdUtils::LRMap>;
extern template class OdeSolver<CapdUtils::LIMap>;

#ifdef __HAVE_MPFR__

extern template class BasicOdeSolver<CapdUtils::MpRMap>;
extern template class OdeSolver<CapdUtils::MpIMap>;

#endif

}
}
