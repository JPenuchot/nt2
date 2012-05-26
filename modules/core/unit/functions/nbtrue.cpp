/*******************************************************************************
 *         Copyright 2003 & onward LASMEA UMR 6602 CNRS/Univ. Clermont II
 *         Copyright 2009 & onward LRI    UMR 8623 CNRS/Univ Paris Sud XI
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/
#define NT2_UNIT_MODULE "nt2::nbtrue function"

#include <nt2/table.hpp>
#include <nt2/include/functions/toint.hpp>
#include <nt2/include/functions/of_size.hpp>
#include <nt2/include/functions/numel.hpp>
#include <nt2/include/functions/size.hpp>
#include <nt2/include/functions/nbtrue.hpp>
#include <nt2/include/functions/sb2b.hpp>
#include <nt2/include/functions/is_greater.hpp>
#include <nt2/include/functions/sb2b.hpp>
#include <nt2/include/functions/sum.hpp>
#include <nt2/include/constants/one.hpp>
#include <nt2/include/constants/zero.hpp>
#include <nt2/include/constants/ten.hpp>

#include <nt2/sdk/unit/module.hpp>
#include <nt2/sdk/unit/tests/basic.hpp>
#include <nt2/sdk/unit/tests/relation.hpp>
#include <nt2/sdk/unit/tests/type_expr.hpp>
#include <nt2/sdk/unit/tests/exceptions.hpp>
#include <nt2/sdk/details/type_id.hpp>

NT2_TEST_CASE_TPL( nbtrue_scalar, (float)(double))//NT2_TYPES )
{
  nt2::int32_t x = nt2::nbtrue(1);
  NT2_TEST_EQUAL( x, 1 );
  
  x = nt2::nbtrue(1,1);
  NT2_TEST_EQUAL( x, 1 );
  
  x = nt2::nbtrue(1,2);
  NT2_TEST_EQUAL( x, 1) ;
  
  x = nt2::nbtrue(0,2);
  NT2_TEST_EQUAL( x, 0) ;
  
}

NT2_TEST_CASE_TPL( nbtrue_expr, NT2_TYPES )
{
  using nt2::_;
  typedef typename nt2::meta::as_integer<T>::type itype_t; 
  nt2::table<T> y( nt2::of_size(5,3) );
  nt2::table<T> y1;
  nt2::table<itype_t> sy1, sy, sy2, sy3; 
  for(int j=1;j<=3;j++)
    for(int i=1;i<=5;i++)
      y(i,j) = i + 5*j;
  disp("y", y); 

//   nt2::table<nt2::logical<T> > l; 
//   l = gt(y, T(13));
//   disp("l ",  l);
//   nt2::table<itype_t > nb; 
//   nb = nt2::nbtrue(l); 
//   //  std::cout << nt2::type_id(nbtrue(l)) << std::endl; 
//   disp("nb ",  nbtrue(l));
//   disp("nb ",  nb);

  sy = nt2::nbtrue(y, 1);
  for(int j=1;j<=3;j++)
     NT2_TEST_EQUAL(nt2::size(y,1) , sy(j));
  NT2_DISP(sy); 
  std::cout <<  nt2::size(y,1) << std::endl; 

  sy = nt2::nbtrue(y, 2);
  for(int j=1;j<=5;j++)
    NT2_TEST_EQUAL(nt2::size(y,2) , sy(j));
  NT2_DISP(sy); 
  std::cout <<  nt2::size(y,2) << std::endl; 
  
  sy = nt2::nbtrue(y, 3);
  for(int j=1;j<=3;j++)
    for(int i=1;i<=5;i++)    
      NT2_TEST_EQUAL(nt2::size(y,3), sy(i, j));
  NT2_DISP(sy); 
  std::cout <<  nt2::size(y,3) << std::endl; 
  
  sy = nt2::nbtrue(y(_));
  NT2_DISP(sy); 
  NT2_TEST_EQUAL(sy(1), nt2::numel(y)); 
 }

