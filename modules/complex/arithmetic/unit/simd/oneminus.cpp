//////////////////////////////////////////////////////////////////////////////
///   Copyright 2003 and onward LASMEA UMR 6602 CNRS/U.B.P Clermont-Ferrand
///   Copyright 2009 and onward LRI    UMR 8623 CNRS/Univ Paris Sud XI
///
///          Distributed under the Boost Software License, Version 1.0
///                 See accompanying file LICENSE.txt or copy at
///                     http://www.boost.org/LICENSE_1_0.txt
//////////////////////////////////////////////////////////////////////////////
#define NT2_UNIT_MODULE "nt2 complex.arithmetic toolbox - oneminus/simd Mode"

//////////////////////////////////////////////////////////////////////////////
// unit test behavior of boost.simd.arithmetic components in scalar mode
//////////////////////////////////////////////////////////////////////////////
/// created by jt the 01/12/2010
/// 
#include <nt2/toolbox/arithmetic/include/functions/oneminus.hpp>
#include <nt2/include/functions/ulpdist.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/dispatch/functor/meta/call.hpp>
#include <nt2/sdk/unit/tests.hpp>
#include <nt2/sdk/unit/module.hpp>
#include <boost/simd/sdk/memory/buffer.hpp>
#include <nt2/toolbox/constant/constant.hpp>
#include <nt2/sdk/complex/dry.hpp>


NT2_TEST_CASE_TPL ( oneminus_real__1_0,  BOOST_SIMD_SIMD_REAL_TYPES)
{
  
  using nt2::oneminus;
  using nt2::tag::oneminus_;
  typedef typename boost::dispatch::meta::as_integer<T>::type iT;
  typedef typename boost::dispatch::meta::call<oneminus_(T)>::type r_t;
  typedef typename nt2::meta::scalar_of<r_t>::type sr_t;
  typedef typename nt2::meta::scalar_of<r_t>::type ssr_t;
  typedef typename boost::dispatch::meta::upgrade<T>::type u_t;
  typedef T wished_r_t;
  typedef typename std::complex<T> cT;
  typedef typename nt2::meta::as_imaginary<T>::type ciT;
  typedef typename nt2::meta::as_dry<T>::type dT;


  // return type conformity test 
  NT2_TEST( (boost::is_same < r_t, wished_r_t >::value) );
  std::cout << std::endl; 
  double ulpd;
  ulpd=0.0;


  // specific values tests
  NT2_TEST_EQUAL(oneminus(nt2::Inf<cT>()), -nt2::Inf<cT>());
  NT2_TEST_EQUAL(oneminus(nt2::Minf<cT>()), -nt2::Minf<cT>());
  NT2_TEST_EQUAL(oneminus(nt2::Mone<cT>()), nt2::Two<cT>());
  NT2_TEST_EQUAL(oneminus(nt2::Nan<cT>()), nt2::Nan<cT>());
  NT2_TEST_EQUAL(oneminus(nt2::One<cT>()), nt2::Zero<cT>());
  NT2_TEST_EQUAL(oneminus(nt2::Zero<cT>()), nt2::One<cT>());
} // end of test for floating_

