///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <list>
#include <vector>
#include <stdexcept>

#include "capd/basic_types.hpp"
#include "map_base.hpp"

#ifdef CARINA_LOG
#include "progress_logger.hpp"
#endif

namespace Carina
{

template<typename MapT>
class GridMap : public Carina::MapBase<MapT>
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    using BoundType = typename ScalarType::BoundType;

    GridMap(MapT& ref, const std::vector<int>& grid) : m_ref(ref), m_grid(grid)
    {}

    VectorType operator() (const VectorType& vec)
    {
        const std::list<VectorType> args = split_vector(vec, m_grid);

        if (args.size() > 0)
        {
            auto it = args.begin();

            VectorType ret = m_ref(*it);

            #ifdef CARINA_LOG
            ProgressLogger logger(std::cout, "Grid", args.size(), 1);
            #endif

            for (++it; it != args.end(); ++it)
            {
                #ifdef CARINA_LOG
                ProgressLogger::Updater updater(logger);
                #endif

                const VectorType img = m_ref(*it);

                capd::vectalg::intervalHull(ret, img, ret);
            }

            return ret;
        }
        else
        {
            throw std::logic_error("No arguments left after split!");
        }
    }

    VectorType operator() (const VectorType& vec, MatrixType& mat)
    {
        const std::list<VectorType> args = split_vector(vec, m_grid);

        if (args.size() > 0)
        {
            auto it = args.begin();

            VectorType ret = m_ref(*it, mat);

            #ifdef CARINA_LOG
            ProgressLogger logger(std::cout, "Grid", args.size(), 1);
            #endif

            for (++it; it != args.end(); ++it)
            {
                #ifdef CARINA_LOG
                ProgressLogger::Updater updater(logger);
                #endif


                MatrixType der;
                const VectorType img = m_ref(*it, der);

                capd::vectalg::intervalHull(ret, img, ret);
                capd::vectalg::intervalHull(mat, der, mat);
            }

            return ret;
        }
        else
        {
            throw std::logic_error("No arguments left after split!");
        }
    }

    unsigned dimension() const noexcept
    {
        return m_ref.dimension();
    }

    unsigned imageDimension() const noexcept
    {
        return m_ref.imageDimension();
    }

    void set_grid(const std::vector<int>& grid)
    {
        m_grid = grid;
    }

    const std::vector<int>& get_grid() const noexcept
    {
        return m_grid;
    }

private:
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Split vector `arg` by splitting it elements into counts determined by `grid`
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static std::list<VectorType> split_vector(const VectorType& arg, std::vector<int> grid)
    {
        if (arg.dimension() == grid.size())
        {
            std::list<VectorType> in = { arg };
            std::list<VectorType> tmp {};

            for (int i = 0; i < arg.dimension(); ++i)
            {
                tmp.clear();

                for (const VectorType& v : in)
                {
                    std::list<VectorType> vecs = split_vector(v, grid[i], i);
                    tmp.splice(tmp.end(), vecs);
                }

                tmp.swap(in);
            }

            return in;
        }
        else
        {
            throw std::logic_error("Mismatch of vector dimension and grid dimension!");
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief  Split vector `arg` by element of index `index` into `count` vectors
    //! @details E.g. for arg == { [-1;1], [-1;1] }, count == 2 and index == 1 the output is:
    //!          {{ [-1;1], [-1;0] }, { [-1;1], [0;1] }}
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static std::list<VectorType> split_vector(const VectorType& arg, int count, int index)
    {
        const std::list<ScalarType> ivs = split_scalar(arg[index], count);

        std::list<VectorType> ret {};

        for (const ScalarType& iv : ivs)
        {
            const VectorType vec = combine_vector(arg, iv, index);
            ret.emplace_back(vec);
        }

        return ret;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Replace element of index `index` in vector `arg` with value `iv`
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static VectorType combine_vector(const VectorType& arg, const ScalarType& iv, int index)
    {
        if (arg.dimension() > index)
        {
            VectorType ret( arg.dimension() );
            for (int i = 0; i < index; ++i)
            {
                ret[i] = arg[i];
            }

            ret[index] = iv;

            for (int i = index + 1; i < arg.dimension(); ++i)
            {
                ret[i] = arg[i];
            }

            return ret;
        }
        else
        {
            throw std::logic_error("Index out of range!");
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief  Split given interval (scalar) into `count` equal intervals (scalars)
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static std::list<ScalarType> split_scalar(const ScalarType& arg, int count)
    {
        if (arg.leftBound() < arg.rightBound())
        {
            const BoundType step = (arg.rightBound() - arg.leftBound()) / count;

            std::list<ScalarType> ret {};

            BoundType split_point = arg.leftBound();

            for (int i = 1; i < count; ++i)
            {
                const BoundType new_split_point = split_point + step;
                ret.emplace_back( split_point, new_split_point );
                split_point = new_split_point;
            }

            ret.emplace_back(split_point, arg.rightBound());
            return ret;
        }
        else
        {
            // Do not split since the interval has width zero.
            return std::list<ScalarType>({ arg });
        }
    }

    MapT& m_ref;

    std::vector<int> m_grid;
};

}
