//==============================================================================
//         Copyright 2015 J.T. Lapreste
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#include <nt2/ad/trigonometric/functions/generic/sincos.hpp>
#include <nt2/include/functions/ad.hpp>
#include <nt2/include/functions/ad_io.hpp>
#include <nt2/include/functions/d_sin.hpp>
#include <nt2/include/functions/d_cos.hpp>
#include <nt2/sdk/unit/tests/relation.hpp>
#include <nt2/sdk/unit/module.hpp>
#include <boost/simd/sdk/config.hpp>
#include <nt2/table.hpp>


template < class T>
void test(const T & x, T &s, T& c)
{
   sincos(x, s, c);
}

using nt2::ad::valder;

NT2_TEST_CASE_TPL ( sincos,  NT2_REAL_TYPES)
{
  valder<T> vx(0.5, 1);
  valder<T> vs, vc;
  test(vx, vs, vc);
  NT2_TEST_EQUAL(vs.der(), nt2::d_sin(vx.val()));
  NT2_TEST_EQUAL(vc.der(), nt2::d_cos(vx.val()));
}


