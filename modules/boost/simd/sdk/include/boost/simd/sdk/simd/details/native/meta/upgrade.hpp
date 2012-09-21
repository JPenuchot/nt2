/*******************************************************************************
 *         Copyright 2003 & onward LASMEA UMR 6602 CNRS/Univ. Clermont II
 *         Copyright 2009 & onward LRI    UMR 8623 CNRS/Univ Paris Sud XI
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/
#ifndef BOOST_SIMD_SDK_SIMD_DETAILS_NATIVE_META_UPGRADE_HPP_INCLUDED
#define BOOST_SIMD_SDK_SIMD_DETAILS_NATIVE_META_UPGRADE_HPP_INCLUDED

#include <boost/mpl/if.hpp>
#include <boost/dispatch/meta/upgrade.hpp>
#include <boost/simd/sdk/simd/native_fwd.hpp>
#include <boost/simd/sdk/simd/meta/vector_of.hpp>
#include <boost/simd/sdk/meta/cardinal_of.hpp>
#include <boost/simd/sdk/simd/meta/is_vectorizable.hpp>
#include <boost/simd/sdk/simd/logical.hpp>

namespace boost { namespace dispatch { namespace meta
{
////////////////////////////////////////////////////////////////////////////
// Extension Point for upgrade on native<T,Ext>.
// Prevent from upgrading native<float,altivec_> to native<double,altivec_>
// which is not supported.
////////////////////////////////////////////////////////////////////////////

  template<class T, class Ext, class Sign>
  struct upgrade< typename boost::simd::native<T, Ext>, Sign >
  {
    typedef typename 
    simd::meta::vector_of< typename dispatch::meta::upgrade<T,Sign>::type
                         , simd::meta::cardinal_of< simd::native<T,Ext> >::value/2
                         >::type type;
  };

////////////////////////////////////////////////////////////////////////////
// Extension Point for upgrade on native<T,Ext>. 
// Prevent from upgrading native<logical<float>,altivec_> to native<logical<double>,altivec_> 
// which is not supported.
////////////////////////////////////////////////////////////////////////////

  template<class T, class Ext, class Sign>
  struct upgrade< typename boost::simd::native<boost::simd::logical<T>, Ext>, Sign >
  {
    typedef typename 
    simd::meta::vector_of< typename dispatch::meta::upgrade<T,Sign>::type
                         , simd::meta::cardinal_of< simd::native< boost::simd::logical<T>
                                                                , Ext> >::value/2
                         >::type type;
  };

} } }

#endif /* BOOST_SIMD_SDK_SIMD_DETAILS_NATIVE_META_UPGRADE_HPP_INCLUDED */
