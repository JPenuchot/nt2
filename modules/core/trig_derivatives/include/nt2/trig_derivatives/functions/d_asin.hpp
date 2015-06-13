//==============================================================================
//         Copyright 2015 J.T. Lapreste
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_TRIG_DERIVATIVES_FUNCTIONS_D_ASIN_HPP_INCLUDED
#define NT2_TRIG_DERIVATIVES_FUNCTIONS_D_ASIN_HPP_INCLUDED
#include <nt2/include/functor.hpp>


namespace nt2 { namespace tag
  {
   /*!
     @brief d_asin generic tag

     Represents the d_asin function in generic contexts.

     @par Models:
        Hierarchy
   **/
    struct d_asin_ : ext::elementwise_<d_asin_>
    {
      /// @brief Parent hierarchy
      typedef ext::elementwise_<d_asin_> parent;
      template<class... Args>
      static BOOST_FORCEINLINE BOOST_AUTO_DECLTYPE dispatch(Args&&... args)
      BOOST_AUTO_DECLTYPE_BODY( dispatching_d_asin_( ext::adl_helper(), static_cast<Args&&>(args)... ) )
    };
  }
  namespace ext
  {
   template<class Site>
   BOOST_FORCEINLINE generic_dispatcher<tag::d_asin_, Site> dispatching_d_asin_(adl_helper, boost::dispatch::meta::unknown_<Site>, ...)
   {
     return generic_dispatcher<tag::d_asin_, Site>();
   }
   template<class... Args>
   struct impl_d_asin_;
  }
  /*!
    inverse sine.

    @par Semantic:

    For every parameter of floating type T0

    @code
    T0 r = d_asin(a0);
    @endcode

    Returns the arc @c r in the interval
    \f$[-\pi/2, \pi/2]\f$ such that <tt>sin(r) == x</tt>.
    If @c x is outside \f$[-1, 1]\f$ the result is Nan.

    @see @funcref{d_asind}, @funcref{d_asinpi}
    @param a0

    @return a value of the same type as the parameter
  **/
  NT2_FUNCTION_IMPLEMENTATION(tag::d_asin_, d_asin, 1)
}

#endif

