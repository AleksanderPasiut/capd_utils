///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "basic_types.hpp"
#include <capd/dynset/C0DoubletonSet.hpp>

namespace CapdUtils
{

template<typename MapT>
using C0Rect2Set = capd::dynset::C0DoubletonSet<typename MapT::MatrixType, capd::C0Rect2Policies>;

}
