///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>

#include <capd_utils/pne_map.hpp>
#include <capd_utils/identity_map.hpp>
#include <capd_utils/idx_list.hpp>
#include <capd_utils/map_base.hpp>

namespace CapdUtils
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Extended circular parallel shooting map (ECPSM)
//!
//! Create map ( x_0, ..., x_{n-1} ) -> ( y_0, ..., y_{n-1} ) of the form:
//!       y_0     = f( x_0 )     - x_1,
//!       y_{k-1} = g( x_{k-1} ) - x_k,
//!       y_{n-1} = h( x_{n-1} ) - x_0,
//!
//! for k in { 2, ..., n-1 }, where n >= 3.
//!
//! For n == 2, map of the following form is created:
//!       y_0 = f( x_0 ) - x_1,
//!       y_1 = h( x_1 ) - x_0.
//!
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT, typename MapF, typename MapG, typename MapH>
class ECPSM : public MapBase<MapT>
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    ECPSM(size_t n, MapF& map_f_ref, MapG& map_g_ref, MapH& map_h_ref)
        : m_n( check_size(n) )
        , m_N( calc_internal_dimension(n, map_f_ref, map_g_ref, map_h_ref) )
        , m_M( calc_extended_dimension(map_f_ref, map_g_ref, map_h_ref) )
        , m_map_f(map_f_ref)
        , m_map_g(map_g_ref)
        , m_map_h(map_h_ref)
        , m_id_f(m_N)
        , m_id_g(m_N)
        , m_id_h(m_M)
        , m_pne_f()
        , m_pne_g()
        , m_pne_h()
        , m_id_c_f()
        , m_id_c_g()
        , m_id_c_h()
    {
        m_pne_g.reserve(m_n-2);
        m_id_c_g.reserve(m_n-2);

        m_pne_f = std::make_unique<PNE<MapT, MapF&>>(
            this->dimension(),
            IdxList<size_t>::create(0, m_M),
            IdxList<int>::create(this->dimension(), 0, m_N),
            std::ref(m_map_f));

        m_id_c_f = std::make_unique<PNE<MapT, decltype(m_id_f)&>>(
            this->dimension(),
            IdxList<size_t>::create(m_M, m_N),
            IdxList<int>::create(this->dimension(), 0, m_N),
            std::ref(m_id_f));

        for (size_t k = 1; k < m_n-1; ++k)
        {
            m_pne_g.emplace_back(
                this->dimension(),
                IdxList<size_t>::create(m_M + (k-1)*m_N, m_N),
                IdxList<int>::create(this->dimension(), k*m_N, m_N),
                std::ref(m_map_g));

            m_id_c_g.emplace_back(
                this->dimension(),
                IdxList<size_t>::create(m_M + k*m_N, m_N),
                IdxList<int>::create(this->dimension(), k*m_N, m_N),
                std::ref(m_id_g));
        }

        m_pne_h = std::make_unique<PNE<MapT, MapH&>>(
            this->dimension(),
            IdxList<size_t>::create(m_M + (m_n-2)*m_N, m_N),
            IdxList<int>::create(this->dimension(), (m_n-1)*m_N, m_M),
            std::ref(m_map_h));

        m_id_c_h = std::make_unique<PNE<MapT, decltype(m_id_h)&>>(
            this->dimension(),
            IdxList<size_t>::create(0, m_M),
            IdxList<int>::create(this->dimension(), (m_n-1)*m_N, m_M),
            std::ref(m_id_h));
    }

    VectorType operator() (const VectorType& vec) override
    {
        VectorType ret( this->imageDimension() );

        ret += (*m_pne_f)(vec);
        ret -= (*m_id_c_f)(vec);

        for (auto & pne : m_pne_g)
        {
            ret += pne(vec);
        }

        for (auto & pne : m_id_c_g)
        {
            ret -= pne(vec);
        }

        ret += (*m_pne_h)(vec);
        ret -= (*m_id_c_h)(vec);

        return ret;
    }

    VectorType operator() (const VectorType& vec, MatrixType& der) override
    {
        der = MatrixType( this->imageDimension(), this->dimension() );

        VectorType ret( this->imageDimension() );
        MatrixType tmp( this->imageDimension(), this->dimension() );

        ret += (*m_pne_f)(vec, tmp);
        der += tmp;

        ret -= (*m_id_c_f)(vec, tmp);
        der -= tmp;

        for (auto & pne : m_pne_g)
        {
            ret += pne(vec, tmp);
            der += tmp;
        }

        for (auto & pne : m_id_c_g)
        {
            ret -= pne(vec, tmp);
            der -= tmp;
        }

        ret += (*m_pne_h)(vec, tmp);
        der += tmp;

        ret -= (*m_id_c_h)(vec, tmp);
        der -= tmp;

        return ret;
    }

    unsigned dimension() const noexcept override
    {
        return m_M + m_N*(m_n-1);
    }

    unsigned imageDimension() const noexcept override
    {
        return dimension();
    }

private:
    static size_t check_size(size_t n)
    {
        if (n >= 2)
        {
            return n;
        }
        else
        {
            throw std::invalid_argument("ECPSM: n value must be greater or equal 2!");
        }
    }

    static size_t calc_internal_dimension(size_t n, MapF& map_f, MapG& map_g, MapH& map_h)
    {
        if (n > 2)
        {
            assert( map_f.imageDimension() == map_g.dimension() );
            assert( map_g.imageDimension() == map_g.dimension() );
            assert( map_g.imageDimension() == map_h.dimension() );
            return map_g.dimension();
        }
        else
        {
            assert( map_f.imageDimension() == map_h.dimension() );
            return map_f.imageDimension();
        }   
    }

    static size_t calc_extended_dimension(MapF& map_f, MapG& map_g, MapH& map_h)
    {
        assert( map_h.imageDimension() == map_f.dimension() );
        return map_f.dimension();
    }

    const size_t m_n;
    const size_t m_N;
    const size_t m_M;

    MapF& m_map_f;
    MapG& m_map_g;
    MapH& m_map_h;

    IdentityMap<MapT> m_id_f;
    IdentityMap<MapT> m_id_g;
    IdentityMap<MapT> m_id_h;

    std::unique_ptr<PNE<MapT, MapF&>> m_pne_f;
    std::vector<PNE<MapT, MapG&>> m_pne_g;
    std::unique_ptr<PNE<MapT, MapH&>> m_pne_h;

    std::unique_ptr<PNE<MapT, decltype(m_id_f)&>> m_id_c_f;
    std::vector<PNE<MapT, decltype(m_id_g)&>> m_id_c_g;
    std::unique_ptr<PNE<MapT, decltype(m_id_h)&>> m_id_c_h;
};

}
