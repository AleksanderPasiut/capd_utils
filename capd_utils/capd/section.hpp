///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "basic_types.hpp"
#include <capd/poincare/AbstractSection.hpp>

namespace CapdUtils
{

template<typename MapT>
using CoordinateSection = capd::poincare::CoordinateSection<typename MapT::MatrixType>;

template<typename MapT>
using AffineSection = capd::poincare::AffineSection<typename MapT::MatrixType>;

}