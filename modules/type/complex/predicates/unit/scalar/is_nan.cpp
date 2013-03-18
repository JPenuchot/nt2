//==============================================================================
//         Copyright 2003 - 2012   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2012   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#define NT2_UNIT_MODULE "nt2 boost.simd.predicates toolbox - is_nan/scalar Mode"

//////////////////////////////////////////////////////////////////////////////
// unit test behavior of boost.simd.predicates components in scalar mode
//////////////////////////////////////////////////////////////////////////////
/// created  by jt the 21/02/2011
///
#include <nt2/toolbox/predicates/include/functions/is_nan.hpp>
#include <boost/simd/sdk/simd/logical.hpp>

#include <boost/dispatch/functor/meta/call.hpp>
#include <nt2/sdk/unit/tests/relation.hpp>
#include <nt2/sdk/unit/tests/type_expr.hpp>
#include <nt2/sdk/unit/module.hpp>
#include <nt2/include/constants/inf.hpp>
#include <nt2/include/constants/nan.hpp>
#include <nt2/include/constants/one.hpp>
#include <nt2/include/constants/zero.hpp>
#include <nt2/include/constants/minf.hpp>

NT2_TEST_CASE_TPL ( is_nan_real__1_0,  BOOST_SIMD_REAL_TYPES)
{
  using nt2::is_nan;
  using nt2::tag::is_nan_;
  typedef std::complex<T> cT;
  typedef nt2::imaginary<T> ciT;
  typedef typename boost::dispatch::meta::call<is_nan_(cT)>::type r_t;

  // return type conformity test
  NT2_TEST_TYPE_IS( typename boost::dispatch::meta::call<is_nan_(cT)>::type, nt2::logical<T> );

  // specific values tests
  NT2_TEST_EQUAL(is_nan(cT(nt2::Inf<T>())),  r_t(false));
  NT2_TEST_EQUAL(is_nan(cT(nt2::Minf<T>())), r_t(false));
  NT2_TEST_EQUAL(is_nan(cT(nt2::Nan<T>())),  r_t(true));
  NT2_TEST_EQUAL(is_nan(cT(nt2::One<T>())),  r_t(false));
  NT2_TEST_EQUAL(is_nan(cT(nt2::Zero<T>())), r_t(false));
  NT2_TEST_EQUAL(is_nan(cT(nt2::Inf<T>(), T(0))),  r_t(false));
  NT2_TEST_EQUAL(is_nan(cT(nt2::Minf<T>(), T(0))), r_t(false));
  NT2_TEST_EQUAL(is_nan(cT(nt2::Nan<T>(), T(0))),  r_t(true));
  NT2_TEST_EQUAL(is_nan(cT(nt2::Nan<T>(), nt2::Inf<T>())),  r_t(true));
  NT2_TEST_EQUAL(is_nan(cT(nt2::One<T>(), T(0))),  r_t(false));
  NT2_TEST_EQUAL(is_nan(cT(nt2::Zero<T>(), T(0))), r_t(false));
  NT2_TEST_EQUAL(is_nan(cT(0, 0)), r_t(false));
  NT2_TEST_EQUAL(is_nan(cT(1, 0))    , r_t(false));
  NT2_TEST_EQUAL(is_nan(cT(0, 2)), r_t(false));
  NT2_TEST_EQUAL(is_nan(cT(0, 1))   , r_t(false));
  NT2_TEST_EQUAL(is_nan(ciT(1))     , r_t(false));
  NT2_TEST_EQUAL(is_nan(ciT(0))     , r_t(false));
  NT2_TEST_EQUAL(is_nan(cT(T(0), nt2::Inf<T>())),  r_t(false));
  NT2_TEST_EQUAL(is_nan(cT(T(0), nt2::Minf<T>())), r_t(false));
  NT2_TEST_EQUAL(is_nan(cT(T(0), nt2::Nan<T>())),  r_t(true));
  NT2_TEST_EQUAL(is_nan(cT(T(0), nt2::One<T>())),  r_t(false));
  NT2_TEST_EQUAL(is_nan(cT(nt2::Inf<T>(), nt2::Nan<T>())),  r_t(true));
  NT2_TEST_EQUAL(is_nan(cT(T(0), nt2::Zero<T>())), r_t(false));
}

