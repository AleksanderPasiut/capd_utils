///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace Carina
{

template<typename MapT>
struct Root
{
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;

    VectorType argument;
    ScalarType value_norm;

    bool operator== (const Root& root) const
    {
        return argument == root.argument;
    }
};

}
