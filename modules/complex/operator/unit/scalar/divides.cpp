//////////////////////////////////////////////////////////////////////////////
///   Copyright 2003 and onward LASMEA UMR 6602 CNRS/U.B.P Clermont-Ferrand
///   Copyright 2009 and onward LRI    UMR 8623 CNRS/Univ Paris Sud XI
///
///          Distributed under the Boost Software License, Version 1.0
///                 See accompanying file LICENSE.txt or copy at
///                     http://www.boost.org/LICENSE_1_0.txt
//////////////////////////////////////////////////////////////////////////////
#define NT2_UNIT_MODULE "nt2 complex.operator toolbox - divides/scalar Mode"

//////////////////////////////////////////////////////////////////////////////
// unit test behavior of boost.simd.operator components in scalar mode
//////////////////////////////////////////////////////////////////////////////
/// created  by jt the 18/02/2011
/// 
#include <nt2/toolbox/operator/include/functions/divides.hpp>
#include <nt2/include/functions/ulpdist.hpp>
#include <boost/simd/sdk/simd/logical.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/dispatch/functor/meta/call.hpp>
#include <nt2/sdk/unit/tests.hpp>
#include <nt2/sdk/unit/module.hpp>
#include <boost/simd/sdk/memory/buffer.hpp>
#include <nt2/toolbox/constant/constant.hpp>

NT2_TEST_CASE_TPL ( divides_real__2_0,  BOOST_SIMD_REAL_TYPES)
{
  
  using boost::simd::divides;
  using boost::simd::tag::divides_;
  typedef std::complex<T> cT; 
  typedef typename boost::dispatch::meta::as_integer<T>::type iT;
  typedef typename boost::dispatch::meta::call<divides_(cT,cT)>::type r_t;
  typedef typename boost::simd::meta::scalar_of<r_t>::type sr_t;
  typedef typename boost::simd::meta::scalar_of<r_t>::type ssr_t;
  typedef typename boost::dispatch::meta::upgrade<T>::type u_t;
  typedef boost::simd::imaginary<T> ciT; 
  typedef cT wished_r_t;

  // return type conformity test 
  NT2_TEST( (boost::is_same < r_t, wished_r_t >::value) );
  std::cout << std::endl; 
  double ulpd;
  ulpd=0.0;


  // specific values tests
  NT2_TEST_EQUAL(divides(cT(boost::simd::Inf<T>()),  cT(boost::simd::Inf<T>())),  cT(boost::simd::Nan<T>(), boost::simd::Nan<T>()));
  NT2_TEST_EQUAL(divides(cT(boost::simd::Minf<T>()), cT(boost::simd::Minf<T>())), cT(boost::simd::Nan<T>(), boost::simd::Nan<T>()));
  NT2_TEST_EQUAL(divides(cT(boost::simd::Nan<T>()),  cT(boost::simd::Nan<T>())),  cT(boost::simd::Nan <T>(), boost::simd::Nan<T>()));   
  NT2_TEST_EQUAL(divides(cT(boost::simd::One<T>()),  cT(boost::simd::Zero<T>())), cT(boost::simd::Inf<T>(), boost::simd::Nan<T>())); 
  NT2_TEST_EQUAL(divides(cT(boost::simd::Zero<T>()), cT(boost::simd::Zero<T>())), cT(boost::simd::Nan<T>(), boost::simd::Nan<T>())); 
  NT2_TEST_EQUAL(divides(cT(0, 1), cT(0, 1)), cT(1, 0));
  NT2_TEST_EQUAL(divides(cT(1, 0), T(1))    , cT(1, 0)); 
  NT2_TEST_EQUAL(divides(cT(0, 2), cT(0, 1)), cT(2, 0));
  NT2_TEST_EQUAL(divides(cT(0, 1), ciT(1))   ,cT(1, 0)); 
  NT2_TEST_EQUAL(divides(ciT(1), ciT(1))     , T(1)); 
  NT2_TEST_EQUAL(divides(ciT(0), ciT(1))     , T(0));
  NT2_TEST_EQUAL(divides(T(1),   ciT(2))     , ciT(-0.5));
  NT2_TEST_EQUAL(divides(cT(1, 1), cT(1, -1)), cT(0, 1)); 
} // end of test for floating_
