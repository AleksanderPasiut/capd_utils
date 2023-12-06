///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "map_base.hpp"

namespace Carina
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Lambda map
//! @details Wrapper for callable object of signature VectorType(const VectorType&, MatrixType&)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT, typename CallableT>
class LambdaMap
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    LambdaMap(CallableT callable, unsigned dim, unsigned img_dim) : m_callable(callable), m_dim(dim), m_img_dim(img_dim)
    {}

    VectorType operator() (const VectorType& vec)
    {
        MatrixType dummy(m_dim, m_img_dim);
        return m_callable(vec, dummy);
    }

    VectorType operator() (const VectorType& vec, MatrixType& mat)
    {
        return m_callable(vec, mat);
    }

    unsigned dimension() const
    {
        return m_dim;
    }

    unsigned imageDimension() const
    {
        return m_img_dim;
    }

private:
    CallableT m_callable;
    unsigned m_dim;
    unsigned m_img_dim;
};

}
