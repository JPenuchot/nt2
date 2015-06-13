//==============================================================================
//         Copyright 2015 J.T. Lapreste
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_AD_ARITHMETIC_FUNCTIONS_GENERIC_ONEPLUS_HPP_INCLUDED
#define NT2_AD_ARITHMETIC_FUNCTIONS_GENERIC_ONEPLUS_HPP_INCLUDED
#include <nt2/include/functions/oneplus.hpp>
#include <nt2/ad/functions/ad.hpp>


namespace nt2
{
  namespace ad
  {
    template < class T>
    valder<T> oneplus(const valder<T>&u)
    {
      return valder<T>(nt2::oneplus(u.val()), u.der());
    };
  }
}

#endif