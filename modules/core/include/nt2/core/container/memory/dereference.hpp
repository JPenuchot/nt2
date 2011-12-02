//==============================================================================
//         Copyright 2003 - 2011   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2011   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_CORE_CONTAINER_MEMORY_DEREFERENCE_HPP_INCLUDED
#define NT2_CORE_CONTAINER_MEMORY_DEREFERENCE_HPP_INCLUDED

//==============================================================================
/**
  * \file
  * \brief Defines and implements the \c nt2::memory::dereference class
  **/
//==============================================================================
#include <nt2/core/settings/details/fusion.hpp>
#include <nt2/core/container/meta/dimensions_of.hpp>
#include <nt2/core/container/meta/storage_order_of.hpp>
#include <iostream>

namespace nt2 { namespace details
{
  //============================================================================
  // Recursively apply [] on the buffer
  //============================================================================
  template<typename Buffer, std::size_t Level, std::size_t Start>
  struct dereference
  {
    //typedef typename meta::dimensions_of<Buffer> size;

    template<typename Position>
    static BOOST_DISPATCH_FORCE_INLINE
    typename meta::dereference_<Buffer&,Level>::type
    apply( Buffer& b, Position const& p )
    {
      typedef typename meta::dereference_<Buffer,1>::type    base_type;
      typedef typename meta::dimensions_of<Buffer>::type     size;
      typedef typename meta::storage_order_of<Buffer>::type  so;

      typedef boost::mpl::int_<Level -1> dim;
      std::cout << "Start " << Start << std::endl;
      typedef typename boost::mpl::apply<so,size,dim >::type new_dim;
      //std::cout <<" S-1-D =" << size::value <<" - " << 1 << " - " << dim::value << std::endl;
      //      std::cout << "pos[" << dim::value << "]: "<<safe_at_c<new_dim::value>(p) << std::endl;
      return  dereference<base_type,Level-1,Start>
              ::apply ( b[safe_at_c<new_dim::value>(p)]
                      , p
                      );

      // return  dereference<base_type,Level-1,Start>
      //         ::apply ( b[safe_at_c<(Level-1)>(p)]
      //                 , p
      //                 );
    }

    template<typename Position>
    static BOOST_DISPATCH_FORCE_INLINE
    typename meta::dereference_<Buffer const&,Level>::type
    apply( Buffer const& b, Position const& p )
    {
      typedef typename meta::dereference_<Buffer,1>::type   base_type;
      typedef typename meta::dimensions_of<Buffer>::type    size;
      typedef typename meta::storage_order_of<Buffer>::type so;

      typedef boost::mpl::int_<Level-1> dim;
      std::cout << "Start " << Start << std::endl;
      typedef typename boost::mpl::apply<so,size,dim >::type new_dim;
      //      std::cout <<" S-1-D =" << size::value <<" - " << 1 << " - " << dim::value << std::endl;
      //      std::cout << "pos[" << dim::value << "]: "<<safe_at_c<new_dim::value>(p) << std::endl;
      return  dereference<base_type,Level -1,Start>
              ::apply ( b[safe_at_c<new_dim::value>(p)]
                      , p
                      );

      // return  dereference<base_type,Level-1,Start>
      //         ::apply ( b[safe_at_c<(Level-1)>(p)]
      //                 , p
      //                 );
    }
  };

  template<typename Buffer, std::size_t Start>
  struct dereference<Buffer,1,Start>
  {
    template<typename Position>
    static BOOST_DISPATCH_FORCE_INLINE
    typename meta::dereference_<Buffer&,1>::type
    apply( Buffer& b, Position const& p )
    {

      typedef typename meta::dimensions_of<Buffer>::type size;
      typedef typename meta::storage_order_of<Buffer>::type so;
      typedef boost::mpl::int_<0> dim;
      typedef typename boost::mpl::apply<so,size,dim>::type new_dim;
      std::cout << "Start " << Start << std::endl;
      //      std::cout <<" S-1-D =" << size::value <<" - " << 1 << " - " << dim::value << std::endl;

      //      std::cout << " pos[0]: " << safe_at_c<new_dim::value>(p) << " new_dim::value " << new_dim::value <<std::endl << std::endl;
      return b[safe_at_c<new_dim::value>(p)];
      //return b[safe_at_c<0>(p)];
    }

    template<typename Position>
    static BOOST_DISPATCH_FORCE_INLINE
    typename meta::dereference_<Buffer const&,1>::type
    apply( Buffer const& b, Position const& p )
    {

      typedef typename meta::dimensions_of<Buffer>::type size;
      typedef typename meta::storage_order_of<Buffer>::type so;
      typedef boost::mpl::int_<0> dim;
      typedef typename boost::mpl::apply<so,size,dim>::type new_dim;
      std::cout << "Start " << Start << std::endl;
      //      std::cout <<" S-1-D =" << size::value <<" - " << 1 << " - " << dim::value << std::endl;
      //      std::cout <<" --> p[0] " << safe_at_c<0>(p) << ", p[1] " << safe_at_c<1>(p) << std::endl;

      //      std::cout << " pos[0]: " << safe_at_c<new_dim::value>(p)  << " new_dim::value " << new_dim::value <<std::endl << std::endl;
      return b[safe_at_c<new_dim::value>(p)];
      //return b[safe_at_c<0>(p)];
    }
  };
} }

namespace nt2 { namespace memory
{
  //============================================================================
  /**
    *
   **/
  //============================================================================
  template<std::size_t Level, typename Buffer, typename Position>
  BOOST_DISPATCH_FORCE_INLINE
  typename meta::dereference_<Buffer&,Level>::type
  dereference( Buffer& b, Position const& p )
  {
    nt2::details::check_all_equal(nt2::details::pop_front_c<Level>(p), 1);
    return nt2::details::dereference<Buffer,Level,Level>::apply(b,p);
  }

  template<std::size_t Level, typename Buffer, typename Position>
  BOOST_DISPATCH_FORCE_INLINE
  typename meta::dereference_<Buffer const&,Level>::type
  dereference( Buffer const& b, Position const& p )
  {
    nt2::details::check_all_equal(nt2::details::pop_front_c<Level>(p), 1);
    return nt2::details::dereference<Buffer,Level,Level>::apply(b,p);
  }
} }

#endif
