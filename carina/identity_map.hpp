///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "map_base.hpp"

namespace Carina
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Identity map
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT>
class IdentityMap : public MapBase<MapT>
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Constructor
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    IdentityMap(size_t dimension) : m_dimension(dimension)
    {}

    VectorType operator() (const VectorType& vec) override
    {
        this->assert_vector_size(vec, m_dimension, "IdentityMap vec vector size mismatch (1)!");
        return vec;
    }

    VectorType operator() (const VectorType& vec, MatrixType& mat) override
    {
        this->assert_vector_size(vec, m_dimension, "IdentityMap vec vector size mismatch (2)!");

        mat = MatrixType::Identity(m_dimension);

        return vec;
    }

    unsigned dimension() const noexcept override
    {
        return m_dimension;
    }

    unsigned imageDimension() const noexcept override
    {
        return m_dimension;
    }

private:
    size_t m_dimension;
};

}
