///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "projection_map.hpp"
#include "extension_map.hpp"
#include "map_base.hpp"
#include "map_compatibility.hpp"

#include <stdexcept>

namespace CapdUtils
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Extend and project (ENP) map
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT, typename MapU>
class ENP : public MapBase<MapT>
{
public:
    static_assert(MapCompatibility<MapT, MapU>::value);

    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Constructor
    //!
    //! @param extension_values vector of extension values for extension map
    //! @param in_idx_list      index list for extension map
    //! @param out_idx_list     index list for projection map
    //! @param map_u_args       parameter pack of arguments for internal map
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename... MapUArgs>
    ENP(const VectorType& extension_values,
        const IdxList<int>& in_idx_list,
        const IdxList<size_t>& out_idx_list,
        MapUArgs... map_u_args)
            : m_map_u(map_u_args...)
            , m_extension(ExtensionMap<MapT>::create(extension_values, in_idx_list ))
            , m_projection(ProjectionMap<MapT>::create(m_map_u.imageDimension(), out_idx_list))
    {
        if ( m_extension.imageDimension() != m_map_u.dimension() )
        {
            throw std::logic_error("Extension / internal map dimensions mismatch!");
        }

        if ( m_map_u.imageDimension() != m_projection.dimension() )
        {
            throw std::logic_error("Internal / projection map dimensions mismatch!");
        }
    }

    VectorType operator() (const VectorType& vec) override
    {
        this->assert_vector_size(vec, m_extension.dimension(), "ENP vec vector size mismatch (1)!");

        const VectorType v1 = m_extension(vec);
        const VectorType v2 = m_map_u(v1);
        const VectorType v3 = m_projection(v2);

        return v3;
    }

    VectorType operator() (const VectorType& vec, MatrixType& mat) override
    {
        this->assert_vector_size(vec, m_extension.dimension(), "ENP vec vector size mismatch (2)!");

        MatrixType m1(m_extension.imageDimension(), m_extension.dimension());
        const VectorType v1 = m_extension(vec, m1);
        this->assert_matrix_size(m1, m_extension.imageDimension(), m_extension.dimension(), "ENP m1 matrix size mismatch!");

        MatrixType m2(m_map_u.imageDimension(), m_map_u.dimension());
        const VectorType v2 = m_map_u(v1, m2);
        this->assert_matrix_size(m2, m_map_u.imageDimension(), m_map_u.dimension(), "ENP m2 matrix size mismatch!");

        MatrixType m3(m_projection.imageDimension(), m_projection.dimension());
        const VectorType v3 = m_projection(v2, m3);
        this->assert_matrix_size(m3, m_projection.imageDimension(), m_projection.dimension(), "ENP m3 matrix size mismatch!");

        mat = m3 * m2 * m1;

        return v3;
    }

    unsigned dimension() const noexcept override
    {
        return m_extension.dimension();
    }

    unsigned imageDimension() const noexcept override
    {
        return m_projection.imageDimension();
    }

    MapU & internal_map() noexcept { return m_map_u; }
    MapT & extend() noexcept { return m_extension; }
    MapT & project() noexcept { return m_projection; }

private:
    MapU m_map_u;
    MapT m_extension;
    MapT m_projection;
};

}
