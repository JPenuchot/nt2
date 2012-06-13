//==============================================================================
//         Copyright 2003 - 2011   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2011   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_CORE_FUNCTIONS_EXPR_LINE_HPP_INCLUDED
#define NT2_CORE_FUNCTIONS_EXPR_LINE_HPP_INCLUDED

#include <nt2/core/functions/line.hpp>
#include <nt2/core/container/dsl.hpp>

namespace nt2 { namespace ext
{
  NT2_FUNCTOR_IMPLEMENTATION( nt2::tag::line_, tag::cpu_,
                              (A0)(A1),
                              (ast_<A0>)
                              (scalar_<integer_<A1> > )
                              )
  {
    typedef typename A0::extent_type                extt_t;
    typedef typename meta::strip<extt_t>::type       ext_t;
    typedef typename  boost::proto::
                      result_of::make_expr< nt2::tag::line_
                                          , container::domain
                                          , A0 const&
                                          , size_t
                                          , size_t 
                                          , box<ext_t>
                                          >::type             result_type;
 
    BOOST_FORCEINLINE result_type operator()(A0 const& a0, A1 const& ind) const
    {
      size_t along = nt2::firstnonsingleton(a0)-1; 
      ext_t sizee;      
      for(int i=0; i < ext_t::size() ; ++i) sizee[i] = 1; 
      sizee[along] =  nt2::extent(a0)[along];  
      return boost::proto::make_expr<nt2::tag::line_, container::domain>
        (boost::cref(a0),along, size_t(ind), boxify(sizee));
    }
  };

  NT2_FUNCTOR_IMPLEMENTATION( nt2::tag::line_, tag::cpu_,
                              (A0)(A1)(A2),
                              (ast_<A0>)
                              (scalar_<arithmetic_<A1> > )
                              (scalar_<integer_<A2> > )
                              )
  {
    typedef typename A0::extent_type                extt_t;
    typedef typename meta::strip<extt_t>::type       ext_t;
    typedef typename  boost::proto::
                      result_of::make_expr< nt2::tag::line_
                                          , container::domain
                                          , A0 const&
                                          , size_t
                                          , size_t 
                                          , box<ext_t>
                                          >::type             result_type;

    BOOST_FORCEINLINE result_type operator()(A0 const& a0, A1 const& ind, A2 const& dim) const
    {
      size_t along = dim-1; 
      ext_t sizee;     
      for(int i=0; i < ext_t::size() ; ++i) sizee[i] = 1; 
      sizee[along] = nt2::extent(a0)[along]; 
      return boost::proto::make_expr<nt2::tag::line_, container::domain>
        (boost::cref(a0),along, size_t(ind), boxify(sizee));
    }
  };

} }

#endif
