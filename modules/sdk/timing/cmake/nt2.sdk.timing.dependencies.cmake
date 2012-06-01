################################################################################
#         Copyright 2003 & onward LASMEA UMR 6602 CNRS/Univ. Clermont II
#         Copyright 2009 & onward LRI    UMR 8623 CNRS/Univ Paris Sud XI
#
#          Distributed under the Boost Software License, Version 1.0.
#                 See accompanying file LICENSE.txt or copy at
#                     http://www.boost.org/LICENSE_1_0.txt
################################################################################

include(nt2.boost)

SET(NT2_SDK.TIMING_DEPENDENCIES_FOUND ${Boost_FOUND})
SET(NT2_SDK.TIMING_DEPENDENCIES_INCLUDE_DIR ${Boost_INCLUDE_DIRS})
SET(NT2_SDK.TIMING_DEPENDENCIES_LIBRARY_DIR ${Boost_LIBRARY_DIRS})
SET(NT2_SDK_TIMING_DEPENDENCIES_EXTRA sdk.config)
SET(NT2_SDK.TIMING_LIBRARIES optimized nt2 debug nt2_d)
