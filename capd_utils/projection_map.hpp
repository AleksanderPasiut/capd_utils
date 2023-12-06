///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <list>
#include <stdexcept>

#include "capd/basic_types.hpp"
#include "idx_list.hpp"

namespace CapdUtils
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Projection map implementation
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT>
class ProjectionMap
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Create projection map
    //!
    //! @param input_size dimension of map input
    //! @param idx_list   list of indices specifying output, e.g. for input_size
    //!                   5 and indices list {2,1}, the generated map will be
    //!                   { x0, x1, x2, x3, x4 } -> { x2, x1 }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static MapT create(size_t input_size, const IdxList<size_t>& idx_list)
    {
        if (idx_list.size() > 0)
        {
            auto func = [input_size, idx_list](Node, Node in[], int, Node out[], int, Node param[], int)
            {
                size_t out_idx = 0;
                for (const size_t& idx : idx_list)
                {
                    if (idx < input_size)
                    {
                        out[out_idx] = in[idx];
                        ++out_idx;
                    }
                    else
                    {
                        throw std::logic_error("Index is out of range!");
                    }
                }
            };

            return MapT(func, input_size, idx_list.size(), 0);
        }
        else
        {
            throw std::logic_error("Index list is empty!");
        }
    }
};

}
