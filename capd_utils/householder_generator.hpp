///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <tuple>

#include <capd_utils/capd/map.hpp>
#include <capd_utils/concat.hpp>

namespace CapdUtils
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Generate matrix of orthonormal basis where the direction of the first vector is specified
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT>
class HouseholderGenerator
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    HouseholderGenerator(VectorType initial_vector)
        : m_matrix( gen_matrix( gen_data( initial_vector )) )
    {}

    const MatrixType& get_matrix() const
    {
        return m_matrix;
    }

private:
    static std::tuple<VectorType, bool> gen_data(const VectorType v)
    {
        if (v.dimension() == 0)
        {
            throw std::logic_error("Dimension of v must not be 0!");
        }

        const VectorType e1 = gen_e_vector(v.dimension(), 0);
        const ScalarType v_norm = v.euclNorm();

        if (v[0] < 0.0)
        {
            VectorType ret = v - e1 * v_norm;
            return std::make_tuple(ret, false);
        }
        else
        {
            VectorType ret = v + e1 * v_norm;
            return std::make_tuple(ret, true);
        }
    }

    static MatrixType gen_matrix(std::tuple<VectorType, bool> data)
    {
        const VectorType w = std::get<0>(data);
        const bool negative = std::get<1>(data);

        MatrixType ret = MatrixType::Identity(w.dimension());

        const ScalarType factor = 2 / capd::vectalg::scalarProduct(w, w);

        for (int i = 1; i <= w.dimension(); ++i)
        {
            for (int j = 1; j <= w.dimension(); ++j)
            {
                ret(i,j) -= factor * w(i) * w(j);
            }
        }

        if (negative)
        {
            ret *= -1.0;
        }

        return ret;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Generate vector of `dimension` with one in element of index `idx` and zeros otherwise
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static VectorType gen_e_vector(unsigned dimension, unsigned idx)
    {
        VectorType ret(dimension);
        ret[idx] = 1.0;
        return ret;
    }

    const MatrixType m_matrix;
};

}
