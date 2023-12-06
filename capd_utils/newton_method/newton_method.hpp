///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <list>
#include <algorithm>

#include <capd_utils/capd/basic_tools.hpp>
#include <capd_utils/capd/gauss_solver.hpp>
#include <capd_utils/capd/norm.hpp>

#include "newton_method.roots_list.hpp"

namespace Carina
{

template<typename MapT, bool is_interval>
class NewtonMethodInternal
{};

template<typename MapT>
class NewtonMethodInternal<MapT, false>
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    NewtonMethodInternal(MapT& map, const VectorType& initial_root, size_t max_steps)
    {
        MatrixType der( initial_root.dimension(), initial_root.dimension() );

        MaxNorm<MapT> norm {};
        RootsList<MapT> roots {};
        Root<MapT> root {};
        root.argument = initial_root;

        for (size_t i = 0;; ++i)
        {
            const VectorType value = map(root.argument, der);
            root.value_norm = norm(value);
            roots.push_back(root);

            if (i < max_steps)
            {
                const VectorType dx = gauss<MapT>(der, value);

                #ifdef CARINA_LOG

                std::cout << "root: " << root.argument << '\n';
                std::cout << "val: " << value << '\n';

                #endif

                Root<MapT> new_root {};
                new_root.argument = root.argument - dx;

                if (roots.is_present(new_root) == false)
                {
                    root = new_root;
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }

        m_root = roots.best_argument();
    }

    const VectorType& get_root() const noexcept
    {
        return m_root;
    }

    const VectorType& get_root_midpoint() const noexcept
    {
        return m_root;
    }

    bool is_successful() const noexcept
    {
        return true;
    }

private:
    VectorType m_root {};
};

template<typename MapT>
class NewtonMethodInternal<MapT, true>
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    NewtonMethodInternal(MapT& map, const VectorType& initial_root, size_t max_steps)
    {
        m_root_midpoint = find_root_midpoint(map, initial_root, max_steps);
        m_root = m_root_midpoint;
        m_successful = bound_solution(map, m_root, m_root_midpoint, max_steps);
    }

    const VectorType& get_root() const noexcept
    {
        return m_root;
    }

    const VectorType& get_root_midpoint() const noexcept
    {
        return m_root_midpoint;
    }

    bool is_successful() const noexcept
    {
        return m_successful;
    }

private:
    static VectorType find_root_midpoint(MapT& map, const VectorType& initial_root, size_t max_steps)
    {
        MatrixType der( initial_root.dimension(), initial_root.dimension() );

        MaxNorm<MapT> norm {};
        RootsList<MapT> roots {};
        Root<MapT> root {};
        root.argument = mid_vector( initial_root );

        for (size_t i = 0;; ++i)
        {
            #ifdef CARINA_LOG

            std::cout << "iteration: " << i << '\n';

            #endif

            const VectorType value = map(root.argument, der);
            der = mid_matrix(der);
            root.value_norm = norm(value);
            roots.push_back(root);

            if (i < max_steps)
            {
                const VectorType dx = mid_vector( gauss<MapT>(der, value) );

                #ifdef CARINA_LOG

                std::cout << "root: " << root.argument << '\n';
                std::cout << "val: " << value << '\n';

                #endif

                Root<MapT> new_root {};
                new_root.argument = mid_vector( root.argument - dx );

                if (roots.is_present(new_root) == false)
                {
                    root = new_root;
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }

        return roots.best_argument();
    }

    bool bound_solution(MapT& map, VectorType& root, VectorType root_midpoint, size_t max_steps)
    {
        if (subset(root_midpoint, root))
        {
            const VectorType val = map(root_midpoint);

            for (size_t step = 0; step < max_steps; ++step)
            {
                const VectorType interior = get_interior(map, root, root_midpoint, val);

                #ifdef CARINA_LOG

                std::cout << "span interior: " << span_vector(interior) << '\n';

                #endif

                root = capd::vectalg::intervalHull(interior, root_midpoint);

                if ( subset(interior, root) )
                {
                    return true;
                }
            }

            return false;
        }
        else
        {
            throw std::logic_error("Root midpoint is not a subset of root!");
        }
    }

    static VectorType get_interior(MapT& map, const VectorType& root, VectorType root_midpoint, VectorType val)
    {
        MatrixType der( root_midpoint.dimension(), root_midpoint.dimension() );
        map(root, der);

        const VectorType dx = gauss<MapT>(der, val);
        const VectorType interior = root_midpoint - dx;

        return interior;
    }

    bool m_successful {};
    VectorType m_root {};
    VectorType m_root_midpoint {};
};

template<typename MapT>
class NewtonMethod
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    NewtonMethod(MapT& map, const VectorType& initial_root, size_t max_steps) 
        : m_internal(map, initial_root, max_steps)
    {}

    const VectorType& get_root() const noexcept
    {
        return m_internal.get_root();
    }

    const VectorType& get_root_midpoint() const noexcept
    {
        return m_internal.get_root_midpoint();
    }

    bool is_successful() const noexcept
    {
        return m_internal.is_successful();
    }

private:
    static constexpr bool is_interval = capd::TypeTraits<ScalarType>::isInterval;
    NewtonMethodInternal<MapT, is_interval> m_internal;
};

}
