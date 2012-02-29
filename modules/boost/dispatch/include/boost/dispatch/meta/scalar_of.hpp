/*******************************************************************************
 *         Copyright 2003 & onward LASMEA UMR 6602 CNRS/Univ. Clermont II
 *         Copyright 2009 & onward LRI    UMR 8623 CNRS/Univ Paris Sud XI
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/
#ifndef BOOST_DISPATCH_META_SCALAR_OF_HPP_INCLUDED
#define BOOST_DISPATCH_META_SCALAR_OF_HPP_INCLUDED

#include <boost/dispatch/meta/value_of.hpp>

namespace boost { namespace dispatch { namespace meta
{
  template<class T>
  struct scalar_of;
}

namespace details
{
  template<class T, class U = typename meta::value_of<T>::type>
  struct scalar_of
    : meta::scalar_of<U>
  {
  };
  
  template<class T>
  struct scalar_of<T, T>
  {
    typedef T type;
  };
}

namespace meta
{
  template<class T>
  struct scalar_of
    : details::scalar_of<T>
  {
  };
} } }

#endif
