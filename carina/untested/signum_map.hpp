///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <carina/map_base.hpp>

namespace Carina
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief CAPD implementation of sgn(x) for x != 0
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT>
class SignumMap : public MapBase<MapT>
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    VectorType operator() (const VectorType& vec) override
    {
        this->assert_vector_size(vec, 1, "SignumMap vec vector size mismatch!");
        if (vec[0] > 0.0)
        {
            return VectorType{ 1.0 };
        }
        else if (vec[0] < 0.0)
        {
            return VectorType{ -1.0 };
        }
        else
        {
            throw std::logic_error("This map is not defined for 0.0");
        }
    }

    VectorType operator() (const VectorType& vec, MatrixType& mat) override
    {
        this->assert_vector_size(vec, 1, "SignumMap vec vector size mismatch!");

        mat = MatrixType(1,1);
        mat(1,1) = 0.0;
        return (*this)(vec);
    }

    unsigned dimension() const noexcept override
    {
        return 1;
    }

    unsigned imageDimension() const noexcept override
    {
        return 1;
    }
};

}
