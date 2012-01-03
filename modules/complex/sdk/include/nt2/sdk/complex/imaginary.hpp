//==============================================================================
//         Copyright 2003 - 2011   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2011   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_SDK_COMPLEX_IMAGINARY_HPP_INCLUDED
#define NT2_SDK_COMPLEX_IMAGINARY_HPP_INCLUDED

#include <nt2/sdk/complex/details/imaginary/meta.hpp>
#include <nt2/sdk/complex/details/imaginary/native.hpp>

namespace nt2
{
  template<class T>
  struct imaginary
  {
    typedef T type;
    T value;
    
    imaginary() {}
    explicit imaginary(T const& value_) : value(value_)
    {
    }
    bool operator == (const imaginary<T>&a) const{return value == a.value; }
    bool operator != (const imaginary<T>&a) const{return value != a.value; }
    
    T&       operator()()       { return value; }
    T const& operator()() const { return value; }
  };
    ////////////////////////////////////////////////////////////////////////////
  // Stream insertion for imaginary<T>
  ////////////////////////////////////////////////////////////////////////////
  template<class T>
  BOOST_DISPATCH_FORCE_INLINE
  std::ostream& operator<<(std::ostream& os, imaginary<T> const& v )
  {
    return os << v() << "*i";
  }
}

#endif
