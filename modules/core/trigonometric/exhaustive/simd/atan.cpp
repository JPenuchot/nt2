//==============================================================================
//         Copyright 2003 - 2012   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2012   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
//////////////////////////////////////////////////////////////////////////////
// exhaustive cover behavior for float values
//////////////////////////////////////////////////////////////////////////////

#include <nt2/trigonometric/include/atan.hpp>
#include <boost/simd/sdk/simd/native.hpp>
#include <nt2/ieee/include/successor.hpp>
#include <nt2/ieee/include/ulpdist.hpp>
#include <nt2/operator/include/splat.hpp>
#include <nt2/include/functions/iround.hpp>
#include <nt2/crlibm/include/atan_rn.hpp>
#include <nt2/include/functions/min.hpp>
#include <iostream>
int main(){
  typedef NT2_SIMD_DEFAULT_EXTENSION  ext_t;
  typedef boost::simd::native<float,ext_t>           n_t;
  typedef boost::simd::native<nt2::int32_t,ext_t>  in_t;
  float mini = -nt2::Pi<float>()/2;
  float maxi = nt2::Pi<float>()/2;
  const nt2::uint32_t N = nt2::meta::cardinal_of<n_t>::value;
  const in_t vN = nt2::splat<in_t>(N);
  const nt2::uint32_t M =  10;
  nt2::uint32_t histo[M+1];
  for(nt2::uint32_t i = 0; i < M; i++) histo[i] = 0;
  float a[N];
  a[0] = mini;
  for(nt2::uint32_t i = 1; i < N; i++) a[i] = nt2::successor(a[i-1], 1);
  n_t a0 = nt2::aligned_load<n_t>(&a[0],0);
  nt2::uint32_t k = 0;
  for(; a0[N-1] < maxi; a0 = nt2::successor(a0, vN))
    {
      n_t z =  nt2::atan(a0);
      for(nt2::uint32_t i = 0; i < N; i++)
        {
          float v = nt2::crlibm::atan_rn(a0[i]);
          float sz = z[i];
          ++histo[nt2::min(M, nt2::iround(2*nt2::ulpdist(v, sz)))];
          ++k;
        }
    }
  for(nt2::uint32_t i = 0; i < M; i++)
    std::cout << i/2.0 << " -> " << histo[i] << std::endl;
  std::cout << k << " valeurs calcul�es" << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;
  for(nt2::uint32_t i = 0; i < M; i++)
    std::cout << i/2.0 << " -> " << (histo[i]*100.0/k) << "%" << std::endl;
  return 0;
}
