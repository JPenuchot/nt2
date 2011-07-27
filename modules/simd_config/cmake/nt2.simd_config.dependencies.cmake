################################################################################
#         Copyright 2003 & onward LASMEA UMR 6602 CNRS/Univ. Clermont II
#         Copyright 2009 & onward LRI    UMR 8623 CNRS/Univ Paris Sud XI
#
#          Distributed under the Boost Software License, Version 1.0.
#                 See accompanying file LICENSE.txt or copy at
#                     http://www.boost.org/LICENSE_1_0.txt
################################################################################

include(nt2.simd)

MESSAGE( STATUS "[boost.simd.config] target system: ${NT2_OS} (${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION})" )
MESSAGE( STATUS "[boost.simd.config] target processor: ${NT2_ARCH}" )

SET(NT2_SIMD_CONFIG_DEPENDENCIES_EXTRA dispatch)

SET(NT2_SIMD_CONFIG_FLAGS ${NT2_CXX_SIMD_FLAGS})
