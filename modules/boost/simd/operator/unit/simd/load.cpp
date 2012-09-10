//==============================================================================
//         Copyright 2003 - 2012 LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2012 LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#define NT2_UNIT_MODULE "nt2 operator toolbox - load/simd Mode"

#include <iostream>
#include <boost/simd/include/functions/load.hpp>

#include <nt2/sdk/unit/tests.hpp>
#include <nt2/sdk/unit/module.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/dispatch/functor/meta/call.hpp>
#include <boost/simd/sdk/memory/aligned_type.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/make_vector.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

NT2_TEST_CASE_TPL( load,  BOOST_SIMD_TYPES)
{
  using boost::simd::load;
  using boost::simd::tag::load_;
  using boost::simd::native;
  using boost::simd::meta::cardinal_of;
  typedef BOOST_SIMD_DEFAULT_EXTENSION  ext_t;

  typedef native<T,ext_t>                        vT;
  typedef typename
          boost::dispatch::
          meta::call<load_(T*,int,boost::dispatch::meta::as_<vT>)>::type r_t;

  NT2_TEST( (boost::is_same<vT,r_t>::value) );

  BOOST_SIMD_ALIGNED_TYPE(T) data[ cardinal_of<vT>::value*3 ];
  for(size_t i=0;i<cardinal_of<vT>::value*3;++i) data[i] = 1+i;

  for(int i=0;i<3;++i)
  {
    r_t v = boost::simd::load<vT>(&data[i*cardinal_of<vT>::value]);
    for(size_t j=0;j<cardinal_of<vT>::value;++j)
      NT2_TEST_EQUAL(v[j] , data[j+i*cardinal_of<vT>::value]);
  }
}

NT2_TEST_CASE_TPL( load_offset,  BOOST_SIMD_TYPES)
{
  using boost::simd::load;
  using boost::simd::tag::load_;
  using boost::simd::native;
  using boost::simd::meta::cardinal_of;
  typedef BOOST_SIMD_DEFAULT_EXTENSION  ext_t;

  typedef native<T,ext_t>                        vT;
  typedef typename
          boost::dispatch::
          meta::call<load_(T*,int,boost::dispatch::meta::as_<vT>)>::type r_t;

  NT2_TEST( (boost::is_same<vT,r_t>::value) );

  BOOST_SIMD_ALIGNED_TYPE(T) data[ cardinal_of<vT>::value*3 ];
  for(size_t i=0;i<cardinal_of<vT>::value*3;++i) data[i] = 1+i;

  {
    r_t v = boost::simd::load<vT>(&data[cardinal_of<vT>::value],-cardinal_of<vT>::value);
    for(size_t j=0;j<cardinal_of<vT>::value;++j)
      NT2_TEST_EQUAL(v[j] , data[j]);
  }

  {
    r_t v = boost::simd::load<vT>(&data[cardinal_of<vT>::value], 0);
    for(size_t j=0;j<cardinal_of<vT>::value;++j)
      NT2_TEST_EQUAL(v[j] , data[j+cardinal_of<vT>::value]);
  }

  {
    r_t v = boost::simd::load<vT>(&data[cardinal_of<vT>::value],+cardinal_of<vT>::value);
    for(size_t j=0;j<cardinal_of<vT>::value;++j)
      NT2_TEST_EQUAL(v[j] , data[j+2*cardinal_of<vT>::value]);
  }
}

NT2_TEST_CASE_TPL( load_suboffset,  BOOST_SIMD_TYPES)
{
  using boost::simd::load;
  using boost::simd::tag::load_;
  using boost::simd::native;
  using boost::simd::meta::cardinal_of;
  typedef BOOST_SIMD_DEFAULT_EXTENSION  ext_t;

  typedef native<T,ext_t>                        vT;
  typedef typename
          boost::dispatch::
          meta::call<load_(T*,int,boost::dispatch::meta::as_<vT>, boost::mpl::int_<1>)>::type r_t;

  NT2_TEST( (boost::is_same<vT,r_t>::value) );

  BOOST_SIMD_ALIGNED_TYPE(T) data[ cardinal_of<vT>::value*3 ];
  for(size_t i=0;i<cardinal_of<vT>::value*3;++i) data[i] = 1+i;

  {
    r_t v = boost::simd::load<vT,1>(&data[0]);
    for(size_t j=0;j<cardinal_of<vT>::value;++j)
      NT2_TEST_EQUAL(v[j] , data[j+1]);
  }

  {
    r_t v = boost::simd::load<vT,1>(&data[0], cardinal_of<vT>::value);
    for(size_t j=0;j<cardinal_of<vT>::value;++j)
      NT2_TEST_EQUAL(v[j] , data[j+cardinal_of<vT>::value+1]);
  }

  {
    r_t v = boost::simd::load<vT,-1>(&data[0],2*cardinal_of<vT>::value);
    for(size_t j=0;j<cardinal_of<vT>::value;++j)
      NT2_TEST_EQUAL(v[j] , data[j+2*cardinal_of<vT>::value-1]);
  }
}

