///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "newton_method.expander.hpp"
#include "type_cast.hpp"

namespace CapdUtils
{

template<typename MapT>
class KrawczykMethodExpander
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    KrawczykMethodExpander(MapT& map) : m_map(map)
    {}

    bool bound_solution(VectorType& root_with_epsilon, VectorType root, size_t max_steps)
    {
        if (subset(root, root_with_epsilon))
        {
            MatrixType invC(root.dimension(), root.dimension());
            const VectorType val = m_map(root, invC);

            invC = matrix_cast<MatrixType>( matrix_cast<RMatrix>(invC) );

            MatrixType C = gaussInverseMatrix<MapT>(invC);
            C = matrix_cast<MatrixType>( matrix_cast<RMatrix>(C) );

            for (size_t step = 0; step < max_steps; ++step)
            {
                const VectorType interior = get_interior(root_with_epsilon, root, val, C);

                #ifdef CARINA_LOG

                std::cout << "span interior: " << span_vector(interior) << '\n';

                #endif

                if ( subset(interior, root_with_epsilon) )
                {
                    combine_root(root_with_epsilon, interior, root);
                    return true;
                }

                combine_root(root_with_epsilon, interior, root);
            }

            return false;
        }
        else
        {
            throw std::logic_error("Root is not a subset of root with epsilon!");
        }
    }

private:
    static void combine_root(VectorType& root_with_epsilon, const VectorType& interior, const VectorType& root)
    {
        for (unsigned i = 0; i < root_with_epsilon.dimension(); ++i)
        {
            root_with_epsilon[i] = capd::intervals::intervalHull(interior[i], root[i]);
        }
    }

    VectorType get_interior(VectorType& root_with_epsilon, VectorType root, VectorType val, const MatrixType& C)
    {
        MatrixType der( root.dimension(), root.dimension() );
        m_map(root_with_epsilon, der);

        const MatrixType id = MatrixType::Identity( root.dimension() );
        return root - C * val + (id - C * der)*(root_with_epsilon - root);
    }

    MapT& m_map;
};

}
