//==============================================================================
//         Copyright 2003 - 2012   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2012   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#include <boost/simd/arithmetic/include/functions/scalar/rsqrt.hpp>
#include <boost/dispatch/functor/meta/call.hpp>

#include <nt2/sdk/unit/tests.hpp>
#include <nt2/sdk/unit/module.hpp>
#include <boost/simd/constant/constant.hpp>

NT2_TEST_CASE_TPL ( rsqrt_real,  BOOST_SIMD_REAL_TYPES)
{

  using boost::simd::rsqrt;
  using boost::simd::tag::rsqrt_;
  typedef typename boost::dispatch::meta::as_integer<T>::type iT;
  typedef typename boost::dispatch::meta::call<rsqrt_(T)>::type r_t;
  typedef typename boost::simd::meta::scalar_of<r_t>::type sr_t;
  typedef typename boost::simd::meta::scalar_of<r_t>::type ssr_t;
  typedef typename boost::dispatch::meta::as_floating<T>::type wished_r_t;


  // return type conformity test
  NT2_TEST( (boost::is_same < r_t, wished_r_t >::value) );
  std::cout << std::endl;

  // specific values tests
  NT2_TEST_ULP_EQUAL(rsqrt(boost::simd::Inf<T>()), boost::simd::Zero<r_t>(), 0);
  NT2_TEST_ULP_EQUAL(rsqrt(boost::simd::Minf<T>()), boost::simd::Nan<r_t>(), 0);
  NT2_TEST_ULP_EQUAL(rsqrt(boost::simd::Mone<T>()), boost::simd::Nan<r_t>(), 0);
  NT2_TEST_ULP_EQUAL(rsqrt(boost::simd::Nan<T>()), boost::simd::Nan<r_t>(), 0);
  NT2_TEST_ULP_EQUAL(rsqrt(boost::simd::One<T>()), boost::simd::One<r_t>(), 0);
  NT2_TEST_ULP_EQUAL(rsqrt(boost::simd::Zero<T>()), boost::simd::Inf<r_t>(), 0);
  NT2_TEST_ULP_EQUAL(rsqrt(boost::simd::Four<T>()), boost::simd::Half<r_t>(), 0);
  NT2_TEST_ULP_EQUAL(rsqrt(T(0.01)), T(10), 0);
} // end of test for floating_
