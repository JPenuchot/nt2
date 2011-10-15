//==============================================================================
//         Copyright 2003 - 2011   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2011   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_CORE_SETTINGS_DETAILS_OF_SIZE_HPP_INCLUDED
#define NT2_CORE_SETTINGS_DETAILS_OF_SIZE_HPP_INCLUDED

#include <nt2/core/settings/size.hpp>
#include <nt2/core/settings/details/of_size_meta.hpp>
#include <nt2/sdk/memory/aligned_type.hpp>
#include <nt2/sdk/memory/meta/align_on.hpp>
#include <nt2/sdk/parameters.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/array.hpp>
#include <boost/fusion/adapted/boost_array.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <cstddef>

namespace nt2
{
  //============================================================================
  /*! of_size_<D0,..,Dn> is a size value containing up to n dimensions.
   **/
  //============================================================================
  template< BOOST_PP_ENUM_PARAMS ( NT2_MAX_DIMENSIONS, std::ptrdiff_t D) >
  struct of_size_
  {
    typedef boost::fusion::boost_array_tag fusion_tag;

    typedef std::size_t         value_type;
    typedef std::size_t&        reference;
    typedef std::size_t const&  const_reference;
    typedef std::size_t*        iterator;
    typedef std::size_t const*  const_iterator;

    //==========================================================================
    // Count non-trivial size values
    //==========================================================================
    #define M0(z,n,t)                                                          \
    : BOOST_PP_CAT(D, BOOST_PP_DEC(BOOST_PP_SUB(NT2_MAX_DIMENSIONS, n))) != 1  \
    ? BOOST_PP_SUB(NT2_MAX_DIMENSIONS, n)

    static const std::size_t
    static_size = 0 ? 0 BOOST_PP_REPEAT(NT2_MAX_DIMENSIONS,M0,~) : 0;
    #undef M0

    //==========================================================================
    // Check if size is entirely known at compile-time
    //==========================================================================
    #define M0(z,n,t) && (BOOST_PP_CAT(D,n) >= 0)
    static const bool
    static_status = (D0 >= 0)
                    BOOST_PP_REPEAT_FROM_TO(1,NT2_MAX_DIMENSIONS,M0,~);
    #undef M0

    //==========================================================================
    // Compute its potential compile-time numel
    //==========================================================================
    #define M0(z,n,t) * (BOOST_PP_CAT(D,n) >= 0 ? BOOST_PP_CAT(D,n) : 1)
    static const std::size_t
    static_numel = (D0 >= 0 ? D0 : 1)
                    BOOST_PP_REPEAT_FROM_TO(1,NT2_MAX_DIMENSIONS,M0,~);
    #undef M0

    //==========================================================================
    // Static size values used internally by MPL/Fusion
    //==========================================================================
    typedef boost::mpl::
            vector_c< std::ptrdiff_t
                    , BOOST_PP_ENUM_PARAMS(NT2_MAX_DIMENSIONS,D)
                    >                                               values_type;

    //==========================================================================
    // Size values storage
    //==========================================================================
    boost::array<std::size_t,static_size> data_;

    //==========================================================================
    // Default constructor either generate [0 1 .. 1] or [D0 ... Dn]
    //==========================================================================
    of_size_()
    {
      data_[0] = (D0 < 0 ) ? 0 : D0;
      default_(boost::mpl::size_t<static_size-1>());
    }

    std::size_t& operator[](std::size_t i)       { return data_[i]; }
    std::size_t  operator[](std::size_t i) const { return data_[i]; }

    iterator        begin()       { return &data_[0];               }
    const_iterator  begin() const { return &data_[0];               }
    iterator        end()         { return &data_[0] + static_size; }
    const_iterator  end()   const { return &data_[0] + static_size; }

    static std::size_t size() { return static_size; }

    boost::array<std::size_t,static_size> const& data() const { return data_; }

    private:

    template<std::size_t N> inline void default_(boost::mpl::size_t<N> const&)
    {
      typedef typename boost::mpl::at_c<values_type,N>::type value;
      data_[N] = (value::value < 0) ? 1U : value::value;
      default_(boost::mpl::size_t<N-1>());
    }

    inline void default_(boost::mpl::size_t<0> const&) {}
  };

  //============================================================================
  // Specialisation for _0D case
  //============================================================================
  template<> struct of_size_<>
  {
    typedef std::size_t value_type;
    typedef std::size_t reference;
    typedef std::size_t const_reference;
    typedef void*       iterator;
    typedef void*       const_iterator;

    static const std::size_t  static_size   = 0;
    static const bool         static_status = true;
    static const std::size_t  static_numel  = 0;

    static std::size_t size() { return 0; }
    const_reference  operator[](std::size_t i) const { return 1; }

    iterator        begin()       { return iterator(0);       }
    const_iterator  begin() const { return const_iterator(0); }
    iterator        end()         { return iterator(0);       }
    const_iterator  end()   const { return const_iterator(0); }
  };

  //============================================================================
  // Defines some usual short-cuts for runtime of_size_
  //============================================================================
  #define M1(z,n,t) t
  #define M0(z,n,t)                                                           \
  typedef of_size_<BOOST_PP_ENUM(n,M1,-1)> BOOST_PP_CAT(BOOST_PP_CAT(_,n),D); \
  /**/

  BOOST_PP_REPEAT_FROM_TO(0,BOOST_PP_INC(NT2_MAX_DIMENSIONS),M0,~)

  #undef M0
  #undef M1

  template< BOOST_PP_ENUM_PARAMS(NT2_MAX_DIMENSIONS, std::ptrdiff_t D1)
          , BOOST_PP_ENUM_PARAMS(NT2_MAX_DIMENSIONS, std::ptrdiff_t D2)>
  bool operator==( of_size_<BOOST_PP_ENUM_PARAMS(NT2_MAX_DIMENSIONS, D1)> const& a0
                 , of_size_<BOOST_PP_ENUM_PARAMS(NT2_MAX_DIMENSIONS, D2)> const& a1
                 )
  {
    std::size_t const* it0 = a0.begin();
    std::size_t const* it1 = a1.begin();

    for(; it0 != a0.end() && it1 != a1.end(); ++it0, ++it1)
      if(*it0 != *it1)
        return false;

    return it0 == a0.end() && it1 == a1.end();
  }
}

#endif
