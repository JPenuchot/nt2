//==============================================================================
//         Copyright 2003 - 2011   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2011   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_CORE_FUNCTIONS_COMMON_ISULPEQUAL_HPP_INCLUDED
#define NT2_CORE_FUNCTIONS_COMMON_ISULPEQUAL_HPP_INCLUDED

#include <nt2/core/functions/isulpequal.hpp>
#include <nt2/include/functions/numel.hpp>
#include <nt2/include/functions/extent.hpp>
#include <nt2/include/functions/havesamesize.hpp>
#include <nt2/include/functions/all.hpp>
#include <nt2/include/functions/is_equal.hpp>
#include <nt2/include/functions/logical_and.hpp>
#include <nt2/include/functions/size.hpp>
#include <nt2/include/functions/max.hpp>
#include <nt2/include/functions/ulpdist.hpp>

namespace nt2 { namespace ext
{
  NT2_FUNCTOR_IMPLEMENTATION( nt2::tag::isulpequal_, tag::cpu_
                            , (A0)(A1)
                            , (scalar_<fundamental_<A0> >)
                              (scalar_<fundamental_<A1> >)
                            )
  {
    typedef bool result_type;

    BOOST_DISPATCH_FORCE_INLINE
    result_type operator()(const A0& a0, const A1& a1) const
    {
      return nt2::ulpdist(a0, a1) <= Half<A0>();
    }
  };

  NT2_FUNCTOR_IMPLEMENTATION( nt2::tag::isulpequal_, tag::cpu_
                            , (A0)(A1)
                            , (ast_<A0>)
                              (ast_<A1>)
                            )
  {
    typedef bool result_type;

    BOOST_DISPATCH_FORCE_INLINE
    result_type operator()(const A0& a0, const A1& a1) const
    {
      typedef typename A0::value_type value_type; 
      if(isempty(a0) && isempty(a1))  return true;
      if(!havesamesize(a0, a1))       return false;

      return nt2::max(nt2::ulpdist(a0, a1)(_))(1) <=  Half<value_type>();
    }
  };

  NT2_FUNCTOR_IMPLEMENTATION( nt2::tag::isulpequal_, tag::cpu_
                            , (A0)(A1)(A2)
                            , (scalar_<fundamental_<A0> >)
                              (scalar_<fundamental_<A1> >)
                              (scalar_<floating_<A2> >)
                            )
  {
    typedef bool result_type;

    BOOST_DISPATCH_FORCE_INLINE
    result_type operator()(const A0& a0, const A1& a1, const A2& a2) const
    {
      return nt2::ulpdist(a0, a1) <= a2;
    }
  };

  NT2_FUNCTOR_IMPLEMENTATION( nt2::tag::isulpequal_, tag::cpu_
                            , (A0)(A1)(A2)
                            , (ast_<A0>)
                              (ast_<A1>)
                              (scalar_<floating_<A2> >)
                              )
  {
    typedef bool result_type;

    BOOST_DISPATCH_FORCE_INLINE
    result_type operator()(const A0& a0, const A1& a1, const A2& a2) const
    {
      typedef typename A0::value_type value_type; 
      if(isempty(a0) && isempty(a1))  return true;
      if(!havesamesize(a0, a1))       return false;

      return nt2::max( nt2::ulpdist(a0, a1)(_))(1) <= a2;
    }
  };


} }

#endif
