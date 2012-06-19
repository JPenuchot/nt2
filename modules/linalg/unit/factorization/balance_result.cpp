//==============================================================================
//         Copyright 2003 - 2012   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2012   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#define NT2_UNIT_MODULE "nt2 linalg toolbox - balance_result factorization"

#include <nt2/table.hpp>
#include <nt2/include/functions/zeros.hpp>
#include <nt2/include/functions/ones.hpp>
#include <nt2/include/functions/eye.hpp>
#include <nt2/include/functions/balance.hpp>
#include <nt2/include/functions/ldexp.hpp>
#include <nt2/include/functions/repnum.hpp>
#include <nt2/include/functions/rif.hpp>
#include <nt2/include/functions/cif.hpp>

#include <nt2/sdk/unit/tests.hpp>
#include <nt2/sdk/unit/module.hpp>
#include <nt2/sdk/unit/tests/exceptions.hpp>

NT2_TEST_CASE_TPL(balance_result, (double))// NT2_REAL_TYPES)
{
  using nt2::_;
  using nt2::tag::factorization::balance_;
  typedef typename nt2::meta::as_integer<T, signed>::type itype_t;
  typedef nt2::table<T> t_t;
  typedef nt2::table<itype_t> it_t;
  T bc[25] =  {
    1.0, 2.0, 0., 0., 0.,
    0. , 2.0, 0., 0., 0.,
    1.,  100.,  10000., 4., 0.2,
    .01,  1.,  100., 0.2, 0.1,
    .0001,  .01,  1., 0., 0.000
  };

  int k = 0;
  t_t b(nt2::of_size(5, 5));
  for(int i=1; i <= 5; i++)
    {
      for(int j=1; j <= 5; j++)
        {
          b(i, j) = bc[k++];
        }

    }
  nt2::details::balance_result<t_t> f(b, 'b');
  nt2::display("b     ", b);
  nt2::display("values", f.values());
  t_t s  = f.scale();
  nt2::display("s    ", s);
  std::cout << "ilo " << f.ilo() << std::endl;
  std::cout << "ihi " << f.ihi() << std::endl;
  t_t t  = f.t();
  nt2::display("t    ", t);
  it_t ipiv  = f.ipiv();
  nt2::display("ipiv    ", ipiv);
  t_t invt  = f.invt();
  nt2::display("invt    ", invt);

}
