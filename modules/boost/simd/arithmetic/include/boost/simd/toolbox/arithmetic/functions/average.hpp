//////////////////////////////////////////////////////////////////////////////
///   Copyright 2003 and onward LASMEA UMR 6602 CNRS/U.B.P Clermont-Ferrand
///   Copyright 2009 and onward LRI    UMR 8623 CNRS/Univ Paris Sud XI
///
///          Distributed under the Boost Software License, Version 1.0
///                 See accompanying file LICENSE.txt or copy at
///                     http://www.boost.org/LICENSE_1_0.txt
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_SIMD_TOOLBOX_ARITHMETIC_FUNCTIONS_AVERAGE_HPP_INCLUDED
#define BOOST_SIMD_TOOLBOX_ARITHMETIC_FUNCTIONS_AVERAGE_HPP_INCLUDED
#include <boost/simd/include/simd.hpp>
#include <boost/dispatch/include/functor.hpp>


/*!
 * \internal functor \endinternal
 * \ingroup boost_simd_arithmetic
 * \defgroup average average function
 *
 * \par Description
 * The function always returns a value of the same type than the entry.
 * Take care that for integers the value returned can differ by one unit
 * from \c ceil((a+b)/2.0) or \c floor((a+b)/2.0), but is always one of
 * the two
 *
 * \par Header file
 * 
 * \code
 * #include <nt2/include/functions/average.hpp>
 * \endcode
 * 
 * 
 * \synopsis
 *
 * \code
 * namespace boost::simd
 * {
 *   template <class A0>
 *     meta::call<tag::average_(A0,A0)>::type
 *     average(const A0 & a0,const A0 & a1);
 * }
 * \endcode
 *
 * \param a0 is the first parameter of average
 * \param a1 is the second parameter of average
 * 
 * \return a value of the common type of the parameters
 *  
 * \internal end_functor \endinternal
**/
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
namespace boost { namespace simd { namespace tag
  {         
    /*!
     * \internal tag \endinternal
     * \file
     * \brief Define the tag average_ of functor average 
     *        in namespace boost::simd::tag
     * \internal end_tag \endinternal
    **/
    struct average_ {};
  }
  BOOST_DISPATCH_FUNCTION_IMPLEMENTATION(tag::average_, average, 2)
} }
 
#endif

// modified by jt the 25/12/2010
