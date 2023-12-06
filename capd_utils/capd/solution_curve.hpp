///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <capd/capdlib.h>

namespace CapdUtils
{

template<typename MapT>
using SolutionCurve = capd::diffAlgebra::SolutionCurve<
    capd::diffAlgebra::Curve<capd::diffAlgebra::BasicCurve<typename MapT::MatrixType>>>;
    
}