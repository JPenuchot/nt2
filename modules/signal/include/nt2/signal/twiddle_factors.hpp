//==============================================================================
//         Copyright 2003 - 2011   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2011   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//         Copyright 2012          Domagoj Saric, Little Endian Ltd.
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_SIGNAL_TWIDDLE_FACTORS_HPP_INCLUDED
#define NT2_SIGNAL_TWIDDLE_FACTORS_HPP_INCLUDED
#ifdef _MSC_VER
    #pragma once
    #define _USE_MATH_DEFINES
#endif

#include <nt2/signal/static_sincos.hpp>

#include <nt2/include/functions/simd/sinecosine.hpp>

#include <boost/simd/sdk/simd/extensions.hpp>
#include <boost/simd/include/functions/simd/arith.hpp>

#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/arithmetic/mul.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>

#include <math.h>

#ifdef BOOST_SIMD_DETECTED
/// \note Quick-fixed tofloat() nop specialization that does not cause MSVC10 to
/// insert brain-dead stack-based native<> copy-construction.
///                                           (19.06.2012.) (Domagoj Saric)
namespace boost
{
namespace simd
{
    template <class Extension, class Enable>
    BOOST_FORCEINLINE
    native<float, Extension, Enable> const & tofloat( native<float, Extension, Enable> const & input ) { return input; }
} // namespace simd
} // namespace boost
#endif // BOOST_SIMD_DETECTED

namespace nt2
{
//------------------------------------------------------------------------------

template <typename T>
struct twiddle_pair
{
    T wr;
    T wi;
};

template <typename T>
struct split_radix_twiddles
{
    twiddle_pair<T> w0;
    twiddle_pair<T> w3;
};


template
<
    unsigned FFTSize,
    typename T
>
struct twiddles_interleaved;
/*
{
    typedef
        BOOST_SIMD_ALIGN_ON( BOOST_SIMD_ARCH_ALIGNMENT )
        boost::array<twiddle_pair<T> const, fft_size / data_vector_size>
    factors_t;
    static factors_t const factors;
};
*/


////////////////////////////////////////////////////////////////////////////////
// Runtime-static initialisation
//  + simple
//  + compile-time lightweight
//  - does not get placed in the read-only text section
//  - creates dynamic initialisers for each N
//  - forces (scalar) sin/cos functions to be present in the binary
////////////////////////////////////////////////////////////////////////////////

namespace detail
{
    /// \note Regardless of the input data layout we always use twiddles
    /// interleaved on vector boundaries in order to improve memory locality and
    /// reduce the number of pointers that must be tracked to help architectures
    /// with a small general purpose register file (32 bit x86 is already
    /// maximally stretched out).
    ///                                       (05.06.2012.) (Domagoj Saric)
    template <typename Vector>
    BOOST_DISPATCH_NOINLINE
    void calculate_twiddles
    (
        split_radix_twiddles<Vector> * BOOST_DISPATCH_RESTRICT const p_twiddles,
        unsigned int                                           const N_int
    )
    {
        typedef Vector vector_t;
        typedef typename vector_t::value_type scalar_t;

        split_radix_twiddles<Vector> * BOOST_DISPATCH_RESTRICT p_w( p_twiddles );

        long double const N( static_cast<int>( N_int ) );

        /// \todo Since cos( a ) = sin( a + Pi/2 ) = -sin( a + 3*Pi/2 ) separate
        /// cos/wr and sin/wi twiddle values could be avoided. The elements of
        /// the array would be half the size (no longer pairs) but there would
        /// have to be extra elements for the cosine offset: N/4 in the first
        /// case (if we store positive sines) or 3N/4 in the second case (if we
        /// store negative sines).
        ///                                   (02.03.2012.) (Domagoj Saric)

        /// \note N/4 values are required for split-radix.
        ///                                   (21.05.2012.) (Domagoj Saric)

        int i( 0 );
        while ( unsigned( i ) < ( N_int / 4 ) )
        {
            long double const w           ( 2 * M_PI / N );
            long double const start_value0( i * 1 * w    );
            long double const start_value3( i * 3 * w    );
            vector_t const input0( boost::simd::arith<vector_t>( static_cast<scalar_t>( start_value0 ), static_cast<scalar_t>( 1 * w ) ) );
            vector_t const input3( boost::simd::arith<vector_t>( static_cast<scalar_t>( start_value3 ), static_cast<scalar_t>( 3 * w ) ) );

            BOOST_ASSERT( input0[ vector_t::static_size - 1 ] <= 1 * 2 * M_PI / 4 );
            BOOST_ASSERT( input3[ vector_t::static_size - 1 ] <= 3 * 2 * M_PI / 4 );

            {
                vector_t & cosine( p_w->w0.wr );
                vector_t & sine  ( p_w->w0.wi );
                sinecosine<small>( input0, sine, cosine );
                sine ^= Mzero<vector_t>();
            }
            {
                vector_t & cosine( p_w->w3.wr );
                vector_t & sine  ( p_w->w3.wi );
                sinecosine<small>( input3, sine, cosine );
                sine ^= Mzero<vector_t>();
            }

            i += vector_t::static_size;

            ++p_w;
        }

        BOOST_ASSERT( p_w == &p_twiddles[ N_int / 4 / vector_t::static_size ] );
    }

