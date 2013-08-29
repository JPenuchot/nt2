//==============================================================================
//         Copyright 2003 - 2011 LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2011 LRI    UMR 8623 CNRS/Univ Paris Sud XI
//         Copyright 2012        MetaScale SAS
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <iostream>
#include <nt2/sdk/unit/stats.hpp>
#include <nt2/sdk/unit/config.hpp>
#include <nt2/sdk/unit/details/main.hpp>
#include <nt2/sdk/unit/details/suite.hpp>
#include <nt2/sdk/error/exception.hpp>
#include <boost/throw_exception.hpp>

void float_control_debug();

namespace nt2 { namespace details
{
  NT2_TEST_UNIT_DECL
  int unit_main(int argc, char* argv[], test_suite const& current_suite)
  {
    bool no_catch = false;
    bool float_debug = false;

    for(int i=1; i<argc; ++i)
    {
      if(!strcmp(argv[i], "--no-catch"))
        no_catch = true;
      else if(!strcmp(argv[i], "--float-debug"))
        float_debug = true;
    }

    if(float_debug)
      float_control_debug();

    #ifndef BOOST_NO_EXCEPTIONS
    if(no_catch)
    {
      current_suite.process();
      return nt2::unit::error_count() ? -1: 0;
    }

    try
    {
      current_suite.process();
      return nt2::unit::error_count() ? -1: 0;
    }
    catch(std::exception const& e)
    {
      std::cout << "uncaught exception: " << e.what() << std::endl;
      return 1;
    }
    catch(...)
    {
      std::cout << "uncaught exception" << std::endl;
      return 1;
    }
    #else
    current_suite.process();
    return nt2::unit::error_count() ? -1: 0;
    #endif
  }
} }
