//==============================================================================
//         Copyright 2015 J.T. Lapreste
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
/*!
  @file
  @brief Definition of d_sinecosine function
**/

#ifndef NT2_TRIG_DERIVATIVES_FUNCTIONS_D_SINECOSINE_HPP_INCLUDED
#define NT2_TRIG_DERIVATIVES_FUNCTIONS_D_SINECOSINE_HPP_INCLUDED
#include <nt2/include/functor.hpp>

namespace nt2 { namespace tag
  {

    template < class T > struct d_sinecosine_ : ext::elementwise_< d_sinecosine_<T> >
    {
      /// @brief Parent hierarchy
      typedef ext::elementwise_< d_sinecosine_<T> > parent;
      template<class... Args>
      static BOOST_FORCEINLINE BOOST_AUTO_DECLTYPE dispatch(Args&&... args)
      BOOST_AUTO_DECLTYPE_HEADER( dispatching( ext::adl_helper(), *(d_sinecosine_*)0, static_cast<Args&&>(args)... ) )
      {
        return dispatching( ext::adl_helper(), d_sinecosine_(), static_cast<Args&&>(args)... );
      }
    };
  }
  /*!
    @brief d_sinecosine

    @c d_sinecosine compute simultaneously the  templatized sine and cosine of the input

    @see @funcref{fast_sincos}, @funcref{sincosine}, @funcref{sincosd}, @funcref{sincos}
    @param a0 angle in radian

    @return A Fusion Sequence containing the  templatized sine and cosine of @c a0
  **/

  NT2_FUNCTION_IMPLEMENTATION_TPL(tag::d_sinecosine_<A0>, d_sinecosine, (A1 const&), 2)


  /*!
    @brief d_sinecosine

    @c d_sinecosine compute simultaneously the templatized sine and cosine of the input

    @see @funcref{fast_sincos}, @funcref{sincosine}, @funcref{sincosd}, @funcref{sincos}
    @param a0 angle in radian
    @param a1 L-Value that will receive the  templatized sine of @c a0

    @return A Fusion Sequence containing the  templatized cosine of @c a0
  **/

    NT2_FUNCTION_IMPLEMENTATION_TPL(tag::d_sinecosine_<A0>, d_sinecosine, (A1 const&)(A2&),3)

  /*!
    @brief  d_sinecosine

    @c d_sinecosine compute simultaneously the templatized sine and cosine of the input

    @see @funcref{fast_sincos}, @funcref{sincosine}, @funcref{sincosd}, @funcref{sincos}
    @param a0 angle in radian
    @param a1 L-Value that will receive the templatized sined of @c a0
    @param a2 L-Value that will receive the templatized cosine of @c a0

  **/
    NT2_FUNCTION_IMPLEMENTATION_TPL(tag::d_sinecosine_<A0>, d_sinecosine, (A1 const&)(A2&)(A2&),3)


    }

#endif
