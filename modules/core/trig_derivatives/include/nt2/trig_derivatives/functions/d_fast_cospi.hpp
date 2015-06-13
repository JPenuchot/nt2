//==============================================================================
//         Copyright 2015 J.T. Lapreste
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_TRIG_DERIVATIVES_FUNCTIONS_D_FAST_COSPI_HPP_INCLUDED
#define NT2_TRIG_DERIVATIVES_FUNCTIONS_D_FAST_COSPI_HPP_INCLUDED
#include <nt2/include/functor.hpp>


namespace nt2 { namespace tag
  {
   /*!
     @brief d_fast_cospi generic tag

     Represents the d_fast_cospi function in generic contexts.

     @par Models:
        Hierarchy
   **/
    struct d_fast_cospi_ : ext::elementwise_<d_fast_cospi_>
    {
      /// @brief Parent hierarchy
      typedef ext::elementwise_<d_fast_cospi_> parent;
      template<class... Args>
      static BOOST_FORCEINLINE BOOST_AUTO_DECLTYPE dispatch(Args&&... args)
      BOOST_AUTO_DECLTYPE_BODY( dispatching_d_fast_cospi_( ext::adl_helper(), static_cast<Args&&>(args)... ) )
    };
  }
  namespace ext
  {
   template<class Site>
   BOOST_FORCEINLINE generic_dispatcher<tag::d_fast_cospi_, Site> dispatching_d_fast_cospi_(adl_helper, boost::dispatch::meta::unknown_<Site>, ...)
   {
     return generic_dispatcher<tag::d_fast_cospi_, Site>();
   }
   template<class... Args>
   struct impl_d_fast_cospi_;
  }
  /*!
    cosine of the angle in pi multiples, in the interval
    \f$[-1/4, 1/4]\f$, nan outside.

    @par Semantic:

    For every parameter of floating type T0

    @code
    T0 r = d_fast_cospi();
    @endcode

    is similar to:

    @code
    T0 r = cospi(x);
    @endcode

    provided that x belongs to \f$[-1/4, 1/4]\f$

    @see @funcref{fast_cos}, @funcref{sincospi}, @funcref{cos}, @funcref{cosd}
    @param a0

    @return a value of the same type as the parameter
  **/
  NT2_FUNCTION_IMPLEMENTATION(tag::d_fast_cospi_, d_fast_cospi, 1)
}

#endif

