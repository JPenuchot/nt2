////////////////////////////////////////////////////////////////////////////////
// Register all tag and extension agnostic call for common code sharing
////////////////////////////////////////////////////////////////////////////////
namespace boost { namespace simd { namespace ext
{
  } } } namespace boost { namespace dispatch { namespace meta { template< class Tag , class A0 > BOOST_FORCEINLINE typename boost::enable_if< mpl::not_< any < mpl::or_ < boost::proto:: is_expr<mpl::_> , boost::dispatch:: meta::is_proxy<mpl::_> > , A0 > > , :: boost :: simd :: ext :: implement< elementwise_<Tag> ( unspecified_<A0> ) , tag::formal_ > >::type dispatching( elementwise_<Tag>, tag::formal_ , unspecified_<A0> const , adl_helper = adl_helper() ) { return :: boost :: simd :: ext :: implement< elementwise_<Tag> ( unspecified_<A0> ) , tag::formal_ >(); } } } } namespace boost { namespace simd { namespace ext {
  template<class A0,class Tag, class Dummy>
  struct implement< elementwise_<Tag>( unspecified_<A0> )
                  , tag::formal_, Dummy
                  >
  {
    BOOST_SIMD_MAP_LOG(Tag)
    typedef typename boost::dispatch::meta::
       call<tag::map_ ( dispatch::functor<Tag>
                      , A0
                      )
           >::type
    result_type;
    BOOST_FORCEINLINE result_type
    operator()(A0 const & a0) const
    {
      return boost::simd::
             map( dispatch::functor<Tag>(), a0 );
    }
  };
  } } } namespace boost { namespace dispatch { namespace meta { template< class Tag , class A0 , class A1 > BOOST_FORCEINLINE typename boost::enable_if< mpl::not_< any < mpl::or_ < boost::proto:: is_expr<mpl::_> , boost::dispatch:: meta::is_proxy<mpl::_> > , A0 , A1 > > , :: boost :: simd :: ext :: implement< elementwise_<Tag> ( unspecified_<A0> , unspecified_<A1> ) , tag::formal_ > >::type dispatching( elementwise_<Tag>, tag::formal_ , unspecified_<A0> const , unspecified_<A1> const , adl_helper = adl_helper() ) { return :: boost :: simd :: ext :: implement< elementwise_<Tag> ( unspecified_<A0> , unspecified_<A1> ) , tag::formal_ >(); } } } } namespace boost { namespace simd { namespace ext {
  template<class A0 , class A1,class Tag, class Dummy>
  struct implement< elementwise_<Tag>( unspecified_<A0> , unspecified_<A1> )
                  , tag::formal_, Dummy
                  >
  {
    BOOST_SIMD_MAP_LOG(Tag)
    typedef typename boost::dispatch::meta::
       call<tag::map_ ( dispatch::functor<Tag>
                      , A0 , A1
                      )
           >::type
    result_type;
    BOOST_FORCEINLINE result_type
    operator()(A0 const & a0 , A1 const & a1) const
    {
      return boost::simd::
             map( dispatch::functor<Tag>(), a0 , a1 );
    }
  };
  } } } namespace boost { namespace dispatch { namespace meta { template< class Tag , class A0 , class A1 , class A2 > BOOST_FORCEINLINE typename boost::enable_if< mpl::not_< any < mpl::or_ < boost::proto:: is_expr<mpl::_> , boost::dispatch:: meta::is_proxy<mpl::_> > , A0 , A1 , A2 > > , :: boost :: simd :: ext :: implement< elementwise_<Tag> ( unspecified_<A0> , unspecified_<A1> , unspecified_<A2> ) , tag::formal_ > >::type dispatching( elementwise_<Tag>, tag::formal_ , unspecified_<A0> const , unspecified_<A1> const , unspecified_<A2> const , adl_helper = adl_helper() ) { return :: boost :: simd :: ext :: implement< elementwise_<Tag> ( unspecified_<A0> , unspecified_<A1> , unspecified_<A2> ) , tag::formal_ >(); } } } } namespace boost { namespace simd { namespace ext {
  template<class A0 , class A1 , class A2,class Tag, class Dummy>
  struct implement< elementwise_<Tag>( unspecified_<A0> , unspecified_<A1> , unspecified_<A2> )
                  , tag::formal_, Dummy
                  >
  {
    BOOST_SIMD_MAP_LOG(Tag)
    typedef typename boost::dispatch::meta::
       call<tag::map_ ( dispatch::functor<Tag>
                      , A0 , A1 , A2
                      )
           >::type
    result_type;
    BOOST_FORCEINLINE result_type
    operator()(A0 const & a0 , A1 const & a1 , A2 const & a2) const
    {
      return boost::simd::
             map( dispatch::functor<Tag>(), a0 , a1 , a2 );
    }
  };
  } } } namespace boost { namespace dispatch { namespace meta { template< class Tag , class A0 , class A1 , class A2 , class A3 > BOOST_FORCEINLINE typename boost::enable_if< mpl::not_< any < mpl::or_ < boost::proto:: is_expr<mpl::_> , boost::dispatch:: meta::is_proxy<mpl::_> > , A0 , A1 , A2 , A3 > > , :: boost :: simd :: ext :: implement< elementwise_<Tag> ( unspecified_<A0> , unspecified_<A1> , unspecified_<A2> , unspecified_<A3> ) , tag::formal_ > >::type dispatching( elementwise_<Tag>, tag::formal_ , unspecified_<A0> const , unspecified_<A1> const , unspecified_<A2> const , unspecified_<A3> const , adl_helper = adl_helper() ) { return :: boost :: simd :: ext :: implement< elementwise_<Tag> ( unspecified_<A0> , unspecified_<A1> , unspecified_<A2> , unspecified_<A3> ) , tag::formal_ >(); } } } } namespace boost { namespace simd { namespace ext {
  template<class A0 , class A1 , class A2 , class A3,class Tag, class Dummy>
  struct implement< elementwise_<Tag>( unspecified_<A0> , unspecified_<A1> , unspecified_<A2> , unspecified_<A3> )
                  , tag::formal_, Dummy
                  >
  {
    BOOST_SIMD_MAP_LOG(Tag)
    typedef typename boost::dispatch::meta::
       call<tag::map_ ( dispatch::functor<Tag>
                      , A0 , A1 , A2 , A3
                      )
           >::type
    result_type;
    BOOST_FORCEINLINE result_type
    operator()(A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3) const
    {
      return boost::simd::
             map( dispatch::functor<Tag>(), a0 , a1 , a2 , a3 );
    }
  };
  } } } namespace boost { namespace dispatch { namespace meta { template< class Tag , class A0 , class A1 , class A2 , class A3 , class A4 > BOOST_FORCEINLINE typename boost::enable_if< mpl::not_< any < mpl::or_ < boost::proto:: is_expr<mpl::_> , boost::dispatch:: meta::is_proxy<mpl::_> > , A0 , A1 , A2 , A3 , A4 > > , :: boost :: simd :: ext :: implement< elementwise_<Tag> ( unspecified_<A0> , unspecified_<A1> , unspecified_<A2> , unspecified_<A3> , unspecified_<A4> ) , tag::formal_ > >::type dispatching( elementwise_<Tag>, tag::formal_ , unspecified_<A0> const , unspecified_<A1> const , unspecified_<A2> const , unspecified_<A3> const , unspecified_<A4> const , adl_helper = adl_helper() ) { return :: boost :: simd :: ext :: implement< elementwise_<Tag> ( unspecified_<A0> , unspecified_<A1> , unspecified_<A2> , unspecified_<A3> , unspecified_<A4> ) , tag::formal_ >(); } } } } namespace boost { namespace simd { namespace ext {
  template<class A0 , class A1 , class A2 , class A3 , class A4,class Tag, class Dummy>
  struct implement< elementwise_<Tag>( unspecified_<A0> , unspecified_<A1> , unspecified_<A2> , unspecified_<A3> , unspecified_<A4> )
                  , tag::formal_, Dummy
                  >
  {
    BOOST_SIMD_MAP_LOG(Tag)
    typedef typename boost::dispatch::meta::
       call<tag::map_ ( dispatch::functor<Tag>
                      , A0 , A1 , A2 , A3 , A4
                      )
           >::type
    result_type;
    BOOST_FORCEINLINE result_type
    operator()(A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4) const
    {
      return boost::simd::
             map( dispatch::functor<Tag>(), a0 , a1 , a2 , a3 , a4 );
    }
  };
  } } } namespace boost { namespace dispatch { namespace meta { template< class Tag , class A0 , class A1 , class A2 , class A3 , class A4 , class A5 > BOOST_FORCEINLINE typename boost::enable_if< mpl::not_< any < mpl::or_ < boost::proto:: is_expr<mpl::_> , boost::dispatch:: meta::is_proxy<mpl::_> > , A0 , A1 , A2 , A3 , A4 , A5 > > , :: boost :: simd :: ext :: implement< elementwise_<Tag> ( unspecified_<A0> , unspecified_<A1> , unspecified_<A2> , unspecified_<A3> , unspecified_<A4> , unspecified_<A5> ) , tag::formal_ > >::type dispatching( elementwise_<Tag>, tag::formal_ , unspecified_<A0> const , unspecified_<A1> const , unspecified_<A2> const , unspecified_<A3> const , unspecified_<A4> const , unspecified_<A5> const , adl_helper = adl_helper() ) { return :: boost :: simd :: ext :: implement< elementwise_<Tag> ( unspecified_<A0> , unspecified_<A1> , unspecified_<A2> , unspecified_<A3> , unspecified_<A4> , unspecified_<A5> ) , tag::formal_ >(); } } } } namespace boost { namespace simd { namespace ext {
  template<class A0 , class A1 , class A2 , class A3 , class A4 , class A5,class Tag, class Dummy>
  struct implement< elementwise_<Tag>( unspecified_<A0> , unspecified_<A1> , unspecified_<A2> , unspecified_<A3> , unspecified_<A4> , unspecified_<A5> )
                  , tag::formal_, Dummy
                  >
  {
    BOOST_SIMD_MAP_LOG(Tag)
    typedef typename boost::dispatch::meta::
       call<tag::map_ ( dispatch::functor<Tag>
                      , A0 , A1 , A2 , A3 , A4 , A5
                      )
           >::type
    result_type;
    BOOST_FORCEINLINE result_type
    operator()(A0 const & a0 , A1 const & a1 , A2 const & a2 , A3 const & a3 , A4 const & a4 , A5 const & a5) const
    {
      return boost::simd::
             map( dispatch::functor<Tag>(), a0 , a1 , a2 , a3 , a4 , a5 );
    }
  };
} } }
