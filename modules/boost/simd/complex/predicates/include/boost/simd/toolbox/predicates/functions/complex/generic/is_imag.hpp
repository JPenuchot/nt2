//==============================================================================
//         Copyright 2003 - 2011 LASMEA UMR 6602 CNRS/Univ. Clermont II         
//         Copyright 2009 - 2011 LRI    UMR 8623 CNRS/Univ Paris Sud XI         
//                                                                              
//          Distributed under the Boost Software License, Version 1.0.          
//                 See accompanying file LICENSE.txt or copy at                 
//                     http://www.boost.org/LICENSE_1_0.txt                     
//==============================================================================
#ifndef BOOST_SIMD_TOOLBOX_OPERATOR_FUNCTIONS_COMPLEX_GENERIC_IS_IMAG_HPP_INCLUDED
#define BOOST_SIMD_TOOLBOX_OPERATOR_FUNCTIONS_COMPLEX_GENERIC_IS_IMAG_HPP_INCLUDED
#include <boost/simd/include/functions/is_eqz.hpp>
#include <boost/simd/include/constants/true.hpp>
#include <boost/simd/sdk/complex/complex.hpp>
#include <boost/simd/sdk/complex/imaginary.hpp>

namespace boost { namespace simd { namespace ext
{
  // complex
  BOOST_SIMD_FUNCTOR_IMPLEMENTATION( boost::simd::tag::is_imag_, tag::cpu_, (A0)
                            , (generic_< complex_< arithmetic_<A0> > >)
                            )
  {
    typedef typename  meta::real_of<A0>::type rA0; 
    typedef typename meta::as_logical<rA0>::type result_type;
    BOOST_SIMD_FUNCTOR_CALL(1)
    {
      return is_eqz(real(a0)); 
    }
  };

  // imaginary
  BOOST_SIMD_FUNCTOR_IMPLEMENTATION( boost::simd::tag::is_imag_, tag::cpu_, (A0), 
                              (generic_< imaginary_< arithmetic_<A0> > > )
                            )
  {
    typedef typename  meta::real_of<A0>::type rA0; 
    typedef typename meta::as_logical<rA0>::type result_type;
    BOOST_SIMD_FUNCTOR_CALL(1)
    {
      return True<rA0>(); 
    }
  };
} } }

#endif
