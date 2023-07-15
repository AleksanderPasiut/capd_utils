///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "capd/basic_types.hpp"

namespace Carina
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Utilities for matrix and vectors concatenation
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT>
class Concat
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief get total dimension of given vectors
    //!
    //! @param It vector iterator
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename It>
    static size_t get_total_dimension(It beg, It end)
    {
        size_t ret = 0;
        for (It it = beg; it != end; ++it)
        {
            ret += it->dimension();
        }

        return ret;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief get total dimension of given matrices
    //!
    //! @param It matrix iterator
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // template<typename It>
    // static std::pair<size_t, size_t> get_total_dimension(It beg, It end, size_t rows, size_t cols)
    // {
    //     std::pair<size_t, size_t> ret {};
    //
    //     It it = beg;
    //
    //     for (size_t i = 0; i < rows; ++i)
    //     {
    //
    //
    //         for (size_t j = 0; j < cols; ++j)
    //         {
    //             if (it != end)
    //             {
    //                 const MatrixType& mat = *it;
    //
    //                 ret.first
    //             }
    //             else
    //             {
    //                 throw std::logic_error("Unexpected end iterator encountered!");
    //             }
    //         }
    //     }
    //
    //
    //
    //     return ret;
    // }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Concatenate vectors
    //!
    //! @param It vector iterator
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename It>
    static VectorType concat_vectors(It beg, It end)
    {
        VectorType ret( get_total_dimension(beg, end) );

        size_t offset = 0;
        for (It it = beg; it != end; ++it)
        {
            copy_vector_on_vector(ret, *it, offset);
            offset += it->dimension();
        }

        return ret;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Concatenate vectors
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static VectorType concat_vectors(const std::initializer_list<VectorType>& init)
    {
        return concat_vectors(init.begin(), init.end());
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Concatenate matrices
    //!
    //! @param It         matrix iterator
    //! @param beg        begin iterator
    //! @param end        end iterator
    //! @param rows       number of rows in block matrix
    //! @param cols       number of cols in block matrix
    //! @param total_rows number of rows in output matrix
    //! @param total_cols number of cols in output matrix
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename It>
    static MatrixType concat_matrices(It beg, It end, size_t rows, size_t cols, size_t total_rows, size_t total_cols)
    {
        assert(rows > 0);
        assert(cols > 0);

        MatrixType ret(total_rows, total_cols);

        It it = beg;

        size_t row_offset = 0;
        for (size_t i = 0; i < rows; ++i)
        {
            size_t col_offset = 0;
            for (size_t j = 0; j < cols; ++j)
            {
                assert(it != end);
                copy_matrix_on_matrix(ret, *it, row_offset, col_offset);
                col_offset += it->dimension().second;
                ++it;
            }

            --it;
            row_offset += it->dimension().first;
            ++it;
        }

        assert(it == end);
        return ret;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Build matrix by concatenating vertical vectors horizontally
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static MatrixType build_matrix_from_vvectors(const std::initializer_list<VectorType>& arg)
    {
        return build_matrix_from_vvectors(arg.begin(), arg.end());
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Build matrix by concatenating horizontal vectors vertically
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static MatrixType build_matrix_from_hvectors(const std::initializer_list<VectorType>& arg)
    {
        return build_matrix_from_hvectors(arg.begin(), arg.end());
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Build matrix by concatenating vertical vectors horizontally
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename It>
    static MatrixType build_matrix_from_vvectors(It begin, It end)
    {
        const size_t vectors_number = std::distance(begin, end);

        if (vectors_number > 0)
        {
            const size_t vector_size = begin->dimension();

            MatrixType ret(vector_size, vectors_number);

            size_t j = 1;
            for (auto it = begin; it != end; ++it, ++j)
            {
                for (size_t i = 1; i <= vector_size; ++i)
                {
                    ret(i, j) = (*it)(i);
                }
            }

            return ret;
        }
        else
        {
            return MatrixType();
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Build matrix by concatenating horizontal vectors vertically
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename It>
    static MatrixType build_matrix_from_hvectors(It begin, It end)
    {
        const size_t vectors_number = std::distance(begin, end);

        if (vectors_number > 0)
        {
            const size_t vector_size = begin->dimension();

            MatrixType ret(vectors_number, vector_size);

            size_t j = 1;
            for (auto it = begin; it != end; ++it, ++j)
            {
                for (size_t i = 1; i <= vector_size; ++i)
                {
                    ret(j, i) = (*it)(i);
                }
            }

            return ret;
        }
        else
        {
            return MatrixType();
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Copy given vector onto another vector replacing the previous contents
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void copy_vector_on_vector(VectorType& dst, const VectorType& src, size_t offset)
    {
        assert(dst.dimension() >= src.dimension() + offset);

        for (const ScalarType & s : src)
        {
            dst[offset++] = s;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Copy given matrix onto another matrix replacing the previous contents
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void copy_matrix_on_matrix(MatrixType& dst, const MatrixType& src, unsigned row_offset, unsigned col_offset)
    {
        assert(dst.dimension().first >= src.dimension().first + row_offset);
        assert(dst.dimension().second >= src.dimension().second + col_offset);

        for (unsigned i = 1; i <= src.dimension().first; ++i)
        {
            for (unsigned j = 1; j <= src.dimension().second; ++j)
            {
                dst(row_offset + i, col_offset + j) = src(i, j);
            }
        }
    }
};

}