    template <typename Vector, unsigned N>
    struct interleaved_twiddle_holder
    {
        interleaved_twiddle_holder()
        {
            calculate_twiddles<Vector>( factors.begin(), N );
        }

        typedef
            BOOST_SIMD_ALIGN_ON( BOOST_SIMD_ARCH_ALIGNMENT )
            boost::array
            <
                split_radix_twiddles<Vector> /*const*/,
                N / 4 / Vector::static_size
            >
            factors_t;

        factors_t /*const*/ factors;
    };
} // namespace detail

template <unsigned N, typename Vector>
struct twiddles_interleaved
{
public:
    static split_radix_twiddles<Vector> const * BOOST_DISPATCH_RESTRICT factors() { return twiddles_.factors.begin(); }

private:
    typedef detail::interleaved_twiddle_holder<Vector, N> twiddle_holder;
    static twiddle_holder const twiddles_;
};

template <unsigned N, typename Vector>
typename twiddles_interleaved<N, Vector>::twiddle_holder const twiddles_interleaved<N, Vector>::twiddles_;


//...zzz...the below approaches are still radix-2 specific and use old/ancient
//...zzz...approaches with strides and separate arrays for real and imaginary
//...zzz...parts (so that they don't need to be parameterized with the SIMD data
//...zzz...vector size)...

////////////////////////////////////////////////////////////////////////////////
// Recursive compile-time initialisation
//  + no dynamic initialisers ((hopefully, depending on the compiler)
//  - very compile-time heavyweight (MSVC10 seems to have a template recursion
//    level limit of ~512 so it cannot compile this for FFT sizes >= 2048 even
//    with the recursion "unrolled" into packs of four values)
//  - questionable whether the resulting "array" would still get placed in the
//    text section considering that individual twiddles are not PODs but have
//    default constructors
////////////////////////////////////////////////////////////////////////////////

namespace detail
{
    template
    <
        template <unsigned N, unsigned I> class SinCos,
        unsigned I,
        unsigned N,
        unsigned stride,
        typename T
    >
    struct twiddle
        :
        twiddle<SinCos, I - ( stride * 4 ), N, stride, T>
    {
        twiddle()
            :
            value0( SinCos<N, I - ( stride * 3 )>::value ),
            value1( SinCos<N, I - ( stride * 2 )>::value ),
            value2( SinCos<N, I - ( stride * 1 )>::value ),
            value3( SinCos<N, I - ( stride * 0 )>::value )
        {}

        T const value0;
        T const value1;
        T const value2;
        T const value3;
    };

    template <template <unsigned N, unsigned I> class SinCos, unsigned N, unsigned stride, typename T>
    struct twiddle<SinCos, 0, N, stride, T> {};
} // namespace detail

template <unsigned N, unsigned Stride, typename T>
struct real_twiddles
{
    static float const * BOOST_DISPATCH_RESTRICT factors() { return factors_.begin(); }

    typedef BOOST_SIMD_ALIGN_ON( BOOST_SIMD_ARCH_ALIGNMENT ) detail::twiddle<static_cosine, N, N, Stride, T> factors_t;
    static factors_t const factors_;
};

template <unsigned N, unsigned Stride, typename T>
typename real_twiddles<N, Stride, T>::factors_t const real_twiddles<N, Stride, T>::factors_;

template <unsigned N, unsigned Stride, typename T>
struct imag_twiddles
{
    static float const * BOOST_DISPATCH_RESTRICT factors() { return factors_.begin(); }

