//==============================================================================
//         Copyright 2015 J.T. Lapreste
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_AD_ARITHMETIC_FUNCTIONS_GENERIC_SQR_ABS_HPP_INCLUDED
#define NT2_AD_ARITHMETIC_FUNCTIONS_GENERIC_SQR_ABS_HPP_INCLUDED
#include <nt2/include/functions/sqr_abs.hpp>
#include <nt2/include/functions/sqr.hpp>
#include <nt2/ad/functions/ad.hpp>
#include <nt2/sdk/complex/meta/is_complex.hpp>
#include <boost/assert.hpp>

namespace nt2
{
  namespace ad
  {
    template < class T>
    valder<T> sqr_abs(const valder<T>&u)
    {
      typedef typename etype<valder<T>>::type elem_t;
      BOOST_ASSERT_MSG(!nt2::meta::is_complex<elem_t>::value, "ad sqr_abs is only defined for floating");
      return sqr(u);
    };
  }
}

#endif