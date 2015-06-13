//==============================================================================
//         Copyright 2015 J.T. Lapreste
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_TRIG_DERIVATIVES_FUNCTIONS_D_FAST_SINCOSD_HPP_INCLUDED
#define NT2_TRIG_DERIVATIVES_FUNCTIONS_D_FAST_SINCOSD_HPP_INCLUDED
#include <nt2/include/functor.hpp>


namespace nt2 { namespace tag
  {
   /*!
     @brief d_fast_sincosd generic tag

     Represents the d_fast_sincosd function in generic contexts.

     @par Models:
        Hierarchy
   **/
    struct d_fast_sincosd_ : ext::elementwise_<d_fast_sincosd_>
    {
      /// @brief Parent hierarchy
      typedef ext::elementwise_<d_fast_sincosd_> parent;
      template<class... Args>
      static BOOST_FORCEINLINE BOOST_AUTO_DECLTYPE dispatch(Args&&... args)
      BOOST_AUTO_DECLTYPE_BODY( dispatching_d_fast_sincosd_( ext::adl_helper(), static_cast<Args&&>(args)... ) )
    };
  }
  namespace ext
  {
   template<class Site>
   BOOST_FORCEINLINE generic_dispatcher<tag::d_fast_sincosd_, Site> dispatching_d_fast_sincosd_(adl_helper, boost::dispatch::meta::unknown_<Site>, ...)
   {
     return generic_dispatcher<tag::d_fast_sincosd_, Site>();
   }
   template<class... Args>
   struct impl_d_fast_sincosd_;
  }
  /*!
    Computes simultaneously sine and cosine from angle in degree
    in the interval \f$[-45, 45]\f$, nan outside.

    @par Semantic:

    For every parameter of floating type T0

    @code
    T0 c, s;
    tie(s, c) = d_fast_sincosd(x);
    @endcode

    is similar to:

    @code
    T0 c = fast_cosd(x);
    T0 s = fast_sind(x);
    @endcode

    @param a0

    @see @funcref{fast_sind}, @funcref{sincosd}, @funcref{fast_cosd}
    @return a pair of value of the same type as the parameter
  **/
  NT2_FUNCTION_IMPLEMENTATION(tag::d_fast_sincosd_, d_fast_sincosd, 1)
  /*!
    Computes simultaneously sine and cosine from angle in degree
    in the interval \f$[-45, 45]\f$, nan outside.

    @par Semantic:

    For every parameter of floating type T0

    @code
    T0 c, s;
    s = d_fast_sincosd(x, c);
    @endcode

    is similar to:

    @code
    T0 c = fast_cosd(x);
    T0 s = fast_sind(x);
    @endcode

    @see @funcref{fast_sind}, @funcref{sincosd}, @funcref{fast_cosd}
    @param a0
    @param a1 L-Value that will receive the cosine of a0
    @return the sine of a0

  **/

  NT2_FUNCTION_IMPLEMENTATION_TPL(tag::d_fast_sincosd_, d_fast_sincosd,(A0 const&)(A1&),2)
  /*!
    Computes simultaneously sine and cosine from angle in degree
    in the interval \f$[-45, 45]\f$, nan outside.

    @par Semantic:

    For every parameter of floating type T0

    @code
    T0 c, s;
    d_fast_sincosd(x, s, c);
    @endcode

    is similar to:

    @code
    T0 c = fast_cosd(x);
    T0 s = fast_sind(x);
    @endcode

    @see @funcref{fast_sind}, @funcref{sincosd}, @funcref{fast_cosd}
    @param a0 input in degree
    @param a1 L-Value that will receive the sine of a0
    @param a2 L-Value that will receive the cosine of a0

    @return a pair of value of the same type as the parameter
  **/

  NT2_FUNCTION_IMPLEMENTATION_TPL(tag::d_fast_sincosd_, d_fast_sincosd,(A0 const&)(A1&)(A2&),3)
}

#endif

