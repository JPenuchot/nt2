//==============================================================================
//         Copyright 2015 J.T. Lapreste
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_TRIG_DERIVATIVES_FUNCTIONS_D_FAST_TANPI_HPP_INCLUDED
#define NT2_TRIG_DERIVATIVES_FUNCTIONS_D_FAST_TANPI_HPP_INCLUDED
#include <nt2/include/functor.hpp>


namespace nt2 { namespace tag
  {
   /*!
     @brief d_fast_tanpi generic tag

     Represents the d_fast_tanpi function in generic contexts.

     @par Models:
        Hierarchy
   **/
    struct d_fast_tanpi_ : ext::elementwise_<d_fast_tanpi_>
    {
      /// @brief Parent hierarchy
      typedef ext::elementwise_<d_fast_tanpi_> parent;
      template<class... Args>
      static BOOST_FORCEINLINE BOOST_AUTO_DECLTYPE dispatch(Args&&... args)
      BOOST_AUTO_DECLTYPE_BODY( dispatching_d_fast_tanpi_( ext::adl_helper(), static_cast<Args&&>(args)... ) )
    };
  }
  namespace ext
  {
   template<class Site>
   BOOST_FORCEINLINE generic_dispatcher<tag::d_fast_tanpi_, Site> dispatching_d_fast_tanpi_(adl_helper, boost::dispatch::meta::unknown_<Site>, ...)
   {
     return generic_dispatcher<tag::d_fast_tanpi_, Site>();
   }
   template<class... Args>
   struct impl_d_fast_tanpi_;
  }
  /*!
    tangent of the angle in pi multiples, in the interval \f$[-1/4, 1/4]\f$, nan outside.

    @par Semantic:

    For every parameter of floating type T0

    @code
    T0 r = d_fast_tanpi(x);
    @endcode

    is similar to:

    @code
    T0 r = tanpi(x);
    @endcode

    @see  @funcref{tanpi}, @funcref{proper_tanpi}, @funcref{tan}
    @param a0

    @return a value of the same type as the parameter
  **/
  NT2_FUNCTION_IMPLEMENTATION(tag::d_fast_tanpi_, d_fast_tanpi, 1)
}

#endif

