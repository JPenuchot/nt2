//==============================================================================
//         Copyright 2003 - 2012   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2012   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#include <nt2/include/functions/roots.hpp>
#include <nt2/include/functions/complexify.hpp>
#include <nt2/include/functions/eye.hpp>
#include <nt2/include/functions/ones.hpp>
#include <boost/type_traits/is_same.hpp>
#include <nt2/sdk/functor/meta/call.hpp>
#include <nt2/sdk/unit/tests/ulp.hpp>
#include <nt2/sdk/unit/module.hpp>

#include <nt2/table.hpp>


NT2_TEST_CASE_TPL ( roots,  NT2_REAL_TYPES)
{

  using nt2::roots;
  using nt2::tag::roots_;
  typedef std::complex<T> cT;
  nt2::table<T> p =  nt2::_(T(1), T(3));
  p(2) = T(-3); p(3) = T(2);
  nt2::table<cT> c = nt2::complexify(nt2::_(T(2), T(-1), T(1)));
  NT2_TEST_ULP_EQUAL(roots(p), c, 5.0);
}






