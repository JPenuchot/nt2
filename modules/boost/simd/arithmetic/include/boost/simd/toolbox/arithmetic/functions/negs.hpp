//////////////////////////////////////////////////////////////////////////////
///   Copyright 2003 and onward LASMEA UMR 6602 CNRS/U.B.P Clermont-Ferrand
///   Copyright 2009 and onward LRI    UMR 8623 CNRS/Univ Paris Sud XI
///
///          Distributed under the Boost Software License, Version 1.0
///                 See accompanying file LICENSE.txt or copy at
///                     http://www.boost.org/LICENSE_1_0.txt
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_SIMD_TOOLBOX_ARITHMETIC_FUNCTIONS_NEGS_HPP_INCLUDED
#define BOOST_SIMD_TOOLBOX_ARITHMETIC_FUNCTIONS_NEGS_HPP_INCLUDED
#include <boost/simd/include/simd.hpp>
#include <boost/dispatch/include/functor.hpp>

/*!
 * \ingroup boost_simd_arithmetic
 * \defgroup negs negs function
 *
 * \par Description
 * returns saturation of $-a_0$ in the input type.
 *
 * \par Header file
 * 
 * \code
 * #include <nt2/include/functions/negs.hpp>
 * \endcode
 * 
 * \par Alias 
 * \arg saturated_neg
 * 
 * \synopsis
 *
 * \code
 * namespace boost::simd
 * {
 *   template <class A0>
 *     meta::call<tag::negs_(A0)>::type
 *     negs(const A0 & a0);
 * }
 * \endcode
 *
 * \param a0 the unique parameter of negs
 * 
 * \return a value of the same type as the parameter
 *  
 * \par Notes
 * In SIMD mode, this function acts elementwise on the inputs vectors elements
 * \par
 *  
**/

namespace boost { namespace simd {
  namespace tag
  {
    /*!
     * \brief Define the tag negs_ of functor negs 
     *        in namespace boost::simd::tag
    **/
    struct negs_ {};
  }
  BOOST_DISPATCH_FUNCTION_IMPLEMENTATION(tag::negs_, negs, 1)
  BOOST_DISPATCH_FUNCTION_IMPLEMENTATION(tag::negs_, saturated_neg, 1)
} }

#endif
