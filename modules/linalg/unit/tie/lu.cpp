//==============================================================================
//         Copyright 2003 - 2012   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2012   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#define NT2_UNIT_MODULE "nt2 linalg toolbox - tied lu function"

#include <nt2/table.hpp>
#include <nt2/include/functions/zeros.hpp>
#include <nt2/include/functions/ones.hpp>
#include <nt2/include/functions/eye.hpp>
#include <nt2/include/functions/lu.hpp>
#include <nt2/include/functions/tie.hpp>

#include <nt2/sdk/unit/tests.hpp>
#include <nt2/sdk/unit/module.hpp>
#include <nt2/sdk/unit/tests/exceptions.hpp>

NT2_TEST_CASE_TPL ( lu, NT2_REAL_TYPES)
{
  typedef nt2::table<T> table_t;
  table_t b = nt2::ones(4, 4, nt2::meta::as_<T>())
                + T(10)*nt2::eye(4, 4, nt2::meta::as_<T>());
  table_t lu, l, u, p;
  nt2::tie(lu) = nt2::lu(b);
  NT2_DISPLAY(lu);

  nt2::tie(l, u) = nt2::lu(b);
  NT2_DISPLAY(l);
  NT2_DISPLAY(u);

  nt2::tie(l, u, p) = nt2::lu(b);
  NT2_DISPLAY(l);
  NT2_DISPLAY(u);
  NT2_DISPLAY(p);

}


