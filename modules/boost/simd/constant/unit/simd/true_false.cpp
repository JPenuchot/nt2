/*******************************************************************************
 *         Copyright 2003 & onward LASMEA UMR 6602 CNRS/Univ. Clermont II
 *         Copyright 2009 & onward LRI    UMR 8623 CNRS/Univ Paris Sud XI
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/
#define NT2_UNIT_MODULE "boost::simd::constants true/false"

#include <boost/simd/include/constants/true.hpp>
#include <boost/simd/include/constants/false.hpp>
#include <nt2/sdk/unit/module.hpp>
#include <nt2/sdk/unit/tests/relation.hpp>
#include <boost/simd/sdk/simd/native.hpp>
#include <boost/dispatch/meta/as_unsigned.hpp>

////////////////////////////////////////////////////////////////////////////////
// Test value of true/false constant for integral types
////////////////////////////////////////////////////////////////////////////////
NT2_TEST_CASE_TPL( true_false_value, BOOST_SIMD_SIMD_TYPES )
{
  using boost::simd::logical;
  typedef BOOST_SIMD_DEFAULT_EXTENSION  ext_t;
  typedef boost::simd::native<T,ext_t>                vT;
  typedef boost::simd::native< boost::simd::logical<T>, ext_t> vlT;
  for(std::size_t i=0; i< boost::simd::meta::cardinal_of<vT>::value;++i){
    std::cout << "for i =  " << i << std::endl;
    NT2_TEST_EQUAL( (boost::simd::False<vlT>())[i], boost::simd::logical<T>(false) );
  }
  for(std::size_t i=0; i< boost::simd::meta::cardinal_of<vT>::value;++i){
    std::cout << "for i =  " << i << std::endl;
    NT2_TEST_EQUAL( (boost::simd::True<vlT>())[i],  boost::simd::logical<T>(true) );
  }
}

NT2_TEST_CASE_TPL( true_false_logical_value, BOOST_SIMD_SIMD_TYPES )
{
  using boost::simd::logical;
  typedef BOOST_SIMD_DEFAULT_EXTENSION  ext_t;
  typedef boost::simd::native< logical<T>,ext_t>                dst_t;
  typedef typename dst_t::type                vT;
  typedef typename boost::dispatch::meta::as_unsigned<T>::type  uns_t;
  std::cout << boost::simd::meta::cardinal_of<vT>::value << std::endl;
  std::cout << boost::simd::meta::cardinal_of<dst_t>::value << std::endl;

  for(std::size_t i=0; i< boost::simd::meta::cardinal_of<vT>::value;++i){
    std::cout << "for i =  " << i << std::endl;
    NT2_TEST_EQUAL( (boost::simd::False<dst_t>())[i], boost::simd::logical<T>(false) );
  }
  for(std::size_t i=0; i< boost::simd::meta::cardinal_of<vT>::value;++i){
    std::cout << "for i =  " << i << std::endl;
    NT2_TEST_EQUAL( (boost::simd::True<dst_t>())[i], boost::simd::logical<T>(true) );
  }
}
