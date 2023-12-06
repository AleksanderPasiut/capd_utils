///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <capd_utils/capd/map.hpp>
#include <capd_utils/concat.hpp>

namespace CapdUtils
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Generate list of n vectors of dimension n.
//! @details First two vectors are normalized v1 and v2. Remaining n-2 vectors are normalized, orthogonal to each other and
//!          orthogonal to the plane spanned by given vectors v1 and v2.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT>
class GGS
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    static std::list<VectorType> gen_vectors(const VectorType& v1, const VectorType& v2)
    {
        if (v1.dimension() == v2.dimension())
        {
            VectorType const u1 = v1 + v2;
            VectorType const u2 = v1 - v2;

            unsigned const idx_1 = find_max_index(u1);
            unsigned const idx_2 = find_max_index(u2, idx_1);

            std::list<VectorType> vecs = gen_init_vectors(v1.dimension(), idx_1, idx_2);
            vecs.push_front(u2 / u2.euclNorm());
            vecs.push_front(u1 / u1.euclNorm());

            std::list<VectorType> ret {};
            for (VectorType vec : vecs)
            {
                for (const VectorType& v : ret)
                {
                    vec -= capd::vectalg::scalarProduct(vec, v) * v;
                }
                vec /= vec.euclNorm();
                ret.push_back(vec);
            }

            ret.pop_front();
            ret.pop_front();

            ret.push_front(v2 / v2.euclNorm());
            ret.push_front(v1 / v1.euclNorm());

            return ret;
        }
        else
        {
            throw std::logic_error("Vector v1 and v2 dimension mismatch!");
        }
    }

private:
    static unsigned find_max_index(const VectorType& v)
    {
        unsigned ret = 0;
        ScalarType value = abs( v[0] );

        for (unsigned i = 1; i < v.dimension(); ++i)
        {
            ScalarType const tmp = abs( v[i] );
            if (tmp < value)
            {
                value = tmp;
                ret = i;
            }
        }

        return ret;
    }

    static unsigned find_max_index(const VectorType& v, unsigned omit)
    {
        unsigned ret = 0;
        ScalarType value = abs( v[0] );

        for (unsigned i = 1; i < v.dimension(); ++i)
        {
            if (i != omit)
            {
                ScalarType const tmp = abs( v[i] );
                if (tmp < value)
                {
                    value = tmp;
                    ret = i;
                }
            }
            else
            {
                continue;
            }
        }

        return ret;
    }

    static std::list<VectorType> gen_init_vectors(unsigned dimension, unsigned idx_1, unsigned idx_2)
    {
        std::list<VectorType> ret {};
        for (unsigned i = 0; i < dimension; ++i)
        {
            if (i != idx_1 && i != idx_2)
            {
                ret.push_back(gen_init_vector(dimension, i));
            }
        }

        return ret;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Generate vector of `dimension` with one in element of index `idx` and zeros otherwise
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static VectorType gen_init_vector(unsigned dimension, unsigned idx)
    {
        VectorType ret(dimension);
        ret[idx] = 1.0;
        return ret;
    }

    static ScalarType abs(const ScalarType& arg)
    {
        return (arg >= ScalarType(0.0)) ? arg : (-arg);
    }
};

}
