///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <capd_utils/capd/gauss_solver.hpp>

namespace Carina
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Solve equation of the form A * X = B with respect to X, where A, X and B are matrices.
//! @details Implemented by performing gauss elimination for respective columns.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT>
typename MapT::MatrixType gaussMatrixSolver(
    const typename MapT::MatrixType& A,
    const typename MapT::MatrixType& B)
{
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    MatrixType ret(B.dimension());

    for (int j = 0; j < ret.numberOfColumns(); ++j)
    {
        VectorType column(ret.numberOfRows());
        for (int i = 0; i < ret.numberOfRows(); ++i)
        {
            column[i] = B(i+1, j+1);
        }

        const VectorType solution = gauss<MapT>(A, column);
        for (int i = 0; i < ret.numberOfRows(); ++i)
        {
            ret(i+1, j+1) = solution[i];
        }
    }

    return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Solve equation of the form A * X = B with respect to X, where A, X and B are matrices.
//! @details This implementation is valid for interval computation only!
//! @return The intersection of the solution obtained from the multiplication by the matrix inverse and with gaussMatrixSolver.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT>
typename MapT::MatrixType gaussMatrixSolverWithIntersect(
    const typename MapT::MatrixType& A,
    const typename MapT::MatrixType& B)
{
    using MatrixType = typename MapT::MatrixType;

    const MatrixType ret_1 = gaussInverseMatrix<MapT>(A) * B;
    const MatrixType ret_2 = gaussMatrixSolver<MapT>(A, B);

    MatrixType ret( ret_1.dimension() );
    capd::vectalg::intersection(ret_1.begin(), ret_2.begin(), ret.begin(), ret.end());
    return ret;
}

}
