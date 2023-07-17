///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "projection_map.hpp"
#include "extension_map.hpp"
#include "map_base.hpp"
#include "map_compatibility.hpp"

#include <stdexcept>

namespace Carina
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Project and extend (PNE) map
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT, typename MapU>
class PNE : public MapBase<MapT>
{
public:
    static_assert(MapCompatibility<MapT, MapU>::value);

    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Constructor
    //!
    //! @param input_size        input dimension
    //! @param in_idx_list       index list for projection map
    //! @param out_idx_list      index list for extension map
    //! @param map_u_args        parameter pack of arguments for internal map
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename... MapUArgs>
    PNE(size_t input_size,
        const IdxList<size_t>& in_idx_list,
        const IdxList<int>& out_idx_list,
        MapUArgs... map_u_args)
            : m_map_u(map_u_args...)
            , m_projection(ProjectionMap<MapT>::create(input_size, in_idx_list))
            , m_extension(ExtensionMap<MapT>::create(out_idx_list))
    {
        if ( m_projection.imageDimension() != m_map_u.dimension() )
        {
            throw std::logic_error("Projection / internal map dimensions mismatch!");
        }

        if ( m_map_u.imageDimension() != m_extension.dimension() )
        {
            throw std::logic_error("Internal / extension map dimensions mismatch!");
        }
    }

    VectorType operator() (const VectorType& vec) override
    {
        this->assert_vector_size(vec, m_projection.dimension(), "PNE vec vector size mismatch (1)!");

        const VectorType v1 = m_projection(vec);
        const VectorType v2 = m_map_u(v1);
        const VectorType v3 = m_extension(v2);

        return v3;
    }

    VectorType operator() (const VectorType& vec, MatrixType& mat) override
    {
        this->assert_vector_size(vec, m_projection.dimension(), "PNE vec vector size mismatch (2)!");

        MatrixType m1(m_projection.imageDimension(), m_projection.dimension());
        const VectorType v1 = m_projection(vec, m1);
        this->assert_matrix_size(m1, m_projection.imageDimension(), m_projection.dimension(), "PNE m1 matrix size mismatch!");

        MatrixType m2(m_map_u.imageDimension(), m_map_u.dimension());
        const VectorType v2 = m_map_u(v1, m2);
        this->assert_matrix_size(m2, m_map_u.imageDimension(), m_map_u.dimension(), "PNE m2 matrix size mismatch!");

        MatrixType m3(m_extension.imageDimension(), m_extension.dimension());
        const VectorType v3 = m_extension(v2, m3);
        this->assert_matrix_size(m3, m_extension.imageDimension(), m_extension.dimension(), "PNE m3 matrix size mismatch!");

        mat = m3 * m2 * m1;

        return v3;
    }

    unsigned dimension() const noexcept override
    {
        return m_projection.dimension();
    }

    unsigned imageDimension() const noexcept override
    {
        return m_extension.imageDimension();
    }

    MapU & internal_map() noexcept { return m_map_u; }
    MapT & extend() noexcept { return m_extension; }
    MapT & project() noexcept { return m_projection; }

private:
    MapU m_map_u;
    MapT m_projection;
    MapT m_extension;
};

}
