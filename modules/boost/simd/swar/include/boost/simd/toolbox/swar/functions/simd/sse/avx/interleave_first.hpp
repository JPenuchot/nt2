//==============================================================================
//         Copyright 2003 - 2012 LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2012 LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef BOOST_SIMD_TOOLBOX_SWAR_FUNCTIONS_SIMD_SSE_AVX_INTERLEAVE_FIRST_HPP_INCLUDED
#define BOOST_SIMD_TOOLBOX_SWAR_FUNCTIONS_SIMD_SSE_AVX_INTERLEAVE_FIRST_HPP_INCLUDED
#ifdef BOOST_SIMD_HAS_AVX_SUPPORT
 
#include <boost/simd/toolbox/swar/functions/interleave_first.hpp>
#include <boost/simd/include/functions/bitwise_cast.hpp>
#include <boost/dispatch/meta/as_floating.hpp>

namespace boost { namespace simd { namespace ext
{
  BOOST_SIMD_FUNCTOR_IMPLEMENTATION( boost::simd::tag::interleave_first_
                                   , boost::simd::tag::avx_
                                   , (A0)(A1)
                                   , ((simd_<single_<A0>,boost::simd::tag::avx_>))
                                     ((simd_<single_<A1>,boost::simd::tag::avx_>))
                                   )
  {
    typedef A0 result_type;

    result_type operator()(__m256 const a0, __m256 const a1) const
    {
      typedef simd::native<double, boost::simd::tag::avx_ >       dtype;
      dtype that0 = bitwise_cast<dtype>(_mm256_permute2f128_ps(a0, a0, 0));
      that0 = _mm256_shuffle_pd(that0, that0, 4); 
      dtype that1 = bitwise_cast<dtype>(_mm256_permute2f128_ps(a1, a1, 0));
      that1 = _mm256_shuffle_pd(that1, that1, 4); 
      return _mm256_unpacklo_ps(bitwise_cast<result_type>(that0),
                                bitwise_cast<result_type>(that1));
    }
  };

  BOOST_SIMD_FUNCTOR_IMPLEMENTATION( boost::simd::tag::interleave_first_
                                   , boost::simd::tag::avx_
                                   , (A0)(A1)
                                   , ((simd_<double_<A0>,boost::simd::tag::avx_>))
                                     ((simd_<double_<A1>,boost::simd::tag::avx_>))
                                   )
  {
    typedef A0 result_type;

    result_type operator()(__m256d const a0, __m256d const a1) const
    {
      result_type that0 = _mm256_permute2f128_pd(a0, a0, 0);
      that0 = _mm256_shuffle_pd(that0, that0, 4); 
      result_type that1 = _mm256_permute2f128_pd(a1, a1, 0);
      that1 = _mm256_shuffle_pd(that1, that1, 4); 
      return _mm256_unpacklo_pd(that0,that1);
    }
  };

  BOOST_SIMD_FUNCTOR_IMPLEMENTATION( boost::simd::tag::interleave_first_
                                   , boost::simd::tag::avx_
                                   , (A0)(A1)
                                   , ((simd_<type32_<A0>,boost::simd::tag::avx_>))
                                     ((simd_<type32_<A1>,boost::simd::tag::avx_>))
                                   )
  {
    typedef A0 result_type;

    result_type operator()(__m256i const a0, __m256i const a1) const
    {
      typedef typename boost::dispatch::meta::as_floating<A0>::type  ftype;
      return  bitwise_cast<result_type>(interleave_first(bitwise_cast<ftype>(a0), bitwise_cast<ftype>(a1))); 
    }
  };

  BOOST_SIMD_FUNCTOR_IMPLEMENTATION( boost::simd::tag::interleave_first_
                                   , boost::simd::tag::avx_
                                   , (A0)(A1)
                                   , ((simd_<type64_<A0>,boost::simd::tag::avx_>))
                                     ((simd_<type64_<A1>,boost::simd::tag::avx_>))
                                   )
  {
    typedef A0 result_type;

    result_type operator()(A0 const a0, A1 const a1) const
    {
      typedef typename boost::dispatch::meta::as_floating<A0>::type  ftype;
      return bitwise_cast<result_type>(interleave_first(bitwise_cast<ftype>(a0), bitwise_cast<ftype>(a1))); 
    }
  };
    
} } }

#endif
#endif
