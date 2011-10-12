//////////////////////////////////////////////////////////////////////////////
///   Copyright 2003 and onward LASMEA UMR 6602 CNRS/U.B.P Clermont-Ferrand
///   Copyright 2009 and onward LRI    UMR 8623 CNRS/Univ Paris Sud XI
///
///          Distributed under the Boost Software License, Version 1.0
///                 See accompanying file LICENSE.txt or copy at
///                     http://www.boost.org/LICENSE_1_0.txt
//////////////////////////////////////////////////////////////////////////////
/*!
 * \file
**/
#ifndef BOOST_SIMD_TOOLBOX_BITWISE_FUNCTIONS_NEGIF_HPP_INCLUDED
#define BOOST_SIMD_TOOLBOX_BITWISE_FUNCTIONS_NEGIF_HPP_INCLUDED
#include <boost/simd/include/simd.hpp>
#include <boost/dispatch/include/functor.hpp>

/*!
 * \ingroup boost_simd_bitwise
 * \defgroup boost_simd_bitwise_negif negif function
 *
 * \par Description
 * The function conditionnaly to a0 returns a1 or -a1
 *
 * \par Header file
 * 
 * \code
 * #include <nt2/include/functions/negif.hpp>
 * \endcode
 * 
 * 
 * \synopsis
 *
 * \code
 * namespace boost::simd
 * {
 *   template <class A0>
 *     meta::call<tag::negif_(A0,A0)>::type
 *     negif(const A0 & a0,const A0 & a1);
 * }
 * \endcode
 *
 * \param a0 the first parameter of negif
 * \param a1 the second parameter of negif
 * 
 * \return always of the type of the second parameter
 *  
 * \par Notes
 * In SIMD mode, this function acts elementwise on the inputs vectors elements
 * \par
 *  
**/

namespace boost { namespace simd { namespace tag
  {         
    /*!
     * \brief Define the tag negif_ of functor negif 
     *        in namespace boost::simd::tag for toolbox boost.simd.bitwise
    **/
    struct negif_ {};
  }
  BOOST_DISPATCH_FUNCTION_IMPLEMENTATION(tag::negif_, negif, 2)
} }

#endif

// modified by jt the 25/12/2010
