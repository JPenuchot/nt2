##############################################################################
###   Copyright 2003 and onward LASMEA UMR 6602 CNRS/U.B.P Clermont-Ferrand
###   Copyright 2009 and onward LRI    UMR 8623 CNRS/Univ Paris Sud XI
###
###          Distributed under the Boost Software License, Version 1.0
###                 See accompanying file LICENSE.txt or copy at
###                     http://www.boost.org/LICENSE_1_0.txt
##############################################################################

SET( SOURCES
  is_included_c.cpp
  is_included.cpp
  is_simd_logical.cpp
  none.cpp
  hmsb.cpp
  sum.cpp
  prod.cpp
  posmin.cpp
  posmax.cpp
  nbtrue.cpp
  inbtrue.cpp
  minimum.cpp
  maximum.cpp
  dot.cpp
  all.cpp
  any.cpp
  compare_equal.cpp
  compare_not_equal.cpp
  compare_less.cpp
  compare_less_equal.cpp
  compare_greater.cpp
  compare_greater_equal.cpp
# List of simd test files for toolbox reduction
   )

nt2_module_add_tests(boost.simd.base.reduction.simd.unit ${SOURCES})
