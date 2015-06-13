//==============================================================================
//         Copyright 2015 J.T. Lapreste
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_AD_TRIGONOMETRIC_FUNCTIONS_GENERIC_CSCD_HPP_INCLUDED
#define NT2_AD_TRIGONOMETRIC_FUNCTIONS_GENERIC_CSCD_HPP_INCLUDED

#include <nt2/trigonometric/include/functions/simd/cscd.hpp>
#include <nt2/trigonometric/include/functions/simd/cotd.hpp>
#include <nt2/include/functions/simd/multiplies.hpp>
#include <nt2/include/functions/simd/unary_minus.hpp>
#include <nt2/include/constants/deginrad.hpp>
#include <nt2/ad/functions/ad.hpp>

namespace nt2
{
  namespace ad
  {
    template < class T>
    valder<T> cscd(const valder<T>&u)
    {
      typedef typename valder<T>::elem_t elem_t;
      auto tmp = nt2::cscd(u.val());
      return valder<T>(tmp, -tmp*Deginrad<elem_t>()*nt2::cotd(u.val())*u.der());
    };
  }
}

#endif