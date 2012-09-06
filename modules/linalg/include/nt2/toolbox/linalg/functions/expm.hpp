/*******************************************************************************
 *         Copyright 2003-2012 LASMEA UMR 6602 CNRS/U.B.P
 *         Copyright 2009-2012 LRI    UMR 8623 CNRS/Univ Paris Sud XI
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/
#ifndef NT2_TOOLBOX_LINALG_FUNCTIONS_EXPM_HPP_INCLUDED
#define NT2_TOOLBOX_LINALG_FUNCTIONS_EXPM_HPP_INCLUDED
#include <nt2/include/functor.hpp>
#include <boost/simd/include/simd.hpp>
#include <boost/dispatch/include/functor.hpp>
#include <nt2/sdk/memory/container.hpp>

/*!
 * \ingroup algebra
 * \defgroup algebra_expm expm
 *
 * \par Description
 * suqre root of a matrix
 *
 * \par Header file
 * 
 * \code
 * #include <nt2/include/functions/expm.hpp>
 * \endcode
 * 
 * 
 * \synopsis
 *
 * \param a the matrix or vector expression a 
 *
 * \param type of expm required
 * 
 * \par Notes
 *   Call the dedicated lapack routines available on the target.
 * \par
 *  
**/

namespace nt2 { namespace tag
  {         
    /*!
     * \brief Define the tag expm_ of functor expm
     *        in namespace nt2::tag for toolbox algebra
    **/
    struct expm_ :  tag::formal_ { typedef tag::formal_ parent; };
  }
  
  BOOST_DISPATCH_FUNCTION_IMPLEMENTATION(tag::expm_, expm, 1)

}

#endif

