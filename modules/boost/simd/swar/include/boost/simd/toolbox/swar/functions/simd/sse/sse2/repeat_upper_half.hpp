//==============================================================================
//         Copyright 2003 - 2012 LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2012 LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef BOOST_SIMD_TOOLBOX_SWAR_FUNCTIONS_SIMD_SSE_SSE2_REPEAT_UPPER_HALF_HPP_INCLUDED
#define BOOST_SIMD_TOOLBOX_SWAR_FUNCTIONS_SIMD_SSE_SSE2_REPEAT_UPPER_HALF_HPP_INCLUDED
#ifdef BOOST_SIMD_HAS_SSE2_SUPPORT

namespace boost { namespace simd { namespace ext
{
  BOOST_SIMD_FUNCTOR_IMPLEMENTATION( boost::simd::tag::repeat_upper_half_
                                   , boost::simd::tag::sse2_
                                   , (A0)
                                   , ((simd_<single_<A0>,boost::simd::tag::sse_>))
                                   )
  {
    typedef A0 result_type;

    result_type operator()(A0 const& a0) const
    {
      return _mm_movehl_ps(a0,a0);
    }
  };
} } }

#endif
#endif