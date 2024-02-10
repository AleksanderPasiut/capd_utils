///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <capd/capdlib.h>

#include "fenv_rounding.hpp"

#ifdef __HAVE_MPFR__

#include <capd/mpcapdlib.h>

#endif

namespace CapdUtils
{

using Real = double;
using Interval = capd::filib::Interval<double, ::filib::native_directed, ::filib::i_mode_normal >;

using RVector = capd::vectalg::Vector<Real, 0>;
using IVector = capd::vectalg::Vector<Interval, 0>;

using RMatrix = capd::vectalg::Matrix<Real, 0, 0>;
using IMatrix = capd::vectalg::Matrix<Interval, 0, 0>;

#ifdef __HAVE_LONG__

using LReal = long double;
using LInterval = capd::intervals::Interval<LReal, capd::rounding::FenvRounding>;

using LRVector = capd::vectalg::Vector<LReal, 0>;
using LIVector = capd::vectalg::Vector<LInterval, 0>;

using LRMatrix = capd::vectalg::Matrix<LReal, 0, 0>;
using LIMatrix = capd::vectalg::Matrix<LInterval, 0, 0>;

#endif

#ifdef __HAVE_MPFR__

using MpReal = capd::multiPrec::MpReal;
using MpInterval = capd::intervals::Interval<MpReal, MpReal>;

using MpRVector = capd::vectalg::Vector<MpReal, 0>;
using MpIVector = capd::vectalg::Vector<MpInterval, 0>;

using MpRMatrix = capd::vectalg::Matrix<MpReal, 0, 0>;
using MpIMatrix = capd::vectalg::Matrix<MpInterval, 0, 0>;

#endif

using Node = capd::autodiff::Node;

}
