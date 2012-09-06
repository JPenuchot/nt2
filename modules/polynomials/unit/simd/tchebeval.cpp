//////////////////////////////////////////////////////////////////////////////
///   Copyright 2003 and onward LASMEA UMR 6602 CNRS/U.B.P Clermont-Ferrand
///   Copyright 2009 and onward LRI    UMR 8623 CNRS/Univ Paris Sud XI
///
///          Distributed under the Boost Software License, Version 1.0
///                 See accompanying file LICENSE.txt or copy at
///                     http://www.boost.org/LICENSE_1_0.txt
//////////////////////////////////////////////////////////////////////////////
#define NT2_UNIT_MODULE "nt2 polynomials toolbox - tchebeval/simd Mode"

//////////////////////////////////////////////////////////////////////////////
// unit test behavior of polynomials components in simd mode
//////////////////////////////////////////////////////////////////////////////
/// created  by jt the 06/03/2011
/// 
#include <nt2/toolbox/polynomials/include/functions/tchebeval.hpp>
#include <nt2/include/functions/ulpdist.hpp>

#include <boost/type_traits/is_same.hpp>
#include <nt2/sdk/functor/meta/call.hpp>
#include <nt2/sdk/unit/tests.hpp>
#include <nt2/sdk/unit/module.hpp>
#include <nt2/sdk/memory/buffer.hpp>
#include <nt2/include/constants/real.hpp>
#include <nt2/include/constants/infinites.hpp>
#include <nt2/include/functions/load.hpp>
#include <boost/array.hpp>

NT2_TEST_CASE_TPL ( tchebeval_real__2_0,  NT2_REAL_TYPES)
{
  using nt2::tchebeval;
  using nt2::tag::tchebeval_;
  using nt2::load; 
  using boost::simd::native;
  using nt2::meta::cardinal_of;
  typedef boost::array<T, 4 > A_t;
  typedef NT2_SIMD_DEFAULT_EXTENSION  ext_t;
  typedef native<T,ext_t>                        n_t;
  typedef n_t                                     vT;
  typedef typename nt2::meta::as_integer<T>::type iT;
  typedef native<iT,ext_t>                       ivT;
  typedef typename nt2::meta::call<tchebeval_(vT,A_t)>::type r_t;
  typedef typename nt2::meta::call<tchebeval_(T,A_t)>::type sr_t;
  typedef typename nt2::meta::scalar_of<r_t>::type ssr_t;

  static const boost::array<T, 4 > A= {{ T(1), T(2), T(3), T(4) }};

  NT2_TEST_EQUAL(tchebeval( nt2::splat<vT>(1), A)[0], T(1.5)); 
  NT2_TEST_EQUAL(tchebeval( nt2::splat<vT>(2), A)[0], T(8.0)); 
  NT2_TEST_EQUAL(tchebeval( nt2::splat<vT>(3), A)[0], T(22.5)); 

} // end of test for floating_
