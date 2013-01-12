//==============================================================================
//         Copyright 2003 - 2012   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2012   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_TOOLBOX_INTEGRATION_OPTIONS_HPP_INCLUDED
#define NT2_TOOLBOX_INTEGRATION_OPTIONS_HPP_INCLUDED

#include <cstddef>
#include <nt2/sdk/option/options.hpp>
#include <nt2/sdk/meta/as_real.hpp>
#include <nt2/include/constants/sqrteps.hpp>
#include <nt2/core/container/table/table.hpp>
#include <nt2/sdk/complex/meta/is_complex.hpp>

namespace nt2
{
  /**
   * Named parameter for passing number of iterations to iterative algortihms
   **/
//  NT2_REGISTER_PARAMETERS(iterations_);
  /**
   * Named parameter for passing number of function evaluation to iterative algortihms
   **/
//  NT2_REGISTER_PARAMETERS(evaluations_);
  namespace range
  {
    /**
     * Named parameter for passing intermediate points
     **/
    NT2_REGISTER_PARAMETERS(waypoints_);

    /**
     * Named parameter for passing intermediate points
     **/
    NT2_REGISTER_PARAMETERS(return_waypoints_);
    /**
     * Named parameter for passing singularity of first point
     **/
    NT2_REGISTER_PARAMETERS(singular_a_);

    /**
     * Named parameter for passing singularity of last point
     **/
    NT2_REGISTER_PARAMETERS(singular_b_);
  }


  namespace tolerance
  {
    /**
     * Named parameter for passing absolute tolerance to iterative algorithms
     **/
    NT2_REGISTER_PARAMETERS(abstol_);

    /**
     * Named parameter for passing relative tolerance to iterative algorithms
     **/
    NT2_REGISTER_PARAMETERS(reltol_);
  }

  namespace limits
  {
    /**
     * Named parameter for passing absolute tolerance to iterative algorithms
     **/
    NT2_REGISTER_PARAMETERS(maxintvcnt_);

    /**
     * Named parameter for passing relative tolerance to iterative algorithms
     **/
    NT2_REGISTER_PARAMETERS(maxfunccnt_);
  }

  // Integ params are the global default parameters for all integrtion routines
  // Each routine as quad,  quadl,  etc. can specialize to provide better
  // default behaviour
  template<typename T, typename V, typename TAG = void> struct integ_params
  {

    typedef V                                                            value_t;
    typedef T                                                            input_t;
    typedef typename meta::as_real<T>::type                               real_t;
    typedef typename meta::as_logical<real_t>                             bool_t;
    typedef container::table<value_t>                                     vtab_t;
    typedef container::table<input_t>                                     itab_t;
    typedef container::table<real_t>                                      rtab_t;
    typedef container::table<bool>                                        btab_t;
    static real_t abstol()          { return nt2::Sqrteps<real_t>();     }
    static real_t reltol()          { return nt2::Sqrteps<real_t>();     }
    static std::size_t maxfunccnt() { return 10000;                      }
    static std::size_t maxintvcnt() { return   650;                      }
    static itab_t waypoints()       { return itab_t(of_size(1, 0));      }
    static bool singular_a()        { return false;                      }
    static bool singular_b()        { return false;                      }
    static bool return_waypoints()  { return false;                      }
  };

//   // integ params for complex < T >  are those of T.
//   template<class T, template < class > class V, typename TAG>
//   struct integ_params < V<T>, TAG> :
//   integ_params<T, TAG>{};

}

namespace nt2 { namespace details
{
  // INTERNAL ONLY
  // integration_settings gather the classical set of settings required
  // for an integration process from either the list of values or from an
  // options pack expressions
  template<typename T, typename V, typename TAG = void> struct integration_settings
  {
    typedef V                                                            value_t;
    typedef T                                                            input_t;
    typedef typename meta::as_real<T>::type                               real_t;
    typedef typename meta::as_logical<real_t>                             bool_t;
    typedef container::table<value_t>                                     vtab_t;
    typedef container::table<input_t>                                     itab_t;
    typedef container::table<real_t>                                      rtab_t;
    typedef nt2::integ_params<input_t, value_t, TAG>                          ip;
    typedef typename meta::is_complex<value_t>::type                 v_is_cplx_t;
    typedef typename boost::mpl::if_<v_is_cplx_t,value_t,input_t>::type result_t;
    typedef container::table<result_t>                                  restab_t;

    integration_settings ( std::size_t        mfc = ip::maxfunccnt()
                           , std::size_t      mic = ip::maxintvcnt()
                           , real_t            at = ip::abstol()
                           , real_t            rt = ip::reltol()
                           , itab_t const&    wpt = ip::waypoints()
                           , bool             sga = ip::singular_a()
                           , bool             sgb = ip::singular_b()
                           , bool             rwp = ip::return_waypoints()
      )
      : maxfunccnt(mfc)
      , maxintvcnt(mic)
      , abstol(at)
      , reltol(rt)
      , waypoints(wpt)
      , singular_a(sga)
      , singular_b(sgb)
      , return_waypoints(rwp)
    {}

    template<class Expr>
    integration_settings ( nt2::details::option_expr<Expr> const& x)
      : maxfunccnt(x(nt2::limits::maxfunccnt_, ip::maxfunccnt() ))
      , maxintvcnt(x(nt2::limits::maxintvcnt_, ip::maxintvcnt() ))
      , abstol    (x(nt2::tolerance::abstol_,  ip::abstol()     ))
      , reltol    (x(nt2::tolerance::reltol_,  ip::reltol()     ))
      , waypoints (x(nt2::range::waypoints_,   ip::waypoints()  ))
      , singular_a(x(nt2::range::singular_a_,  ip::singular_a() ))
      , singular_b(x(nt2::range::singular_b_,  ip::singular_b() ))
      , return_waypoints(x(nt2::range::return_waypoints_,  ip::return_waypoints() ))

    {}
    void display_options() const
    {
      std::cout << "maxfunccnt "<< maxfunccnt << std::endl;
      std::cout << "maxintvcnt "<< maxintvcnt << std::endl;
      std::cout << "    abstol "<<     abstol << std::endl;
      std::cout << "    reltol "<<     reltol << std::endl;
      std::cout << " waypoints "<< waypoints  << std::endl;
      std::cout << "singular_a "<< singular_a << std::endl;
      std::cout << "singular_b "<< singular_b << std::endl;
      std::cout << "returnwp   "<< return_waypoints << std::endl;

    }
    std::size_t      maxfunccnt;
    std::size_t      maxintvcnt;
    real_t               abstol;
    real_t               reltol;
    itab_t            waypoints;
    bool             singular_a;
    bool             singular_b;
    bool       return_waypoints;
  };
} }

#endif
