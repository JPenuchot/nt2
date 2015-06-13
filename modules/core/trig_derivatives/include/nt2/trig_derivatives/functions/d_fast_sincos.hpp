//==============================================================================
//         Copyright 2015 J.T. Lapreste
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================

#ifndef NT2_TRIG_DERIVATIVES_FUNCTIONS_D_FAST_SINCOS_HPP_INCLUDED
#define NT2_TRIG_DERIVATIVES_FUNCTIONS_D_FAST_SINCOS_HPP_INCLUDED
#include <nt2/include/functor.hpp>

namespace nt2
{
  namespace tag
  {
   /*!
     @brief d_fast_sincos generic tag

     Represents the d_fast_sincos function in generic contexts.

     @par Models:
        Hierarchy
   **/
    struct d_fast_sincos_ : ext::elementwise_<d_fast_sincos_>
    {
      /// @brief Parent hierarchy
      typedef ext::elementwise_<d_fast_sincos_> parent;
      template<class... Args>
      static BOOST_FORCEINLINE BOOST_AUTO_DECLTYPE dispatch(Args&&... args)
      BOOST_AUTO_DECLTYPE_BODY( dispatching_d_fast_sincos_( ext::adl_helper(), static_cast<Args&&>(args)... ) )
    };
  }
  namespace ext
  {
   template<class Site>
   BOOST_FORCEINLINE generic_dispatcher<tag::d_fast_sincos_, Site> dispatching_d_fast_sincos_(adl_helper, boost::dispatch::meta::unknown_<Site>, ...)
   {
     return generic_dispatcher<tag::d_fast_sincos_, Site>();
   }
   template<class... Args>
   struct impl_d_fast_sincos_;
  }
  /*!
    Computes simultaneously sine and cosine  in the interval
    \f$[-\pi/4, \pi/4]\f$, nan outside.

    @par Semantic:

    For every parameter of floating type T0

    @code
    T0 c, s;
    tie(s, c) = d_fast_sincos(x);
    @endcode

    is similar to:

    @code
    T0 c = fast_cos(x);
    T0 s = fast_sin(x);
    @endcode

    @see @funcref{sine}, @funcref{fast_sin}, @funcref{sincos}, @funcref{fast_cos}
    @param a0 input

    @return a pair of value of the same type as the parameter
  **/
  NT2_FUNCTION_IMPLEMENTATION(tag::d_fast_sincos_, d_fast_sincos, 1)
  /*!
    Computes simultaneously sine and cosine  in the interval
    \f$[-\pi/4, \pi/4]\f$, nan outside.

    @par Semantic:

    For every parameter of floating type T0

    @code
    T0 c, s;
    s = d_fast_sincos(x, c);
    @endcode

    is similar to:

    @code
    T0 c = fast_cos(x);
    T0 s = fast_sin(x);
    @endcode

    @see @funcref{sine}, @funcref{fast_sin}, @funcref{sincos}, @funcref{fast_cos}
    @param a0 input
    @param a1 L-Value that will receive the cosine of a0
    @return the sine of a0

  **/
  NT2_FUNCTION_IMPLEMENTATION_TPL(tag::d_fast_sincos_, d_fast_sincos,(A0 const&)(A1&),2)
  /*!
    Computes simultaneously sine and cosine  in the interval
    \f$[-\pi/4, \pi/4]\f$, nan outside.

    @par Semantic:

    For every parameter of floating type T0

    @code
    T0 c, s;
    d_fast_sincos(x, s, c);
    @endcode

    is similar to:

    @code
    T0 c = fast_cos(x);
    T0 s = fast_sin(x);
    @endcode

    @see @funcref{sine}, @funcref{fast_sin}, @funcref{sincos}, @funcref{fast_cos}
    @param a0 Value to decompose
    @param a1 L-Value that will receive the sine of @c a0
    @param a2 L-Value that will receive the cosine of @c a0

  **/
  NT2_FUNCTION_IMPLEMENTATION_TPL(tag::d_fast_sincos_, d_fast_sincos,(A0 const&)(A1&)(A2&),3)
}

#endif

