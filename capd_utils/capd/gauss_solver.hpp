///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "basic_types.hpp"

namespace Carina
{

template<typename MapT>
typename MapT::VectorType gauss(
    const typename MapT::MatrixType& A,
    const typename MapT::VectorType& b)
{
    return capd::matrixAlgorithms::gauss<
        typename MapT::MatrixType,
        typename MapT::VectorType>(A, b);
}

template<typename MapT>
typename MapT::MatrixType gaussInverseMatrix(
    const typename MapT::MatrixType& arg)
{
    return capd::matrixAlgorithms::gaussInverseMatrix(arg);
}

}
