///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <list>

#include "newton_method.hpp"

namespace CapdUtils
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Newton multisearch
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT>
class NewtonMethodMultisearcher
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;
    
    NewtonMethodMultisearcher(MapT& map) : m_map(map), m_dimension(map.dimension())
    {}

    template<typename SimilarityFunctionT, typename RngT>
    std::list<VectorType> find_roots(
        size_t max_attempts,
        size_t steps_per_attempt,
        size_t max_expected_roots,
        SimilarityFunctionT similarity_function,
        RngT rng)
    {
        std::list<VectorType> ret {};

        for (size_t i = 0; i < max_attempts; ++i)
        {
            VectorType initial_root(m_dimension);

            for (int j = 0; j < initial_root.dimension(); ++j)
            {
                initial_root[j] = rng();
            }

            try
            {
                NewtonMethod<MapT> m_internal_searcher(m_map, initial_root, steps_per_attempt);

                const VectorType root = m_internal_searcher.get_root();

                put_in_list(ret, root, similarity_function);

                if (ret.size() >= max_expected_roots)
                {
                    break;
                }
            }
            catch (...)
            {}
        }

        return ret;
    }

private:
    template<typename SimilarityFunctionT>
    static void put_in_list(std::list<VectorType>& out, const VectorType& arg, SimilarityFunctionT similarity_function)
    {
        for (const VectorType& vec : out)
        {
            if (similarity_function(vec, arg))
            {
                return;
            }
        }

        out.emplace_back(arg);
    }

    MapT& m_map;
    size_t m_dimension;
};

}
