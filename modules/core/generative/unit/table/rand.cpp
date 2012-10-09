/*******************************************************************************
 *         Copyright 2003 & onward LASMEA UMR 6602 CNRS/Univ. Clermont II
 *         Copyright 2009 & onward LRI    UMR 8623 CNRS/Univ Paris Sud XI
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/
#define NT2_UNIT_MODULE "nt2::rand function"

#include <nt2/table.hpp>
#include <nt2/include/functions/size.hpp>
#include <nt2/include/functions/rand.hpp>
#include <nt2/include/functions/abs.hpp>
#include <nt2/include/functions/mean.hpp>

#include <nt2/sdk/unit/module.hpp>
#include <nt2/sdk/unit/tests/basic.hpp>
#include <nt2/sdk/unit/tests/relation.hpp>
#include <nt2/sdk/unit/tests/exceptions.hpp>

NT2_TEST_CASE( rand_size )
{
  NT2_TEST_EQUAL( nt2::extent( nt2::rand(nt2::of_size(4,5)) ), nt2::of_size(4,5 ) );
  NT2_TEST_EQUAL( nt2::size( nt2::rand(nt2::of_size(4,5)), 1 ), 4u );
  NT2_TEST_EQUAL( nt2::size( nt2::rand(nt2::of_size(4,5)), 2 ), 5u );

  NT2_TEST_EQUAL( nt2::extent( nt2::rand(nt2::of_size(4,5,6)) ), nt2::of_size(4,5,6 ) );
  NT2_TEST_EQUAL( nt2::size( nt2::rand(nt2::of_size(4,5,6)), 1 ), 4u );
  NT2_TEST_EQUAL( nt2::size( nt2::rand(nt2::of_size(4,5,6)), 2 ), 5u );
  NT2_TEST_EQUAL( nt2::size( nt2::rand(nt2::of_size(4,5,6)), 3 ), 6u );

  NT2_TEST_EQUAL( nt2::extent( nt2::rand(nt2::of_size(4,5,6,7)) ), nt2::of_size(4,5,6,7 ) );
  NT2_TEST_EQUAL( nt2::size( nt2::rand(nt2::of_size(4,5,6,7)), 1 ), 4u );
  NT2_TEST_EQUAL( nt2::size( nt2::rand(nt2::of_size(4,5,6,7)), 2 ), 5u );
  NT2_TEST_EQUAL( nt2::size( nt2::rand(nt2::of_size(4,5,6,7)), 3 ), 6u );
  NT2_TEST_EQUAL( nt2::size( nt2::rand(nt2::of_size(4,5,6,7)), 4 ), 7u );
}

NT2_TEST_CASE( rand_nd_untyped )
{
  nt2::table<double> x1 = nt2::rand(1000);
  for(int i=1;i<=64;++i) NT2_TEST( x1(i) >= 0. && x1(i) < 1 );

  nt2::table<double> x2 = nt2::rand(8,8);
  for(int i=1;i<=64;++i) NT2_TEST( x2(i) >= 0. && x2(i) < 1 );

  nt2::table<double> x3 = nt2::rand(8,4,2);
  for(int i=1;i<=64;++i) NT2_TEST( x3(i) >= 0. && x3(i) < 1 );

  nt2::table<double> x4 = nt2::rand(4,4,2,2);
  for(int i=1;i<=64;++i) NT2_TEST( x4(i) >= 0. && x4(i) < 1 );
}

NT2_TEST_CASE_TPL( rand_nd_typed, NT2_REAL_TYPES )
{
  nt2::table<T> x1 = nt2::rand(8, nt2::meta::as_<T>() );
  for(int i=1;i<=64;++i) NT2_TEST( x1(i) >= T(0) && x1(i) < T(1) );

  nt2::table<T> x2 = nt2::rand(8,8, nt2::meta::as_<T>() );
  for(int i=1;i<=64;++i) NT2_TEST( x2(i) >= T(0) && x2(i) < T(1) );

  nt2::table<T> x3 = nt2::rand(8,4,2, nt2::meta::as_<T>() );
  for(int i=1;i<=64;++i) NT2_TEST( x3(i) >= T(0) && x3(i) < T(1) );

  nt2::table<T> x4 = nt2::rand(4,4,2,2, nt2::meta::as_<T>() );
  for(int i=1;i<=64;++i) NT2_TEST( x4(i) >= T(0) && x4(i) < T(1) );
}

NT2_TEST_CASE( rand_of_size )
{
  nt2::table<double> x2 = nt2::rand(nt2::of_size(8,8) );
  for(int i=1;i<=64;++i) NT2_TEST( x2(i) >= 0. && x2(i) < 1. );

  nt2::table<double> x3 = nt2::rand(nt2::of_size(8,4,2) );
  for(int i=1;i<=64;++i) NT2_TEST( x3(i) >= 0. && x3(i) < 1. );

  nt2::table<double> x4 = nt2::rand(nt2::of_size(4,4,2,2) );
  for(int i=1;i<=64;++i) NT2_TEST( x4(i) >= 0. && x4(i) < 1. );
}

NT2_TEST_CASE_TPL( rand_typed_of_size, NT2_REAL_TYPES )
{
  nt2::table<T> x2 = nt2::rand(nt2::of_size(8,8), nt2::meta::as_<T>() );
  for(int i=1;i<=64;++i) NT2_TEST( x2(i) >= T(0) && x2(i) < T(1) );

  nt2::table<T> x3 = nt2::rand(nt2::of_size(8,4,2), nt2::meta::as_<T>() );
  for(int i=1;i<=64;++i) NT2_TEST( x3(i) >= T(0) && x3(i) < T(1) );

  nt2::table<T> x4 = nt2::rand(nt2::of_size(4,4,2,2), nt2::meta::as_<T>() );
  for(int i=1;i<=64;++i) NT2_TEST( x4(i) >= T(0) && x4(i) < T(1) );
}

NT2_TEST_CASE( rand_expr )
{
  nt2::table<int> a( nt2::of_size(4,5) );

  nt2::table<double> x1 = nt2::rand( nt2::size(a) );
  for(int i=1;i<=20;++i) NT2_TEST( x1(i) >= 0. && x1(i) < 1. );

  NT2_TEST_ASSERT( x1 = nt2::rand(a) );
}

NT2_TEST_CASE_TPL( rand_typed_expr, NT2_REAL_TYPES )
{
  nt2::table<int> a( nt2::of_size(4,5) );

  nt2::table<T> x1 = nt2::rand( nt2::size(a), nt2::meta::as_<T>() );
  for(int i=1;i<=20;++i) NT2_TEST( x1(i) >= 0. && x1(i) < 1. );

  NT2_TEST_ASSERT( x1 = nt2::rand(a, nt2::meta::as_<T>()) );
}
