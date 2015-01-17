//==============================================================================
//         Copyright 2003 - 2012   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2012   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#include <boost/simd/bitwise/include/functions/bitset.hpp>
#include <boost/simd/sdk/simd/io.hpp>
#include <boost/dispatch/meta/as_integer.hpp>
#include <nt2/sdk/unit/tests.hpp>
#include <nt2/sdk/unit/module.hpp>
#include <nt2/sdk/unit/tests/type_expr.hpp>

#include <boost/simd/include/constants/one.hpp>
#include <boost/simd/include/constants/mone.hpp>
#include <boost/simd/include/constants/zero.hpp>
#include <boost/simd/include/constants/valmax.hpp>
#include <boost/simd/include/constants/three.hpp>


NT2_TEST_CASE_TPL ( bitset_uit,  BOOST_SIMD_UNSIGNED_TYPES)
{

  using boost::simd::bitset;
  using boost::simd::tag::bitset_;
  typedef typename boost::dispatch::meta::as_integer<T, unsigned>::type iT;
  typedef typename boost::dispatch::meta::call<bitset_(T,iT)>::type r_t;
  typedef T wished_r_t;


  // return type conformity test
  NT2_TEST_TYPE_IS( r_t, wished_r_t );

  // specific values tests
  NT2_TEST_EQUAL(bitset(T(2),1), T(3));
  NT2_TEST_EQUAL(bitset(T(3),2), T(3));
  NT2_TEST_EQUAL(bitset(T(3),3), T(7));
} // end of test for unsigned_int_

NT2_TEST_CASE_TPL ( bitset_si,  BOOST_SIMD_INTEGRAL_SIGNED_TYPES)
{

  using boost::simd::bitset;
  using boost::simd::tag::bitset_;
  typedef typename boost::dispatch::meta::as_integer<T, unsigned>::type iT;
  typedef typename boost::dispatch::meta::call<bitset_(T,iT)>::type r_t;
  typedef iT wished_r_t;


  // return type conformity test
  NT2_TEST_TYPE_IS( r_t, wished_r_t );

  // specific values tests
  NT2_TEST_EQUAL(bitset(T(2),1),T(3));
  NT2_TEST_EQUAL(bitset(T(2),2), T(2));
  NT2_TEST_EQUAL(bitset(T(-1), 1), T(-1));
} // end of test for unsigned_int_
