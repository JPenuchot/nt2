//==============================================================================
//         Copyright 2015 J.T. Lapreste
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_AD_PREDICATES_FUNCTIONS_GENERIC_NEGATE_HPP_INCLUDED
#define NT2_AD_PREDICATES_FUNCTIONS_GENERIC_NEGATE_HPP_INCLUDED

#include <nt2/include/functions/negate.hpp>
#include <nt2/sdk/meta/as_logical.hpp>
#include <nt2/include/functions/sign.hpp>
#include <nt2/ad/functions/ad.hpp>

namespace nt2
{
  namespace ad
  {
    template < class A1, class A2> BOOST_FORCEINLINE
    valder<A1> negate(const valder<A1> & a1
                     , const valder<A2> & a2)
    {
      return valder<A1>(nt2::negate(a1.val(), a2.val()),
                        nt2::sign(a1.val())*a2.der());
    }
    template < class A1, class A2> BOOST_FORCEINLINE
    valder<A2> negate(const A1 & a1
                     , const valder<A2> & a2)
    {
      return valder<A2>(nt2::negate(a1, a2.val())
                                   , nt2::sign(a1)*a2.der());
    }
    template < class A1, class A2> BOOST_FORCEINLINE
    valder<A1> negate(const valder<A1> & a1
                     , const A2 & a2)
    {
      typedef typename valder<A2>::elem_t elem_t;
      return valder<A2>(nt2::negate(a1.val(), a2)
                       , Zero<elem_t>());
    }
  }
}

#endif