//==============================================================================
//         Copyright 2003 - 2011   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2011   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_CORE_FUNCTIONS_SIMD_OUTER_FOLD_HPP_INCLUDED
#define NT2_CORE_FUNCTIONS_SIMD_OUTER_FOLD_HPP_INCLUDED

#include <nt2/core/functions/outer_fold.hpp>
#include <boost/fusion/include/pop_front.hpp>
#include <boost/fusion/include/pop_back.hpp>
#include <nt2/sdk/config/cache.hpp>


namespace nt2 { namespace ext
{
  //============================================================================
  // Generates outer_fold
  //============================================================================
  NT2_FUNCTOR_IMPLEMENTATION( nt2::tag::outer_fold_, boost::simd::tag::simd_, (A0)(S0)(A1)(A2)(A3)(A4)
                            , ((expr_< table_< unspecified_<A0>, S0 >
                                     , nt2::tag::terminal_
                                     , boost::mpl::long_<0>
                                     >
                              ))
                              (ast_< A1>)
                              (unspecified_<A2>)
                              (unspecified_<A3>)
                              (unspecified_<A4>)
                            )
  {
    typedef void                                                              result_type;
    typedef typename A0::value_type                                           value_type;
    typedef typename boost::remove_reference<A1>::type::extent_type           extent_type;
    typedef boost::simd::native<value_type,BOOST_SIMD_DEFAULT_EXTENSION>      target_type;

    BOOST_FORCEINLINE result_type operator()(A0& out, A1& in, A2 const& neutral, A3 const& bop, A4 const& uop) const
    {
      extent_type ext = in.extent();
      static const std::ptrdiff_t N = boost::simd::meta::cardinal_of<target_type>::value;
      typename A0::index_type::type bs;
      std::ptrdiff_t ilow   = boost::fusion::at_c<0>(bs);// it's not 0 it's ext.size() -1;
      std::ptrdiff_t olow   = boost::fusion::at_c<0>(bs);
      std::ptrdiff_t ibound = ilow + ext[ext.size()-1]; 


      //std::ptrdiff_t numel  = nt2::numel(boost::fusion::pop_back(ext));
      
      // Workaround to have nt2::numel(boost::fusion::pop_back(ext));
      std::ptrdiff_t numel  = 1;
      for(std::ptrdiff_t m = 0; m!= ext.size()-1 ; ++m)
        numel*=ext[m];

      std::ptrdiff_t obound = olow + (numel);

      std::ptrdiff_t cache_line_size = nt2::config::cache_line_size(2); // in byte
      std::ptrdiff_t nb_vec = cache_line_size/(sizeof(value_type)*N);
      std::ptrdiff_t cache_bound = (nb_vec)*N;
      std::ptrdiff_t bound  = olow + ((numel)/cache_bound) * cache_bound;


      // std::cout << "numel = " << numel << "\tnb_vec = " << nb_vec << "\tN = "<<N << "\t sizeof(value_type) = "<<sizeof(value_type) <<"\n";
      // std::cout << "cache_bound = " << cache_bound << "\tbound = " << bound << "\tobound = " << obound << "\n";

      std::ptrdiff_t new_dim = 1;

      if(numel >= cache_bound){
      for(std::ptrdiff_t j=olow; j!=bound; j+=cache_bound)
        {
          //Initialise 
          for(std::ptrdiff_t k=0, id=j; k!=nb_vec; ++k, id+=N){
            nt2::run(out, as_aligned(boost::fusion::vector_tie(id,new_dim)), meta::as_<target_type>()) = neutral(nt2::meta::as_<target_type>());
          }


          for(std::ptrdiff_t i=ilow; i!=ibound; ++i){
            for(std::ptrdiff_t k=0, id=j; k!=nb_vec; ++k, id+=N){

              nt2::run(out, as_aligned(boost::fusion::vector_tie(id,new_dim)),
                bop(nt2::run(out, as_aligned(boost::fusion::vector_tie(id,new_dim)), meta::as_<target_type>())
                    ,nt2::run(in, as_aligned(boost::fusion::vector_tie(id,i)), meta::as_<target_type>())));
              
            }
          }
            
        }

      // scalar part
      for(std::ptrdiff_t j=bound; j!=obound; ++j){ 
          out(j,1) = neutral(nt2::meta::as_<value_type>());
          for(std::ptrdiff_t i=ilow; i!=ibound; ++i){
            out(j,1) = bop(out(j,1), nt2::run(in, boost::fusion::vector_tie(j,i), meta::as_<value_type>()));
          }
        }
      }

      else {
        for(std::ptrdiff_t j=olow; j!=obound; ++j){ 
          out(j,1) = neutral(nt2::meta::as_<value_type>());
          for(std::ptrdiff_t i=ilow; i!=ibound; ++i){
            out(j,1) = bop(out(j,1), nt2::run(in, boost::fusion::vector_tie(j,i), meta::as_<value_type>()));
          }
        }
      }

    }
  };

} }

#endif
