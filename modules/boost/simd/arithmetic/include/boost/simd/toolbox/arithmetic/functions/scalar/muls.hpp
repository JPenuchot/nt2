//==============================================================================
//         Copyright 2003 - 2011 LASMEA UMR 6602 CNRS/Univ. Clermont II         
//         Copyright 2009 - 2011 LRI    UMR 8623 CNRS/Univ Paris Sud XI         
//                                                                              
//          Distributed under the Boost Software License, Version 1.0.          
//                 See accompanying file LICENSE.txt or copy at                 
//                     http://www.boost.org/LICENSE_1_0.txt                     
//==============================================================================
#ifndef BOOST_SIMD_TOOLBOX_ARITHMETIC_FUNCTIONS_SCALAR_MULS_HPP_INCLUDED
#define BOOST_SIMD_TOOLBOX_ARITHMETIC_FUNCTIONS_SCALAR_MULS_HPP_INCLUDED
#include <boost/simd/sdk/simd/logical.hpp>
#include <boost/simd/include/functions/genmask.hpp>
#include <boost/simd/include/functions/adds.hpp>
#include <boost/simd/include/functions/abs.hpp>
#include <boost/simd/include/functions/is_ltz.hpp>
#include <boost/simd/include/functions/bitofsign.hpp>
#include <boost/simd/include/functions/saturate.hpp>
#include <boost/dispatch/meta/downgrade.hpp>
#include <boost/simd/include/constants/valmin.hpp>
#include <boost/simd/include/constants/valmax.hpp>
#include <boost/simd/include/constants/zero.hpp>
#
namespace boost { namespace simd { namespace ext
{
  BOOST_SIMD_FUNCTOR_IMPLEMENTATION( boost::simd::tag::muls_, tag::cpu_
                            , (A0)
                            , (scalar_< unsigned_<A0> >)(scalar_< unsigned_<A0> >)
                            )
  {
    typedef A0 result_type;
    BOOST_SIMD_FUNCTOR_CALL_REPEAT(2)
    {
      typedef typename dispatch::meta::upgrade<A0>::type utype; 
      utype res = utype(a0)*utype(a1);
      return static_cast<A0>(res) | genmask(static_cast<A0>(res >> sizeof(A0)*8));       
    }
  };

  BOOST_SIMD_FUNCTOR_IMPLEMENTATION( boost::simd::tag::muls_, tag::cpu_
                            , (A0)
                            , (scalar_< integer_<A0> >)(scalar_< integer_<A0> >)
                            )
  {

    typedef A0 result_type;
    BOOST_SIMD_FUNCTOR_CALL_REPEAT(2)
    {
      typedef typename dispatch::meta::upgrade<A0>::type uptype;
      return A0(saturate<A0>(uptype(a0)*uptype(a1)));
    }
  };

  BOOST_SIMD_FUNCTOR_IMPLEMENTATION( boost::simd::tag::muls_, tag::cpu_
                            , (A0)
                            , (scalar_< uint64_<A0> >)(scalar_< uint64_<A0> >)
                            )
  {
    typedef A0 result_type;
    BOOST_SIMD_FUNCTOR_CALL_REPEAT(2)
    {
      if (a1 == 0 || a0 == 0) return Zero<A0>(); 
      if (a1 >= a0)
      {
        A0 z = Valmax<A0>()/a1;
        if (z < a0)
        {
          return  Valmax<A0>();
          }
        else
        {
          return a0*a1; 
        }
      }
      else
      {
        return muls(a1, a0); 
      }

    }
  };

  BOOST_SIMD_FUNCTOR_IMPLEMENTATION( boost::simd::tag::muls_, tag::cpu_
                            , (A0)
                            , (scalar_< int64_<A0> >)(scalar_< int64_<A0> >)
                            )
  {
    typedef A0 result_type;
    BOOST_SIMD_FUNCTOR_CALL_REPEAT(2)
    {
      typedef typename meta::as_logical<A0>::type bA0; 
      if (a1 == 0 || a0 == 0) return Zero<A0>();
      typedef typename dispatch::meta::as_integer<A0, unsigned>::type untype;
      A0 sign =  bitofsign(a0)^bitofsign(a1);
      untype aa0 = boost::simd::abs(a0);
      untype aa1 = boost::simd::abs(a1);
      if (aa1 >= aa0)
      {
        untype z = Valmax<A0>()/aa1;
        if (z < aa0)
        {
          return sign ? Valmin<A0>():Valmax<A0>();
        }
        else
        {
          return a0*a1; 
        }
      }
      else
      {
        return muls(a1, a0); 
      }
    }
  };
} } }

#endif
