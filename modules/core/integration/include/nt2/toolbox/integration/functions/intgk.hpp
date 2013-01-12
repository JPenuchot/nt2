/*******************************************************************************
 *         Copyright 2003-2012 LASMEA UMR 6602 CNRS/U.B.P
 *         Copyright 2011-2012 LRI    UMR 8623 CNRS/Univ Paris Sud XI
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/
#ifndef NT2_TOOLBOX_INTEGRATION_FUNCTIONS_INTGK_HPP_INCLUDED
#define NT2_TOOLBOX_INTEGRATION_FUNCTIONS_INTGK_HPP_INCLUDED

/*!
 * \file
 * \brief Defines and implements the nt2::intgk function
 */
#include <nt2/toolbox/integration/interface.hpp>

namespace nt2
{
  namespace tag
  {
    struct intgk_ : ext::unspecified_<intgk_>
    {
      typedef ext::unspecified_<intgk_> parent;
    };
    // definition  of abstol constant for intgk method
    BOOST_SIMD_CONSTANT_REGISTER( Intgkabstol, double
                                  , 0, 0x3727c5ac             //1.0e-5
                                  , 0x3ddb7cdfd9d7bdbbll      //1.0e-10
      );
    // definition  of reltol constant for intgk method
    BOOST_SIMD_CONSTANT_REGISTER( Intgkreltol, double
                                  , 0, 0x38d1b717             //1.0e-4
                                  , 0x3eb0c6f7a0b5ed8dll      //1.0e-6
      );
  }
  BOOST_SIMD_CONSTANT_IMPLEMENTATION(tag::Intgkabstol, Intgkabstol);
  BOOST_SIMD_CONSTANT_IMPLEMENTATION(tag::Intgkreltol, Intgkreltol);

  // specialization of abstol for intgk method
  template<class T, class V> struct integ_params<T, V, tag::intgk_>
  : integ_params<T, V, void>
  {
    typedef typename nt2::integ_params<T, V, void>::real_t real_t;
    static real_t        abstol(){return Intgkabstol<real_t>(); }
    static real_t        reltol(){return Intgkreltol<real_t>(); }
  };

  //============================================================================
  /*!
   * Apply intgk algorithm to integrate a function over a real interval
   *
   * \param func  Function to optimize
   * \param x    required points in the interval in ascending order
   *             (x can be replaced by 2 abscissae a and b)
   * \param opt   Options pack related to the tolerance handling
   *
   * \return  a tuple containing the results of the integration, the last error value,
   * the number of required function evaluation and a boolean
   * notifying success of the whole process.
   */
  //============================================================================

  template<class F, class X> BOOST_FORCEINLINE
  typename details::integration<F, X, tag::intgk_>::result_type
  intgk(F f, X x)
  {
    return details::integration<F, X, tag::intgk_>::call(f, x);
  }

  template<class F, class X, class Xpr> BOOST_FORCEINLINE
  typename details::integration<F, X, tag::intgk_>::result_type
  intgk(F f, X x, nt2::details::option_expr<Xpr> const& opt)
  {
    return details::integration<F, X, tag::intgk_>(f, x, opt);
  }

  template<class F, class A> BOOST_FORCEINLINE
  typename details::integration<F, typename details::h2_t<A>::ab_t, tag::intgk_>::result_type
  intgk(F f, A a, A b)
  {
    typedef typename details::h2_t<A>::ab_t ab_t;
    return details::integration<F, ab_t, tag::intgk_>::call(f, nt2::cath(a, b));
  }

  template<class F, class A, class Xpr> BOOST_FORCEINLINE
  typename details::integration<F, typename details::h2_t<A>::ab_t, tag::intgk_>::result_type
  intgk(F f, A a, A b, nt2::details::option_expr<Xpr> const& opt)
  {
    typedef typename details::h2_t<A>::ab_t ab_t;
    return details::integration<F, ab_t, tag::intgk_>::call(f, nt2::cath(a, b), opt);
  }

}

#endif

