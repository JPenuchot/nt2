//==============================================================================
//         Copyright 2015 J.T. Lapreste
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_AD_PREDICATES_FUNCTIONS_GENERIC_IS_REAL_HPP_INCLUDED
#define NT2_AD_PREDICATES_FUNCTIONS_GENERIC_IS_REAL_HPP_INCLUDED

#include <nt2/include/functions/is_real.hpp>
#include <nt2/ad/functions/ad.hpp>
#include <nt2/sdk/meta/as_logical.hpp>

namespace nt2
{
  namespace ad
  {
    template < class L> BOOST_FORCEINLINE
    typename meta::as_logical<L>::type
    is_real(const valder<L> & l)
    {
      return nt2::is_real(l.val());
    }
  }
}

#endif