///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <carina/map_base.hpp>

namespace Carina
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief CAPD implementation of abs(x) for x != 0
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT>
class AbsoluteMap : public MapBase<MapT>
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    VectorType operator() (const VectorType& vec) override
    {
        this->assert_vector_size(vec, 1, "AbsoluteMap vec vector size mismatch!");
        if (vec[0] > 0.0)
        {
            return VectorType{ vec[0] };
        }
        else if (vec[0] < 0.0)
        {
            return VectorType{ -vec[0] };
        }
        else
        {
            throw std::logic_error("This map is not defined for 0.0");
        }
    }

    VectorType operator() (const VectorType& vec, MatrixType& mat) override
    {
        this->assert_vector_size(vec, 1, "AbsoluteMap vec vector size mismatch!");

        mat = MatrixType(1,1);
        if (vec[0] > 0.0)
        {
            mat(1,1) = 1.0;
            return VectorType{ vec[0] };
        }
        else if (vec[0] < 0.0)
        {
            mat(1,1) = -1.0;
            return VectorType{ -vec[0] };
        }
        else
        {
            throw std::logic_error("This map is not defined for 0.0");
        }
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
