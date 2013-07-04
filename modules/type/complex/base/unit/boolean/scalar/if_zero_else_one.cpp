//==============================================================================
//         Copyright 2003 - 2011 LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2011 LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#define NT2_UNIT_MODULE "nt2 boost.simd.boolean toolbox - if_zero_else_one/scalar Mode"

#include <nt2/include/functions/if_zero_else_one.hpp>
#include <nt2/include/constants/nan.hpp>
#include <nt2/include/constants/inf.hpp>
#include <nt2/include/constants/minf.hpp>
#include <nt2/include/constants/true.hpp>
#include <nt2/include/constants/false.hpp>
#include <nt2/sdk/functor/meta/call.hpp>
#include <nt2/sdk/unit/tests/relation.hpp>
#include <nt2/sdk/unit/tests/type_expr.hpp>
#include <nt2/sdk/unit/module.hpp>
#include <nt2/sdk/complex/complex.hpp>
#include <nt2/sdk/complex/dry.hpp>
#include <nt2/sdk/complex/meta/as_complex.hpp>
#include <nt2/sdk/complex/meta/as_dry.hpp>
#include <nt2/sdk/simd/logical.hpp>

NT2_TEST_CASE_TPL ( if_zero_else_one_real__2_0,  NT2_REAL_TYPES)
{

  using nt2::if_zero_else_one;
  using nt2::tag::if_zero_else_one_;
  using nt2::logical;
  typedef std::complex<T> cT;
  typedef typename nt2::meta::call<if_zero_else_one_(cT)>::type r_t;
  typedef cT wished_r_t;

  NT2_TEST_TYPE_IS( r_t, wished_r_t);
  NT2_TEST_EQUAL(if_zero_else_one(nt2::False< nt2::logical<T> >()), cT(1));
  NT2_TEST_EQUAL(if_zero_else_one(nt2::True< nt2::logical<T> >()), cT(0));
  NT2_TEST_EQUAL(if_zero_else_one(nt2::Inf<cT>()), cT(0));
  NT2_TEST_EQUAL(if_zero_else_one(nt2::Minf<cT>()),  cT(0));
  NT2_TEST_EQUAL(if_zero_else_one(nt2::Nan<cT>()) ,  cT(0));
  NT2_TEST_EQUAL(if_zero_else_one(nt2::Zero<cT>()),  cT(1));
} // end of test for floating_
