//==============================================================================
//         Copyright 2003 - 2011   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2011   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_CORE_FUNCTIONS_TABLE_FUNCTION_HPP_INCLUDED
#define NT2_CORE_FUNCTIONS_TABLE_FUNCTION_HPP_INCLUDED

#include <nt2/include/functor.hpp>
#include <nt2/core/functions/function.hpp>
#include <nt2/core/settings/size.hpp>
#include <nt2/core/container/dsl/generator.hpp>
#include <nt2/core/container/meta/is_colon.hpp>
#include <nt2/dsl/functions/run.hpp>
#include <nt2/include/functions/multiplies.hpp>

#include <boost/dispatch/meta/strip.hpp>
#include <boost/fusion/include/pop_front.hpp>
#include <boost/fusion/include/fold.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/enum.hpp>

namespace nt2 { namespace container
{
  // helper to get static or -1
  template<class T, class Enable = void>
  struct mpl_value
   : boost::mpl::int_<-1>
  {
  };
  
  template<class T>
  struct mpl_value<T, typename boost::enable_if< boost::is_class<typename meta::strip<T>::type> >::type >
   : meta::strip<T>::type
  {
  };
  
  // size deduction per argument
  struct function_size
  {
    template<class Sz, class Children, int N, class Enable = void>
    struct impl
    {
      typedef typename boost::fusion::result_of::
      fold< typename boost::
            remove_reference< typename size_transform::template
                              result<size_transform( typename boost::fusion::result_of::
                                                     at_c< Children
                                                         , N
                                                         >::type
                                                   )
                                    >::type
                            >::type
          , boost::mpl::size_t<1>
          , nt2::functor<tag::multiplies_>
          >::type
      result_type;
      
      BOOST_DISPATCH_FORCE_INLINE
      result_type operator()(Sz, Children children) const
      {
        return boost::fusion::fold(
            size_transform()(boost::fusion::at_c<N>(children))
          , boost::mpl::size_t<1>()
          , nt2::functor<tag::multiplies_>()
          );
      }
    };
    
    template<class Sz, class Children, int N>
    struct impl<Sz, Children, N, typename boost::enable_if<
      meta::is_colon< typename boost::fusion::result_of::
                      at_c<Children, N>::type
                    >
    >::type>
    {
      typedef typename boost::fusion::result_of::
      at_c<Sz, N>::type result_type;
      
      BOOST_DISPATCH_FORCE_INLINE
      result_type operator()(Sz sz, Children) const
      {
        return boost::fusion::at_c<N>(sz);
      }
    };
  };
  
  // build size
  template<int N, class F, class Sizes, class Children>
  struct make_size;
  
  #define M1(z,n,t) mpl_value< typename F::template impl<Sizes, Children, n>::result_type >::value
  #define M2(z,n,t) that[n] = typename F::template impl<Sizes, Children, n>()(sz, children);
  
  #define M3(z,n,t)                                                            \
  template<class Dummy>                                                        \
  struct impl<n, Dummy>                                                        \
  {                                                                            \
    BOOST_DISPATCH_FORCE_INLINE                                                \
    static result_type call(Sizes sz, Children children)                       \
    {                                                                          \
      result_type that;                                                        \
      BOOST_PP_REPEAT(n, M2, ~)                                                \
      return that;                                                             \
    }                                                                          \
  };
  
  #define M0(z,n,t)                                                            \
  template<class F, class Sizes, class Children>                               \
  struct make_size<n, F, Sizes, Children>                                      \
  {                                                                            \
    typedef of_size_<                                                          \
               BOOST_PP_ENUM(n, M1, ~)                                         \
            >            result_type;                                          \
                                                                               \
                                                                               \
    template<int N, class Dummy = void>                                        \
    struct impl;                                                               \
                                                                               \
    BOOST_PP_REPEAT(n, M3, ~)                                                  \
                                                                               \
    BOOST_DISPATCH_FORCE_INLINE                                                \
    result_type operator()(Sizes sz, Children children) const                  \
    {                                                                          \
      return impl<result_type::static_size>::call(sz, children);               \
    }                                                                          \
  };
  
  BOOST_PP_REPEAT(NT2_MAX_DIMENSIONS, M0, ~)
  #undef M0
  #undef M1
  #undef M2
  #undef M3
  
