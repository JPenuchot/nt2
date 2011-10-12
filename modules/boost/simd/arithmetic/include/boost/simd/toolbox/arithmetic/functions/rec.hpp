//////////////////////////////////////////////////////////////////////////////
///   Copyright 2003 and onward LASMEA UMR 6602 CNRS/U.B.P Clermont-Ferrand
///   Copyright 2009 and onward LRI    UMR 8623 CNRS/Univ Paris Sud XI
///
///          Distributed under the Boost Software License, Version 1.0
///                 See accompanying file LICENSE.txt or copy at
///                     http://www.boost.org/LICENSE_1_0.txt
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_SIMD_TOOLBOX_ARITHMETIC_FUNCTIONS_REC_HPP_INCLUDED
#define BOOST_SIMD_TOOLBOX_ARITHMETIC_FUNCTIONS_REC_HPP_INCLUDED
#include <boost/simd/include/simd.hpp>
#include <boost/dispatch/include/functor.hpp>


/*!
 * \internal functor \endinternal
 * \ingroup boost_simd_arithmetic
 * \defgroup rec rec function
 *
 * \par Description
 * return the inverse of the input as a floating point value.
 *
 * \par Header file
 * 
 * \code
 * #include <nt2/include/functions/rec.hpp>
 * \endcode
 * 
 * 
 * \synopsis
 *
 * \code
 * namespace boost::simd
 * {
 *   template <class A0>
 *     meta::call<tag::rec_(A0)>::type
 *     rec(const A0 & a0);
 * }
 * \endcode
 *
 * \param a0 is the unique parameter of rec
 * 
 * \return always return floating point values
 *  
 * \internal end_functor \endinternal
**/
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
namespace boost { namespace simd { namespace tag
  {         
    /*!
     * \internal tag \endinternal
     * \file
     * \brief Define the tag rec_ of functor rec 
     *        in namespace boost::simd::tag
     * \internal end_tag \endinternal
    **/
    struct rec_ {};
  }
  BOOST_DISPATCH_FUNCTION_IMPLEMENTATION(tag::rec_, rec, 1)
} }
 
#endif

// modified by jt the 25/12/2010
