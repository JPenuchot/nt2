//==============================================================================
//         Copyright 2003 - 2011   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2011   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_SDK_CONFIG_CACHE_HPP_INCLUDED
#define NT2_SDK_CONFIG_CACHE_HPP_INCLUDED

#include <nt2/sdk/config/details/cache/cache.hpp>

/*!
 *\file cache.hpp
 *\Runtime function to get cache informations
*/

namespace nt2{ namespace config{

  enum{L1cache=1, L2cache=2, L3cache=3 };

  int cache_size(int const& cache_level)
  {
    return details::detected_cache.cache_size(cache_level);
  }

  int cache_line_size(int const& cache_level)
  {
    return details::detected_cache.cache_line_size(cache_level);
  }

} }

#endif /* NT2_SDK_CONFIG_CACHE_HPP */
