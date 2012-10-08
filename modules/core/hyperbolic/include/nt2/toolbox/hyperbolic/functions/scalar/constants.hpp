//==============================================================================
//         Copyright 2003 - 2011 LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2011 LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_TOOLBOX_HYPERBOLIC_FUNCTIONS_SCALAR_CONSTANTS_HPP_INCLUDED
#define NT2_TOOLBOX_HYPERBOLIC_FUNCTIONS_SCALAR_CONSTANTS_HPP_INCLUDED

#include <boost/simd/sdk/constant/constant.hpp>
#include <boost/simd/sdk/constant/register.hpp>

namespace nt2
{
  namespace tag
  {
    BOOST_SIMD_CONSTANT_REGISTER( Oneo_120, double
                                , 0, 0x3c088889
                                , 0x3f81111111111111ll
                                );

    BOOST_SIMD_CONSTANT_REGISTER( Moneo_6, double
                                , 0, 0xbe2aaaab
                                , 0xbfc5555555555555ll
                                );

    BOOST_SIMD_CONSTANT_REGISTER( Oneo_6, double
                                , 0, 0x3e2aaaab
                                , 0x3fc5555555555555ll
                                );
  }

  BOOST_SIMD_CONSTANT_IMPLEMENTATION(tag::Oneo_120, Oneo_120);
  BOOST_SIMD_CONSTANT_IMPLEMENTATION(tag::Moneo_6, Moneo_6);
  BOOST_SIMD_CONSTANT_IMPLEMENTATION(tag::Oneo_6, Oneo_6);
}

#endif
