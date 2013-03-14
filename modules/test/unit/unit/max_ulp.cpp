//==============================================================================
//         Copyright 2003 - 2012 LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2012 LRI    UMR 8623 CNRS/Univ Paris Sud XI
//         Copyright 2012        MetaScale SAS
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#include <nt2/sdk/unit/details/main.hpp>
#include <nt2/sdk/unit/details/ulp.hpp>
#include <nt2/sdk/unit/tests/ulp.hpp>
#include <boost/type_traits/is_integral.hpp>

#include <cmath>
#include <vector>
#include <limits>
#include <typeinfo>
#include <iostream>

template<class T> bool check_ulp_fundamental()
{
  T value(1), different_value;
  std::vector< nt2::details::failed_value<T,T> >  fails;

  different_value = value + 16*std::numeric_limits<T>::epsilon();

  std::cout << "Check for: " << typeid(T).name() << "\t";
  double u0 = 0;
  bool ok = nt2::unit::max_ulp(value,value,0.5,fails,u0);
  std::cout << "max_ulp(a,a) = " << u0 << "\t";

  fails.clear();
  double u1 = 0;
  ok = nt2::unit::max_ulp(value,different_value,8,fails, u1);
  std::cout << "max_ulp(a,b) = " << u1 << "\t";
  std::cout << ((!u0 && u1==8) ? "OK" : "NOT OK") << "\n";

  return !u0 && u1==8;
}

template<class T> bool check_ulp_sequence()
{
  std::vector<T> values(17), different_values(17);
  std::vector< nt2::details::failed_value<T,T> >  fails;

  for(std::size_t i=0;i<values.size();++i)
    values[i] = different_values[i] = T(1);

  different_values[9] += 16*std::numeric_limits<T>::epsilon();
  different_values[7] += 5*std::numeric_limits<T>::epsilon();

  std::cout << "Check for: " << typeid(T).name() << "\t";
  double u0 = 0;
  bool ok = nt2::unit::max_ulp(values,values,0.5,fails,u0);
  std::cout << "max_ulp(a,a) = " << u0 << "\t";

  fails.clear();
  double u1 = 0;
  ok = nt2::unit::max_ulp(values,different_values,8,fails, u1);
  std::cout << "max_ulp(a,b) = " << u1 << "\t";
  std::cout << ((!u0 && u1==8) ? "OK" : "NOT OK")  << "\n";

  return (!u0 && u1==8);
}

NT2_UNIT_MAIN_SPEC int NT2_UNIT_MAIN(int argc, char* argv[])
{
  std::cout << "Check for basic types\n";
  bool  check =   check_ulp_fundamental<double>()
              &&  check_ulp_fundamental<float>();

  std::cout << "\nCheck for Sequence\n";
        check =   check_ulp_sequence<double>()
              &&  check_ulp_sequence<float>();

  return check ? 0 : 1;
}
