///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <capd_utils/pne_map.hpp>
#include <capd_utils/identity_map.hpp>
#include <capd_utils/idx_list.hpp>
#include <capd_utils/map_base.hpp>

namespace CapdUtils
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Circular parallel shooting map (CPSM)
//!
//! Create map ( x_0, ..., x_{n-1} ) -> ( y_0, ..., y_{n-1} ) of the form:
//!       y_{k-1} = f( x_{k-1} ) - x_k,
//!       y_{n-1} = f( x_{n-1} ) - x_0,
//!
//! for k in { 1, ..., n-1 }.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT, typename MapU>
class CPSM : public MapBase<MapT>
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    CPSM(size_t n, MapU& map_u)
        : m_n( check_size(n) )
        , m_map_u(map_u)
        , m_N( get_dimension(m_map_u) )
        , m_id(m_N)
        , m_pne_container()
        , m_id_container()
    {
        m_pne_container.reserve(m_n);
        m_id_container.reserve(m_n);

        for (size_t k = 0; k < m_n; ++k)
        {
            m_pne_container.emplace_back(
                m_n * m_N,
                IdxList<size_t>::create(k*m_N, m_N),
                IdxList<int>::create(m_n*m_N, k*m_N, m_N),
                std::ref(m_map_u));
        }

        for (size_t k = 1; k < m_n; ++k)
        {
            m_id_container.emplace_back(
                m_n * m_N,
                IdxList<size_t>::create(k*m_N, m_N),
                IdxList<int>::create(m_n*m_N, (k-1)*m_N, m_N),
                std::ref(m_id));
        }

        m_id_container.emplace_back(
            m_n * m_N,
            IdxList<size_t>::create(0, m_N),
            IdxList<int>::create(m_n*m_N, (m_n-1)*m_N, m_N),
            std::ref(m_id));
    }

    VectorType operator() (const VectorType& vec) override
    {
        VectorType ret( this->imageDimension() );

        for (auto & pne : m_pne_container)
        {
            ret += pne(vec);
        }

        for (auto & pne : m_id_container)
        {
            ret -= pne(vec);
        }

        return ret;
    }

    VectorType operator() (const VectorType& vec, MatrixType& der) override
    {
        der = MatrixType( this->imageDimension(), this->dimension() );

        VectorType ret( this->imageDimension() );
        MatrixType tmp( this->imageDimension(), this->dimension() );

        for (auto & pne : m_pne_container)
        {
            ret += pne(vec, tmp);
            der += tmp;
        }

        for (auto & pne : m_id_container)
        {
            ret -= pne(vec, tmp);
            der -= tmp;
        }

        return ret;
    }

    unsigned dimension() const noexcept override
    {
        return m_n * m_N;
    }

    unsigned imageDimension() const noexcept override
    {
        return m_n * m_N;
    }

private:
    static size_t check_size(size_t n)
    {
        if (n > 0)
        {
            return n;
        }
        else
        {
            throw std::invalid_argument("CPSM: n value must be greater than 0!");
        }
    }

    static unsigned get_dimension(const MapU& map_u)
    {
        if ( map_u.dimension() == map_u.imageDimension() )
        {
            return map_u.dimension();
        }
        else
        {
            throw std::invalid_argument("CPSM: internal map dimension and image dimension must be equal!");
        }
    }

    const size_t m_n;
    MapU& m_map_u;

    const unsigned m_N;
    IdentityMap<MapT> m_id;

    std::vector<PNE<MapT, MapU&>> m_pne_container;
    std::vector<PNE<MapT, decltype(m_id)&>> m_id_container;
};

}
