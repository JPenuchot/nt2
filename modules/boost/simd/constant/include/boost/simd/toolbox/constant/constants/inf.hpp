//==============================================================================
//         Copyright 2003 - 2011   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2011   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
/*!
 * \file
**/
#ifndef BOOST_SIMD_TOOLBOX_CONSTANT_CONSTANTS_INF_HPP_INCLUDED
#define BOOST_SIMD_TOOLBOX_CONSTANT_CONSTANTS_INF_HPP_INCLUDED

#include <boost/simd/include/simd.hpp>
#include <boost/simd/sdk/meta/int_c.hpp>
#include <boost/simd/sdk/meta/float.hpp>
#include <boost/simd/sdk/meta/double.hpp>
#include <boost/simd/sdk/constant/common.hpp>
#include <boost/simd/sdk/constant/constant.hpp>
#include <boost/simd/toolbox/constant/include/constants/valmax.hpp>

/*!
 * \ingroup boost_simd_constant
 * \defgroup inf Inf function
 *
 * \par Description
 * Constant Inf
 * \arg Maximum value of the type for integer types
 * \arg \f$= \infty \f$ for floating points
 * \par
 * The value of this constant are type dependant. This means that for different
 * types they do not represent the same mathematical number.
 *
 * \par Header file
 * 
 * \code
 * #include <nt2/include/functions/inf.hpp>
 * \endcode
 * 
 * 
 * \synopsis
 *
 * \code
 * namespace boost::simd
 * {
 *   template <class T,class A0>
 *     meta::call<tag::inf_(A0)>::type
 *     Inf();
 * }
 * \endcode
 *
 * 
 * \param T template parameter of Inf
 * 
 * \return type T value
 *  
 *  
**/

namespace boost { namespace simd
{
  namespace tag
  {
    /*!
     * \brief Define the tag Inf of functor Inf 
     *        in namespace boost::simd::tag for toolbox boost.simd.constant
    **/
    struct Inf 
    { 
      typedef double default_type;
      template<class Target, class Dummy=void> 
      struct apply : Valmax::apply<Target,Dummy> {};  
    };

    template<class Dummy>
    struct Inf::apply<float,Dummy>  : meta::single_<0x7F800000> {};

    template<class Dummy>
    struct Inf::apply<double,Dummy> : meta::double_<0x7FF0000000000000ULL> {};
  }

  BOOST_SIMD_CONSTANT_IMPLEMENTATION(boost::simd::tag::Inf, Inf)
} }

#endif
