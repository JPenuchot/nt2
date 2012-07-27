//////////////////////////////////////////////////////////////////////////////
///   Copyright 2003 and onward LASMEA UMR 6602 CNRS/U.B.P Clermont-Ferrand
///   Copyright 2009 and onward LRI    UMR 8623 CNRS/Univ Paris Sud XI
///
///          Distributed under the Boost Software License, Version 1.0
///                 See accompanying file LICENSE.txt or copy at
///                     http://www.boost.org/LICENSE_1_0.txt
//////////////////////////////////////////////////////////////////////////////
#define NT2_UNIT_MODULE "nt2 boost.simd.bitwise toolbox - ffs/scalar Mode"

//////////////////////////////////////////////////////////////////////////////
// unit test behavior of boost.simd.bitwise components in scalar mode
//////////////////////////////////////////////////////////////////////////////
/// created  by jt the 18/02/2011
/// 
#include <boost/simd/toolbox/bitwise/include/functions/ffs.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/dispatch/functor/meta/call.hpp>
#include <nt2/sdk/unit/tests.hpp>
#include <nt2/sdk/unit/module.hpp>
#include <boost/simd/sdk/memory/buffer.hpp>
#include <boost/simd/toolbox/constant/constant.hpp>
#include <boost/dispatch/details/ignore_unused.hpp>


NT2_TEST_CASE_TPL ( ffs_real__1_0,  BOOST_SIMD_REAL_TYPES)
{
  
  using boost::simd::ffs;
  using boost::simd::tag::ffs_;
  typedef typename boost::dispatch::meta::as_integer<T>::type iT;
  typedef typename boost::dispatch::meta::call<ffs_(T)>::type r_t;
  typedef typename boost::simd::meta::scalar_of<r_t>::type sr_t;
  typedef typename boost::simd::meta::scalar_of<r_t>::type ssr_t;
  typedef typename boost::dispatch::meta::upgrade<T>::type u_t;
  typedef typename boost::dispatch::meta::as_integer<T, unsigned>::type wished_r_t;


  // return type conformity test 
  NT2_TEST( (boost::is_same < r_t, wished_r_t >::value) );
  std::cout << std::endl; 

  // specific values tests
  NT2_TEST_EQUAL(ffs(boost::simd::Inf<T>()), sr_t(boost::simd::Nbmantissabits<T>()+1));
  NT2_TEST_EQUAL(ffs(boost::simd::Minf<T>()), sr_t(boost::simd::Nbmantissabits<T>()+1));
  NT2_TEST_EQUAL(ffs(boost::simd::Nan<T>()), sr_t(boost::simd::One<r_t>()));
  NT2_TEST_EQUAL(ffs(boost::simd::Signmask<T>()), sr_t(sizeof(T)*8));
  NT2_TEST_EQUAL(ffs(boost::simd::Zero<T>()), sr_t(boost::simd::Zero<r_t>()));
} // end of test for real_

NT2_TEST_CASE_TPL ( ffs_signed_int__1_0,  BOOST_SIMD_INTEGRAL_SIGNED_TYPES)
{
  
  using boost::simd::ffs;
  using boost::simd::tag::ffs_;
  typedef typename boost::dispatch::meta::as_integer<T>::type iT;
  typedef typename boost::dispatch::meta::call<ffs_(T)>::type r_t;
  typedef typename boost::simd::meta::scalar_of<r_t>::type sr_t;
  typedef typename boost::simd::meta::scalar_of<r_t>::type ssr_t;
  typedef typename boost::dispatch::meta::upgrade<T>::type u_t;
  typedef typename boost::dispatch::meta::as_integer<T, unsigned>::type wished_r_t;


  // return type conformity test 
  NT2_TEST( (boost::is_same < r_t, wished_r_t >::value) );
  std::cout << std::endl; 


  // specific values tests
  NT2_TEST_EQUAL(ffs(boost::simd::One<T>()), boost::simd::One<r_t>());
  NT2_TEST_EQUAL(ffs(boost::simd::Signmask<T>()), sizeof(T)*8);
  NT2_TEST_EQUAL(ffs(boost::simd::Zero<T>()), boost::simd::Zero<r_t>());
} // end of test for signed_int_

NT2_TEST_CASE_TPL ( ffs_unsigned_int__1_0,  BOOST_SIMD_UNSIGNED_TYPES)
{
  
  using boost::simd::ffs;
  using boost::simd::tag::ffs_;
  typedef typename boost::dispatch::meta::as_integer<T>::type iT;
  typedef typename boost::dispatch::meta::call<ffs_(T)>::type r_t;
  typedef typename boost::simd::meta::scalar_of<r_t>::type sr_t;
  typedef typename boost::simd::meta::scalar_of<r_t>::type ssr_t;
  typedef typename boost::dispatch::meta::upgrade<T>::type u_t;
  typedef typename boost::dispatch::meta::as_integer<T, unsigned>::type wished_r_t;
  
  
  // return type conformity test 
  NT2_TEST( (boost::is_same < r_t, wished_r_t >::value) );
  std::cout << std::endl; 
  
  // specific values tests
  NT2_TEST_EQUAL(ffs(boost::simd::One<T>()), boost::simd::One<r_t>());
  NT2_TEST_EQUAL(ffs(boost::simd::Zero<T>()), boost::simd::Zero<r_t>());
  for(int i=1; i < boost::simd::Valmax<char>(); i*= 2)
    {
      std::cout << boost::simd::ffs(T(i)) << std::endl; 
    }
} // end of test for unsigned_int_
