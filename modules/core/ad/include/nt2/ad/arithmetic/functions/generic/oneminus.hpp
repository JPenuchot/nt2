//==============================================================================
//         Copyright 2015 J.T. Lapreste
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_AD_ARITHMETIC_FUNCTIONS_GENERIC_ONEMINUS_HPP_INCLUDED
#define NT2_AD_ARITHMETIC_FUNCTIONS_GENERIC_ONEMINUS_HPP_INCLUDED
#include<nt2/include/functions/oneminus.hpp>
#include<nt2/include/functions/unary_minus.hpp>
#include<nt2/ad/functions/ad.hpp>


namespace nt2
{
  namespace ad
  {
    template < class T>
    valder<T> oneminus(const valder<T>&u)
    {
      return valder<T>(nt2::oneminus(u.val()), -u.der());
    };
  }
}

#endif