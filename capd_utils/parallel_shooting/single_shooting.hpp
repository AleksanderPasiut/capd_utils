///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <capd_utils/map_base.hpp>
#include <capd_utils/map_compatibility.hpp>

#include <capd_utils/extract.hpp>
#include <capd_utils/concat.hpp>

namespace CapdUtils
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Single shooting map
//!
//! @details F(x_1, x_2) = f_1 (x_1) - x_2
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT, typename MapU>
class SingleShooting : public MapBase<MapT>
{
public:
    static_assert(MapCompatibility<MapT, MapU>::value);

    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    SingleShooting(MapU map) : m_map(map)
    {}

    VectorType operator() (const VectorType& vec) override
    {
        this->assert_vector_size(vec, this->dimension(), "SingleShooting vec vector size mismatch (1)!");

        const VectorType x0 = Extract<MapT>::get_vector(vec, 0, m_map.dimension());
        const VectorType x1 = Extract<MapT>::get_vector(vec, m_map.dimension(), m_map.imageDimension());

        return m_map(x0) - x1;
    }

    VectorType operator() (const VectorType& vec, MatrixType& mat) override
    {
        this->assert_vector_size(vec, this->dimension(), "SingleShooting vec vector size mismatch (2)!");

        const VectorType x0 = Extract<MapT>::get_vector(vec, 0, m_map.dimension());
        const VectorType x1 = Extract<MapT>::get_vector(vec, m_map.dimension(), m_map.imageDimension());

        mat = MatrixType( imageDimension(), dimension() );

        const MatrixType id = -MatrixType::Identity(m_map.imageDimension());

        MatrixType der( m_map.imageDimension(), m_map.dimension() );
        const VectorType ret = m_map(x0, der) - x1;
        this->assert_matrix_size(der, m_map.imageDimension(), m_map.dimension(), "SingleShooting der matrix size mismatch!");

        Concat<MapT>::copy_matrix_on_matrix(mat, der, 0, 0);
        Concat<MapT>::copy_matrix_on_matrix(mat, id, 0, m_map.dimension());
        
        return ret;
    }

    unsigned dimension() const noexcept override
    {
        return m_map.dimension() + m_map.imageDimension();
    }

    unsigned imageDimension() const noexcept override
    {
        return m_map.imageDimension();
    }

    unsigned internalMapDimension() const noexcept
    {
        return m_map.dimension();
    }

private:
    MapU m_map;
};

}
