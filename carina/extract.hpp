///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <stdexcept>

namespace Carina
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Extract vectors from vectors or matrices
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT>
class Extract
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Extract vector from vector
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static VectorType get_vector(const VectorType& arg, int offset, int size)
    {
        if (offset + size <= arg.dimension())
        {
            VectorType ret(size);

            for (int i = 0; i < size; ++i)
            {
                ret[i] = arg[offset + i];
            }

            return ret;
        }
        else
        {
            throw std::logic_error("Offset and/or size are out of range!");
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Extract vertical vector of index `column` from matrix `arg`
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static VectorType get_vvector(const MatrixType& arg, int column)
    {
        if (0 < column && column <= arg.dimension().second)
        {
            VectorType ret( arg.dimension().first );

            for (int i = 1; i <= arg.dimension().first; ++i)
            {
                ret(i) = arg(i, column);
            }

            return ret;
        }
        else
        {
            throw std::logic_error("Column index is out of range!");
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Extract vertical vector of index `column` from matrix `arg`
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static VectorType get_hvector(const MatrixType& arg, int row)
    {
        if (0 < row && row <= arg.dimension().first)
        {
            VectorType ret( arg.dimension().second );

            for (int i = 1; i <= arg.dimension().second; ++i)
            {
                ret(i) = arg(row, i);
            }

            return ret;
        }
        else
        {
            throw std::logic_error("Row index is out of range!");
        }
    }

    static MatrixType get_matrix(const MatrixType& arg, int row_idx, int col_idx, int row_count, int col_count)
    {
        if (row_idx + row_count <= arg.dimension().first)
        {
            if (col_idx + col_count <= arg.dimension().second)
            {
                MatrixType ret( row_count, col_count );

                for (int i = 1; i <= row_count; ++i)
                {
                    for (int j = 1; j <= col_count; ++j)
                    {
                        ret(i,j) = arg(i+row_idx, j+col_idx);
                    }
                }

                return ret;
            }   
            else
            {
                throw std::logic_error("Column index + column count is out of range!");
            }
        }
        else
        {
            throw std::logic_error("Row index + row count is out of range!");
        }
    }
};

}
