///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "capd/basic_types.hpp"

namespace CapdUtils
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! Covering factors
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct CoveringFactors
{
    Real internal;
    Real external;

    CoveringFactors(const Interval& X, const Interval& Y) : internal(NAN), external(NAN)
    {
        auto f = [&X](Real x) -> Real
        {
            return (2*x - X.leftBound() - X.rightBound()) / (X.rightBound() - X.leftBound());
        };

        const Real a = -f( Y.leftBound()  ) - 1.0;
        const Real b = +f( Y.rightBound() ) - 1.0;

        internal = std::min(a,b);
        external = std::max(a,b);
    }

    CoveringFactors(const IVector& X, const IVector& Y) : internal(NAN), external(NAN)
    {
        assert( X.dimension() == Y.dimension() );

        if (X.dimension() > 0)
        {
            {
                CoveringFactors cf( X[0], Y[0] );
                *this = cf;
            }

            for (unsigned i = 1; i < X.dimension(); ++i)
            {
                CoveringFactors cf( X[i], Y[i] );
                internal = std::min(this->internal, cf.internal);
                external = std::max(this->external, cf.external);
            }
        }
    }
};

}
