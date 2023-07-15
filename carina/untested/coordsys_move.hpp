///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <carina/local_coordinate_system.hpp>

#include <list>
#include <stdexcept>
#include <sstream>

namespace Carina
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @todo
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT, typename MapU>
class CoordsysMove
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    using Coordsys = Carina::LocalCoordinateSystem<MapT>;

    static Coordsys eval(MapU& map, const Coordsys& coordsys)
    {
        MatrixType der( coordsys.get_directions_matrix().dimension() );

        const VectorType new_origin = map( coordsys.get_origin(), der );
        const MatrixType new_directions_matrix = der * coordsys.get_directions_matrix();

        return Coordsys( new_origin, new_directions_matrix );
    }
};

}
