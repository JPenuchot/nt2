//==============================================================================
//         Copyright 2003 - 2012   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2012   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#define NT2_UNIT_MODULE "nt2 boost.simd.swar toolbox - repeat_lower_half"

#include <boost/simd/sdk/simd/native.hpp>
#include <boost/simd/include/functions/repeat_lower_half.hpp>
#include <nt2/sdk/unit/tests.hpp>
#include <nt2/sdk/unit/module.hpp>

NT2_TEST_CASE_TPL(repeat_lower_half, (float))
{
  using boost::simd::native;
  using boost::simd::meta::cardinal_of;
  using boost::simd::repeat_lower_half;

  typedef BOOST_SIMD_DEFAULT_EXTENSION      ext_t;
  typedef native<T,boost::simd::tag::sse_>     vT;
  const std::size_t card = cardinal_of<vT>::value;
  vT a,b,c;
  for(std::size_t i=1; i<=card; ++i)
  { a[i-1]=T(i); }
  c = boost::simd::repeat_lower_half(a);
  for(std::size_t i=0; i<card; ++i)
    NT2_TEST_EQUAL(c[i],((i<(card/2))?a[i]:a[(i-card/2)]));
} 