  template<class Expr, int N>
  struct size_impl<tag::function_, N, Expr>
  {
    typedef typename boost::proto::result_of::
    child_c<Expr, 0>::type                             child0;
    
    typedef typename boost::fusion::result_of::
    pop_front<Expr>::type                              childN;
    
    typedef typename size_transform::template
    result<size_transform(child0)>::type               sz;
    
    typedef make_size<N-1, function_size, sz, childN>  impl;
    typedef typename impl::result_type result_type;
    
    BOOST_DISPATCH_FORCE_INLINE
    result_type operator()(Expr& e) const
    {
      return impl()(
        size_transform()(boost::proto::child_c<0>(e))
      , boost::fusion::pop_front(e)
      );
    }
  };
  
  template<class Expr, int N>
  struct generator_impl<tag::function_, N, Expr>
  {
    typedef typename boost::proto::result_of::
    child_c<Expr, 0>::type                          child0;
    
    typedef typename boost::dispatch::meta::
    scalar_of< typename boost::dispatch::meta::
               semantic_of<child0>::type
             >::type                                s0;

    typedef typename meta::
    strip<typename size_impl< boost::proto::tag::
                              function, N, Expr
                            >::result_type>::type   extent_type;

    typedef typename boost::mpl::
    if_< boost::is_same<extent_type, _0D>
       , s0
       , typename boost::dispatch::meta::
         transfer_qualifiers< table_container< typename meta::
                                               strip<s0>::type
                                             , nt2::settings(extent_type)
                                             >
                            , s0
                            >::type
       >::type                                      type;       
    typedef expression<Expr, type>                  result_type;

    BOOST_DISPATCH_FORCE_INLINE
    result_type operator()(Expr& e) const
    {
      return result_type(e, size_transform()(e));
    }
  };
  
  // TODO
  template<class Expr, class State>
  struct function_state
  {
    typedef State result_type;
    BOOST_DISPATCH_FORCE_INLINE
    result_type operator()(Expr const&, State const& state) const
    {
      return state;
    }
  };
  
}    
namespace ext
{
  NT2_FUNCTOR_IMPLEMENTATION( nt2::tag::run_, tag::cpu_
                            , (Expr)
                            , ((expr_< scalar_< unspecified_<Expr> >
                                     , nt2::container::domain
                                     , nt2::tag::function_
                                     >
                              ))
                            )
  {
    typedef typename boost::proto::result_of::child_c<Expr const&, 0>::type  child0;
    typedef typename boost::fusion::result_of::pop_front<Expr const>::type   childN;

    typedef typename boost::dispatch::meta::semantic_of<Expr>::type semantic_type;
    typedef typename meta::as_<typename meta::strip<semantic_type>::type> target;

    typedef typename meta::call<tag::run_(child0, childN, target)>::type     result_type;

    BOOST_DISPATCH_FORCE_INLINE result_type
    operator()(Expr const& expr) const
    {
      return nt2::run( boost::proto::child_c<0>(expr)
                     , boost::fusion::pop_front(expr)
                     , target()
                     );
    }
  };
  
  NT2_FUNCTOR_IMPLEMENTATION( nt2::tag::run_, tag::cpu_
                            , (Expr)(State)(Data)
                            , ((expr_< unspecified_<Expr>
                                     , nt2::container::domain
                                     , nt2::tag::function_
                                     >
                              ))
                              (fusion_sequence_<State>)
                              (unspecified_<Data>)
                            )
  {
    typedef typename boost::proto::result_of::child_c<Expr const&, 0>::type  child0;
    typedef typename boost::fusion::result_of::pop_front<Expr const>::type   childN;
    
    typedef typename container::function_state<Expr, State>::result_type     new_state;
    typedef typename meta::call<tag::run_(child0, new_state, Data)>::type    result_type;
      
    BOOST_DISPATCH_FORCE_INLINE result_type
    operator()(Expr const& expr, State const& state, Data const& data) const
    {
      return nt2::run( boost::proto::child_c<0>(expr)
                     , container::function_state<childN, State>()
                       (boost::fusion::pop_front(expr), state)
                     , data
                     );
    }
  };

} }

#endif
