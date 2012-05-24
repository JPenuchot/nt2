//==============================================================================
//         Copyright 2003 - 2012   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2012   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_OPTIONS_HPP_INCLUDED
#define NT2_OPTIONS_HPP_INCLUDED

#include <nt2/sdk/meta/as.hpp>

namespace nt2
{
  namespace details
  {
    // Markee for in place option
    struct in_place_  {};
    struct meshgrid_  {};
    struct whole_     {};
  }

  // Lower/upper
  static const char lower_    = 'L';
  static const char upper_    = 'U';

  meta::as_<details::in_place_> const in_place_ = {};
  meta::as_<details::whole_>    const whole_    = {};
  meta::as_<details::meshgrid_> const meshgrid_ = {};

}

#endif
