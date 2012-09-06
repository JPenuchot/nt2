//==============================================================================
//         Copyright 2003 - 2012   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2012   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#define NT2_UNIT_MODULE "nt2 linalg toolbox - qr_ solvers"

#include <nt2/table.hpp>
#include <nt2/include/functions/zeros.hpp>
#include <nt2/include/functions/ones.hpp>
#include <nt2/include/functions/eye.hpp>
#include <nt2/include/functions/full_qr_solve.hpp>
#include <nt2/include/functions/ldexp.hpp>
#include <nt2/include/functions/isulpequal.hpp>
#include <nt2/include/functions/mtimes.hpp>

#include <nt2/sdk/unit/tests.hpp>
#include <nt2/sdk/unit/module.hpp>
#include <nt2/sdk/unit/tests/exceptions.hpp>
#include <nt2/sdk/unit/tests/basic.hpp>

NT2_TEST_CASE_TPL(full_qr_solver_result, NT2_REAL_TYPES)
{
  using nt2::_;
  using nt2::tag::solvers::full_qr_solve_;
  typedef typename nt2::meta::as_integer<T, signed>::type itype_t;
  typedef nt2::table<T> t_t;
  typedef nt2::table<itype_t> it_t;
  t_t a =       nt2::ones (4, 4, nt2::meta::as_<T>())
        + T(10)*nt2::eye  (4, 4, nt2::meta::as_<T>());
  t_t b = nt2::ones(4, 1, nt2::meta::as_<T>());
  nt2::display("a     ", a);
  nt2::display("b     ", b);
  typedef typename nt2::meta::call<full_qr_solve_(t_t const&, t_t const&, char)>::type result_type;
  result_type f = nt2::solvers::full_qr_solve(a, b, 'N');

  nt2::display("values", f.x());
  NT2_TEST(nt2::isulpequal(b, mtimes(a, f.x()), T(2.0))); 
 }