NT2_TEST_CASE_TPL( load_gather, BOOST_SIMD_TYPES)
{
  using boost::simd::load;
  using boost::simd::tag::load_;
  using boost::simd::native;
  using boost::simd::meta::cardinal_of;
  typedef BOOST_SIMD_DEFAULT_EXTENSION  ext_t;

  typedef native<T,ext_t>                                       vT;
  typedef typename boost::dispatch::meta::as_integer<T>::type   iT;
  typedef typename boost::dispatch::meta::as_integer<vT>::type viT;
  typedef typename
          boost::dispatch::
          meta::call<load_(T*,viT,boost::dispatch::meta::as_<vT>)>::type r_t;

  srand(time(NULL));

  NT2_TEST( (boost::is_same<vT,r_t>::value) );

  BOOST_SIMD_ALIGNED_TYPE(T) data[ cardinal_of<vT>::value*3 ];
  for(size_t i=0;i<cardinal_of<vT>::value*3;++i) data[i] = 1+i;

  viT index;
  for(size_t i=0;i<cardinal_of<viT>::value;++i)
    index[i] = rand() % (cardinal_of<vT>::value*3);

  r_t v = boost::simd::load<vT>(&data[0], index);

  for(size_t j=0;j<cardinal_of<vT>::value;++j)
  {
    NT2_TEST_EQUAL(v[j] , data[index[j]]);
  }
}

struct foo { double d; float f; char c; };
BOOST_FUSION_ADAPT_STRUCT(foo,(double,d)(float,f)(char,c))

NT2_TEST_CASE( load_sequence )
{
  using boost::simd::load;
  using boost::simd::tag::load_;
  using boost::simd::native;
  using boost::simd::meta::cardinal_of;
  typedef BOOST_SIMD_DEFAULT_EXTENSION  ext_t;

  srand(time(NULL));

  BOOST_SIMD_ALIGNED_TYPE(char)   cdata[ cardinal_of< native<char  ,ext_t> >::value ];
  BOOST_SIMD_ALIGNED_TYPE(float)  fdata[ cardinal_of< native<float ,ext_t> >::value ];
  BOOST_SIMD_ALIGNED_TYPE(double) ddata[ cardinal_of< native<double,ext_t> >::value ];
  for(size_t i=0;i<cardinal_of< native<char  ,ext_t> >::value;++i) 
  { cdata[i] = char(1+i); }
  for(size_t i=0;i<cardinal_of< native<float ,ext_t> >::value;++i) 
  { fdata[i] = float(1+i); }
  for(size_t i=0;i<cardinal_of< native<double ,ext_t> >::value;++i) 
  { ddata[i] = double(1+i); }

  typedef native<foo, ext_t> seq_t;

  seq_t v = load<seq_t>(boost::fusion::make_vector(&ddata[0], &fdata[0], &cdata[0]), 0);

  for(size_t j=0;j<cardinal_of< native<double,ext_t> >::value;++j)  
    NT2_TEST_EQUAL(boost::fusion::at_c<0>(v)[j] , ddata[j]);
  for(size_t j=0;j<cardinal_of< native<float,ext_t> >::value;++j)    
    NT2_TEST_EQUAL(boost::fusion::at_c<1>(v)[j] , fdata[j]);
  for(size_t j=0;j<cardinal_of< native<char,ext_t> >::value;++j)  
    NT2_TEST_EQUAL(boost::fusion::at_c<2>(v)[j] , cdata[j]);
}