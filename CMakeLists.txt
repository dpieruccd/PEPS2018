cmake_minimum_required(VERSION 2.8)
project(MonteCarlo-pricer CXX)
add_compile_options(-std=c++11)
# Prefix
if (NOT PREFIX)
    set(PREFIX ${CMAKE_CURRENT_BINARY_DIR})
endif (NOT PREFIX)
set(CMAKE_INSTALL_PREFIX ${PREFIX} CACHE STRING "Installation prefix." FORCE)

if (CXX)
    set(CMAKE_CXX_COMPILER ${CXX})
endif (CXX)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

# Release or Debug
if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif ()


# Detect Boost
set(Boost_NO_BOOST_CMAKE ON)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
message(STATUS "Boost include directory ${Boost_INCLUDE_DIR}")

# Detect PNL
find_package(Pnl REQUIRED)
set(LIBS ${LIBS} ${PNL_LIBRARIES})
include_directories(${PNL_INCLUDE_DIRS})
if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    add_definitions(-DPNL_RANGE_CHECK_OFF)
endif()

# Add math lib
set(LIBS ${LIBS} m)


# Print compiler flags
get_directory_property(DirDefs COMPILE_DEFINITIONS)
message(STATUS "COMPILE_DEFINITIONS = ${DirDefs}")
if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    message(STATUS "C++ Flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_LINKER_EXE_FLAGS}" )
else()
    message(STATUS "C++ Flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_LINKER_EXE_FLAGS}" )
endif()

##############################
# Add subdirectory and main
add_subdirectory(src)
add_executable(pricer src/pricer.cpp)
target_link_libraries(pricer BS MC OP UT ${LIBS})


##############################
# Tests
enable_testing()
add_test(NAME BlackScholesModel COMMAND BlackScholes)
add_test(NAME AsianOption COMMAND AsianOption)
add_test(NAME BasketOption COMMAND BasketOption)
add_test(NAME PerformanceOption COMMAND PerformanceOption)
add_test(NAME Opt COMMAND Opt)
add_test(NAME MonteCarlo COMMAND MonteCarlo)
add_test(NAME Delta COMMAND Delta)
add_test(NAME PLasian COMMAND PLasian)
add_test(NAME PLbasket COMMAND PLbasket)
add_test(NAME PLcall COMMAND PLcall)
add_test(NAME PriceSupZero COMMAND PriceSupZero)
add_test(NAME SimulMarket COMMAND SimulMarket)

set_tests_properties(BlackScholesModel PROPERTIES PASS_REGULAR_EXPRESSION "Test passed")
