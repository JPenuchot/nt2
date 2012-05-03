//==============================================================================
//         Copyright 2003 - 2012   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2012   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_CORE_CONTAINER_DSL_DETAILS_GENERATE_AS_HPP_INCLUDED
#define NT2_CORE_CONTAINER_DSL_DETAILS_GENERATE_AS_HPP_INCLUDED

namespace nt2 { namespace container { namespace ext
{
  //============================================================================
  // This is the factorized generator for all generator that just generate an
  // expression with the exact same semantic than one of their children.
  //============================================================================
  template<class Expr, int N> struct generate_as
  {
    typedef typename boost::proto::result_of::child_c<Expr,N>::type   expr_t;
    typedef typename boost::dispatch::meta::semantic_of<expr_t>::type sema_t;

    typedef expression< typename boost::remove_const<Expr>::type
                      , sema_t
                      >                                          result_type;

    BOOST_FORCEINLINE result_type operator()(Expr& e) const
    {
      return result_type(e);
    }
  };

  //============================================================================
  // This is the factorized size_of for all size_of that just generate an
  // expression with the exact same extent than one of their children.
  //============================================================================
  template<class Expr, int N> struct size_as
  {
    typedef typename boost::proto::result_of::child_c<Expr,N>::type expr_t;
    typedef typename expr_t::extent_type                            result_type;

    BOOST_FORCEINLINE result_type operator()(Expr& e) const
    {
      return boost::proto::child_c<N>(e).extent();
    }
  };

  //============================================================================
  // This is the factorized size_of for all size_of that just generate an
  // expression with the exact same extent than one of contained in a box child.
  //============================================================================
  template<class Expr, int N> struct boxed_size_of
  {
    // The size is contained in the first child : box<Size>
    typedef typename boost::proto::result_of::child_c<Expr&,N>::type seq_term;
    typedef typename boost::proto::result_of::value<seq_term>::type result_type;

    BOOST_FORCEINLINE result_type operator()(Expr& e) const
    {
      return boost::proto::value( boost::proto::child_c<N>(e) );
    }
  };
} } }

#endif
