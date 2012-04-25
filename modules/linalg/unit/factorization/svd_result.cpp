//==============================================================================
//         Copyright 2003 - 2012   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2012   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#define NT2_UNIT_MODULE "nt2 linalg toolbox - svd_result factorization"

#include <nt2/table.hpp>
#include <nt2/include/functions/zeros.hpp>
#include <nt2/include/functions/ones.hpp>
#include <nt2/include/functions/eye.hpp>
#include <nt2/include/functions/svd.hpp>
#include <nt2/include/functions/ldexp.hpp>

#include <nt2/sdk/unit/tests.hpp>
#include <nt2/sdk/unit/module.hpp>
#include <nt2/sdk/unit/tests/exceptions.hpp>

NT2_TEST_CASE_TPL(svd_result, NT2_REAL_TYPES)
{
  using nt2::_; 
  using nt2::tag::factorization::svd_;
  typedef typename nt2::meta::as_integer<T, signed>::type itype_t; 
  typedef nt2::table<T> t_t;
  typedef nt2::table<itype_t> it_t; 
  t_t b =       nt2::ones (4, 4, nt2::meta::as_<T>())
        + T(10)*nt2::eye  (4, 4, nt2::meta::as_<T>());

  nt2::details::svd_result<t_t> f(b);

  nt2::disp("b     ", b); 
  nt2::disp("values", f.values());
  t_t u  = f.u();
  t_t vt = f.vt();
  t_t w  = f.w(); 
  t_t sg = f.singular(); 
  nt2::disp("u    ", u);
  nt2::disp("vt    ", vt);
  nt2::disp("w    ", w);
  nt2::disp("sg   ", sg);
//   itype_t e; 
//   T m =  f.absdet(e); 
//   std::cout << "asbdet order " << e << std::endl;
//   std::cout << "asbdet mant  " << m << std::endl;
//   std::cout << "asbdet       " << nt2::ldexp(m, e) << std::endl;
//   std::cout << "asbdet       " << f.absdet()<< std::endl;
//   std::cout << "rank         " << f.rank()  << std::endl;
//   std::cout << "signdet      " << f.signdet()<< std::endl;
//   std::cout << "det          " << f.det()<< std::endl;
  }
