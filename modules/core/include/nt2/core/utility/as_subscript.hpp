//==============================================================================
//         Copyright 2003 - 2011   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2011   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_CORE_UTILITY_AS_INDEX_HPP_INCLUDED
#define NT2_CORE_UTILITY_AS_INDEX_HPP_INCLUDED

#include <nt2/include/constants/one.hpp>
#include <nt2/include/functions/simd/plus.hpp>
#include <nt2/include/functions/simd/splat.hpp>
#include <nt2/include/functions/simd/modulo.hpp>
#include <nt2/include/functions/simd/divides.hpp>
#include <nt2/include/functions/simd/bitwise_cast.hpp>
#include <boost/dispatch/meta/as_unsigned.hpp>
#include <boost/fusion/adapted/array.hpp>
#include <boost/fusion/include/size.hpp>
#include <boost/fusion/include/at.hpp>
#include <boost/mpl/int.hpp>

namespace nt2 { namespace details
{
  //============================================================================
  // Case for no base index
  //============================================================================
  template<class Size,class Position>
  struct as_subscript
  {
    typedef typename meta::strip<Position>::type                      base_t;
    typedef typename boost::dispatch::meta::as_unsigned<base_t>::type type_t;
    typedef boost::fusion::result_of::size<Size>                      dims_t;
    typedef boost::array<type_t,dims_t::value>                        result_type;

    BOOST_DISPATCH_FORCE_INLINE result_type
    operator()(const Size& size, const Position& pos) const
    {
      result_type sub;
      type_t p = bitwise_cast<type_t>(pos);

      eval( sub, p, size
          , boost::mpl::int_<0>()
          , boost::mpl::int_<dims_t::value>()
          , boost::mpl::bool_<dims_t::value==2>()
          );

      return sub;
    }

    // Normal case
    template<class N, class M>
    BOOST_DISPATCH_FORCE_INLINE void
    eval( result_type& sub, const type_t& p
        , Size const& s, N const&, M const& m
        , boost::mpl::false_ const&
        ) const
    {
      type_t ls     = splat<type_t>(boost::fusion::at_c<N::value>(s));
      sub[N::value] = p % ls;
      eval( sub, p/ls, s
          , boost::mpl::int_<N::value+1>()
          , m
          , boost::mpl::bool_<(N::value+1 == M::value-2)>()
          );
    }

    template<class N, class M>
    BOOST_DISPATCH_FORCE_INLINE void
    eval( result_type& sub, type_t const& p
        , Size const& s, N const&, M const& m
        , boost::mpl::true_ const&
        ) const

    {
      type_t ls       = splat<type_t>(boost::fusion::at_c<N::value>(s));
      sub[N::value]   = p % ls;
      sub[N::value+1] = p / ls;
    }

    BOOST_DISPATCH_FORCE_INLINE void
    eval( result_type& sub, const type_t& p, Size const&
        , boost::mpl::int_<0> const &, boost::mpl::int_<1> const &
        , boost::mpl::false_ const&
        ) const
    {
      sub[0] = p;
    }
  };
} }

namespace nt2
{
  template<class Size,class Position>
  typename details::as_subscript<Size,Position>::result_type
  as_subscript(Size const& sz, Position const& p)
  {
    return details::as_subscript<Size,Position>()(sz,p);
  }
}
#endif
