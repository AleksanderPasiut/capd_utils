///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <carina/map_base.hpp>
#include <carina/concat.hpp>
#include <carina/local_coordinate_system.hpp>

namespace Carina
{

template<typename MapT>
class AffineMap : private MapBase<MapT>
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    AffineMap(VectorType origin, VectorType multiplier) : m_origin(origin), m_multiplier( Concat<MapT>::build_matrix_from_hvectors({ multiplier } ) )
    {
        if (origin.dimension() != multiplier.dimension())
        {
            throw std::logic_error("AffineMap parameter dimensions mismatch (1)!");
        }
    }

    AffineMap(VectorType origin, MatrixType multiplier) : m_origin(origin), m_multiplier(multiplier)
    {
        if (origin.dimension() != multiplier.dimension().second)
        {
            throw std::logic_error("AffineMap parameter dimensions mismatch (2)!");
        }
    }

    AffineMap(const LocalCoordinateSystem<MapT>& coeffs) : m_origin(coeffs.get_origin()), m_multiplier(coeffs.get_directions_matrix())
    {}

    VectorType operator() (const VectorType& vec, MatrixType& mat) override
    {
        this->assert_vector_size(vec, m_origin.dimension(), "AffineMap vec vector mismatch (1)!");

        mat = m_multiplier;
        return m_multiplier * vec + m_origin;
    }

    VectorType operator() (const VectorType& vec) override
    {
        this->assert_vector_size(vec, m_origin.dimension(), "AffineMap vec vector mismatch (2)!");

        return m_multiplier * vec + m_origin;
    }

    unsigned dimension() const noexcept override
    {
        return m_origin.dimension();
    }

    unsigned imageDimension() const noexcept override
    {
        return m_multiplier.dimension().first;
    }

private:
    const VectorType m_origin;
    const MatrixType m_multiplier;
};


template<typename MapT>
class AffineMap2 : private MapBase<MapT>
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    AffineMap2(VectorType offset, VectorType multiplier) : m_offset(offset), m_multiplier( Concat<MapT>::build_matrix_from_hvectors({ multiplier } ) )
    {
        if (offset.dimension() != multiplier.dimension())
        {
            throw std::logic_error("AffineMap2 parameter dimensions mismatch (1)!");
        }
    }

    AffineMap2(VectorType offset, MatrixType multiplier) : m_offset(offset), m_multiplier(multiplier)
    {
        if (offset.dimension() != multiplier.dimension().second)
        {
            throw std::logic_error("AffineMap2 parameter dimensions mismatch (2)!");
        }
    }

    VectorType operator() (const VectorType& vec, MatrixType& mat) override
    {
        this->assert_vector_size(vec, m_offset.dimension(), "AffineMap2 vec vector mismatch (1)!");

        mat = m_multiplier;
        return m_multiplier * (vec - m_offset);
    }

    VectorType operator() (const VectorType& vec) override
    {
        this->assert_vector_size(vec, m_offset.dimension(), "AffineMap2 vec vector mismatch (2)!");

        return m_multiplier * (vec - m_offset);
    }

    unsigned dimension() const noexcept override
    {
        return m_offset.dimension();
    }

    unsigned imageDimension() const noexcept override
    {
        return m_multiplier.dimension().first;
    }

private:
    const VectorType m_offset;
    const MatrixType m_multiplier;
};

}
