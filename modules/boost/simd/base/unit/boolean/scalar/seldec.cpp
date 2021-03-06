//==============================================================================
//         Copyright 2003 - 2012   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2012   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#include <boost/simd/boolean/include/functions/seldec.hpp>
#include <boost/simd/sdk/simd/io.hpp>
#include <boost/dispatch/meta/as_integer.hpp>
#include <nt2/sdk/unit/tests/relation.hpp>
#include <nt2/sdk/unit/module.hpp>
#include <nt2/sdk/unit/tests/type_expr.hpp>

#include <boost/simd/include/constants/one.hpp>
#include <boost/simd/include/constants/zero.hpp>
#include <boost/simd/include/constants/mone.hpp>

NT2_TEST_CASE_TPL ( seldec_signed_int__2_0,  BOOST_SIMD_INTEGRAL_SIGNED_TYPES)
{
  using boost::simd::seldec;
  using boost::simd::tag::seldec_;
  typedef typename boost::dispatch::meta::call<seldec_(T, T)>::type r_t;
  typedef T wished_r_t;

  // return type conformity test
  NT2_TEST_TYPE_IS( r_t, wished_r_t );

  // specific values tests
  NT2_TEST_EQUAL(seldec(boost::simd::One<T>(), boost::simd::Mone<T>()), boost::simd::Mtwo<T>());
  NT2_TEST_EQUAL(seldec(boost::simd::One<T>(), boost::simd::One<T>()), boost::simd::Zero<T>());
  NT2_TEST_EQUAL(seldec(boost::simd::One<T>(), boost::simd::Zero<T>()), boost::simd::Mone<T>());
}

NT2_TEST_CASE_TPL ( seldec_unsigned_int__2_0,  BOOST_SIMD_UNSIGNED_TYPES)
{
  using boost::simd::seldec;
  using boost::simd::tag::seldec_;
  typedef typename boost::dispatch::meta::call<seldec_(T, T)>::type r_t;
  typedef T wished_r_t;

  // return type conformity test
  NT2_TEST_TYPE_IS( r_t, wished_r_t );

  // specific values tests
  NT2_TEST_EQUAL(seldec(boost::simd::One<T>(), boost::simd::One<T>()), boost::simd::Zero<T>());
  NT2_TEST_EQUAL(seldec(boost::simd::One<T>(), boost::simd::Two<T>()), boost::simd::One<T>());
  NT2_TEST_EQUAL(seldec(boost::simd::One<T>(), boost::simd::Zero<T>()), boost::simd::Valmax<T>());
}

NT2_TEST_CASE_TPL( seldec_floating, BOOST_SIMD_REAL_TYPES)
{
  using boost::simd::seldec;
  using boost::simd::tag::seldec_;
  typedef typename boost::dispatch::meta::call<seldec_(T, T)>::type r_t;
  typedef T wished_r_t;

  // return type conformity test
  NT2_TEST_TYPE_IS( r_t, wished_r_t );

  // specific values tests
  NT2_TEST_EQUAL(seldec(boost::simd::One<T>(), boost::simd::One<T>()), boost::simd::Zero<T>());
  NT2_TEST_EQUAL(seldec(boost::simd::One<T>(), boost::simd::Two<T>()), boost::simd::One<T>());
  NT2_TEST_EQUAL(seldec(boost::simd::One<T>(), boost::simd::Zero<T>()), boost::simd::Mone<T>());
}
