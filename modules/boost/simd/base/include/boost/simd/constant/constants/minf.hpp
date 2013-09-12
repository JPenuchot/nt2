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
#ifndef BOOST_SIMD_CONSTANT_CONSTANTS_MINF_HPP_INCLUDED
#define BOOST_SIMD_CONSTANT_CONSTANTS_MINF_HPP_INCLUDED

#include <boost/simd/include/functor.hpp>
#include <boost/simd/meta/float.hpp>
#include <boost/simd/meta/int_c.hpp>
#include <boost/simd/meta/double.hpp>
#include <boost/simd/sdk/meta/make_dependent.hpp>
#include <boost/simd/constant/hierarchy.hpp>
#include <boost/simd/sdk/config.hpp>

/*!
 * \ingroup boost_simd_constant
 * \defgroup boost_simd_constant_minf Minf
 *
 * \par Description
 * Constant Minf
 * \par
 * The value of this constant is type dependant. This means that for different
 * types it does not represent the same mathematical number.
 *
 * \par Header file
 *
 * \code
 * #include <nt2/include/functions/minf.hpp>
 * \endcode
 *
 *
 * \synopsis
 *
 * \code
 * namespace boost::simd
 * {
 *   template <class T,class A0>
 *     meta::call<tag::minf_(A0)>::type
 *     Minf();
 * }
 * \endcode
 *
 *
 * \param T template parameter of Minf
 *
 * \return type T value
 *
 *
**/

namespace boost { namespace simd
{
  namespace tag
  {
    struct Valmin;

    /*!
     * \brief Define the tag Minf of functor Minf
     *        in namespace boost::simd::tag for toolbox boost.simd.constant
    **/
    struct Minf : ext::pure_constant_<Minf>
    {
      typedef double                    default_type;
      typedef ext::pure_constant_<Minf> parent;

      template<class Target, class Dummy=void>
      struct apply : meta::make_dependent<Valmin, Dummy>::type::template apply<Target,Dummy> {};
    };

    template<class T, class Dummy>
    struct  Minf::apply<boost::dispatch::meta::single_<T>,Dummy>
          : meta::single_<0xFF800000> {};

    template<class T, class Dummy>
    struct  Minf::apply<boost::dispatch::meta::double_<T>,Dummy>
          : meta::double_<0xFFF0000000000000ULL> {};
  }

  BOOST_SIMD_CONSTANT_IMPLEMENTATION(boost::simd::tag::Minf, Minf)
} }

#include <boost/simd/constant/include/constants/valmin.hpp>
#include <boost/simd/constant/common.hpp>

#endif