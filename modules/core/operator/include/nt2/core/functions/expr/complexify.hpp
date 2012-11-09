//==============================================================================
//         Copyright 2003 - 2011   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2011   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_CORE_FUNCTIONS_EXPR_COMPLEXIFY_HPP_INCLUDED
#define NT2_CORE_FUNCTIONS_EXPR_COMPLEXIFY_HPP_INCLUDED

#include <nt2/core/functions/complexify.hpp>
#include <nt2/include/functions/imag.hpp>
#include <nt2/sdk/complex/meta/as_complex.hpp>
#include <nt2/sdk/complex/meta/as_real.hpp>
#include <nt2/include/constants/zero.hpp>

namespace nt2 { namespace ext
{

  NT2_FUNCTOR_IMPLEMENTATION( nt2::tag::complexify_, tag::cpu_
                            , (A0)
                            , (generic_< fundamental_<A0> >)
                            )
  {
    typedef typename meta::as_complex<A0>::type result_type;
    result_type operator()(A0 const& a0) const
    {
      return result_type(a0);
    }
  };

  NT2_FUNCTOR_IMPLEMENTATION( nt2::tag::complexify_, tag::cpu_
                            , (A0)
                            , (generic_< complex_<fundamental_<A0> > > )
                            )
  {
    typedef A0 result_type;
    result_type operator()(A0 const& a0) const
    {
      return a0;
    }
  };

  NT2_FUNCTOR_IMPLEMENTATION( nt2::tag::complexify_, tag::cpu_
                            , (A0)
                            , (generic_< imaginary_< fundamental_<A0 > > >)
                            )
  {
    typedef typename meta::as_complex<A0>::type result_type;
    typedef typename meta::as_real<A0>::type r_type;
    result_type operator()(A0 const& a0) const
    {
      return result_type(Zero<r_type>(), imag(a0));
    }
  };

  NT2_FUNCTOR_IMPLEMENTATION( nt2::tag::complexify_, tag::cpu_
                            , (A0)
                            , (generic_< dry_< fundamental_<A0 > > >)
                            )
  {
    typedef typename meta::as_complex<A0>::type result_type;
    typedef typename meta::as_real<A0>::type r_type;
    result_type operator()(A0 const& a0) const
    {
      return result_type(real(a0));
    }
  };
} }

#endif
