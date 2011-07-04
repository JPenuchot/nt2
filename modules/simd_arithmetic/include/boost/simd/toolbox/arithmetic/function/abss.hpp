//////////////////////////////////////////////////////////////////////////////
///   Copyright 2003 and onward LASMEA UMR 6602 CNRS/U.B.P Clermont-Ferrand
///   Copyright 2009 and onward LRI    UMR 8623 CNRS/Univ Paris Sud XI
///
///          Distributed under the Boost Software License, Version 1.0
///                 See accompanying file LICENSE.txt or copy at
///                     http://www.boost.org/LICENSE_1_0.txt
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_SIMD_TOOLBOX_ARITHMETIC_FUNCTION_ABSS_HPP_INCLUDED
#define BOOST_SIMD_TOOLBOX_ARITHMETIC_FUNCTION_ABSS_HPP_INCLUDED
#include <boost/simd/include/simd.hpp>
#include <boost/simd/include/functor.hpp>
#include <boost/simd/toolbox/arithmetic/include.hpp>

namespace boost { namespace simd {
  namespace tag
  {
    struct abss_ {};
  }
  BOOST_SIMD_FUNCTION_IMPLEMENTATION(tag::abss_, abss, 1)
  BOOST_SIMD_FUNCTION_IMPLEMENTATION(tag::abss_, sturated_abs, 1)
} }

#include <boost/simd/toolbox/operator.hpp>
#include <boost/simd/toolbox/arithmetic/function/scalar/abss.hpp>
#include <boost/simd/toolbox/arithmetic/function/simd/all/abss.hpp> 

#endif
