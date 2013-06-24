//==============================================================================
//         Copyright 2003 - 2012 LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2013 LRI    UMR 8623 CNRS/Univ Paris Sud XI
//         Copyright 2012        MetaScale SAS
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#include <nt2/sdk/bench/benchmark.hpp>

#include <nt2/table.hpp>
#include <nt2/include/functions/qr.hpp>
#include <nt2/include/functions/rand.hpp>
#include <nt2/include/functions/zeros.hpp>
#include <nt2/include/functions/tie.hpp>
#include "../../flops/qr.hpp"

template<typename T> struct qr_2_2;

template<typename T> NT2_EXPERIMENT(qr_2_2< nt2::table<T> >)
{
  public:
  qr_2_2( std::size_t h_, std::size_t w_)
      : NT2_EXPRIMENT_CTOR(1.,"GFLOPS")
      , h(h_), w(w_)
  {}

  virtual void run() const
  {
    nt2::tie(Q,R) = nt2::qr(input,0);
  }

  virtual double compute(nt2::benchmark_result_t const& r) const
  {
    return ( (FLOPS_GEQRF(h,w) + FLOPS_ORGQR(h,w,k)) /r.second)/1000.;
  }

  virtual void info(std::ostream& os) const
  {
    os << "(" << h << "x" << w << ")";
  }

  virtual void reset() const
  {
    k = std::min(h,w);
    Q = nt2::zeros(h,w, nt2::meta::as_<T>());
    R = nt2::zeros(h,w, nt2::meta::as_<T>());
    input  = nt2::rand(h,w, nt2::meta::as_<T>());
  }

  private:
  std::size_t   h,w;
  mutable std::size_t k;
  mutable nt2::table<T> input, Q,R;
};

NT2_RUN_EXPERIMENT_TPL( qr_2_2, (nt2::table<float>)(nt2::table<double>), (4,4) );
NT2_RUN_EXPERIMENT_TPL( qr_2_2, (nt2::table<float>)(nt2::table<double>), (63,65) );
NT2_RUN_EXPERIMENT_TPL( qr_2_2, (nt2::table<float>)(nt2::table<double>), (1025,1025) );
NT2_RUN_EXPERIMENT_TPL( qr_2_2, (nt2::table<float>)(nt2::table<double>), (2049,2049) );

