//==============================================================================
//         Copyright 2015 J.T. Lapreste
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_TRIG_DERIVATIVES_FUNCTIONS_D_SIND_HPP_INCLUDED
#define NT2_TRIG_DERIVATIVES_FUNCTIONS_D_SIND_HPP_INCLUDED
#include <nt2/include/functor.hpp>


namespace nt2 { namespace tag
  {
   /*!
     @brief d_sind generic tag

     Represents the d_sind function in generic contexts.

     @par Models:
        Hierarchy
   **/
    struct d_sind_ : ext::elementwise_<d_sind_>
    {
      /// @brief Parent hierarchy
      typedef ext::elementwise_<d_sind_> parent;
      template<class... Args>
      static BOOST_FORCEINLINE BOOST_AUTO_DECLTYPE dispatch(Args&&... args)
      BOOST_AUTO_DECLTYPE_BODY( dispatching_d_sind_( ext::adl_helper(), static_cast<Args&&>(args)... ) )
    };
  }
  namespace ext
  {
   template<class Site>
   BOOST_FORCEINLINE generic_dispatcher<tag::d_sind_, Site> dispatching_d_sind_(adl_helper, boost::dispatch::meta::unknown_<Site>, ...)
   {
     return generic_dispatcher<tag::d_sind_, Site>();
   }
   template<class... Args>
   struct impl_d_sind_;
  }
  /*!
    sine of the angle in degree.

    @par Semantic:

    For every parameter of floating type T0

    @code
    T0 r = d_sind(x);
    @endcode

    is similar to:

    @code
    T0 r = sin(Deginrad<T0>()*x);
    @endcode

    @see @funcref{sine}, @funcref{fast_sin}, @funcref{sincosd}, @funcref{sin}, @funcref{sinpi}
    @param a0

    @return a value of the same type as the parameter
  **/
  NT2_FUNCTION_IMPLEMENTATION(tag::d_sind_, d_sind, 1)
}

#endif

