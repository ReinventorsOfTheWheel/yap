# Copyright Louis Dionne 2016
# Copyright Zach Laine 2016
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

###############################################################################
# Boost
###############################################################################
find_package(Boost 1.62.0 EXACT)
if (Boost_INCLUDE_DIRS)
  add_library(boost INTERFACE)
  target_include_directories(boost INTERFACE ${Boost_INCLUDE_DIRS})
else ()
  message("-- Boost was not found; attempting to download it if we haven't already...")
  include(ExternalProject)
  ExternalProject_Add(install-Boost
    PREFIX ${CMAKE_BINARY_DIR}/dependencies/boost_1_62_0
    URL https://sourceforge.net/projects/boost/files/boost/1.62.0/boost_1_62_0.tar.bz2
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
  )

  ExternalProject_Get_Property(install-Boost SOURCE_DIR)
  add_library(boost INTERFACE)
  target_include_directories(boost INTERFACE ${SOURCE_DIR})
  add_dependencies(boost install-Boost)
  unset(SOURCE_DIR)
endif ()


###############################################################################
# GoogleTest
###############################################################################
add_subdirectory(${CMAKE_SOURCE_DIR}/googletest-release-1.8.0)
target_include_directories(gtest      INTERFACE ${CMAKE_HOME_DIRECTORY}/googletest-release-1.8.0/googletest/include)
target_include_directories(gtest_main INTERFACE ${CMAKE_HOME_DIRECTORY}/googletest-release-1.8.0/googletest/include)


###############################################################################
# Google Benchmark
###############################################################################
add_subdirectory(${CMAKE_SOURCE_DIR}/benchmark-v1.1.0)
target_include_directories(benchmark INTERFACE ${CMAKE_HOME_DIRECTORY}/benchmark-v1.1.0/include)


###############################################################################
# Autodiff (see https://github.com/fqiang/autodiff_library)
###############################################################################
add_library(autodiff_library
  ${CMAKE_SOURCE_DIR}/example/autodiff_library/ActNode.cpp
  ${CMAKE_SOURCE_DIR}/example/autodiff_library/BinaryOPNode.cpp
  ${CMAKE_SOURCE_DIR}/example/autodiff_library/Edge.cpp
  ${CMAKE_SOURCE_DIR}/example/autodiff_library/EdgeSet.cpp
  ${CMAKE_SOURCE_DIR}/example/autodiff_library/Node.cpp
  ${CMAKE_SOURCE_DIR}/example/autodiff_library/OPNode.cpp
  ${CMAKE_SOURCE_DIR}/example/autodiff_library/PNode.cpp
  ${CMAKE_SOURCE_DIR}/example/autodiff_library/Stack.cpp
  ${CMAKE_SOURCE_DIR}/example/autodiff_library/Tape.cpp
  ${CMAKE_SOURCE_DIR}/example/autodiff_library/UaryOPNode.cpp
  ${CMAKE_SOURCE_DIR}/example/autodiff_library/VNode.cpp
  ${CMAKE_SOURCE_DIR}/example/autodiff_library/autodiff.cpp
)
target_include_directories(autodiff_library PUBLIC ${CMAKE_SOURCE_DIR}/example/autodiff_library)
target_link_libraries(autodiff_library boost)
