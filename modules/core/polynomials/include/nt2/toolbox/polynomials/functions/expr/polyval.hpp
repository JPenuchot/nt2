//==============================================================================
//         Copyright 2003 - 2011 LASMEA UMR 6602 CNRS/Univ. Clermont II         
//         Copyright 2009 - 2011 LRI    UMR 8623 CNRS/Univ Paris Sud XI         
//                                                                              
//          Distributed under the Boost Software License, Version 1.0.          
//                 See accompanying file LICENSE.txt or copy at                 
//                     http://www.boost.org/LICENSE_1_0.txt                     
//==============================================================================
#ifndef NT2_TOOLBOX_POLYNOMIALS_FUNCTIONS_EXPR_POLYVAL_HPP_INCLUDED
#define NT2_TOOLBOX_POLYNOMIALS_FUNCTIONS_EXPR_POLYVAL_HPP_INCLUDED
#include <nt2/toolbox/polynomials/functions/polyval.hpp>
#include <nt2/include/functions/scalar/fma.hpp>
#include <nt2/include/functions/isempty.hpp>
#include <nt2/include/functions/zeros.hpp>
#include <nt2/include/functions/repnum.hpp>
#include <nt2/include/functions/vandermonde.hpp>
#include <nt2/include/functions/sqrt.hpp>
#include <nt2/include/constants/zero.hpp>
#include <nt2/include/functions/infs.hpp>
#include <nt2/include/functions/tr_solve.hpp>
#include <nt2/toolbox/polynomials/category.hpp>
#include <nt2/sdk/meta/fusion.hpp>
#include <boost/fusion/adapted/array.hpp>
#include <nt2/include/functions/tie.hpp>
#include <nt2/include/functions/trans.hpp>
#include <nt2/sdk/error/warning.hpp>

/////////////////////////////////////////////////////////////////////////////
// Implementation when type A0 is arithmetic_
/////////////////////////////////////////////////////////////////////////////
namespace nt2 { namespace ext
{
  NT2_FUNCTOR_IMPLEMENTATION( nt2::tag::polyval_, tag::cpu_
                            , (A0)(A1)
                            , (unspecified_<A0>)(scalar_<floating_<A1> > )
                            )
  {

    typedef typename A0::value_type value_type;
    typedef A1 result_type; 
    NT2_FUNCTOR_CALL(2)
    {
      if (isempty(a0)) return Zero<A1>(); 
      A1 ans = a0(1);
      for(size_t i = 2; i <= numel(a0); ++i)
      {
        ans = fma(ans, a1, a0(i));
      }
      return ans;
    }
  };


