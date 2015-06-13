//==============================================================================
//         Copyright 2015          J.T. Lapreste
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_LINALG_OPTIONS_HPP_INCLUDED
#define NT2_LINALG_OPTIONS_HPP_INCLUDED

#include <nt2/core/options.hpp>
#include <nt2/sdk/meta/policy.hpp>
#include <nt2/include/functions/forward.hpp>
#include <nt2/include/functions/backward.hpp>
#include <nt2/include/functions/centered.hpp>
#include <nt2/include/functions/vand.hpp>

namespace nt2
{
  namespace ext
  {
//     struct forward_    : boost::mpl::true_  {}; //used in deriv, jac
//     struct backward_   : boost::mpl::true_  {}; //used in deriv, jac
//     struct centered_   : boost::mpl::true_  {}; //used in deriv, jac
    struct pow2den_    : boost::mpl::true_  {}; //used in forward,  bacward, centered, deriv, jac
    struct absolute_   : boost::mpl::true_  {}; //used in forward,  bacward, centered, deriv, jac
    struct relative_   : boost::mpl::true_  {}; //used in forward,  bacward, centered, deriv, jac
  }

  nt2::policy<nt2::tag::forward_>      const forward_;
  nt2::policy<nt2::tag::backward_>     const backward_;
  nt2::policy<nt2::tag::centered_>     const centered_;
  nt2::policy<nt2::tag::vand_>         const vand_;
  nt2::policy<ext::pow2den_>           const pow2den_;
  nt2::policy<ext::absolute_>          const absolute_;
  nt2::policy<ext::relative_>          const relative_;
 }

#endif