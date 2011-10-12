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
#ifndef BOOST_SIMD_TOOLBOX_CONSTANT_CONSTANTS_MAXEXPONENT_HPP_INCLUDED
#define BOOST_SIMD_TOOLBOX_CONSTANT_CONSTANTS_MAXEXPONENT_HPP_INCLUDED

#include <boost/simd/include/simd.hpp>
#include <boost/simd/sdk/meta/int_c.hpp>
#include <boost/simd/sdk/constant/common.hpp>
#include <boost/simd/sdk/constant/constant.hpp>

/*!
 * \ingroup boost_simd_constant
 * \defgroup maxexponent Maxexponent function
 *
 * \par Description
 * Constant Maxexponent, this is the maximum exposant
 * for floating point numbers, i.e. 127 for float , 1023 for double
 * \par
 * The value of this constant are type dependant. This means that for different
 * types they do not represent the same mathematical number.
 *
 * \par Header file
 * 
 * \code
 * #include <nt2/include/functions/maxexponent.hpp>
 * \endcode
 * 
 * 
 * \synopsis
 *
 * \code
 * namespace boost::simd
 * {
 *   template <class T,class A0>
 *     meta::call<tag::maxexponent_(A0)>::type
 *     Maxexponent();
 * }
 * \endcode
 *
 * 
 * \param T template parameter of Maxexponent
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
     * \brief Define the tag Maxexponent of functor Maxexponent 
     *        in namespace boost::simd::tag for toolbox boost.simd.constant
    **/
    struct Maxexponent 
    { 
      template<class Target, class Dummy=void> 
      struct  apply : meta::int_c<Target,0> {};  
    };

    template<class Dummy>
    struct  Maxexponent::apply<float,Dummy> 
          : meta::int_c<boost::simd::int32_t,127> {};

    template<class Dummy>
    struct  Maxexponent::apply<double,Dummy> 
          : meta::int_c<boost::simd::int64_t,1023> {};

  }

  BOOST_SIMD_CONSTANT_IMPLEMENTATION(boost::simd::tag::Maxexponent, Maxexponent)
} }

#endif
