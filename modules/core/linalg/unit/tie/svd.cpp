//==============================================================================
//         Copyright 2003 - 2012   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2012   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#define NT2_UNIT_MODULE "nt2 linalg toolbox - tied svd function"

#include <nt2/table.hpp>
#include <nt2/include/functions/zeros.hpp>
#include <nt2/include/functions/ones.hpp>
#include <nt2/include/functions/eye.hpp>
#include <nt2/include/functions/svd.hpp>
#include <nt2/include/functions/tie.hpp>
#include <nt2/include/functions/isulpequal.hpp>
#include <nt2/include/functions/trans.hpp>
#include <nt2/include/functions/mtimes.hpp>
#include <nt2/include/functions/globalmax.hpp>
#include <nt2/include/functions/ulpdist.hpp>
#include <nt2/include/functions/isulpequal.hpp>
#include <nt2/sdk/unit/tests.hpp>
#include <nt2/sdk/unit/module.hpp>
#include <nt2/sdk/unit/tests/exceptions.hpp>
#include <nt2/sdk/unit/tests/basic.hpp>

NT2_TEST_CASE_TPL ( svd, NT2_REAL_TYPES)
{
  typedef nt2::table<T> table_t;
  table_t z, b = nt2::ones(4, 5, nt2::meta::as_<T>())
                + T(10)*nt2::eye(4, 5, nt2::meta::as_<T>());

  NT2_DISPLAY(b);
  z = nt2::svd(b);
  NT2_DISPLAY(z); // just vector of singular values

  table_t u, w, v;
  nt2::tie(u, w, v) = nt2::svd(b, 'N'); //no economy
  NT2_DISPLAY(u);
  NT2_DISPLAY(w);
  NT2_DISPLAY(v);
  table_t zz = nt2::mtimes(u, nt2::mtimes(w, nt2::trans(v)));
  std::cout <<        nt2::globalmax(nt2::ulpdist(b, zz)) << std::endl;
  
  NT2_TEST(nt2::isulpequal(nt2::mtimes(u, nt2::mtimes(w, nt2::trans(v))), b, T(15.0)));
  
  nt2::tie(u, w, v) = nt2::svd(b, 'R'); //economy 0
  NT2_DISPLAY(u);
  NT2_DISPLAY(w);
  NT2_DISPLAY(v);

  nt2::tie(u, w, v) = nt2::svd(b, 'L');//economy 'econ'
  NT2_DISPLAY(u);
  NT2_DISPLAY(w);
  NT2_DISPLAY(v);
}
