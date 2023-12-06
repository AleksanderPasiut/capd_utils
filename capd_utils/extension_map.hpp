///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <list>
#include <stdexcept>

#include "capd/basic_types.hpp"
#include "idx_list.hpp"

namespace Carina
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Extension map implementation
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT>
class ExtensionMap
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Create extension map
    //!
    //! @param values   extension values specifying outputs
    //! @param idx_list indices specifying output source, e.g. for values
    //!                 vector { v0, v1, v2, v3, v4 } and indices list
    //!                 { -1, 1, 0, -1, -1 }, the generated map will be
    //!                 { x0, x1 } -> { v0, x1, x0, v3, v4 }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static MapT create(const VectorType& values, const IdxList<int>& idx_list)
    {
        if (idx_list.size() > 0)
        {
            if (values.dimension() == idx_list.size())
            {
                const size_t input_size = get_input_size(idx_list);

                auto func = [values, idx_list](Node, Node in[], int, Node out[], int, Node param[], int)
                {
                    size_t out_idx = 0;
                    for (const int& idx : idx_list)
                    {
                        if (idx >= 0)
                        {
                            out[out_idx] = in[idx];
                        }
                        else
                        {
                            out[out_idx] = param[out_idx];
                        }

                        ++out_idx;
                    }
                };

                MapT ret = MapT(func, input_size, idx_list.size(), values.dimension());

                for (int i = 0; i < values.dimension(); ++i)
                {
                    ret.setParameter(i, values[i]);
                }

                return ret;
            }
            else
            {
                throw std::logic_error("Values vector and index list size mismatch!");
            }
        }
        else
        {
            throw std::logic_error("Index list is empty!");
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Create extension map
    //!
    //! Simplified version for zero extension vector.
    //!
    //! @param idx_list indices specifying output source
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static MapT create(const IdxList<int>& idx_list)
    {
        if (idx_list.size() > 0)
        {
            const size_t input_size = get_input_size(idx_list);

            auto func = [idx_list](Node, Node in[], int, Node out[], int, Node param[], int)
            {
                size_t out_idx = 0;
                for (const int& idx : idx_list)
                {
                    if (idx >= 0)
                    {
                        out[out_idx] = in[idx];
                    }
                    else
                    {
                        Node zero = Node(0.0);
                        out[out_idx] = zero;
                    }

                    ++out_idx;
                }
            };

            return MapT(func, input_size, idx_list.size(), 0);
        }
        else
        {
            throw std::logic_error("Index list is empty!");
        }
    }

private:
    static size_t get_input_size(const IdxList<int>& idx_list)
    {
        size_t ret = 0;
        for (const int& idx : idx_list)
        {
            if ((idx+1) > static_cast<int>(ret))
            {
                ret = idx+1;
            }
        }

        return ret;
    }
};

}
