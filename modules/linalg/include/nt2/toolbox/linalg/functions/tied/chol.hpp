//==============================================================================
//         Copyright 2003 - 2012   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2012   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_TOOLBOX_LINALG_FUNCTIONS_TIED_CHOL_HPP_INCLUDED
#define NT2_TOOLBOX_LINALG_FUNCTIONS_TIED_CHOL_HPP_INCLUDED

#include <nt2/toolbox/linalg/functions/chol.hpp>
#include <nt2/toolbox/linalg/functions/factorizations/chol.hpp>
#include <nt2/include/functions/assign.hpp>
#include <nt2/include/functions/tie.hpp>

namespace nt2 { namespace ext
{
  //============================================================================
  // This version of chol is called whenever a tie(a,p) = chol(...) is captured
  // before assign is resolved. As a tieable function, chol retrieves rhs/lhs
  // pair as inputs
  //============================================================================
  NT2_FUNCTOR_IMPLEMENTATION( nt2::tag::chol_, tag::cpu_
                            , (A0)(N0)(A1)(N1)
                            , ((node_<A0, nt2::tag::chol_, N0>))
                              ((node_<A1, nt2::tag::tie_ , N1>))
                            )
  {
    typedef void                                                    result_type;
    typedef typename boost::proto::result_of::child_c<A1&,0>::type  child0;
    typedef typename meta::strip<child0>::type                      dest_t;
    typedef typename meta::
            call< nt2::tag::
                  factorization::chol_(dest_t&,char,nt2::details::in_place_)
                >::type                                             fact_t;

    BOOST_FORCEINLINE result_type operator()( A0& a0, A1& a1 ) const
    {
      // Retrieve the up/lo options
      char ul = uplo(a0, N0() );

      // Copy data in output first
      boost::proto::child_c<0>(a1) = boost::proto::child_c<0>(a0);

      // Factorize in place
      fact_t f = factorization::chol(boost::proto::child_c<0>(a1),ul,in_place_);

      // Retrieve correct version with minimum amount of (re)allocation
      if(ul == 'U') boost::proto::child_c<0>(a1) = f.upper_result();
      else          boost::proto::child_c<0>(a1) = f.lower_result();

      // Retrieve status
      status(a1, N1(), f.status());
    }

    private:
    //==========================================================================
    // INTERNAL ONLY
    // Extract a 'U' or a 'L' from lower_/upper_ or mpl::void_
    //==========================================================================
    BOOST_FORCEINLINE char uplo(A0 const&, boost::mpl::long_<1> const&) const
    {
      return 'U';
    }

    BOOST_FORCEINLINE char uplo(A0 const& in, boost::mpl::long_<2> const&) const
    {
      return boost::proto::value(boost::proto::child_c<1>(in));
    }

    //==========================================================================
    // INTERNAL ONLY
    // If nargout is 2, fill the status
    //==========================================================================
    template<class Status> BOOST_FORCEINLINE
    void status(A1&, boost::mpl::long_<1> const&, Status const& ) const {}

    template<class Status> BOOST_FORCEINLINE
    void status(A1& out, boost::mpl::long_<2> const&, Status const& s) const
    {
      boost::proto::value(boost::proto::child_c<1>(out)) = s;
    }
  };
} }

#endif