  //============================================================================
  // This version of balance is called whenever a tie(...) = balance(...) is captured
  // before assign is resolved. As a tieable function, balance retrieves rhs/lhs
  // pair as inputs
  //============================================================================
  NT2_FUNCTOR_IMPLEMENTATION( nt2::tag::polyval_, tag::cpu_
                              , (A0)(N0)(A1)(N1)
                              , ((node_<A0, nt2::tag::polyval_, N0, nt2::container::domain>))
                              ((node_<A1, nt2::tag::tie_ , N1, nt2::container::domain>))
                            )
  {
    typedef void                                                    result_type;
    typedef typename boost::proto::result_of::child_c<A1&,0>::type       v_type;
    typedef typename boost::proto::result_of::child_c<A0&,0>::type       p_type; 
    typedef typename boost::proto::result_of::child_c<A0&,1>::type       x_type; 
    typedef typename meta::strip<v_type>::type                              v_t;
    typedef typename v_t::value_type                                 value_type; 

    BOOST_FORCEINLINE result_type operator()( A0& a0, A1& a1 ) const
    {
      // Copy data in output first
      v_type& v = boost::proto::child_c<0>(a1);
      v.resize(a0.extent());
      val(a0, a1, N1(), N0());
    }

  private:
    template < class S,  class T > BOOST_FORCEINLINE
      void compute_val(const S& p, const T& x, v_type & v)const
    {
      if (isempty(p))
      {
        v =  nt2::zeros(size(x));
      }
      else
      {
        v = repnum(p(1), size(x));
        for(size_t i = 2; i <= numel(p); ++i)
        {
          v = fma(v, x, p(i));
        }
      }
    }

    template < class S,  class T, class U,  class V> BOOST_FORCEINLINE
      void compute_delta(const S& x, const T& r,
                         const U& df,  const V& normr,
                         const size_t & nc,
                         table<value_type>& delta)const
    {
//         NT2_WARNING(df == 0, "zero degree of freedom implies infinite error bounds."); 
        BOOST_AUTO_TPL(vnd, nt2::vandermonde(x, nc));
        BOOST_AUTO_TPL(err, nt2::tr_solve(nt2::trans(r), nt2::trans(vnd), 'L', 'N', 'N'));
        value_type fact =   (normr/nt2::sqrt(value_type(df)));
        delta(nt2::_) =nt2::sqrt(oneplus(sum(sqr(err),1)))*fact;
    }
    template < class T > BOOST_FORCEINLINE
      void val(A0& a0, A1& a1,
               boost::mpl::long_<1> const &, const T&) const
    {
      p_type & p = boost::proto::child_c<0>(a0);
      x_type & x = boost::proto::child_c<1>(a0);
      v_type & v = boost::proto::child_c<0>(a1);
      compute_val(p, x, v); 
    }

    BOOST_FORCEINLINE
    void val(A0& a0, A1& a1,
             boost::mpl::long_<1> const &, boost::mpl::long_<3> const &) const
    {
      typedef typename boost::proto::result_of::child_c<A0&,2>::type       mu_type;
      p_type & p = boost::proto::child_c<0>(a0);
      x_type & x = boost::proto::child_c<1>(a0);
      mu_type & mu= boost::proto::child_c<2>(a0);
      v_type & v = boost::proto::child_c<0>(a1);
      compute_val(p, (x-mu(1))/mu(2), v);       
    }    
    BOOST_FORCEINLINE
    void val(A0& a0, A1& a1,
             boost::mpl::long_<1> const &, boost::mpl::long_<6> const &) const
    {
      typedef typename boost::proto::result_of::child_c<A0&,5>::type       mu_type;
      p_type & p = boost::proto::child_c<0>(a0);
      x_type & x = boost::proto::child_c<1>(a0);
      mu_type & mu= boost::proto::child_c<5>(a0);
      v_type & v = boost::proto::child_c<0>(a1);
      compute_val(p, (x-mu(1))/mu(2), v);       
    }
    BOOST_FORCEINLINE
    void val(A0& a0, A1& a1,
             boost::mpl::long_<2> const &, boost::mpl::long_<5> const &) const
    {
      typedef typename boost::proto::result_of::child_c<A0&,2>::type        r_type;
      typedef typename boost::proto::result_of::child_c<A0&,3>::type       df_type;
      typedef typename boost::proto::result_of::child_c<A0&,4>::type       nr_type;
      typedef typename boost::proto::result_of::child_c<A1&,1>::type    delta_type; 
      p_type & p = boost::proto::child_c<0>(a0);
      x_type & x = boost::proto::child_c<1>(a0);
      r_type & r = boost::proto::child_c<2>(a0);
      df_type & df= boost::proto::child_c<3>(a0);
      nr_type & nr= boost::proto::child_c<4>(a0);
      v_type & v = boost::proto::child_c<0>(a1);
      compute_val(p, x, v);       
      table<value_type> delta(extent(x)); 
      std::cout << size(x) << std::endl;
      std::cout << size(delta)<< std::endl;
      compute_delta(x, r, df, nr, nt2::numel(p), delta);
      boost::proto::child_c<1>(a1) = delta; 
    }
    BOOST_FORCEINLINE
    void val(A0& a0, A1& a1,
             boost::mpl::long_<2> const &, boost::mpl::long_<6> const &) const
    {
      typedef typename boost::proto::result_of::child_c<A0&,2>::type        r_type;
      typedef typename boost::proto::result_of::child_c<A0&,3>::type       df_type;
      typedef typename boost::proto::result_of::child_c<A0&,4>::type       nr_type;
      typedef typename boost::proto::result_of::child_c<A0&,5>::type       mu_type;
      typedef typename boost::proto::result_of::child_c<A1&,1>::type    delta_type; 
      p_type & p = boost::proto::child_c<0>(a0);
      x_type & x = boost::proto::child_c<1>(a0);
      r_type & r = boost::proto::child_c<2>(a0);
      df_type & df= boost::proto::child_c<3>(a0);
      nr_type & nr= boost::proto::child_c<4>(a0);
      mu_type & mu= boost::proto::child_c<5>(a0);
      v_type & v = boost::proto::child_c<0>(a1);
      BOOST_AUTO_TPL(xred, (x-mu(1))/mu(2)); 
      compute_val(p, xred, v);       
      table<value_type> delta(of_size(extent(x)));
      std::cout << size(x) << std::endl;
      std::cout << size(delta)<< std::endl;
      delta.resize(extent(x)); 
      compute_delta(xred, r, df, nr, nt2::numel(p), delta);
      boost::proto::child_c<1>(a1) = delta;
    }

  };  
} }


#endif
