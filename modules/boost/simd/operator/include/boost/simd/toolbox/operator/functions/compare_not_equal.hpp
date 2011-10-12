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
#ifndef BOOST_SIMD_TOOLBOX_OPERATOR_FUNCTIONS_COMPARE_NOT_EQUAL_HPP_INCLUDED
#define BOOST_SIMD_TOOLBOX_OPERATOR_FUNCTIONS_COMPARE_NOT_EQUAL_HPP_INCLUDED
#include <boost/simd/include/simd.hpp>
#include <boost/dispatch/include/functor.hpp>

#include <boost/simd/toolbox/operator/specific/common.hpp>

#include <boost/proto/tags.hpp>

/*!
 * \ingroup boost_simd_operator
 * \defgroup boost_simd_operator_compare_not_equal compare_not_equal function
 *
 * \par Description
 * return a bool that is the result of the lexicographic
 * test for != on elements of the entries
 * I.e. return true if and only if two corresponding entries
 * elements are not equal.
 * \par
 * It is probably not what you wish. Have a look to <tt>is_not_equal</tt>
 * \par
 * Infix notation can be used with operator '!='
 *
 * \par Header file
 * 
 * \code
 * #include <nt2/include/functions/compare_not_equal.hpp>
 * \endcode
 * 
 * \par Alias 
 * \arg compare_neq
 * 
 * \synopsis
 *
 * \code
 * namespace boost::simd
 * {
 *   template <class A0>
 *     meta::call<tag::compare_not_equal_(A0,A0)>::type
 *     compare_not_equal(const A0 & a0,const A0 & a1);
 * }
 * \endcode
 *
 * \param a0 the first parameter of compare_not_equal
 * \param a1 the second parameter of compare_not_equal
 * 
 * \return always a scalar value
 *  
 * \par Notes
 * In SIMD mode, this function acts elementwise on the inputs vectors elements
 * \par
 * This is a reduction operation. As such it has not real interest outside
 * SIMD mode.
 * \par
 * Such an operation has always a scalar result which translate a property
 * of the whole SIMD vector.
 * \par
 * If usable and used in scalar mode, it reduces to the operation as acting
 * on a one element vector.
 *  
**/

namespace boost { namespace simd
{
  namespace tag
  {
    /*!
    _
     *        in namespace boost::simd::tag for toolbox boost.simd.operator
    **/
    typedef boost::proto::tag::not_equal_to compare_not_equal_;
  }

  BOOST_DISPATCH_FUNCTION_IMPLEMENTATION(tag::compare_not_equal_, compare_not_equal , 2 )
  BOOST_DISPATCH_FUNCTION_IMPLEMENTATION(tag::compare_not_equal_, compare_neq       , 2 )
} }

#endif
