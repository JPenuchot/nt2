//==============================================================================
//         Copyright 2003 - 2012 LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2012 LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_UNIT_DETAILS_DEMANGLE_HPP_INCLUDED
#define NT2_UNIT_DETAILS_DEMANGLE_HPP_INCLUDED


#if (__GNUC__ && __cplusplus && __GNUC__ >= 3)
//==============================================================================
// Includes abi::__cxa_demangle
//==============================================================================
#include <cxxabi.h>
#endif

#include <string>
#include <cstdlib>
#include <typeinfo>
#include <iostream>

namespace nt2 {  namespace details
{
  // INTERNAL ONLY
  // demangle a type name retrieved through typeid()
  inline std::string demangle(const char* name)
  {
    #if(__GNUC__ && __cplusplus && __GNUC__ >= 3)
    std::size_t len;
    int         stat;

    char* realname = ::abi::__cxa_demangle(name,NULL,&len,&stat);

    if(realname != NULL)
    {
      std::string out(realname);
      ::free(realname);
      return out;
    }
    else
    {
      return std::string("?");
    }
    #else
    std::string out(name);
    return out;
    #endif
  }

  // INTERNAL ONLY
  // Stream some indentation on a output stream
  inline std::ostream& indent(std::ostream& os, size_t depth)
  {
    for(size_t i=0; i<depth; ++i)
      os << ' ';

    return os;
  }
} }

#endif
