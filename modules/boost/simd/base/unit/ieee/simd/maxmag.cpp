//==============================================================================
//         Copyright 2003 - 2013   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2013   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#include <boost/simd/ieee/include/functions/maxmag.hpp>

#include <boost/dispatch/functor/meta/call.hpp>
#include <boost/simd/sdk/simd/native.hpp>
#include <nt2/sdk/unit/tests/relation.hpp>
#include <nt2/sdk/unit/module.hpp>
#include <boost/simd/sdk/config.hpp>
#include <boost/simd/sdk/simd/io.hpp>

#include <boost/simd/include/constants/mone.hpp>
#include <boost/simd/include/constants/one.hpp>
#include <boost/simd/include/constants/zero.hpp>
#include <boost/simd/include/constants/inf.hpp>
#include <boost/simd/include/constants/minf.hpp>
#include <boost/simd/include/constants/nan.hpp>

NT2_TEST_CASE_TPL ( maxmag_real,  BOOST_SIMD_SIMD_REAL_TYPES)
{
  using boost::simd::maxmag;
  using boost::simd::tag::maxmag_;
  using boost::simd::native;
  typedef BOOST_SIMD_DEFAULT_EXTENSION  ext_t;
  typedef native<T,ext_t>                  vT;
  typedef typename boost::dispatch::meta::call<maxmag_(vT,vT)>::type r_t;

  // specific values tests
#ifndef BOOST_SIMD_NO_INVALIDS
  NT2_TEST_EQUAL(maxmag(boost::simd::Inf<vT>(), boost::simd::Inf<vT>()), boost::simd::Inf<r_t>());
  NT2_TEST_EQUAL(maxmag(boost::simd::Minf<vT>(), boost::simd::Minf<vT>()), boost::simd::Minf<r_t>());
  NT2_TEST_EQUAL(maxmag(boost::simd::Nan<vT>(), boost::simd::Nan<vT>()), boost::simd::Nan<r_t>());
#endif
  NT2_TEST_EQUAL(maxmag(boost::simd::Mone<vT>(), boost::simd::Mone<vT>()), boost::simd::Mone<r_t>());
  NT2_TEST_EQUAL(maxmag(boost::simd::One<vT>(), boost::simd::One<vT>()), boost::simd::One<r_t>());
  NT2_TEST_EQUAL(maxmag(boost::simd::Zero<vT>(), boost::simd::Zero<vT>()), boost::simd::Zero<r_t>());
  NT2_TEST_EQUAL(maxmag(boost::simd::Mone<vT>(), boost::simd::Zero<vT>()), boost::simd::Mone<r_t>());
  NT2_TEST_EQUAL(maxmag(boost::simd::Mone<vT>(), boost::simd::One<vT>()), boost::simd::One<r_t>());
}

NT2_TEST_CASE_TPL ( maxmag_si,  BOOST_SIMD_SIMD_INTEGRAL_SIGNED_TYPES)
{
  using boost::simd::maxmag;
  using boost::simd::tag::maxmag_;
  using boost::simd::native;
  typedef BOOST_SIMD_DEFAULT_EXTENSION  ext_t;
  typedef native<T,ext_t>                  vT;
  typedef typename boost::dispatch::meta::call<maxmag_(vT,vT)>::type r_t;

  // specific values tests
  NT2_TEST_EQUAL(maxmag(boost::simd::Mone<vT>(), boost::simd::Mone<vT>()), boost::simd::Mone<r_t>());
  NT2_TEST_EQUAL(maxmag(boost::simd::One<vT>(), boost::simd::One<vT>()), boost::simd::One<r_t>());
  NT2_TEST_EQUAL(maxmag(boost::simd::Zero<vT>(), boost::simd::Zero<vT>()), boost::simd::Zero<r_t>());
  NT2_TEST_EQUAL(maxmag(boost::simd::Mone<vT>(), boost::simd::Zero<vT>()), boost::simd::Mone<r_t>());
  NT2_TEST_EQUAL(maxmag(boost::simd::Mone<vT>(), boost::simd::One<vT>()), boost::simd::One<r_t>());
}

NT2_TEST_CASE_TPL ( maxmag_ui,  BOOST_SIMD_SIMD_UNSIGNED_TYPES)
{
  using boost::simd::maxmag;
  using boost::simd::tag::maxmag_;
  using boost::simd::native;
  typedef BOOST_SIMD_DEFAULT_EXTENSION  ext_t;
  typedef native<T,ext_t>                  vT;
  typedef typename boost::dispatch::meta::call<maxmag_(vT,vT)>::type r_t;

  // specific values tests
  NT2_TEST_EQUAL(maxmag(boost::simd::One<vT>(), boost::simd::One<vT>()), boost::simd::One<r_t>());
  NT2_TEST_EQUAL(maxmag(boost::simd::Zero<vT>(), boost::simd::Zero<vT>()), boost::simd::Zero<r_t>());
}
