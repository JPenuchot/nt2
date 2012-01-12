//==============================================================================
//         Copyright 2003 - 2011   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2011   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_CORE_SETTINGS_PADDING_HPP_INCLUDED
#define NT2_CORE_SETTINGS_PADDING_HPP_INCLUDED

#include <boost/mpl/int.hpp>
#include <nt2/core/settings/misc.hpp>
#include <nt2/core/settings/option.hpp>
#include <boost/simd/sdk/memory/parameters.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>

namespace nt2
{
  //============================================================================
  /*!
   * Specify a global padding strategy will be used.
   **/
  //============================================================================
  template<std::ptrdiff_t Value = 1>
  struct global_padding_ : boost::mpl::int_<Value> {};

  //============================================================================
  /*!
   * Specify a lead dimension padding strategy will be used.
   **/
  //============================================================================
  template<std::ptrdiff_t Value = BOOST_SIMD_CONFIG_ALIGNMENT>
  struct lead_padding_  : boost::mpl::int_<Value> {};

  //============================================================================
  /*!
   * Specify that current container doesn't pad its memory.
   **/
  //============================================================================
  struct no_padding_ {};

  namespace tag
  {
    //==========================================================================
    /*!
     * Option tag for padding options
     **/
    //==========================================================================
    struct global_padding_  {};
    struct lead_padding_    {};
  }
}

namespace nt2 { namespace meta
{
  //============================================================================
  /*! global_padding_ with alignment value
   *  return padding_strategy of global_ with the alignment value
   **/
  //============================================================================
  template<class Default, std::ptrdiff_t N>
  struct  option< global_padding_<N>, tag::global_padding_, Default >
  {
    typedef global_padding_<N> type;
  };

  //============================================================================
  /*! lead_padding_ with alignment value
   *  return padding_strategy of lead_ with the alignment value
   **/
  //============================================================================
  template<class Default, std::ptrdiff_t N>
  struct  option< lead_padding_<N>, tag::lead_padding_, Default >
  {
    typedef lead_padding_<N> type;
  };

  //============================================================================
  /*!
   * no_padding_ option
   **/
  //============================================================================
  template<class Default>
  struct  option< no_padding_, tag::lead_padding_, Default >
  {
    typedef lead_padding_<1> type;
  };

  template<class Default>
  struct  option< no_padding_, tag::global_padding_, Default >
  {
    typedef global_padding_<1> type;
  };
} }

#endif
