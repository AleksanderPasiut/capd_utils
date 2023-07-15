///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "map_base.hpp"
#include "pi.hpp"

namespace Carina
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief CAPD implementation of 2 argument arctan function.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT>
class Arctan2 : public MapBase<MapT>
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    Arctan2()
        : m_map_1(func_1, 2, 1, 1)//"par: pi; var: x, y; fun: atan(y / x);")
        , m_map_2A(func_2A, 2, 1, 1)//"par: pi; var: x, y; fun: atan(y / x) + pi;")
        , m_map_2B(func_2B, 2, 1, 1)//"par: pi; var: x, y; fun: atan(y / x) - pi;")
        , m_map_3(func_3, 2, 1, 1)//"par: pi; var: x, y; fun:  pi/2 - atan(x / y);")
        , m_map_4(func_4, 2, 1, 1)//"par: pi; var: x, y; fun: -pi/2 - atan(x / y);")
    {
        m_map_1.setParameter(0, get_pi<ScalarType>());
        m_map_2A.setParameter(0, get_pi<ScalarType>());
        m_map_2B.setParameter(0, get_pi<ScalarType>());
        m_map_3.setParameter(0, get_pi<ScalarType>());
        m_map_4.setParameter(0, get_pi<ScalarType>());
    }

    VectorType operator() (const VectorType& vec, MatrixType& mat) override
    {
        this->assert_vector_size(vec, 2, "Arctan2 vec vector size mismatch!");

        const ScalarType x = vec[0];
        const ScalarType y = vec[1];

        if ( capd::abs(x) > capd::abs(y) )
        {
            if ( x > 0 )
            {
                return m_map_1(vec, mat);
            }
            else
            {
                if (y > 0)
                {
                    return m_map_2A(vec, mat);
                }
                else
                {
                    return m_map_2B(vec, mat);
                }
            }
        }
        else
        {
            if ( y > 0 )
            {
                return m_map_3(vec, mat);
            }
            else
            {
                return m_map_4(vec, mat);
            }
        }
    }

    VectorType operator() (const VectorType& vec) override
    {
        MatrixType dummy(1, 2);
        return (*this)(vec, dummy);
    }

    unsigned dimension() const noexcept override
    {
        return 2;
    }

    unsigned imageDimension() const noexcept override
    {
        return 1;
    }

private:
    static void func_1(Node, Node in[], int, Node out[], int, Node param[], int)
    {
        Node& pi = param[0];

        Node& x = in[0];
        Node& y = in[1];

        out[0] = atan(y / x);
    }

    static void func_2A(Node, Node in[], int, Node out[], int, Node param[], int)
    {
        Node& pi = param[0];

        Node& x = in[0];
        Node& y = in[1];

        out[0] = atan(y / x) + pi;
    }

    static void func_2B(Node, Node in[], int, Node out[], int, Node param[], int)
    {
        Node& pi = param[0];

        Node& x = in[0];
        Node& y = in[1];

        out[0] = atan(y / x) - pi;
    }

    static void func_3(Node, Node in[], int, Node out[], int, Node param[], int)
    {
        Node& pi = param[0];

        Node& x = in[0];
        Node& y = in[1];

        out[0] = pi/2 - atan(x / y);
    }

    static void func_4(Node, Node in[], int, Node out[], int, Node param[], int)
    {
        Node& pi = param[0];

        Node& x = in[0];
        Node& y = in[1];

        out[0] = -pi/2 - atan(x / y);
    }

    MapT m_map_1;
    MapT m_map_2A;
    MapT m_map_2B;
    MapT m_map_3;
    MapT m_map_4;
};

}
