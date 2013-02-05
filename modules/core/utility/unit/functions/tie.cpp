/*******************************************************************************
 *         Copyright 2003 & onward LASMEA UMR 6602 CNRS/Univ. Clermont II
 *         Copyright 2009 & onward LRI    UMR 8623 CNRS/Univ Paris Sud XI
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/
#define NT2_UNIT_MODULE "nt2 function tie"

#include <nt2/table.hpp>
#include <nt2/include/functions/tie.hpp>

#include <nt2/sdk/unit/module.hpp>
#include <nt2/sdk/unit/tests/relation.hpp>

NT2_TEST_CASE( extent )
{
  using nt2::table;
  using nt2::of_size_;
  using boost::mpl::_;

  table<double> d;
  table<float>  f;
  table<short>  s;
  table<char>   c;

  NT2_TEST_EQUAL( nt2::tie(d)       , d.extent() );
  NT2_TEST_EQUAL( nt2::tie(f,d)     , f.extent() );
  NT2_TEST_EQUAL( nt2::tie(f,d,s)   , f.extent() );
  NT2_TEST_EQUAL( nt2::tie(c,f,d,s) , c.extent() );
}
