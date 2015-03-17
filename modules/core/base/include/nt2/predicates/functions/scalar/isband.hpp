//==============================================================================
//         Copyright 2015 J.T. Lapreste
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_PREDICATES_FUNCTIONS_SCALAR_ISBAND_HPP_INCLUDED
#define NT2_PREDICATES_FUNCTIONS_SCALAR_ISBAND_HPP_INCLUDED

#include <nt2/predicates/functions/isband.hpp>

namespace nt2 { namespace ext
{

  BOOST_DISPATCH_IMPLEMENT  ( isband_, tag::cpu_
                            , (A0)(A1)(A2)
                            , (scalar_<unspecified_<A0> >)
                              (scalar_<integer_<A1> >)
                              (scalar_<integer_<A2> >)
                            )
  {
    typedef bool result_type;
    BOOST_FORCEINLINE result_type operator()(const A0&,  A1 , A2 ) const
    {
      return true;
    }
  };
} }

#endif