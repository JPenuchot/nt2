//==============================================================================
//         Copyright 2003 - 2011   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2011   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_CORE_FUNCTIONS_SIMD_INNER_FOLD_HPP_INCLUDED
#define NT2_CORE_FUNCTIONS_SIMD_INNER_FOLD_HPP_INCLUDED

#include <nt2/core/functions/inner_fold.hpp>
#include <boost/simd/sdk/simd/native.hpp>
#include <boost/simd/toolbox/swar/include/functions/splatted_sum.hpp>

namespace nt2 { namespace ext
{
  //============================================================================
  // Generates inner_fold
  //============================================================================
  NT2_FUNCTOR_IMPLEMENTATION( nt2::tag::inner_fold_, boost::simd::tag::simd_, (A0)(S0)(A1)(A2)(A3)
                            , ((expr_< table_< unspecified_<A0>, S0 >
                                     , nt2::tag::terminal_
                                     , boost::mpl::long_<0>
                                     >
                              ))
                              (ast_< A1>)
                              (unspecified_<A2>)
                              (unspecified_<A3>)
                            )
  {
    typedef void                                                               result_type;
    typedef typename A0::value_type                                            value_type;
    typedef typename boost::remove_reference<A1>::type::extent_type            extent_type;
    typedef BOOST_SIMD_DEFAULT_EXTENSION                                       extension_type;
    typedef boost::simd::native<value_type,extension_type>                     native_type;

    BOOST_FORCEINLINE result_type operator()(A0& out, A1& in, A2 const& neutral, A3 const& op ) const
    {
      extent_type ext = in.extent();
      std::ptrdiff_t card = boost::simd::meta::cardinal_of<native_type>();
      std::ptrdiff_t nb_iter = ext[0]/card;
      native_type vec_0;
      native_type vec_1;

      if(ext.size() == 4){
        for(std::ptrdiff_t c_3 = 1; c_3 <= ext[3]; ++c_3){
          for(std::ptrdiff_t c_2 = 1; c_2 <= ext[2]; ++c_2){
            for(std::ptrdiff_t c_1 = 1; c_1 <= ext[1]; ++c_1){
              out(1,c_1,c_2,c_3) = neutral(nt2::meta::as_<value_type>());
              vec_0 = neutral(nt2::meta::as_<native_type>());
              vec_1 = neutral(nt2::meta::as_<native_type>());

              for(std::ptrdiff_t c_0 = 0; c_0 < nb_iter; ++c_0){
                for(std::ptrdiff_t j = 0; j < card; ++j){
                  vec_1[j] = in(c_0*card + j + 1, c_1, c_2, c_3);
                }
                vec_0 = op(vec_0, vec_1);
              }
              //FIXME : Made specialisation of functor fold_ to avoid to use splatted_sum in general case
              out(1,c_1,c_2,c_3) = boost::simd::splatted_sum(vec_0)[0];

              for(std::ptrdiff_t c_0 = nb_iter*card; c_0 < ext[0]; ++c_0){
                out(1,c_1,c_2,c_3) = op(out(1,c_1,c_2,c_3), in(c_0, c_1, c_2, c_3));
              }
            }
          }
        }
      }
      else if(ext.size() == 3){
        for(std::ptrdiff_t c_2 = 1; c_2 <= ext[2]; ++c_2){
          for(std::ptrdiff_t c_1 = 1; c_1 <= ext[1]; ++c_1){
            out(1,c_1,c_2) = neutral(nt2::meta::as_<value_type>());
            vec_0 = neutral(nt2::meta::as_<native_type>());
            vec_1 = neutral(nt2::meta::as_<native_type>());

              for(std::ptrdiff_t c_0 = 0; c_0 < nb_iter; ++c_0){
                for(std::ptrdiff_t j = 0; j < card; ++j){
                  vec_1[j] = in(c_0*card + j + 1, c_1, c_2);
                }
                vec_0 = op(vec_0, vec_1);
              }
              //FIXME : Made specialisation of functor fold_ to avoid to use splatted_sum in general case
              out(1,c_1,c_2) = boost::simd::splatted_sum(vec_0)[0];

              for(std::ptrdiff_t c_0 = nb_iter*card; c_0 < ext[0]; ++c_0){
                out(1,c_1,c_2) = op(out(1,c_1,c_2), in(c_0, c_1, c_2));
              }


          }
        }
      }
      else if(ext.size() == 2){
        for(std::ptrdiff_t c_1 = 1; c_1 <= ext[1]; ++c_1){
          out(1,c_1) = neutral(nt2::meta::as_<value_type>());
            vec_0 = neutral(nt2::meta::as_<native_type>());
            vec_1 = neutral(nt2::meta::as_<native_type>());

              for(std::ptrdiff_t c_0 = 0; c_0 < nb_iter; ++c_0){
                for(std::ptrdiff_t j = 0; j < card; ++j){
                  vec_1[j] = in(c_0*card + j + 1, c_1);
                }
                vec_0 = op(vec_0, vec_1);
              }
              //FIXME : Made specialisation of functor fold_ to avoid to use splatted_sum in general case
              out(1,c_1) = boost::simd::splatted_sum(vec_0)[0];

              for(std::ptrdiff_t c_0 = nb_iter*card; c_0 < ext[0]; ++c_0){
                out(1,c_1) = op(out(1,c_1), in(c_0, c_1));
              }
        }
      }
    }
  };

} }

#endif
