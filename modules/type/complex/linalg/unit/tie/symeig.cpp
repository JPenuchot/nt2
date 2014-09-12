//==============================================================================
//         Copyright 2003 - 2012   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2012   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#define NT2_UNIT_MODULE "nt2 linalg toolbox - symeig factorization"

#include <nt2/table.hpp>
#include <nt2/include/functions/zeros.hpp>
#include <nt2/include/functions/ones.hpp>
#include <nt2/include/functions/eye.hpp>
#include <nt2/include/functions/symeig.hpp>
#include <nt2/include/functions/mtimes.hpp>
#include <nt2/include/functions/ctranspose.hpp>
#include <nt2/include/functions/globalmax.hpp>
#include <nt2/include/functions/ulpdist.hpp>
#include <nt2/include/functions/triu.hpp>
#include <nt2/include/functions/tril.hpp>

#include <nt2/sdk/unit/tests.hpp>
#include <nt2/sdk/unit/module.hpp>
#include <nt2/sdk/unit/tests/exceptions.hpp>


NT2_TEST_CASE_TPL ( symeig, NT2_REAL_TYPES)
{
  typedef std::complex<T>  cT;
  typedef nt2::table<cT> ct_t;
  typedef nt2::table<T>   t_t;

  ct_t v, d, b = nt2::ones(4, 4, nt2::meta::as_<T>())
                + T(10)*nt2::eye(4, 4, nt2::meta::as_<T>());
  t_t w;
  NT2_DISPLAY(b);
  w = nt2::symeig(b);
  NT2_DISPLAY(w);

  nt2::tie(d, v) = nt2::symeig(b);
  NT2_DISPLAY(v);
  NT2_DISPLAY(d);
  NT2_DISPLAY(b);
  NT2_TEST_ULP_EQUAL(b, nt2::mtimes(v, nt2::mtimes(d, nt2::ctrans(v))), 10.0);

}
NT2_TEST_CASE_TPL ( symeig_m_test, NT2_REAL_TYPES)
{
  typedef std::complex<T> cT;
  typedef nt2::table<cT> ct_t;
  typedef nt2::table<T>   t_t;
  ct_t v, z;
  t_t d;
  T bb[9] = {-149,    -50,   -154,
             537,    180,    546,
             -27,     -9,    -25 };
  //Note that only the upper part is used!
  ct_t b(nt2::of_size(3, 3));
  int k = 0;
  for(int i=1; i <= 3; ++i)
    {
      for(int j=1; j <= 3; ++j)
        {
          b(i, j) = bb[k++];
        }
    }
  NT2_DISPLAY(b);
  nt2::tie(d, v) = nt2::symeig(b);
  NT2_DISPLAY(v);
  NT2_DISPLAY(d);
  z =  mtimes(mtimes(v, d), nt2::trans(v));
  NT2_DISPLAY(z);
  std::cout <<        nt2::globalmax(nt2::ulpdist(nt2::triu(b), nt2::triu(z))) << std::endl;
  ct_t zz = nt2::triu(z);
  ct_t bbb= nt2::triu(b);
  NT2_DISPLAY(zz);
  NT2_DISPLAY(bbb);
  NT2_TEST_ULP_EQUAL(nt2::triu(z), nt2::triu(b), T(16.0));
}
