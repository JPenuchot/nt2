//==============================================================================
//         Copyright 2003 - 2011 LASMEA UMR 6602 CNRS/Univ. Clermont II         
//         Copyright 2009 - 2011 LRI    UMR 8623 CNRS/Univ Paris Sud XI         
//                                                                              
//          Distributed under the Boost Software License, Version 1.0.          
//                 See accompanying file LICENSE.txt or copy at                 
//                     http://www.boost.org/LICENSE_1_0.txt                     
//==============================================================================
#ifndef BOOST_SIMD_TOOLBOX_ARITHMETIC_FUNCTION_SCALAR_IDIVCEIL_HPP_INCLUDED
#define BOOST_SIMD_TOOLBOX_ARITHMETIC_FUNCTION_SCALAR_IDIVCEIL_HPP_INCLUDED
#include <boost/simd/include/constants/digits.hpp>
#include <boost/simd/include/functions/sign.hpp>
#include <boost/simd/include/functions/abs.hpp>
#include <boost/simd/include/functions/rdivide.hpp>
#include <boost/simd/include/functions/tofloat.hpp>
#include <boost/simd/include/functions/iceil.hpp>


/////////////////////////////////////////////////////////////////////////////
// Implementation when type A0 is arithmetic_
/////////////////////////////////////////////////////////////////////////////
namespace boost { namespace simd { namespace ext
{
  BOOST_SIMD_FUNCTOR_IMPLEMENTATION( boost::simd::tag::idivceil_, tag::cpu_
                            , (A0)(A1)
                            , (scalar_< arithmetic_<A0> >)(scalar_< arithmetic_<A1> >)
                            )
  {

    typedef typename dispatch::meta::result_of<dispatch::meta::arithmetic(A0,A1)>::type result_type;

    BOOST_SIMD_FUNCTOR_CALL(2)
    {
      typedef typename meta::result_of<meta::floating(result_type)>::type ftype;
      ftype r = boost::simd::ceil(ftype(a0)/ftype(a1));
      if (r > boost::simd::Valmax<result_type>())
	return boost::simd::Valmax<result_type>();
      else if (r <  boost::simd::Valmin<result_type>())
	return boost::simd::Valmin<result_type>();
      else return result_type(r); 
    }
  };

  /////////////////////////////////////////////////////////////////////////////
  // Implementation when type A0 is unsigned_
  /////////////////////////////////////////////////////////////////////////////
  BOOST_SIMD_FUNCTOR_IMPLEMENTATION( boost::simd::tag::idivceil_, tag::cpu_
			      , (A0)(A1)
			      , (scalar_< unsigned_<A0> >)(scalar_< unsigned_<A1> >)
			      )
  {
    
    typedef typename dispatch::meta::result_of<dispatch::meta::arithmetic(A0,A1)>::type result_type;
    
    BOOST_SIMD_FUNCTOR_CALL(2)
    {
	return (a1) ? boost::simd::rdivide((a0+(a1-boost::simd::One<result_type>())), a1)
	            : boost::simd::Valmax<result_type>();
    }
  };
  
  /////////////////////////////////////////////////////////////////////////////
  // Implementation when type A0 is real_
  /////////////////////////////////////////////////////////////////////////////
  BOOST_SIMD_FUNCTOR_IMPLEMENTATION(boost::simd::tag::idivceil_, tag::cpu_,
			     (A0)(A1),
			     (scalar_< real_<A0> >)(scalar_< real_<A1> > )
			     )
  {
    typedef typename dispatch::meta::result_of<dispatch::meta::arithmetic(A0,A1)>::type typ;
    typedef typename dispatch::meta::as_integer<typ>::type result_type;

    BOOST_SIMD_FUNCTOR_CALL(2)
    {
      return boost::simd::iceil(a0/a1);
    }
  };
} } }

#endif
