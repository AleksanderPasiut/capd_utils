///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "map_base.hpp"
#include "arctan2_map.hpp"

namespace Carina
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief (r,alpha) -> (x,y)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT>
class PolarCoordinates : public MapT
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    PolarCoordinates() : MapT(func, 2, 2, 0)
    {}

private:
    static void func(Node, Node in[], int, Node out[], int, Node param[], int)
    {
        Node& r = in[0];
        Node& alpha = in[1];

        out[0] = r * cos(alpha);
        out[1] = r * sin(alpha);
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief (x,y) -> (r,alpha)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT>
class PolarCoordinatesInverse : public MapBase<MapT>
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    PolarCoordinatesInverse()
        : m_map_r(func, 2, 1, 0)
        , m_map_alpha()
    {}

    VectorType operator() (const VectorType& vec) override
    {
        this->assert_vector_size(vec, 2, "PolarCoordinatesInverse vec vector size mismatch (1)!");

        const VectorType ret_r = m_map_r(vec);
        const VectorType ret_alpha = m_map_alpha(vec);

        return VectorType{ ret_r[0], ret_alpha[0] };
    }

    VectorType operator() (const VectorType& vec, MatrixType& mat) override
    {
        this->assert_vector_size(vec, 2, "PolarCoordinatesInverse vec vector size mismatch (2)!");

        MatrixType mat_r(1, 2);
        const VectorType ret_r = m_map_r(vec, mat_r);

        MatrixType mat_alpha(1, 2);
        const VectorType ret_alpha = m_map_alpha(vec, mat_alpha);

        mat = MatrixType{ { mat_r(1, 1), mat_r(1, 2) }, { mat_alpha(1, 1), mat_alpha(1, 2) } };
        return VectorType{ ret_r[0], ret_alpha[0] };
    }

    unsigned dimension() const noexcept override
    {
        return 2;
    }

    unsigned imageDimension() const noexcept override
    {
        return 2;
    }

private:
    static void func(Node, Node in[], int, Node out[], int, Node param[], int)
    {
        Node& x = in[0];
        Node& y = in[1];

        out[0] = sqrt(sqr(x) + sqr(y));
    }

    MapT m_map_r;
    Arctan2<MapT> m_map_alpha;
};

}
