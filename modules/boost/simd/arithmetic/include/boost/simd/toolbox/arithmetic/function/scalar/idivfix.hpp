//==============================================================================
//         Copyright 2003 - 2011 LASMEA UMR 6602 CNRS/Univ. Clermont II         
//         Copyright 2009 - 2011 LRI    UMR 8623 CNRS/Univ Paris Sud XI         
//                                                                              
//          Distributed under the Boost Software License, Version 1.0.          
//                 See accompanying file LICENSE.txt or copy at                 
//                     http://www.boost.org/LICENSE_1_0.txt                     
//==============================================================================
#ifndef BOOST_SIMD_TOOLBOX_ARITHMETIC_FUNCTION_SCALAR_IDIVFIX_HPP_INCLUDED
#define BOOST_SIMD_TOOLBOX_ARITHMETIC_FUNCTION_SCALAR_IDIVFIX_HPP_INCLUDED

#include <boost/simd/include/functions/rdivide.hpp>
#include <boost/simd/include/functions/toint.hpp>

namespace boost { namespace simd { namespace ext
{
  BOOST_SIMD_FUNCTOR_IMPLEMENTATION( boost::simd::tag::idivfix_, tag::cpu_, (A0)(A1)
                            , (scalar_< arithmetic_<A0> >)
                              (scalar_< arithmetic_<A1> >)
                            )
  {
    typedef typename dispatch::meta::result_of<dispatch::meta::arithmetic(A0,A1)>::type result_type;
    BOOST_SIMD_FUNCTOR_CALL(2)
    {
      return boost::simd::rdivide(a0, a1);
    }

  };

  BOOST_SIMD_FUNCTOR_IMPLEMENTATION( boost::simd::tag::idivfix_, tag::cpu_, (A0)(A1)
                            , (scalar_< real_<A0> > )(scalar_< real_<A1> > )
                            )

  {
    typedef typename meta::
            as_integer < typename meta::
                                  result_of<meta::arithmetic(A0,A1)>::type
                       >::type                                      result_type;
    BOOST_SIMD_FUNCTOR_CALL(2)
    {
      return boost::simd::toint(a0/a1); 
    }
  };
} } }

#endif
