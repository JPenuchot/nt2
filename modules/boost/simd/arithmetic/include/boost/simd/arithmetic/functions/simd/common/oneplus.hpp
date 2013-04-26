//==============================================================================
//         Copyright 2003 - 2011 LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2011 LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef BOOST_SIMD_ARITHMETIC_FUNCTIONS_SIMD_COMMON_ONEPLUS_HPP_INCLUDED
#define BOOST_SIMD_ARITHMETIC_FUNCTIONS_SIMD_COMMON_ONEPLUS_HPP_INCLUDED

#include <boost/simd/arithmetic/functions/oneplus.hpp>
#include <boost/simd/include/functions/simd/plus.hpp>
#include <boost/simd/include/functions/simd/if_else.hpp>
#include <boost/simd/include/functions/simd/is_equal.hpp>
#include <boost/simd/include/constants/one.hpp>
#include <boost/simd/include/constants/valmax.hpp>

namespace boost { namespace simd { namespace ext
{
  BOOST_SIMD_FUNCTOR_IMPLEMENTATION(boost::simd::tag::oneplus_, tag::cpu_,
                          (A0)(X),
                          ((simd_<arithmetic_<A0>,X>))
                         )
  {
    typedef A0 result_type;
    BOOST_SIMD_FUNCTOR_CALL_REPEAT(1)
    {
       return if_else(is_equal(a0, Valmax<result_type>()), a0, a0+One<A0>());
    }
  };

  BOOST_SIMD_FUNCTOR_IMPLEMENTATION( boost::simd::tag::oneplus_, tag::cpu_
                                     , (A0)(X)
                                     , ((simd_< floating_<A0>,X >))
                                    )
  {
    typedef A0 result_type;
    BOOST_SIMD_FUNCTOR_CALL(1)
      {
        return a0+One<A0>();
      }
  };
} } }
#endif