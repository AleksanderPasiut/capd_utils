///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <type_traits>

namespace Carina
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Map compatibility check utility
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT, typename MapU>
class MapCompatibility
{
public:
    using MapU_Bare = typename std::remove_reference<MapU>::type;

    static constexpr bool scalar_compatibility = std::is_same<typename MapT::ScalarType, typename MapU_Bare::ScalarType>::value;
    static constexpr bool vector_compatibility = std::is_same<typename MapT::VectorType, typename MapU_Bare::VectorType>::value;
    static constexpr bool matrix_compatibility = std::is_same<typename MapT::MatrixType, typename MapU_Bare::MatrixType>::value;

    static constexpr bool value = scalar_compatibility && vector_compatibility && matrix_compatibility;
};

}
