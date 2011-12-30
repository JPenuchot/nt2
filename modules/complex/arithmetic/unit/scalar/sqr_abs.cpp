//////////////////////////////////////////////////////////////////////////////
///   Copyright 2003 and onward LASMEA UMR 6602 CNRS/U.B.P Clermont-Ferrand
///   Copyright 2009 and onward LRI    UMR 8623 CNRS/Univ Paris Sud XI
///
///          Distributed under the Boost Software License, Version 1.0
///                 See accompanying file LICENSE.txt or copy at
///                     http://www.boost.org/LICENSE_1_0.txt
//////////////////////////////////////////////////////////////////////////////
#define NT2_UNIT_MODULE "nt2 boost.simd.arithmetic toolbox - sqr_abs/scalar Mode"

//////////////////////////////////////////////////////////////////////////////
// unit test behavior of boost.simd.arithmetic components in scalar mode
//////////////////////////////////////////////////////////////////////////////
/// created by jt the 30/11/2010
/// 
#include <nt2/toolbox/arithmetic/include/functions/sqr_abs.hpp>
#include <nt2/include/functions/ulpdist.hpp>
#include <nt2/include/constants/i.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/dispatch/functor/meta/call.hpp>
#include <nt2/sdk/unit/tests.hpp>
#include <nt2/sdk/unit/module.hpp>
#include <boost/simd/sdk/memory/buffer.hpp>
#include <nt2/toolbox/constant/constant.hpp>


NT2_TEST_CASE_TPL ( sqr_abs_real__1_0,  BOOST_SIMD_REAL_TYPES)
{
  
  using boost::simd::sqr_abs;
  using boost::simd::tag::sqr_abs_;
  typedef typename boost::dispatch::meta::as_integer<T>::type iT;
  typedef typename boost::dispatch::meta::call<sqr_abs_(T)>::type r_t;
  typedef typename boost::simd::meta::scalar_of<r_t>::type sr_t;
  typedef typename boost::simd::meta::scalar_of<r_t>::type ssr_t;
  typedef typename boost::dispatch::meta::upgrade<T>::type u_t;
  typedef typename std::complex<T> cT;
  typedef T wished_r_t;


  // return type conformity test 
  NT2_TEST( (boost::is_same < r_t, wished_r_t >::value) );
  std::cout << std::endl; 
  double ulpd;
 ulpd=0.0; 

  // std::cout << nt2::type_id(boost::simd::I<T>()) << std::endl; 
  // specific values tests
   NT2_TEST_EQUAL(sqr_abs(cT(1)), T(1));
   NT2_TEST_EQUAL(sqr_abs(cT(boost::simd::Inf<T>())), boost::simd::Inf<T>());
   NT2_TEST_EQUAL(sqr_abs(cT(boost::simd::Minf<T>())), boost::simd::Inf<T>());
   NT2_TEST_EQUAL(sqr_abs(cT(boost::simd::Mone<T>())), boost::simd::One<T>());
   NT2_TEST_EQUAL(sqr_abs(cT(boost::simd::Nan<T>())), boost::simd::Nan<T>());
   NT2_TEST_EQUAL(sqr_abs(cT(boost::simd::One<T>())), boost::simd::One<T>());
   NT2_TEST_EQUAL(sqr_abs(cT(boost::simd::Valmax<T>())), boost::simd::Inf<T>());
   NT2_TEST_EQUAL(sqr_abs(cT(boost::simd::Valmin<T>())), boost::simd::Inf<T>());
   NT2_TEST_EQUAL(sqr_abs(cT(boost::simd::Zero<T>())), boost::simd::Zero<T>());
   std::complex < T > a(1, 0);
   NT2_TEST_EQUAL(sqr_abs(a), boost::simd::One<T>());
   std::complex < T > b(1, 2);
   NT2_TEST_EQUAL(sqr_abs(b), boost::simd::Five<T>());
} // end of test for floating_

