//==============================================================================
//         Copyright 2003 - 2011 LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2011 LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_TRIG_DERIVATIVES_FUNCTIONS_GENERIC_D_ACOSD_HPP_INCLUDED
#define NT2_TRIG_DERIVATIVES_FUNCTIONS_GENERIC_D_ACOSD_HPP_INCLUDED
#include <nt2/trig_derivatives/functions/d_acosd.hpp>
#include <nt2/include/functions/simd/indeg.hpp>
#include <nt2/include/functions/d_acos.hpp>

namespace nt2 { namespace ext
{

  BOOST_DISPATCH_IMPLEMENT  ( d_acosd_, tag::cpu_
                            , (A0)
                            , (generic_<unspecified_<A0>>)
                            )
  {
    typedef A0 result_type;
    BOOST_FORCEINLINE result_type operator()(const A0& u) const
    {
      return indeg(d_acos(u));
    }
  };
} }


#endif