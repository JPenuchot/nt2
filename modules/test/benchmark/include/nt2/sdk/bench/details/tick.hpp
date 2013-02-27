//==============================================================================
//         Copyright 2003 - 2012 LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2012 LRI    UMR 8623 CNRS/Univ Paris Sud XI
//         Copyright 2012        MetaScale SAS
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_SDK_BENCH_DETAILS_TICK_HPP_INCLUDED
#define NT2_SDK_BENCH_DETAILS_TICK_HPP_INCLUDED

#include <nt2/sdk/timing/now.hpp>

namespace nt2 { namespace details
{
  struct second_
  {
    static        double  call();
    static const  char*   unit();
  };

  struct cycle_
  {
    static        double  call();
    static const  char*   unit();
  };
} }

#endif
