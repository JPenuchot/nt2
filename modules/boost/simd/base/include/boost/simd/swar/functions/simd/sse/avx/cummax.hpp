//==============================================================================
//          Copyright 2014    Jean-Thierry Lapreste
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef BOOST_SIMD_SWAR_FUNCTIONS_SIMD_SSE_AVX_CUMMAX_HPP_INCLUDED
#define BOOST_SIMD_SWAR_FUNCTIONS_SIMD_SSE_AVX_CUMMAX_HPP_INCLUDED
#ifdef BOOST_SIMD_HAS_AVX_SUPPORT

#include <boost/simd/swar/functions/cummax.hpp>
#include <boost/simd/include/functions/simd/splat.hpp>
#include <boost/simd/include/functions/simd/extract.hpp>
#include <boost/simd/include/functions/simd/max.hpp>
#include <boost/simd/sdk/meta/scalar_of.hpp>

/////////////////////////////////////////////////////////////////////////////
// Implementation when type A0 is arithmetic_
/////////////////////////////////////////////////////////////////////////////
namespace boost { namespace simd { namespace ext
{
  BOOST_DISPATCH_IMPLEMENT (cummax_, boost::simd::tag::avx_,
                            (A0),
                            ((simd_<arithmetic_<A0>,boost::simd::tag::avx_>))
                           )
  {
    typedef A0 result_type;
    BOOST_SIMD_FUNCTOR_CALL_REPEAT(1)
    {
      typedef typename meta::scalar_of<A0>::type sctype;
      typedef simd::native<sctype, boost::simd::tag::sse_ >  svtype;
      svtype a000 = _mm256_extractf128_si256(a0, 0);
      svtype a011 = _mm256_extractf128_si256(a0, 1);
      svtype a00 =  cummax(a000);
      svtype a01 =  cummax(a011);
      svtype z = splat<svtype>(a00[meta::cardinal_of<svtype>::value-1]);
      __m256i that = _mm256_castsi128_si256(a00);
      return _mm256_insertf128_si256(that, max(a01, z), 1);
    }
  };

/////////////////////////////////////////////////////////////////////////////
// Implementation when type A0 is double
/////////////////////////////////////////////////////////////////////////////


  BOOST_DISPATCH_IMPLEMENT         (cummax_, boost::simd::tag::avx_,
                                    (A0),
                                    ((simd_<double_<A0>,boost::simd::tag::avx_>))
                                   )
  {
    typedef A0 result_type;
    BOOST_SIMD_FUNCTOR_CALL_REPEAT(1)
    {
      typedef typename meta::scalar_of<A0>::type sctype;
      typedef simd::native<sctype, boost::simd::tag::sse_ >  svtype;
      svtype a000 = _mm256_extractf128_pd(a0, 0);
      svtype a011 = _mm256_extractf128_pd(a0, 1);
      svtype a00 =  cummax(a000);
      svtype a01 =  cummax(a011);
      svtype z = splat<svtype>(a00[meta::cardinal_of<svtype>::value-1]);
      __m256d that = _mm256_castpd128_pd256(a00);
      return _mm256_insertf128_pd(that, max(a01, z), 1);
    }
  };

/////////////////////////////////////////////////////////////////////////////
// Implementation when type A0 is float
/////////////////////////////////////////////////////////////////////////////


  BOOST_DISPATCH_IMPLEMENT         (cummax_, boost::simd::tag::avx_,
                                    (A0),
                                    ((simd_<single_<A0>,boost::simd::tag::avx_>))
                                   )
  {
    typedef A0 result_type;
    BOOST_SIMD_FUNCTOR_CALL_REPEAT(1)
    {
      typedef typename meta::scalar_of<A0>::type sctype;
      typedef simd::native<sctype, boost::simd::tag::sse_ >  svtype;
      svtype a011;
      a011=  _mm256_extractf128_ps(a0, 1);
      svtype a000;
      a000 =  _mm256_extractf128_ps(a0, 0);
      svtype a00 =  cummax(a000);
      svtype a01 =  cummax(a011);
      svtype z = splat<svtype>(a00[meta::cardinal_of<svtype>::value-1]);
      __m256 that = _mm256_castps128_ps256(a00);
      return _mm256_insertf128_ps(that, max(a01, z), 1);
    }
  };
} } }
#endif
#endif
