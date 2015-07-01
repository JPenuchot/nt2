//==============================================================================
//         Copyright 2015 J.T. Lapreste
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#include <nt2/exp_derivatives/include/functions/realsqrt.hpp>
#include <nt2/include/functions/realsqrt.hpp>
#include <nt2/include/functions/sqr.hpp>
#include <nt2/include/functions/sqrteps.hpp>
#include <nt2/include/functor.hpp>
#include <nt2/sdk/functor/meta/call.hpp>
#include <nt2/sdk/unit/module.hpp>
#include <nt2/sdk/unit/tests/ulp.hpp>
#include <boost/simd/sdk/config.hpp>
#include <nt2/include/functor.hpp>
#include <nt2/table.hpp>


#include <nt2/include/constants/half.hpp>

// template <class Tag, long N, class X >  BOOST_FORCEINLINE
// auto  d(const X& x)
//   -> decltype(nt2::functor<Tag>()(/*boost::mpl::long_<N>(),*/ x))
// {
//   return nt2::functor<Tag>()(/*boost::mpl::long_<N>(),*/ x);
// }

NT2_TEST_CASE_TPL ( realsqrt,  NT2_REAL_TYPES)
{
  NT2_TEST_ULP_EQUAL(nt2::realsqrt(nt2::Half<T>()),
                     nt2::realsqrt<0>(nt2::Half<T>())
                    , 0.5);
  NT2_TEST_ULP_EQUAL(T(0.707106781186548),
                     nt2::realsqrt<1>(nt2::Half<T>())
                    , 2.5);
  NT2_TEST_ULP_EQUAL(-T(0.707106781186548),
                     nt2::realsqrt<2>(nt2::Half<T>())
                    , 2.5);


}


