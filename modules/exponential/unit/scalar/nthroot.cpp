//////////////////////////////////////////////////////////////////////////////
///   Copyright 2003 and onward LASMEA UMR 6602 CNRS/U.B.P Clermont-Ferrand
///   Copyright 2009 and onward LRI    UMR 8623 CNRS/Univ Paris Sud XI
///
///          Distributed under the Boost Software License, Version 1.0
///                 See accompanying file LICENSE.txt or copy at
///                     http://www.boost.org/LICENSE_1_0.txt
//////////////////////////////////////////////////////////////////////////////
#define NT2_UNIT_MODULE "nt2 exponential toolbox - nthroot/scalar Mode"

//////////////////////////////////////////////////////////////////////////////
// unit test behavior of exponential components in scalar mode
//////////////////////////////////////////////////////////////////////////////
/// created by jt the 08/12/2010
/// 
#include <nt2/toolbox/exponential/include/functions/nthroot.hpp>
#include <nt2/include/functions/ulpdist.hpp>
#include <boost/type_traits/is_same.hpp>
#include <nt2/sdk/functor/meta/call.hpp>
#include <nt2/sdk/meta/as_integer.hpp>
#include <nt2/sdk/meta/as_floating.hpp>
#include <nt2/sdk/meta/as_signed.hpp>
#include <nt2/sdk/meta/upgrade.hpp>
#include <nt2/sdk/meta/downgrade.hpp>
#include <nt2/sdk/meta/scalar_of.hpp>
#include <boost/dispatch/meta/as_floating.hpp>
#include <boost/type_traits/common_type.hpp>
#include <nt2/sdk/unit/tests.hpp>
#include <nt2/sdk/unit/module.hpp>
#include <nt2/sdk/memory/buffer.hpp>
#include <nt2/toolbox/constant/constant.hpp>


NT2_TEST_CASE_TPL ( nthroot_real__2_0,  NT2_REAL_TYPES)
{
  
  using nt2::nthroot;
  using nt2::tag::nthroot_;
  typedef typename nt2::meta::as_integer<T>::type iT;
  typedef typename nt2::meta::call<nthroot_(T,iT)>::type r_t;
  typedef typename nt2::meta::scalar_of<r_t>::type ssr_t;
  typedef typename nt2::meta::upgrade<T>::type u_t;
  typedef typename boost::dispatch::meta::as_floating<T>::type wished_r_t;


  // return type conformity test 
  NT2_TEST( (boost::is_same < r_t, wished_r_t >::value) );
  std::cout << std::endl; 
  double ulpd;
  ulpd=0.0;


  // specific values tests
  NT2_TEST_ULP_EQUAL(nthroot(T(-8),iT(3)), r_t(-2), 0.5);
  NT2_TEST_ULP_EQUAL(nthroot(T(256),iT(4)), r_t(4), 0.5);
  NT2_TEST_ULP_EQUAL(nthroot(T(8),iT(3)), r_t(2), 0.5);
  NT2_TEST_ULP_EQUAL(nthroot(nt2::Inf<T>(),iT(3)), nt2::Inf<r_t>(), 0.5);
  NT2_TEST_ULP_EQUAL(nthroot(nt2::Inf<T>(),iT(4)), nt2::Inf<r_t>(), 0.5);
  NT2_TEST_ULP_EQUAL(nthroot(nt2::Minf<T>(),iT(3)), nt2::Minf<r_t>(), 0.5);
  NT2_TEST_ULP_EQUAL(nthroot(nt2::Minf<T>(),iT(4)), nt2::Nan<r_t>(), 0.5);
  NT2_TEST_ULP_EQUAL(nthroot(nt2::Mone<T>(),iT(3)), nt2::Mone<r_t>(), 0.5);
  NT2_TEST_ULP_EQUAL(nthroot(nt2::Mone<T>(),iT(4)), nt2::Nan<r_t>(), 0.5);
  NT2_TEST_ULP_EQUAL(nthroot(nt2::Nan<T>(),iT(3)), nt2::Nan<r_t>(), 0.5);
  NT2_TEST_ULP_EQUAL(nthroot(nt2::Nan<T>(),iT(4)), nt2::Nan<r_t>(), 0.5);
  NT2_TEST_ULP_EQUAL(nthroot(nt2::One<T>(),iT(3)), nt2::One<r_t>(), 0.5);
  NT2_TEST_ULP_EQUAL(nthroot(nt2::One<T>(),iT(4)), nt2::One<r_t>(), 0.5);
  NT2_TEST_ULP_EQUAL(nthroot(nt2::Zero<T>(),iT(3)), nt2::Zero<r_t>(), 0.5);
  NT2_TEST_ULP_EQUAL(nthroot(nt2::Zero<T>(),iT(4)), nt2::Zero<r_t>(), 0.5);
} // end of test for floating_

NT2_TEST_CASE_TPL ( nthroot_signed_int__2_0,  NT2_INTEGRAL_SIGNED_TYPES)
{
  
  using nt2::nthroot;
  using nt2::tag::nthroot_;
  typedef typename nt2::meta::as_integer<T>::type iT;
  typedef typename nt2::meta::call<nthroot_(T,iT)>::type r_t;
  typedef typename nt2::meta::scalar_of<r_t>::type ssr_t;
  typedef typename nt2::meta::upgrade<T>::type u_t;
  typedef typename boost::dispatch::meta::as_floating<T>::type wished_r_t;


  // return type conformity test 
  NT2_TEST( (boost::is_same < r_t, wished_r_t >::value) );
  std::cout << std::endl; 
  double ulpd;
  ulpd=0.0;


  // specific values tests
  NT2_TEST_ULP_EQUAL(nthroot(T(-8),iT(3)), r_t(-2), 0.5);
  NT2_TEST_ULP_EQUAL(nthroot(T(8),iT(3)), r_t(2), 0.5);
  NT2_TEST_ULP_EQUAL(nthroot(T(81),iT(4)), r_t(3), 0.5);
} // end of test for signed_int_

NT2_TEST_CASE_TPL ( nthroot_unsigned_int__2_0,  NT2_UNSIGNED_TYPES)
{
  
  using nt2::nthroot;
  using nt2::tag::nthroot_;
  typedef typename nt2::meta::as_integer<T>::type iT;
  typedef typename nt2::meta::call<nthroot_(T,iT)>::type r_t;
  typedef typename nt2::meta::scalar_of<r_t>::type ssr_t;
  typedef typename nt2::meta::upgrade<T>::type u_t;
  typedef typename boost::dispatch::meta::as_floating<T>::type wished_r_t;


  // return type conformity test 
  NT2_TEST( (boost::is_same < r_t, wished_r_t >::value) );
  std::cout << std::endl; 
  double ulpd;
  ulpd=0.0;


  // specific values tests
  NT2_TEST_ULP_EQUAL(nthroot(T(8),iT(3)), r_t(2), 0.5);
  NT2_TEST_ULP_EQUAL(nthroot(T(81),iT(4)), r_t(3), 0.5);
} // end of test for unsigned_int_
