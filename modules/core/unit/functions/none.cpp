/*******************************************************************************
 *         Copyright 2003 & onward LASMEA UMR 6602 CNRS/Univ. Clermont II
 *         Copyright 2009 & onward LRI    UMR 8623 CNRS/Univ Paris Sud XI
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/
#define NT2_UNIT_MODULE "nt2::none function"

#include <nt2/table.hpp>
#include <nt2/include/functions/toint.hpp>
#include <nt2/include/functions/of_size.hpp>
#include <nt2/include/functions/none.hpp>
#include <nt2/include/functions/logical_andnot.hpp>
#include <nt2/include/functions/is_true.hpp>
#include <nt2/include/constants/true.hpp>

#include <nt2/sdk/unit/module.hpp>
#include <nt2/sdk/unit/tests/basic.hpp>
#include <nt2/sdk/unit/tests/relation.hpp>
#include <nt2/sdk/unit/tests/type_expr.hpp>
#include <nt2/sdk/unit/tests/exceptions.hpp>

NT2_TEST_CASE_TPL( none_scalar, NT2_TYPES )
{
  T x = nt2::none(nt2::True<T>());
  NT2_TEST_EQUAL( x, (nt2::True<T>()) );

  x = nt2::none(nt2::True<T>(),1);
  NT2_TEST_EQUAL( x, nt2::True<T>() );

  x = nt2::none(nt2::True<T>(),2);
  NT2_TEST_EQUAL( x, (nt2::True<T>()) );

}

NT2_TEST_CASE_TPL( none_expr, NT2_TYPES )
{
  using nt2::_;
  nt2::table<T> y( nt2::of_size(5,3) );
  nt2::table<T> sy( nt2::of_size(1,3) );
  for(int j=1;j<=3;j++)
    for(int i=1;i<=5;i++)
      y(i,j) = i + 10*j;
  disp("y", y);
  sy = nt2::none(y);
  for(int j=1;j<=3;j++)
    {
    bool z = true;
    for(int i=1;i<=5;i++) z= nt2::logical_andnot(z, y(i, j));
    NT2_TEST_EQUAL(nt2::is_nez(z), sy(j));
    }

//   disp("sy", sy);
//   sy = nt2::none(y, 1);
//   for(int j=1;j<=3;j++)
//     for(int i=1;i<=5;i++)
//       NT2_TEST_LESSER_EQUAL(y(i, j), sy(j));
//   disp("sy", sy);
//   sy = nt2::none(y, 2);
//   for(int j=1;j<=3;j++)
//     for(int i=1;i<=5;i++)
//       NT2_TEST_LESSER_EQUAL(y(i, j), sy(i));
//   disp("sy", sy);
//   sy = nt2::none(y, 3);
//   for(int j=1;j<=3;j++)
//     for(int i=1;i<=5;i++)
//       NT2_TEST_LESSER_EQUAL(y(i, j), sy(i, j));
//   disp("sy", sy);
  sy = nt2::none(y(_));
  disp(sy);
  NT2_TEST_EQUAL(sy(1), True<T>());
}

