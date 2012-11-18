//==============================================================================
//         Copyright 2003 - 2011   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2011   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_TOOLBOX_COMPLEX_CONSTANTS_GENERIC_VALMIN_HPP_INCLUDED
#define NT2_TOOLBOX_COMPLEX_CONSTANTS_GENERIC_VALMIN_HPP_INCLUDED

#include <nt2/include/constants/zero.hpp>
#include <nt2/include/constants/valmin.hpp>
#include <nt2/sdk/complex/imaginary.hpp>
#include <iostream>
#include <nt2/table.hpp>

namespace nt2 { namespace ext
{
  NT2_FUNCTOR_IMPLEMENTATION ( boost::simd::tag::Valmin, tag::cpu_, (A0)
                             , ((target_< generic_< imaginary_< arithmetic_<A0> > > >))
                             )
  {
    typedef typename A0::type result_type;
    BOOST_FORCEINLINE result_type operator()(A0 const&) const
    {
      return Zero<result_type>();
    }
  };

  NT2_FUNCTOR_IMPLEMENTATION ( boost::simd::tag::Valmin, tag::cpu_, (A0)
                             , ((target_< generic_< complex_< arithmetic_<A0> > > >))
                             )
  {
    typedef typename A0::type result_type;
    BOOST_FORCEINLINE result_type operator()(A0 const&) const
    {
      return Zero<result_type>();
    }
  };
} }

#endif
