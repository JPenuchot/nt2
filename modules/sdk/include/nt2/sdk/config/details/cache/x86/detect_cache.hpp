//==============================================================================
//         Copyright 2003 - 2011   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2011   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_SDK_CONFIG_DETAILS_CACHE_X86_DETECT_CACHE_HPP_INCLUDED
#define NT2_SDK_CONFIG_DETAILS_CACHE_X86_DETECT_CACHE_HPP_INCLUDED

#include <nt2/sdk/config/arch.hpp>

#ifdef NT2_ARCH_X86
#include <boost/simd/sdk/config/details/detector/cpuid.hpp>
#include <boost/simd/sdk/config/details/get_vendor.hpp>
#include <nt2/sdk/error/assert.hpp>
#include <boost/array.hpp>
#include <vector>

namespace bsc = boost::simd::config;

namespace nt2{ namespace config{ namespace details{

  inline int get_range(int reg, int begin, int end)
  {
    int mask=0x00000000;
    BOOST_ASSERT_MSG( begin < end, "Invalid iterators");
    for(int i = 0; i < (end - begin); i++) { mask <<= 1; mask |= 0x1; }
    return ((reg >> begin) & mask);
  }

  template<class Seq>
  inline void detect_cache(Seq& cache_sizes_, Seq& cache_line_sizes_)
  {
    int regs[4] = {0,0,0,0};
    int byte0, byte1, byte2, byte3;
    int cache_ecx = 0;
    int cache_eax = 0;

    switch(bsc::get_vendor())
    {
      case intel :

        do{
            bsc::x86::cpuidex(regs, 0x00000004, cache_ecx);
            cache_eax = regs[0] & 0x0000001F;
            if(cache_eax == 1 || cache_eax == 3)
            {
              int level      = (regs[0] & 0x000000E0) >> 5;
              int ways       = (regs[1] & 0xFFC00000) >> 22;
              int partitions = (regs[1] & 0x003FF000) >> 12;
              int line_size  = (regs[1] & 0x00000FFF) >>  0;
              int sets       = (regs[2]);
              int size       = (ways+1)*(partitions+1)*(line_size+1)*(sets+1);

              cache_sizes_[level-1]      = size/1024;
              cache_line_sizes_[level-1] = line_size+1;
            }
            cache_ecx++;
        }while(cache_eax != 0x00000000);

        break;

      case amd :

        bsc::x86::cpuidex(regs,0x80000005,0);
        cache_line_sizes_[0] = regs[2] & 0x000000FF;
        cache_sizes_[0] = regs[2] >> 24;

        regs[0] = regs[1] = regs[2] = regs[3] = 0;
        bsc::x86::cpuidex(regs,0x80000006,0);

        if(get_range(regs[0], 0, 4) != 0)
        {
          cache_line_sizes_[1] = regs[2] & 0x000000FF;
          cache_sizes_[1] = regs[2] >> 16;
          cache_line_sizes_[2] = regs[3] & 0x000000FF;
          cache_sizes_[2] = (((regs[3] & 0xFFFC000) >> 18)*512); // D[31;18] = l3 cache size in 512KB
        }

        break;

      default : break;

      }
  }

} } }

#endif

#endif /* NT2_SDK_CONFIG_DETAILS_CACHE_X86_DETECT_CACHE_HPP */
