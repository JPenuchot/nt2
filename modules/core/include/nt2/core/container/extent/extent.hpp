//==============================================================================
//         Copyright 2003 - 2011   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2011   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//=============================================================================
#ifndef NT2_CORE_CONTAINER_EXTENT_EXTENT_HPP_INCLUDED
#define NT2_CORE_CONTAINER_EXTENT_EXTENT_HPP_INCLUDED

/*!
 * \file
 */

#include <boost/mpl/assert.hpp>
#include <nt2/sdk/parameters.hpp>
#include <nt2/core/container/dsl.hpp>
#include <nt2/core/settings/size.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>

namespace nt2 { namespace tag
{
  //============================================================================
  /*! Tag representing an extent terminal in proto abstract syntax tree      **/
  //============================================================================
  struct extent_ {};
} }

namespace nt2 { namespace container
{
  //============================================================================
  /*!
   * \ingroup containershelper
   *
   * \ref extent is a \container representing the dimension set of another
   * container. It acts as a \container of integers where the ith value is the
   * number of element stored by a container along its ith dimensions.
   *
   * \tparam Dimensions Type representing the kind of \ref extent defined.
   * \ref extent can be defined using:
   *  - \ref _2D, ..., \ref _4D to define a dynamic \ref extent where values
   *  can be modified.
   *  - \ref of_size_ to define a static \ref extent where values are specified
   *  as compile time integral constants.
   *
   * \par Rationale:
   *
   * In \matlab, size of a container is itself a \matlab container. Due to the
   * lack of notion of constructors for matrix in \matlab, no ambiguity arises
   * from this fact. In \nt2 however, if we want to have both containers being
   * constructed from a size (or \ref extent) or another \container, we need to
   * have a dedicated type to represent size of a \container.
   * \ref extent represents such a type and is designed to fulfill \container
   * concept requirement and be discriminated in other \container constructors
   * call.
   *
   * \par Models:
   *
   * \container
   *
   * \see
   *
   * of_size_
   */
  //============================================================================
  template<typename Sizes, typename Dummy>
  struct  extent
        : expression< typename boost::proto::terminal<Sizes>::type
                    , Sizes
                    >
  {
    //==========================================================================
    // Container type definition
    //==========================================================================
    typedef typename Sizes::value_type      value_type;
    typedef typename Sizes::reference       reference;
    typedef typename Sizes::const_reference const_reference;
    typedef typename Sizes::iterator        iterator;
    typedef typename Sizes::const_iterator  const_iterator;    
    typedef std::size_t                     size_type;
    typedef std::size_t                     base_type;
    typedef std::size_t                     difference_type;

    //==========================================================================
    typedef nt2::tag::container_ fusion_tag;

    //==========================================================================
    /*! Integral constant carrying the number of dimensions of current extent */
    //==========================================================================
    static const size_t static_dimensions = Sizes::static_size;

    static const size_t static_status = Sizes::static_status;

    //==========================================================================
    /*! Type of the parent expression                                         */
    //==========================================================================
    typedef expression< typename boost::proto::terminal<Sizes>::type
                      , Sizes
                      >                                             parent;

    //==========================================================================
    /*!
     * \ref extent default constructor build an \ref extent which is :
     *  - filled with the static size it represents if it has been instanciated
     *  from an static size descriptor (see \ref of_size_).
     *  - a representation of an empty \ref extent, ie an \ref extent where
     *  all dimensions are equal to one, except for the first one which is equal
     *  to zero.
     *
     *  \usage
     *
     *  \code
     *  #include <nt2/core/container/extent.hpp>
     *
     *  int main()
     *  {
     *    nt2::container::extent<nt2::_2D> x;
     *    assert( x(1) == 0 && x(2) == 1 );
     *
     *    nt2::container::extent<nt2::of_size_<3,4,5> > y;
     *    assert( y(1) == 3 && y(2) == 4 && y(3) == 5 );
     *  }
     *  \endcode
     */
    //==========================================================================
    extent() {}

    //==========================================================================
    /*!
     * \ref extent copy constructor. If the \ref extent being constructed is a
     * static \ref extent, a static assert is triggered.
     *
     * \param src  \ref extent to be copied
     *
     * \usage
     *
     * \code
     * #include <nt2/core/container/extent.hpp>
     *
     * int main()
     * {
     *   nt2::container::extent<nt2::_2D> x(3,7);
     *   nt2::container::extent<nt2::_2D> y(x);
     *   assert( y(1) == 3 && y(2) == 7 );
     *  }
     *  \endcode
     */
    //==========================================================================
    inline extent( extent const& src )
    {
      boost::proto::value(*this) = boost::proto::value(src);
    }

    #if defined(DOXYGEN_ONLY)
    //==========================================================================
    /*!
     * \ref extent constructor from a list of integral values initializes its
     * nth first elements and fill the remaining ones with 1.
     *
     * \param values List of integral values to use for initialization
     *
     * \usage
     *
     * \code
     * #include <nt2/core/container/extent.hpp>
     *
     * int main()
     * {
     *   nt2::container::extent<nt2::_3D> x(3,7,5);
     *   assert( y(1) == 3 && y(2) == 7 && y(3) == 5 );
     *  }
     *  \endcode
     */
    //==========================================================================
    template<class ...Values> inline explicit extent( Values... const& values );
    #endif

    //==========================================================================
    #define M1(z,n,t) boost::proto::value(*this)[n]= BOOST_PP_CAT(d,n); \
    /**/

    #define M0(z,n,t)                                                             \
    private:                                                                      \
    template<BOOST_PP_ENUM_PARAMS(n,class U)> inline void                         \
    init( BOOST_PP_ENUM_BINARY_PARAMS(n,const U,& d), boost::mpl::false_ const&)  \
    {                                                                             \
      BOOST_PP_REPEAT(n,M1,~)                                                     \
      for(std::size_t i=n;i<static_dimensions;++i)                                \
        boost::proto::value(*this)[i] = 1;                                        \
    }                                                                             \
                                                                                  \
    template<BOOST_PP_ENUM_PARAMS(n,class U)> inline void                         \
    init( BOOST_PP_ENUM_BINARY_PARAMS(n,const U,& d), boost::mpl::true_ const& )  \
    {                                                                             \
      BOOST_MPL_ASSERT_MSG( !Sizes::static_status                                 \
                          , STATIC_EXTENT_CANT_BE_VALUE_INITIALIZED               \
                          , (Sizes)                                               \
                          );                                                      \
    }                                                                             \
    public:                                                                       \
    template<BOOST_PP_ENUM_PARAMS(n,class U)> inline explicit                     \
    extent( BOOST_PP_ENUM_BINARY_PARAMS(n,const U, & d)                           \
          , typename  boost::                                                     \
                      enable_if_c<boost::is_integral<U0>::value>::type* = 0       \
          )                                                                       \
    {                                                                             \
      init(BOOST_PP_ENUM_PARAMS(n,d), boost::mpl::bool_<Sizes::static_status>()); \
    }                                                                             \
    /**/

    BOOST_PP_REPEAT_FROM_TO(1,BOOST_PP_INC(NT2_MAX_DIMENSIONS),M0,~)

    #undef M0
    #undef M1
    //==========================================================================
    
    //==========================================================================
        /*!
     * \ref extent constructor from an arbitrary container expression.
     *
     * \usage
     *
     * \code
     * #include <nt2/core/container/extent.hpp>
     *
     * int main()
     * {
     *   nt2::container::extent<nt2::_3D> x(3,7,5);
     *   nt2::container::extent<nt2::_3D> y(x+2);
     *   assert( y(1) == 5 && y(2) == 9 && y(3) == 7 );
     *  }
     *  \endcode
     */
    //==========================================================================
    template<class Xpr,class Result>
    BOOST_DISPATCH_FORCE_INLINE
    extent(expression<Xpr,Result> const& xpr) { *this = xpr; }

    template<class Xpr,class Result>
    BOOST_DISPATCH_FORCE_INLINE 
    extent& operator=(expression<Xpr,Result> const& xpr)
    {
      // Check the affectation is right
      nt2::evaluate( nt2::assign(*this, xpr) );
      return *this;
    }

    //==========================================================================
    /**!
     * Acces to the ith element of the extent which represents 
     **/
    //==========================================================================
    reference operator()(std::size_t i)
    {
      return boost::proto::value(*this)[i-1];
    }

    const_reference operator()(std::size_t i) const
    {
      return boost::proto::value(*this)[i-1];
    }

    //==========================================================================
    /*! Return the number of dimensions stored in the extent.                 */
    //==========================================================================
    inline size_type size() const { return static_dimensions; }

    //==========================================================================
    /*! Return if an \ref extent is empty.                                    */
    //==========================================================================
    inline bool empty() const { return (size() == 0); }

    //==========================================================================
    /*!
     * Return the lowest valid index to access the ith dimension of current \ref
     * extent.
     *
     * \param i Dimension to retrieve the lower index from.
     * \return The lowest valid index of the \ref extent along this dimension.
     *
     * \see Container
     */
    //==========================================================================
    inline base_type lower(std::size_t i) const { return 1; }

     //==========================================================================
    /*!
     * Return the highest valid index to access the ith dimension of current \ref
     * extent.
     *
     * \param i Dimension to retrieve the lower index from.
     * \return The highest valid index of the \ref extent along this dimension.
     *
     * \see Container
     */
    //==========================================================================
    inline difference_type upper(std::size_t i) const
    {
      return (i==2 && static_dimensions) ? static_dimensions : 1;
    }

    //==========================================================================
    /*!
     * Return the value of the size of current \ref extent along the ith
     * dimension.
     *
     * \param i Dimension to retrieve the size from.
     * \return The size of the \ref extent along this dimension.
     */
    //==========================================================================
    inline size_type  size(std::size_t i) const
    {
      return (i==2 && static_dimensions) ? static_dimensions : 1;
    }

    iterator        begin()       { return boost::proto::value(*this).begin(); }
    const_iterator  begin() const { return boost::proto::value(*this).begin(); }

    iterator        end()         { return boost::proto::value(*this).end();  }
    const_iterator  end()   const { return boost::proto::value(*this).end();  }
  };
} }

//==============================================================================
// Give extent a semantic 
//==============================================================================
namespace boost { namespace dispatch { namespace meta
{
  template<typename Sizes>
  struct  semantic_of< nt2::container::extent<Sizes> >
        : semantic_of< typename nt2::container::extent<Sizes>::parent >
  {
  };
} } }

#endif
