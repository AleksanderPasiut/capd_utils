///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <capd_utils/householder_generator.hpp>
#include <capd_utils/projection_map.hpp>

namespace CapdUtils
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Generate matrix of orthonormal basis where the directions of the first two vectors are specified
//!
//! @details Direction of the first vector is taken directly from `initial_vector`. Direction of the second vector is 
//!          projection of `projected_vector` on hyperplane defined by `initial_vector`.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT>
class HouseholderGenerator2
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    HouseholderGenerator2(VectorType initial_vector, VectorType projected_vector)
        : m_matrix( gen_matrix(initial_vector, projected_vector) )
    {}

    const MatrixType& get_matrix() const
    {
        return m_matrix;
    }

private:
    static MatrixType gen_matrix(VectorType v, VectorType u)
    {
        if (v.dimension() < 2)
        {
            throw std::logic_error("Dimension of v must not be less than 2!");
        }

        if (v.dimension() != u.dimension())
        {
            throw std::logic_error("Dimension of v must not be equal dimension of u!");
        }

        const unsigned n = v.dimension();

        HouseholderGenerator<MapT> gen(v);
        const MatrixType mat_v = gen.get_matrix();

        MapT proj
        {
            ProjectionMap<MapT>::create(v.dimension(), IdxList<size_t>::create(1, n-1))
        };

        MatrixType P(n - 1, n);
        const VectorType u_proj = proj(mat_v * u, P);

        HouseholderGenerator<MapT> gen_u(u_proj);
        const MatrixType mat_u = gen_u.get_matrix();

        MatrixType mat_u_e = MatrixType::Identity(n);
        Concat<MapT>::copy_matrix_on_matrix(mat_u_e, mat_u, 1, 1);

        return mat_v * mat_u_e;
    }

    const MatrixType m_matrix;
};

}
