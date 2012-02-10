//==============================================================================
//         Copyright 2003 - 2011   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2011   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_CORE_UTILITY_POSITION_CATEGORY_HPP_INCLUDED
#define NT2_CORE_UTILITY_POSITION_CATEGORY_HPP_INCLUDED

#include <nt2/sdk/meta/property_of.hpp>
#include <nt2/sdk/meta/hierarchy_of.hpp>
#include <nt2/core/settings/forward/alignment.hpp>

namespace boost { namespace dispatch { namespace meta
{
  template<class Seq, class A, class Origin>
  struct position_ : boost::dispatch::meta::fusion_sequence_<std::ptrdiff_t>
  {
    typedef boost::dispatch::meta::fusion_sequence_<std::ptrdiff_t> parent;
  };
  
  template<class Seq, class Origin>
  struct position_< Seq, nt2::aligned_, Origin > : position_< Seq, nt2::unaligned_, Origin >
  {
    typedef position_< Seq, nt2::unaligned_, Origin > parent;
  };
} } }

namespace nt2 { namespace ext
{
  using boost::dispatch::meta::position_;
} }

#endif
