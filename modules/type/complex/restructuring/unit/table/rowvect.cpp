/*******************************************************************************
 *         Copyright 2003 & onward LASMEA UMR 6602 CNRS/Univ. Clermont II
 *         Copyright 2009 & onward LRI    UMR 8623 CNRS/Univ Paris Sud XI
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/
#define NT2_UNIT_MODULE "nt2::rowvect function"

#include <nt2/table.hpp>
#include <nt2/include/functions/rowvect.hpp>
#include <nt2/include/functions/size.hpp>
#include <nt2/include/functions/ones.hpp>
#include <nt2/include/functions/ndims.hpp>
#include <nt2/include/functions/isequal.hpp>
#include <nt2/include/functions/transpose.hpp>

#include <nt2/sdk/unit/module.hpp>
#include <nt2/sdk/unit/tests/relation.hpp>
#include <nt2/sdk/unit/tests/basic.hpp>
#include <nt2/sdk/unit/tests/exceptions.hpp>


NT2_TEST_CASE_TPL( rowvectc, NT2_TYPES)
{
  typedef std::complex<T> cT;
  nt2::table<cT> r;
  nt2::table<cT, nt2::_2D> y( nt2::of_size(4,4) );
  for(int j=1;j<=4;j++)
    for(int i=1;i<=4;i++)
      y(i,j) = cT(i + 10*j, j);
  r = nt2::rowvect(y);
  NT2_TEST_EQUAL(r, nt2::trans(y(nt2::_)));
  NT2_TEST_EQUAL(nt2::rowvect(y), nt2::trans(y(nt2::_)));
}
