//==============================================================================
//         Copyright 2003 - 2012   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2012   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#define NT2_UNIT_MODULE "nt2 linalg toolbox - lsq_lse_result solvers"

#include <nt2/table.hpp>
#include <nt2/include/functions/zeros.hpp>
#include <nt2/include/functions/ones.hpp>
#include <nt2/include/functions/eye.hpp>
#include <nt2/include/functions/expand.hpp>
#include <nt2/include/functions/triu.hpp>
#include <nt2/include/functions/lsq_lse_solve.hpp>
#include <nt2/include/functions/globalmax.hpp>
#include <nt2/include/functions/isulpequal.hpp>
#include <nt2/include/functions/transpose.hpp>
#include <nt2/include/functions/mtimes.hpp>

#include <nt2/sdk/unit/tests.hpp>
#include <nt2/sdk/unit/module.hpp>
#include <nt2/sdk/unit/tests/exceptions.hpp>
#include <nt2/sdk/unit/tests/basic.hpp>

NT2_TEST_CASE_TPL(svd_solve_result, NT2_REAL_TYPES)
{
  using nt2::_;
  using nt2::tag::solvers::lsq_lse_solve_;
  typedef nt2::table<T> t_t;
  t_t a = nt2::expand(nt2::triu(nt2::ones(4, 4, nt2::meta::as_<T>())), 2, 4);
  a(2, 2) = T(0);
  t_t b = nt2::expand(T(2)*nt2::eye (4, 4, nt2::meta::as_<T>()), 2, 4);
  b(2, 3) = T(1);
  t_t c = _(T(4), T(-1), T(3))(_);
  t_t d = T(2)*nt2::ones(2, 1, nt2::meta::as_<T>());
  nt2::display("a     ", a);
  nt2::display("b     ", b);
  nt2::display("c     ", c);
  nt2::display("d     ", d);
  typedef typename nt2::meta::call<lsq_lse_solve_(t_t const&, t_t const&, t_t const&, t_t const&)>::type result_type;

  result_type f = nt2::solvers::lsq_lse_solve(a, b, c, d);
  nt2::display("x", f.x());
  nt2::display("residuals ", f.residuals());
  std::cout << "status " << f.status() << std::endl;
  // nt2::display("status    ", f.status());
  t_t z = nt2::mtimes(a, f.x());
  NT2_DISPLAY(z);
  NT2_TEST(nt2::isulpequal(z, c, T(1.0)));
 }
