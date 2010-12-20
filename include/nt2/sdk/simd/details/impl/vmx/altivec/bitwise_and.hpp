/*******************************************************************************
 *         Copyright 2003 & onward LASMEA UMR 6602 CNRS/Univ. Clermont II
 *         Copyright 2009 & onward LRI    UMR 8623 CNRS/Univ Paris Sud XI
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/
#ifndef NT2_SDK_SIMD_DETAILS_IMPL_VMX_ALTIVEC_BITWISE_AND_HPP_INCLUDED
#define NT2_SDK_SIMD_DETAILS_IMPL_VMX_ALTIVEC_BITWISE_AND_HPP_INCLUDED

#include <nt2/sdk/meta/as_integer.hpp>
#include <nt2/sdk/simd/native_cast.hpp>

////////////////////////////////////////////////////////////////////////////////
// Overload registration
////////////////////////////////////////////////////////////////////////////////
NT2_REGISTER_DISPATCH ( tag::bitwise_and_, tag::cpu_, (A0)(A1)
                      , ((simd_<type32_<A0>,tag::altivec_>))
                        ((simd_<type32_<A1>,tag::altivec_>))
                      );

NT2_REGISTER_DISPATCH ( tag::bitwise_and_, tag::cpu_, (A0)(A1)
                      , ((simd_<type16_<A0>,tag::altivec_>))
                        ((simd_<type16_<A1>,tag::altivec_>))
                      );

NT2_REGISTER_DISPATCH ( tag::bitwise_and_, tag::cpu_, (A0)(A1)
                      , ((simd_<type8_<A0>,tag::altivec_>))
                        ((simd_<type8_<A1>,tag::altivec_>))
                      );

////////////////////////////////////////////////////////////////////////////////
// Overloads implementation
////////////////////////////////////////////////////////////////////////////////
namespace nt2 { namespace ext
{
  //////////////////////////////////////////////////////////////////////////////
  // The 8 bits version holds the generic code used by all other
  //////////////////////////////////////////////////////////////////////////////
  template<class Dummy>
  struct  call< tag::bitwise_and_ ( tag::simd_(tag::type8_,tag::altivec_)
                                  , tag::simd_(tag::type8_,tag::altivec_)
                                  )
              , tag::cpu_, Dummy
              >
        : callable
  {
    template<class Sig>           struct result;
    template<class This,class A0,class A1>
    struct result<This(A0,A1)> : meta::strip<A0> {};

    NT2_FUNCTOR_CALL(2)
    {
      A0 other = simd::native_cast<A0>( a1 );
      A0 that = { vec_and(a0(),other()) };
      return that;
    }
  };

  //////////////////////////////////////////////////////////////////////////////
  // All other versions (16,32,64) forward their calls to the 8 bits version
  //////////////////////////////////////////////////////////////////////////////
  template<class Dummy>
  struct  call< tag::bitwise_and_ ( tag::simd_(tag::type16_,tag::altivec_)
                                  , tag::simd_(tag::type16_,tag::altivec_)
                                  )
              , tag::cpu_, Dummy
              >
        : call< tag::bitwise_and_ ( tag::simd_(tag::type8_,tag::altivec_)
                                  , tag::simd_(tag::type8_,tag::altivec_)
                                  )
              , tag::cpu_, Dummy
              >
  {};

  template<class Dummy>
  struct  call< tag::bitwise_and_ ( tag::simd_(tag::type32_,tag::altivec_)
                                  , tag::simd_(tag::type32_,tag::altivec_)
                                  )
              , tag::cpu_, Dummy
              >
        : call< tag::bitwise_and_ ( tag::simd_(tag::type8_,tag::altivec_)
                                  , tag::simd_(tag::type8_,tag::altivec_)
                                  )
              , tag::cpu_, Dummy
              >
  {};
} }

#endif
