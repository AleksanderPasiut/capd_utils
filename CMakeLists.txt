cmake_minimum_required(VERSION 3.10)
project(carina)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

set(HEADERS_LIST
    carina/capd/basic_types.hpp
    carina/capd/basic_tools.internal.hpp
    carina/capd/basic_tools.hpp
    carina/capd/c0rect2set.hpp
    carina/capd/c1rect2set.hpp
    carina/capd/fenv_rounding.hpp
    carina/capd/gauss_solver.hpp
    carina/capd/map.hpp
    carina/capd/norm.hpp
    carina/capd/ode_solver.hpp
    carina/capd/poincare_map.hpp
    carina/capd/section.hpp
    carina/capd/solution_curve.hpp
    carina/capd/timemap.hpp

    carina/affine_map.hpp
    carina/arctan2_map.hpp
    carina/c1_map.hpp
    carina/composite_map.hpp
    carina/concat.hpp
    carina/constrained_function.hpp
    carina/covering_factors.hpp
    carina/direct_sum.hpp
    carina/enp_map.hpp
    carina/eigenpair.hpp
    carina/eigenproblem.hpp
    carina/eigenproblem.searcher.hpp
    carina/extension_map.hpp
    carina/extract.hpp
    carina/fixed_point_condition.hpp
    carina/gauss.hpp
    carina/ggs.hpp
    carina/grid_map.hpp
    carina/householder_generator.hpp
    carina/householder_generator2.hpp
    carina/identity_map.hpp
    carina/idx_list.hpp
    carina/image_sum.hpp
    carina/local_coordinate_system.hpp
    carina/local_map.base.hpp
    carina/local_map.hpp
    carina/local_timemap_wrapper.hpp
    carina/local_poincare_wrapper.hpp
    carina/map_base.hpp
    carina/map_compatibility.hpp
    carina/pi.hpp
    carina/pne_map.hpp
    carina/polar_coordinates.hpp
    carina/progress_logger.hpp
    carina/projection_map.hpp
    carina/readable_scalar.hpp
    carina/readable_interval.hpp
    carina/readable_mpreal.hpp
    carina/scalar_parser.hpp

    carina/type_cast.hpp
    carina/type_cast.internal.hpp
    carina/type_adapter_map.hpp

    carina/timemap_wrapper.hpp
    carina/timemap_wrapper_extended.hpp
    carina/poincare_wrapper.hpp

    carina/krawczyk_method.expander.hpp

    carina/parallel_shooting/psm.hpp
    carina/parallel_shooting/cpsm.hpp
    carina/parallel_shooting/ecpsm.hpp
    carina/parallel_shooting/epsm.hpp
    carina/parallel_shooting/epsmr.hpp

    carina/parallel_shooting/single_shooting.hpp
    carina/parallel_shooting/parallel_shooting.hpp
    carina/parallel_shooting/parallel_shooting_init.hpp

    carina/newton_method/newton_method.root.hpp
    carina/newton_method/newton_method.roots_list.hpp
    carina/newton_method/newton_method.hpp
    carina/newton_method/newton_method.multisearcher.hpp
    )

set(SOURCES_LIST
    carina/capd/map.cpp
    carina/capd/ode_solver.cpp
    carina/capd/poincare_map.cpp
    carina/capd/timemap.cpp

    carina/number_string_info.cpp
    carina/number_string_utilities.cpp
    )

add_library(${PROJECT_NAME} STATIC ${SOURCES_LIST})

add_subdirectory(CAPD)
add_dependencies(${PROJECT_NAME} capd2)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC capd2)


