///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <carina/capd/section.hpp>

namespace Carina
{

template<typename MapT>
class PointSectionRelation
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    static bool isBeforeSection(AffineSection<MapT> section, VectorType point)
    {
        const VectorType x = point - section.getOrigin();
        return capd::vectalg::scalarProduct(x, section.getNormalVector()) < ScalarType(0.0);
    }
};

}
