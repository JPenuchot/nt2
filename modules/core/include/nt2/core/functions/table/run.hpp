//==============================================================================
//         Copyright 2003 - 2011   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2011   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_CORE_FUNCTIONS_TABLE_RUN_HPP_INCLUDED
#define NT2_CORE_FUNCTIONS_TABLE_RUN_HPP_INCLUDED

#include <nt2/include/functor.hpp>
#include <nt2/dsl/functions/run.hpp>
#include <nt2/include/functions/assign.hpp>
#include <nt2/include/functions/extent.hpp>
#include <nt2/core/container/table/table.hpp>
#include <nt2/core/container/meta/runner.hpp>
#include <boost/dispatch/meta/terminal_of.hpp>
#include <nt2/core/container/meta/loop_nest.hpp>
#include <nt2/core/container/meta/lhs_terminal.hpp>

namespace nt2 { namespace ext
{
  //============================================================================
  // When an assign(lhs,rhs) expression is run, we perform the evaluation of rhs
  // then store it in lhs. Depending on the lhs nature (real terminal or a node
  // containing a call to any indexing function, the result of the evaluation is
  // returned, usually as non-const reference.
  //============================================================================
  NT2_FUNCTOR_IMPLEMENTATION( nt2::tag::run_, tag::cpu_
                            , (A0)(S0)
                            , ((expr_<  table_< unspecified_<A0>, S0 >
                                     , nt2::container::domain
                                     , nt2::tag::assign_
                                     >
                              ))
                            )
  {
    typedef typename boost::dispatch::meta::
            terminal_of < typename boost::dispatch::meta::
                          semantic_of<A0>::type
                        >::type                             result_type;

    BOOST_DISPATCH_FORCE_INLINE result_type
    operator()(A0 const& a0) const
    {
      typename A0::index_type::type indices;
      meta::for_each(indices, extent(a0), meta::runner<A0 const&>(a0));
      return meta::lhs_terminal()(a0);
    }
  };

  //============================================================================
  // When an arbitrary expression is run, we perform its evaluation into a
  // local temporary container of proper type.This temporary is then returned by
  // value.
  //============================================================================
  NT2_FUNCTOR_IMPLEMENTATION( nt2::tag::run_, tag::cpu_
                            , (A0)(S0)
                            , ((ast_<table_< unspecified_<A0>, S0 > >))
                            )
  {
    typedef typename boost::
    remove_reference< typename boost::dispatch::meta::
                      terminal_of< typename boost::dispatch::meta::
                                   semantic_of<A0>::type
                                 >::type
                    >::type                                result_type;

    BOOST_DISPATCH_FORCE_INLINE result_type
    operator()(A0 const& a0) const
    {
      result_type tmp;
      run(assign(tmp, a0));
      return tmp;
    }
  };
} }

#endif
