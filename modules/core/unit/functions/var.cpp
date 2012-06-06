/*******************************************************************************
 *         Copyright 2003 & onward LASMEA UMR 6602 CNRS/Univ. Clermont II
 *         Copyright 2009 & onward LRI    UMR 8623 CNRS/Univ Paris Sud XI
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/
#define NT2_UNIT_MODULE "nt2::var function"

#include <nt2/table.hpp>
#include <nt2/include/functions/var.hpp>
#include <nt2/include/functions/sum.hpp>
#include <nt2/include/functions/size.hpp>
#include <nt2/include/functions/firstnonsingleton.hpp>

#include <nt2/sdk/unit/module.hpp>
#include <nt2/sdk/unit/tests/relation.hpp>

NT2_TEST_CASE_TPL( var_scalar, (float)(double))//NT2_TYPES )
{
  T x = nt2::var(T(42));
  NT2_TEST_EQUAL( x, T(42) );

  x = nt2::var(T(42),1);
  NT2_TEST_EQUAL( x, T(42) );

  x = nt2::var(T(42),0);
  NT2_TEST_EQUAL( x, T(42) );

}

NT2_TEST_CASE_TPL( var, (float)(double))//NT2_TYPES )
{
  nt2::table<T> y( nt2::of_size(5,3) );
  nt2::table<T> sy;
  nt2::table<T> sy2;
  
  
  for(int j=1;j<=3;j++)
    for(int i=1;i<=5;i++)
      y(i,j) = i + 10*j;
  disp("y", y); 
  sy = nt2::sum(y)/T(nt2::size(y, nt2::firstnonsingleton(y)));
  sy2 = nt2::var(y);
  disp("sy2", sy2);
  for(int j=1;j<=size(sy, 2);j++)
    for(int i=1;i<=size(sy, 1);i++)
      NT2_TEST_EQUAL(sy(i,j), sy2(i, j));
  
  sy = nt2::sum(y)/T(nt2::size(y, 1)); 
  sy2 = nt2::var(y, 1);
  disp("sy2", sy2);
  for(int j=1;j<=size(sy, 2);j++)
    for(int i=1;i<=size(sy, 1);i++)
      NT2_TEST_EQUAL(sy(i,j), sy2(i, j));
  
  sy = nt2::sum(y)/T(nt2::size(y, 2)); 
  sy2 = nt2::var(y, 2);
  disp("sy2", sy2);
  for(int j=1;j<=size(sy, 2);j++)
    for(int i=1;i<=size(sy, 1);i++)
      NT2_TEST_EQUAL(sy(i,j), sy2(i, j));
  
  sy = nt2::sum(y)/T(nt2::size(y, 3)); 
  sy2 = nt2::var(y, 3);
  disp("sy2", sy2);
  for(int j=1;j<=size(sy, 2);j++)
    for(int i=1;i<=size(sy, 1);i++)
      NT2_TEST_EQUAL(sy(i,j), sy2(i, j)); 
  
  
}

