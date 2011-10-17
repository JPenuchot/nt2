#include <iostream>
#include <nt2/sdk/details/type_id.hpp>

#include <nt2/sdk/timing/ctic.hpp>
#include <nt2/include/functions/cos.hpp>
#include <nt2/include/functions/fast_cos.hpp>
#include <nt2/include/functions/of_size.hpp>
#include <nt2/core/container/table/table.hpp>
#include <nt2/toolbox/operator/functions.hpp>
#include <nt2/include/functions/function.hpp>

#include <boost/simd/sdk/simd/meta/vector_of.hpp>
#include <boost/simd/sdk/simd/meta/native_cardinal.hpp>
#include <boost/simd/sdk/simd/meta/is_vectorizable.hpp>

/*
 for(int y=1;y<=d1;++y)
  {
    for(int x=1;x<=d0;++x)
    {    
      boost::array<int,2> p = {{x,y}};
      std::cout << (boost::proto::value(b).data())(p) << " ";       
    }
    std::cout << "\n";
  } 
*/
namespace nt2 { namespace ext
{
  // terminal, does load
  NT2_FUNCTOR_IMPLEMENTATION( nt2::tag::terminal_, tag::cpu_
                                , (A0)(S0)(State)(Data)
                                , ((ast_<table_< unspecified_<A0>, S0 > >))
                                  (fusion_sequence_<State>)
                                  (unspecified_<Data>)
                                )
  {
    typedef typename boost::dispatch::meta::
    scalar_of< typename boost::dispatch::meta::
               semantic_of<A0>::type
             >::type                                       result_type;
    
    result_type operator()(A0 const& a0, State const& state, Data const& ) const
    {
       return (boost::proto::value(a0))(state);    
    }
  };

  NT2_FUNCTOR_IMPLEMENTATION( nt2::tag::terminal_, tag::cpu_
                            , (A0)(State)(Data)
                            , ((ast_<scalar_< unspecified_<A0> > >))
                              (fusion_sequence_<State>)
                              (unspecified_<Data>)
                            )
  {
    typedef typename boost::dispatch::meta::
    scalar_of< typename boost::dispatch::meta::
               semantic_of<A0>::type
             >::type                                       result_type;

    template<class A0_> BOOST_DISPATCH_FORCE_INLINE
    result_type operator()(A0_& a0, State const& state, Data const&) const
    {
       return boost::proto::value(a0);
    }
  };
} }


#include <vector>

template< class T
        , bool Enable = boost::simd::
                        meta::is_vectorizable<T,BOOST_SIMD_DEFAULT_EXTENSION>::value
        > 
struct evaluation_type
{
  typedef nt2::meta::as_<T> type;
};

template<class T> 
struct  evaluation_type<T, true> 
{
  typedef typename boost::simd::meta::
          vector_of<T, boost::simd::meta::native_cardinal<T>::value>::type base;
  typedef nt2::meta::as_<base>                                             type;
};

struct foo { int i; int j; };

int main()
{
  using nt2::container::table;
    
#if 0
  int iter, d0,d1;
  std::cin >> iter >> d0 >> d1;
  
  std::cout << "std::cos applied on linear std::vector:\n";
  {
    std::vector<double> cc;
    std::vector<double> a(d0*d1),b(d0*d1);
    for(int x=0;x<d0*d1;++x) b[x] = a[x] = 0.;

    for(int i=0;i<iter;++i)
    {
      nt2::ctic();
      for(int x=0;x<d0*d1;++x)
          b[x] = std::cos(a[x]);
      cc.push_back(nt2::ctoc(false)/(1.*d0*d1));
    }

    std::cout << b[0] << " " << b[d0*d1/2-1] << " " << b[b.size()-1] << "\n";
    std::sort(cc.begin(),cc.end());
    std::cout << cc[cc.size()/2 - 1] << " c/elements\n";
  }

  {
    std::vector<double> cc;
    std::vector<double> a(d0*d1),b(d0*d1);
    for(int x=0;x<d0*d1;++x) b[x] = a[x] = 0.;
    std::cout << "nt2::cos applied on linear std::vector:\n";

    for(int i=0;i<iter;++i)
    {
      nt2::ctic();
      for(int x=0;x<d0*d1;++x)
          b[x] = nt2::cos(a[x]);
      cc.push_back(nt2::ctoc(false)/(1.*d0*d1));
    }

    std::cout << b[0] << " " << b[d0*d1/2-1] << " " << b[b.size()-1] << "\n";
    std::sort(cc.begin(),cc.end());
    std::cout << cc[cc.size()/2 - 1] << " c/elements\n";
  }

  std::cout << "nt2::cos applied on nt2::table:\n";
  {
    std::vector<double> cc;

    table<float,nt2::_2D> b( nt2::of_size(d0,d1) ), a( nt2::of_size(d0,d1) );

    for(int y=1;y<=d1;++y)
    {
      for(int x=1;x<=d0;++x)
      {    
        boost::array<int,2> p = {{x,y}}; 
        (boost::proto::value(a).data())(p) = 0;
      }
    } 

    for(int i=0;i<iter;++i)
    {
      nt2::ctic();
      b = nt2::cos(a);
      cc.push_back(nt2::ctoc(false)/(1.*d0*d1));
    }
    
    std::sort(cc.begin(),cc.end());
    std::cout << cc[cc.size()/2 - 1] << " c/elements\n";
  }

  a = -(b + c*(a+(c * -c))) + b;
  a = b + c;
  a = -b;
  b += c;
  c += -a;
  //c(a) += a;                // function undefined
  //c(a) = b(c) + c(a)*b(a);
#endif

  table<double> t;
  nt2::run(nt2::assign(t, t(1)));

}
