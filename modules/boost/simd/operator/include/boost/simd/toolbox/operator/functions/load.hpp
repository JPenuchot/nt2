//==============================================================================
//         Copyright 2003 - 2011   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2011   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef BOOST_SIMD_TOOLBOX_OPERATOR_FUNCTIONS_LOAD_HPP_INCLUDED
#define BOOST_SIMD_TOOLBOX_OPERATOR_FUNCTIONS_LOAD_HPP_INCLUDED
#include <boost/simd/include/simd.hpp>
#include <boost/dispatch/include/functor.hpp>

////////////////////////////////////////////////////////////////////////////////
// Load from memory functor and function
// Documentation: http://nt2.lri.fr/sdk/memory/functions/load.html
////////////////////////////////////////////////////////////////////////////////
#include <boost/dispatch/meta/as.hpp>
#include <boost/dispatch/functor/preprocessor/function.hpp>


/*!
 * \internal functor \endinternal
 * \ingroup boost_simd_operator
 * \defgroup load load function
 *
 * \par Description
 * Load a data of type T from the memory zone given by (a0,a1)
 * with or without a sub-type level offset
 *
 * \par Header file
 * 
 * \code
 * #include <nt2/include/functions/load.hpp>
 * \endcode
 * 

 * 
 * \synopsis
 *
 * \code
 * namespace boost::simd
 * {
 *   template<class T,class A0,class A1> inline
 *   typename boost::dispatch::meta::call<tag::load_ ( A0 const&, A1 const&
 *                                                 , boost::dispatch::meta::as_<T>
 *                                                 )
 *                                       >::type
 *   load(A0 const& a0,A1 const& a1 );
 *
 *   template<class T,int Offset,class A0,class A1> inline
 *   typename boost::dispatch::meta::call<tag::load_ ( A0 const&, A1 const&
 *                                                 , boost::dispatch::meta::as_<T>
 *                                                 , boost::mpl::int_<Offset>
 *                                                 )
 *                                       >::type
 *   load(A0 const& a0,A1 const& a1 ); 
 * }
 * \endcode
 *
 * \param a0 is the first parameter of load
 * \param a1 is the second parameter of load
 * 
 * \param T is a template parameter of load
 * 
 * \return a value of the common type of the parameters
 *  
 * \internal end_functor \endinternal
**/

namespace boost { namespace simd
{
  namespace tag { struct load_ {}; }
    /*!
     * \internal tag \endinternal
     * \file
     * \brief Define the tag load_ of functor load 
     *        in namespace boost::simd::tag
     * \internal end_tag \endinternal
     */

  //////////////////////////////////////////////////////////////////////////////
  // Load a data of type T from the memory zone given by (a0,a1)
  //////////////////////////////////////////////////////////////////////////////
  template<class T,class A0,class A1> inline
  typename boost::dispatch::meta::call<tag::load_ ( A0 const&, A1 const&
                                                , boost::dispatch::meta::as_<T>
                                                )
                            >::type
  load(A0 const& a0,A1 const& a1 )
  {
    typename boost::dispatch::make_functor<tag::load_, A0>::type callee;
    return callee(a0,a1,boost::dispatch::meta::as_<T>());
  }

  //////////////////////////////////////////////////////////////////////////////
  // Load a data of type T from the memory zone given by (a0,a1) and a sub-type
  // level offset
  //////////////////////////////////////////////////////////////////////////////
  template<class T,int Offset, class A0,class A1> inline
  typename boost::dispatch::meta::call<tag::load_ ( A0 const&, A1 const&
                                                , boost::dispatch::meta::as_<T>
                                                , boost::mpl::int_<Offset>
                                                )
                            >::type
  load(A0 const& a0,A1 const& a1 )
  {
    typename boost::dispatch::make_functor<tag::load_, A0>::type callee;
    return callee(a0,a1,boost::dispatch::meta::as_<T>(),boost::mpl::int_<Offset>());
  }
} }

#endif
