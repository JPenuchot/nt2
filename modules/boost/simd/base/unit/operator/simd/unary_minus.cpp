//==============================================================================
//         Copyright 2003 - 2012   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2012   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#include <boost/simd/operator/include/functions/unary_minus.hpp>

#include <boost/dispatch/functor/meta/call.hpp>
#include <boost/simd/sdk/simd/native.hpp>
#include <nt2/sdk/unit/tests/relation.hpp>
#include <nt2/sdk/unit/tests/type_expr.hpp>
#include <nt2/sdk/unit/module.hpp>
#include <boost/simd/sdk/config.hpp>
#include <boost/simd/sdk/simd/io.hpp>

#include <boost/simd/include/constants/one.hpp>
#include <boost/simd/include/constants/mone.hpp>
#include <boost/simd/include/constants/zero.hpp>
#include <boost/simd/include/constants/inf.hpp>
#include <boost/simd/include/constants/minf.hpp>
#include <boost/simd/include/constants/nan.hpp>
#include <boost/simd/include/constants/false.hpp>
#include <boost/simd/include/constants/true.hpp>

NT2_TEST_CASE_TPL ( unary_minus_real,  BOOST_SIMD_SIMD_REAL_TYPES)
{
  using boost::simd::unary_minus;
  using boost::simd::tag::unary_minus_;
  using boost::simd::native;
  using boost::simd::meta::cardinal_of;
  typedef BOOST_SIMD_DEFAULT_EXTENSION  ext_t;
  typedef native<T,ext_t>                  vT;
  typedef typename boost::dispatch::meta::call<unary_minus_(vT)>::type r_t;

  // specific values tests
  NT2_TEST_EQUAL(unary_minus(boost::simd::Inf<vT>()), boost::simd::Minf<r_t>());
  NT2_TEST_EQUAL(unary_minus(boost::simd::Minf<vT>()), boost::simd::Inf<r_t>());
  NT2_TEST_EQUAL(unary_minus(boost::simd::Nan<vT>()), boost::simd::Nan<r_t>());
  NT2_TEST_EQUAL(unary_minus(boost::simd::One<vT>()), boost::simd::Mone<r_t>());
  NT2_TEST_EQUAL(unary_minus(boost::simd::Zero<vT>()), boost::simd::Mzero<r_t>());
} // end of test for floating_

NT2_TEST_CASE_TPL ( unary_minus_signed_int,  BOOST_SIMD_SIMD_INTEGRAL_SIGNED_TYPES)
{
  using boost::simd::unary_minus;
  using boost::simd::tag::unary_minus_;
  using boost::simd::native;
  typedef BOOST_SIMD_DEFAULT_EXTENSION  ext_t;
  typedef native<T,ext_t>                  vT;
  typedef typename boost::dispatch::meta::call<unary_minus_(vT)>::type r_t;

  // specific values tests
  NT2_TEST_EQUAL(unary_minus(boost::simd::Mone<vT>()), boost::simd::One<r_t>());
  NT2_TEST_EQUAL(unary_minus(boost::simd::One<vT>()), boost::simd::Mone<r_t>());
  NT2_TEST_EQUAL(unary_minus(boost::simd::Two<vT>()), -boost::simd::Two<r_t>());
  NT2_TEST_EQUAL(unary_minus(boost::simd::Zero<vT>()), boost::simd::Zero<r_t>());
} // end of test for signed_int_