    typedef BOOST_SIMD_ALIGN_ON( BOOST_SIMD_ARCH_ALIGNMENT ) detail::twiddle<static_sine, N, N, Stride, T> factors_t;
    static factors_t const factors_;
};

template <unsigned N, unsigned Stride, typename T>
typename imag_twiddles<N, Stride, T>::factors_t const imag_twiddles<N, Stride, T>::factors_;


////////////////////////////////////////////////////////////////////////////////
// Preprocessor + compile-time initialisation
//  + fully static
//  + bearable compile-time hit
//  - too heavy on the preprocessor (requires a smarter rewrite to workaround
//    Boost.Preprocessor and compiler limits)
//  - requires in advance definition of explicit specializations for all sizes
//    that will be used in a particular application
////////////////////////////////////////////////////////////////////////////////


#define NT2_AUX_REAL_TWIDDLE( z, i, context )   static_cosine<BOOST_PP_TUPLE_ELEM( 3, 0, context ), i * BOOST_PP_TUPLE_ELEM( 3, 2, context )>::value,
#define NT2_AUX_IMAG_TWIDDLE( z, i, context ) - static_sine  <BOOST_PP_TUPLE_ELEM( 3, 0, context ), i * BOOST_PP_TUPLE_ELEM( 3, 2, context )>::value,

#define NT2_AUX_TWIDDLE( z, i, context )                                                                                                      0\
{                                                                                                                                                      \
    { BOOST_PP_REPEAT_FROM_TO( BOOST_PP_MUL( i, BOOST_PP_TUPLE_ELEM( 3, 1, context ) ), BOOST_PP_MUL( BOOST_PP_INC( i ), BOOST_PP_TUPLE_ELEM( 3, 1, context ) ), NT2_AUX_REAL_TWIDDLE, context ) },  \
    { BOOST_PP_REPEAT_FROM_TO( BOOST_PP_MUL( i, BOOST_PP_TUPLE_ELEM( 3, 1, context ) ), BOOST_PP_MUL( BOOST_PP_INC( i ), BOOST_PP_TUPLE_ELEM( 3, 1, context ) ), NT2_AUX_IMAG_TWIDDLE, context ) },  \
},

#define NT2_AUX_TWIDDLES( fft_size, stride )                                               \
template <typename T>                                                                      \
struct twiddles<fft_size, stride, T>                                                       \
{                                                                                          \
    static std::size_t const data_vector_size = twiddle_pair<T>::vector_t::static_size;    \
    typedef BOOST_SIMD_ALIGN_ON( BOOST_SIMD_ARCH_ALIGNMENT )                               \
    boost::array                                                                           \
    <                                                                                      \
        twiddle_pair<T> const,                                                             \
        fft_size / stride / data_vector_size                                               \
    >                                                                                      \
    factors_t;                                                                             \
    static factors_t const factors;                                                        \
};                                                                                         \
                                                                                           \
template <typename T>                                                                      \
typename twiddles<fft_size, stride, T>::factors_t const                                    \
         twiddles<fft_size, stride, T>::factors =                                          \
{{                                                                                         \
    BOOST_PP_REPEAT                                                                        \
    (                                                                                      \
        BOOST_PP_DIV( BOOST_PP_DIV( fft_size, stride ), data_vector_size ),                \
        NT2_AUX_TWIDDLE,                                                                   \
        ( fft_size, data_vector_size, stride )                                             \
    )                                                                                      \
}}

/* Desired output for for fft_size = 8, data_vector_size = 4, stride = 1:
 *   template
 *   <
 *       unsigned fft_size,
 *       unsigned data_vector_size,
 *       unsigned stride,
 *       typename T
 *   >
 *   typename twiddles<fft_size, data_vector_size, stride, T>::factors_t const                  
 *            twiddles<fft_size, data_vector_size, stride, T>::factors =
 *   {{
 *       {
 *           {   static_cosine<fft_size, 0 * stride>::value,   static_cosine<fft_size, 1 * stride>::value,   static_cosine<fft_size, 2 * stride>::value,   static_cosine<fft_size, 3 * stride>::value, },
 *           { - static_sine  <fft_size, 0 * stride>::value, - static_sine  <fft_size, 1 * stride>::value, - static_sine  <fft_size, 2 * stride>::value, - static_sine  <fft_size, 3 * stride>::value, },
 *       },
 *       {
 *           {   static_cosine<fft_size, 4 * stride>::value,   static_cosine<fft_size, 5 * stride>::value,   static_cosine<fft_size, 6 * stride>::value,   static_cosine<fft_size, 7 * stride>::value, },
 *           { - static_sine  <fft_size, 4 * stride>::value, - static_sine  <fft_size, 5 * stride>::value, - static_sine  <fft_size, 6 * stride>::value, - static_sine  <fft_size, 7 * stride>::value, },
 *       },
 *   }};
 */

// Broken (breaches Boost.Preprocessor and compiler limits for FFT sizes 256+)...
/*
    NT2_AUX_TWIDDLES(  128, 4, 1 );
    NT2_AUX_TWIDDLES(  128, 4, 2 );
    NT2_AUX_TWIDDLES(  256, 4, 1 );
    NT2_AUX_TWIDDLES(  256, 4, 2 );
    NT2_AUX_TWIDDLES(  512, 4, 1 );
    NT2_AUX_TWIDDLES(  512, 4, 2 );
    NT2_AUX_TWIDDLES( 1024, 4, 1 );
    NT2_AUX_TWIDDLES( 1024, 4, 2 );
    NT2_AUX_TWIDDLES( 2048, 4, 1 );
    NT2_AUX_TWIDDLES( 2048, 4, 2 );
�*/

//------------------------------------------------------------------------------
} // namespace nt2
//------------------------------------------------------------------------------
#endif // twiddle_factors_hpp

