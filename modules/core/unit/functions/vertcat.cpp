/*******************************************************************************
 *         Copyright 2003 & onward LASMEA UMR 6602 CNRS/Univ. Clermont II
 *         Copyright 2009 & onward LRI    UMR 8623 CNRS/Univ Paris Sud XI
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/
#define NT2_UNIT_MODULE "nt2::vertcat function"

#include <nt2/table.hpp>
#include <nt2/include/functions/size.hpp>
#include <nt2/include/functions/vertcat.hpp>
#include <nt2/include/functions/rif.hpp>
#include <nt2/include/functions/cif.hpp>  

#include <nt2/sdk/unit/module.hpp>
#include <nt2/sdk/unit/tests/basic.hpp>
#include <nt2/sdk/unit/tests/relation.hpp>
#include <nt2/sdk/unit/tests/type_expr.hpp>
#include <nt2/sdk/unit/tests/exceptions.hpp>

NT2_TEST_CASE( vertcat_scalar )
{
  nt2::table<int> c = nt2::vertcat(1, 2.5); 
  NT2_DISP(c); 
  nt2::table<float> d = nt2::vertcat(1.3f, 3); 
  NT2_DISP(d); 
  nt2::table<double> e = nt2::vertcat(1.5); 
  NT2_DISP(e);
  double f = nt2::vertcat(1.5);
  std::cout << f << std::endl; 
}
NT2_TEST_CASE( vertcat_size )
{
  typedef short int T; 
  nt2::table<T> a = nt2::rif(nt2::of_size(3, 3), nt2::meta::as_<T>());
  nt2::table<T> b = nt2::cif(nt2::of_size(5, 3), nt2::meta::as_<T>());
  NT2_DISP(a); 
  NT2_DISP(b); 
  nt2::table<T> c = vertcat(a); 
  NT2_DISP(c); 
  nt2::table<T> d = vertcat(a, b); 
  NT2_DISP(d); 
}
NT2_TEST_CASE( vertcat_size2 )
{
  typedef short int T; 
  nt2::table<T> a = nt2::rif(nt2::of_size(5, 7), nt2::meta::as_<T>());
  nt2::table<T> b = nt2::cif(nt2::of_size(9, 7), nt2::meta::as_<T>());
  NT2_DISP(a); 
  NT2_DISP(b); 
  nt2::table<T> c = vertcat(a); 
  NT2_DISP(c); 
  nt2::table<T> d = vertcat(a, b); 
  NT2_DISP(d); 
}
