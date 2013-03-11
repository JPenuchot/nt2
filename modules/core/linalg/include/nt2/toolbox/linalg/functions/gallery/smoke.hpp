/*******************************************************************************
 *         Copyright 2003-2012 LASMEA UMR 6602 CNRS/U.B.P
 *         Copyright 2011-2012 LRI    UMR 8623 CNRS/Univ Pasmoke Sud XI
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/
#ifndef NT2_TOOLBOX_LINALG_FUNCTIONS_GALLERY_SMOKE_HPP_INCLUDED
#define NT2_TOOLBOX_LINALG_FUNCTIONS_GALLERY_SMOKE_HPP_INCLUDED
#include <nt2/toolbox/linalg/functions/smoke.hpp>
#include <nt2/include/functions/horzcat.hpp>
#include <nt2/include/functions/from_diag.hpp>
#include <nt2/include/functions/cumprod.hpp>
#include <nt2/include/functions/expipi.hpp>
#include <nt2/include/functions/repnum.hpp>
#include <nt2/include/functions/ones.hpp>
#include <nt2/include/functions/is_eqz.hpp>
#include <nt2/include/functions/deltaf.hpp>
#include <nt2/include/functions/if_else.hpp>
namespace nt2
{

  namespace ext
  {

    NT2_FUNCTOR_IMPLEMENTATION( nt2::tag::smoke_, tag::cpu_,
                                (A0)(T),
                                (scalar_<integer_<A0> >)
                                (target_<scalar_<floating_<T> > > )
                              )
    {
      typedef typename T::type t_t;
      BOOST_DISPATCH_RETURNS(2, (A0 const& n, T const& t),
                             (nt2::from_diag(nt2::cumprod(nt2::repnum( nt2::expipi(Two<t_t>()/t_t(n)),nt2::of_size(1, n))))+
                              nt2::from_diag(nt2::ones(n-1, 1, T()), 1)+nt2::deltaf(n, 1, nt2::of_size(n, n), T())
                             )
                            )
        };
   NT2_FUNCTOR_IMPLEMENTATION( nt2::tag::smoke_, tag::cpu_,
                                (A0)(A1)(T),
                                (scalar_<integer_<A0> >)
                                (scalar_<integer_<A1> >)
                                (target_<scalar_<floating_<T> > > )
                              )
   {
     typedef typename T::type t_t;
     BOOST_DISPATCH_RETURNS(3, (A0 const& n, A1 const& k,T const& t),
                            (nt2::if_else(nt2::is_eqz(t_t(k)),
                                          nt2::smoke(n, T())-nt2::deltaf(n, 1, nt2::of_size(n, n), T()),
                                          nt2::smoke(n, T()))
                            )
                           )
       };
  }

  meta::call<tag::smoke_(const ptrdiff_t &, meta::as_<double> const &)>::type
  smoke(ptrdiff_t n)
  {
    return nt2::smoke(n,  meta::as_<double>());
  }
  template<class T>
  typename meta::call<tag::smoke_(const ptrdiff_t &, typename meta::as_<T> const &)>::type
  smoke(ptrdiff_t n)
  {
    return nt2::smoke(n,  meta::as_<T>());
  }
  meta::call<tag::smoke_(const ptrdiff_t &, const ptrdiff_t &, meta::as_<double> const &)>::type
  smoke(ptrdiff_t n, ptrdiff_t k)
  {
    return nt2::smoke(n, k, meta::as_<double>());
  }
  template<class T>
  typename meta::call<tag::smoke_(const ptrdiff_t &, const ptrdiff_t &, typename meta::as_<T> const &)>::type
  smoke(ptrdiff_t n, ptrdiff_t k)
  {
    return nt2::smoke(n, k, meta::as_<T>());
  }

}


#endif
