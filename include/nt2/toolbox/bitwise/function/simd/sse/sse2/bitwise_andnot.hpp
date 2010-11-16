//////////////////////////////////////////////////////////////////////////////
///   Copyright 2003 and onward LASMEA UMR 6602 CNRS/U.B.P Clermont-Ferrand
///   Copyright 2009 and onward LRI    UMR 8623 CNRS/Univ Paris Sud XI
///
///          Distributed under the Boost Software License, Version 1.0
///                 See accompanying file LICENSE.txt or copy at
///                     http://www.boost.org/LICENSE_1_0.txt
//////////////////////////////////////////////////////////////////////////////
#ifndef NT2_TOOLBOX_BITWISE_FUNCTION_SIMD_SSE_SSE2_BITWISE_ANDNOT_HPP_INCLUDED
#define NT2_TOOLBOX_BITWISE_FUNCTION_SIMD_SSE_SSE2_BITWISE_ANDNOT_HPP_INCLUDED
#include <nt2/sdk/meta/strip.hpp>


namespace nt2 { namespace functors
{
  //  no special validate for bitwise_andnot


  /////////////////////////////////////////////////////////////////////////////
  // Implementation when type A0 is double
  /////////////////////////////////////////////////////////////////////////////
  template<class Info>
  struct call<bitwise_andnot_,tag::simd_(tag::arithmetic_,tag::sse_),double,Info> : callable
  {
    template<class Sig> struct result;
    template<class This,class A0,class A1>
    struct result<This(A0,A1)> : meta::strip<A0>{};//

    NT2_FUNCTOR_CALL(2)
    {
      A0 that = {_mm_andnot_pd(simd::native_cast<A0>(a1), a0)};
      return that;
    }
  };


  /////////////////////////////////////////////////////////////////////////////
  // Implementation when type A0 is float
  /////////////////////////////////////////////////////////////////////////////
  template<class Info>
  struct call<bitwise_andnot_,tag::simd_(tag::arithmetic_,tag::sse_),float,Info> : callable
  {
    template<class Sig> struct result;
    template<class This,class A0,class A1>
    struct result<This(A0,A1)> : meta::strip<A0>{};//

    NT2_FUNCTOR_CALL(2) 
    {
      A0 that = {_mm_andnot_ps(simd::native_cast<A0>(a1), a0)};
      return that;
    }
  };


  /////////////////////////////////////////////////////////////////////////////
  // Implementation when type A0 is arithmetic_
  /////////////////////////////////////////////////////////////////////////////
  template<class Info>
  struct call<bitwise_andnot_,tag::simd_(tag::arithmetic_,tag::sse_),arithmetic_,Info> : callable
  {
    template<class Sig> struct result;
    template<class This,class A0,class A1>
    struct result<This(A0,A1)> : meta::strip<A0>{};//

    NT2_FUNCTOR_CALL(2)
    {
      A0 that = {_mm_andnot_si128(simd::native_cast<A0>(a1), a0)};
      return that;
    }
  };

} }

#endif
/// Revised by jt the 15/11/2010
