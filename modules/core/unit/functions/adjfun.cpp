/*******************************************************************************
 *         Copyright 2003 & onward LASMEA UMR 6602 CNRS/Univ. Clermont II
 *         Copyright 2009 & onward LRI    UMR 8623 CNRS/Univ Paris Sud XI
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/
#define NT2_UNIT_MODULE "nt2::sum1 function"

#include <nt2/table.hpp>
#include <nt2/include/functions/adjfun.hpp>
#include <nt2/include/functions/sum.hpp>
#include <nt2/include/functions/abs.hpp>
#include <nt2/include/functions/size.hpp>
#include <nt2/include/functions/rec.hpp>
#include <nt2/include/functions/is_eqz.hpp>
#include <nt2/include/functions/if_else.hpp>
#include <nt2/include/functions/ones.hpp>
#include <nt2/include/functions/sqr.hpp>
#include <nt2/include/functions/zeros.hpp>
#include <nt2/include/constants/two.hpp>

#include <nt2/sdk/unit/module.hpp>
#include <nt2/sdk/unit/tests/relation.hpp>

// NT2_TEST_CASE_TPL( adjfun_scalar, (float)(double))//NT2_TYPES )
// {
//   T x = nt2::adjfun(T(42));
//   NT2_TEST_EQUAL( x, T(42) );

//   x = nt2::adjfun(T(42),1);
//   NT2_TEST_EQUAL( x, T(42) );

//   x = nt2::adjfun(T(42),0);
//   NT2_TEST_EQUAL( x, T(42) );

// }

NT2_TEST_CASE_TPL( adjfun, (float)(double))//NT2_TYPES )
{
  using nt2::_; 
  nt2::table<T> y( nt2::of_size(5,3) );
  nt2::table<T> sy; 
  nt2::table<T> sy1, sy2, sy3;


  for(int j=1;j<=size(y, 2);j++)
    for(int i=1;i<=size(y, 1);i++)
      y(i,j) = nt2::sqr(i + j);
  nt2::functor<nt2::tag::multiplies_> f; 
  NT2_DISP(y); 

   sy = nt2::adjfun(f, y);
  NT2_DISP(sy); 
  sy1 = nt2::adjfun(f, y, 1);
  NT2_DISP(sy1); 
  sy2 = nt2::adjfun(f, y, 2); 
  NT2_DISP(sy2); 
  sy3 = nt2::adjfun(f, y, 3); 
  NT2_DISP(sy3); 




}

