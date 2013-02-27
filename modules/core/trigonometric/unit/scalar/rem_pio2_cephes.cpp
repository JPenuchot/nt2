//////////////////////////////////////////////////////////////////////////////
///   Copyright 2003 and onward LASMEA UMR 6602 CNRS/U.B.P Clermont-Ferrand
///   Copyright 2009 and onward LRI    UMR 8623 CNRS/Univ Paris Sud XI
///
///          Distributed under the Boost Software License, Version 1.0
///                 See accompanying file LICENSE.txt or copy at
///                     http://www.boost.org/LICENSE_1_0.txt
//////////////////////////////////////////////////////////////////////////////
#define NT2_UNIT_MODULE "nt2 trigonometric toolbox - rem_pio2_cephes/scalar Mode"

//////////////////////////////////////////////////////////////////////////////
// unit test behavior of trigonometric components in scalar mode
//////////////////////////////////////////////////////////////////////////////
/// created  by jt the 11/02/2011
///
#include <nt2/toolbox/trigonometric/include/functions/rem_pio2_cephes.hpp>
#include <boost/fusion/tuple.hpp>
#include <nt2/toolbox/trigonometric/constants.hpp>
#include <nt2/toolbox/trigonometric/include/functions/rem_pio2_medium.hpp>

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

template < class T, class N>
inline T correct(const T& a, N n)
{
  T z = a+n*nt2::Pio_2<T>();
  return  (z > nt2::Pi<T>()) ? z-2*nt2::Pi<T>() :z;
}

NT2_TEST_CASE_TPL ( rem_pio2_cephes_real__1_0,  NT2_REAL_TYPES)
{

  using nt2::rem_pio2_cephes;
  using nt2::tag::rem_pio2_cephes_;
  typedef typename nt2::meta::as_integer<T>::type iT;
  typedef typename nt2::meta::call<rem_pio2_cephes_(T)>::type r_t;
  typedef typename nt2::meta::scalar_of<r_t>::type ssr_t;
  typedef typename nt2::meta::upgrade<T>::type u_t;
  typedef typename nt2::meta::call<rem_pio2_cephes_(T)>::type wished_r_t;


  // return type conformity test
  NT2_TEST( (boost::is_same < r_t, wished_r_t >::value) );
  std::cout << std::endl;

  // specific values tests
  typedef typename nt2::meta::strip<typename boost::fusion::result_of::at_c<r_t,0>::type>::type r_t0;
  typedef typename nt2::meta::strip<typename boost::fusion::result_of::at_c<r_t,1>::type>::type r_t1;
  typedef typename nt2::meta::strip<typename boost::fusion::result_of::at_c<r_t,2>::type>::type r_t2;
  {
    r_t res = rem_pio2_cephes(nt2::Zero<T>());
    NT2_TEST_ULP_EQUAL( boost::fusion::get<0>(res), nt2::Zero<r_t0>(), 0.5);
    NT2_TEST_ULP_EQUAL( boost::fusion::get<1>(res), nt2::Zero<r_t1>(), 0.5);
    NT2_TEST_ULP_EQUAL( boost::fusion::get<2>(res), nt2::Zero<r_t2>(), 0.5);
  }

  // specific values tests
  typedef typename nt2::meta::strip<typename boost::fusion::result_of::at_c<r_t,0>::type>::type r_t0;
  typedef typename nt2::meta::strip<typename boost::fusion::result_of::at_c<r_t,1>::type>::type r_t1;
  typedef typename nt2::meta::strip<typename boost::fusion::result_of::at_c<r_t,2>::type>::type r_t2;
  {
    r_t res = rem_pio2_cephes(nt2::Zero<T>());
    NT2_TEST_ULP_EQUAL( boost::fusion::get<0>(res), nt2::Zero<r_t0>(), 0.5);
    NT2_TEST_ULP_EQUAL( boost::fusion::get<1>(res), nt2::Zero<r_t1>(), 0.5);
    NT2_TEST_ULP_EQUAL( boost::fusion::get<2>(res), nt2::Zero<r_t2>(), 0.5);
  }
  // specific values tests
  typedef typename nt2::meta::strip<typename boost::fusion::result_of::at_c<r_t,0>::type>::type r_t0;
  typedef typename nt2::meta::strip<typename boost::fusion::result_of::at_c<r_t,1>::type>::type r_t1;
  typedef typename nt2::meta::strip<typename boost::fusion::result_of::at_c<r_t,2>::type>::type r_t2;
  {
    r_t res = rem_pio2_cephes(nt2::Pi<T>());
    std::cout << nt2::Pi<T>() << std::endl;
    std::cout << correct(boost::fusion::get<0>(res), boost::fusion::get<2>(res)) << std::endl;
    std::cout << boost::fusion::get<0>(res) << std::endl;
    std::cout << boost::fusion::get<1>(res) << std::endl;
    std::cout << boost::fusion::get<2>(res) << std::endl;
    res = rem_pio2_cephes(-nt2::Pi<T>());
    res = rem_pio2_cephes(3*nt2::Pi<T>()/4);
    std::cout << 3*nt2::Pi<T>()/4 << std::endl;
    std::cout << correct(boost::fusion::get<0>(res), boost::fusion::get<2>(res))<< std::endl;
    std::cout << boost::fusion::get<0>(res) << std::endl;
    std::cout << boost::fusion::get<1>(res) << std::endl;
    std::cout << boost::fusion::get<2>(res) << std::endl;
    res = rem_pio2_cephes(-3*nt2::Pi<T>()/4);
    std::cout << -3*nt2::Pi<T>()/4 << std::endl;
    std::cout << correct(boost::fusion::get<0>(res), boost::fusion::get<2>(res))<< std::endl;
    std::cout << boost::fusion::get<0>(res) << std::endl;
    std::cout << boost::fusion::get<1>(res) << std::endl;
    std::cout << boost::fusion::get<2>(res) << std::endl;
    res = rem_pio2_cephes(nt2::Pi<T>()/2);
    std::cout << correct(boost::fusion::get<0>(res), boost::fusion::get<2>(res))<< std::endl;
    std::cout << nt2::Pi<T>()/2 << std::endl;
    std::cout << boost::fusion::get<0>(res) << std::endl;
    std::cout << boost::fusion::get<1>(res) << std::endl;
    std::cout << boost::fusion::get<2>(res) << std::endl;
    res = rem_pio2_cephes(-nt2::Pi<T>()/2);
    std::cout << correct(boost::fusion::get<0>(res), boost::fusion::get<2>(res))<< std::endl;
    std::cout << -nt2::Pi<T>()/2 << std::endl;
    std::cout << boost::fusion::get<0>(res) << std::endl;
    std::cout << boost::fusion::get<1>(res) << std::endl;
    std::cout << boost::fusion::get<2>(res) << std::endl;
  }
} // end of test for floating_
