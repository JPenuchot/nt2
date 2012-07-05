//==============================================================================
//         Copyright 2003 - 2011   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2011   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//         Copyright 2012          Domagoj Saric, Little Endian Ltd.
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_SIGNAL_STATIC_FFT_HPP_INCLUDED
#define NT2_SIGNAL_STATIC_FFT_HPP_INCLUDED
#ifdef _MSC_VER
    #pragma once
    #pragma inline_recursion( on )
#endif // _MSC_VER


#if defined( _MSC_VER )

    #define BOOST_NOTHROW_NOALIAS __declspec( nothrow noalias )
    #define BOOST_FASTCALL __fastcall
    #define BOOST_UNREACHABLE_CODE()  BOOST_ASSERT_MSG( false    , "This code should not be reached." ); __assume( false     )
    #define BOOST_ASSUME( condition ) BOOST_ASSERT_MSG( condition, "Assumption broken."               ); __assume( condition )

#elif defined( __GNUC__ )

    #define BOOST_NOTHROW_NOALIAS __attribute__(( nothrow, pure  ))
    #ifdef __i386__
        #ifdef __clang__
            #define BOOST_FASTCALL __attribute__(( regparm( 3 ) ))
        #else
            #define BOOST_FASTCALL __attribute__(( regparm( 3 ), sseregparm, hot ))
        #endif // __clang__
    #else
        #define BOOST_FASTCALL
    #endif // __i386__

    // http://en.chys.info/2010/07/counterpart-of-assume-in-gcc
    // http://nondot.org/sabre/LLVMNotes/BuiltinUnreachable.txt
    #if ( __clang_major__ >= 2 ) || ( ( ( __GNUC__ * 10 ) + __GNUC_MINOR__ ) >= 45 )
        #define BOOST_UNREACHABLE_CODE()  BOOST_ASSERT_MSG( false    , "This code should not be reached." ); __builtin_unreachable()
        #define BOOST_ASSUME( condition ) BOOST_ASSERT_MSG( condition, "Assumption broken."               ); do { if ( !( condition ) ) __builtin_unreachable(); } while ( 0 )
    #else
        #define BOOST_UNREACHABLE_CODE()  BOOST_ASSERT_MSG( false    , "This code should not be reached." )
        #define BOOST_ASSUME( condition ) BOOST_ASSERT_MSG( condition, "Assumption broken."               )
    #endif

#else

    #define BOOST_NOTHROW_NOALIAS
    #define BOOST_FASTCALL
    #define BOOST_UNREACHABLE_CODE()  BOOST_ASSERT_MSG( false    , "This code should not be reached." )
    #define BOOST_ASSUME( condition ) BOOST_ASSERT_MSG( condition, "Assumption broken."               )

#endif

#include <boost/control/switch.hpp>
#include <boost/control/case.hpp>
#include <boost/detail/endian.hpp>

// FIXME: make the code work without those assumptions
#include <boost/simd/sdk/simd/extensions.hpp>
#include <boost/simd/sdk/memory/parameters.hpp>
#undef BOOST_SIMD_CONFIG_ALIGNMENT
#undef BOOST_SIMD_ARCH_ALIGNMENT
#undef BOOST_SIMD_DEFAULT_EXTENSION
#define BOOST_SIMD_CONFIG_ALIGNMENT 16
#define BOOST_SIMD_ARCH_ALIGNMENT 16
#define BOOST_SIMD_DEFAULT_EXTENSION boost::simd::tag::sse_

#include <nt2/signal/twiddle_factors.hpp>

#include <nt2/include/functions/simd/multiplies.hpp>
#include <nt2/include/functions/simd/plus.hpp>
#include <nt2/include/functions/simd/unary_minus.hpp>
#include <nt2/include/functions/simd/unaligned_load.hpp>
#include <nt2/include/functions/simd/unaligned_store.hpp>

#include <nt2/include/functions/scalar/log2.hpp>
#include <nt2/include/functions/scalar/ffs.hpp>

#include <boost/simd/sdk/simd/native.hpp>
#include <boost/simd/toolbox/constant/constants/half.hpp>
#include <boost/simd/toolbox/constant/constants/mzero.hpp>
#include <boost/simd/toolbox/constant/constants/zero.hpp>

#include <boost/assert.hpp>
#include <boost/cstdint.hpp>
#include <boost/integer/static_log2.hpp>
#include <boost/mpl/range_c.hpp>

#include <utility>
//------------------------------------------------------------------------------
namespace nt2
{
//------------------------------------------------------------------------------

/// \note
///   A work-in-progress implementation of a "partially compile-time"
/// power-of-two FFT.
///                                           (13.02.2012.) (Domagoj Saric)

// Source Dr. Dobbs article.
// http://drdobbs.com/cpp/199500857
// http://drdobbs.com/cpp/199702312
// http://gfft.sourceforge.net
// also found here:
// http://eetimes.com/design/signal-processing-dsp/4017495/A-Simple-and-Efficient-FFT-Implementation-in-C--Part-I
// and used here:
// http://performous.org

// Original Todd Veldhuizen implementation:
// http://www.oonumerics.org/blitz/examples/fft.html

// Generic "polymorphic" radix-n framework:
// http://osl.iu.edu/publications/prints/2002/zalewski_algorithms_2002.pdf
// http://www.springerlink.com/content/63nmtp2b6x426lrp
// http://books.google.hr/books?id=lqL4wL5aSkkC&pg=PA35&lpg=PA35&dq=class+radix_mapping&source=bl&ots=HDYK7Snudk&sig=-ZdI32QFWvrIHPPRiKpQ3Emvk_E&hl=hr&sa=X&ei=jqfET6iuJoLU-gbE0ImTCg&ved=0CEkQ6AEwAA#v=onepage&q=class%20radix_mapping&f=false

// General DFT/FFT information and tutorials:
// http://www.katjaas.nl/fourier/fourier.html
// http://altdevblogaday.com/2011/05/17/understanding-the-fourier-transform
// http://cnx.org/content/m16334/latest/?collection=col10550/latest
// http://en.wikipedia.org/wiki/Cooley%E2%80%93Tukey_FFT_algorithm
// http://www.engineeringproductivitytools.com/stuff/T0001/index.html
// http://www.briangough.com/fftalgorithms.pdf
// http://cnx.org/content/col10550/latest
// http://cnx.org/content/m16336/latest (FFTW)
// http://www.freeinfosociety.com/media/pdf/2804.pdf
// http://cr.yp.to/f2mult/mateer-thesis.pdf
// http://cacs.usc.edu/education/phys516/c12-2.pdf
// https://ccrma.stanford.edu/~jos/mdft/mdft.html
// http://www.slideshare.net/op205/fast-fourier-transform-presentation
// http://www.alwayslearn.com/DFT%20and%20FFT%20Tutorial/DFTandFFT_FFT_Overview.html
// http://www.cv.nrao.edu/course/astr534/FourierTransforms.html
// http://www.netlib.org/fftpack
// http://astro.berkeley.edu/~jrg/ngst/fft/fft.html
// http://web.cs.dal.ca/~jborwein/Preprints/Talks/CARMA-CE/FFT.pdf
// http://www.mersenneforum.org/showthread.php?t=120&page=2
// http://www.mersenneforum.org/showthread.php?t=10610&page=3

// Algorithmic improvements:
//   http://cr.yp.to/arith/tangentfft-20070919.pdf
//   http://www.apple.com/acg
//   http://jsat.ewi.tudelft.nl/content/volume7/JSAT7_13_Haynal.pdf
//   http://infoscience.epfl.ch/record/33931/files/VetterliN84.pdf
//   http://infoscience.epfl.ch/record/33921/files/VetterliD89.pdf
//   http://math.berkeley.edu/~strain/273.F10/duhamel.vetterli.fft.review.pdf
//   http://www.eurasip.org/Proceedings/Eusipco/2002/articles/paper086.pdf
//   http://spiral.ece.cmu.edu:8080/pub-spiral/pubfile/recfft_15.pdf
//   http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.2.4190&rep=rep1&type=pdf
//   http://www.proofwiki.org/wiki/ProofWiki:Books/H.J._Nussbaumer/Fast_Fourier_Transform_and_Convolution_Algorithms
//   http://www.cse.yorku.ca/~aboelaze/publication/FFT06.pdf
//   http://home.comcast.net/~kmbtib
//   http://www.freeinfosociety.com/media/pdf/2804.pdf
//   http://jsat.ewi.tudelft.nl/content/volume7/JSAT7_13_Haynal.pdf
//   http://www.scribd.com/doc/53503260/%E2%80%9CTwiddle-factor-based-FFT
//   http://front.cc.nctu.edu.tw/Richfiles/5477-03071119230013828.pdf
//   http://crd-legacy.lbl.gov/~dhbailey/dhbpapers/fftzp.pdf
//   http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.36.5717
//   http://www.springerlink.com/content/px24125362v0q04r
//   http://www.pvk.ca/Blog/LowLevel/napa-fft2-implementation-notes.html
//   http://crd-legacy.lbl.gov/~dhbailey/dhbpapers/fftq.pdf
//   http://en.wikipedia.org/wiki/Bruun's_FFT_algorithm
//   http://infoscience.epfl.ch/record/33931/files/VetterliN84.pdf?version=1
//   http://lcav.epfl.ch/people/martin.vetterli
//   http://fmi.spiruharet.ro/bodorin/articles/brdfpvdro.pdf (bit reversal)
// - split/higher radix FFT
//   http://en.wikipedia.org/wiki/Split-radix_FFT_algorithm
//   http://cr.yp.to/bib/entries.html
//   http://cr.yp.to/bib/1984/duhamel.html
//   http://cr.yp.to/bib/1986/duhamel.html
//   http://cr.yp.to/bib/1986/sorensen.html
//   http://cnx.org/content/m12031/latest
//   http://www.fftw.org/newsplit.pdf
//   http://groups.yahoo.com/group/pi-hacks/message/747
//   http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.59.3006&rep=rep1&type=pdf (FFTW)
//   http://www.ces.clemson.edu/~janoski/reu/2008/FFT-book.pdf
//   http://cr.yp.to/f2mult/mateer-thesis.pdf
//   http://www.datasheetarchive.com/indexdl/Datasheet-078/DSAE0074796.pdf (Intel AP-808)
//   http://wwwdim.uqac.ca/~daudet/Cours/Architecture-bac/DOCUMENTS/repertoire435/MMX-et-SSE-par-Michel-Langlais/exemples-d-Intel/split%20radix%20fft
//   http://www.cmlab.csie.ntu.edu.tw/cml/dsp/training/coding/transform/fft.html
//   http://front.cc.nctu.edu.tw/Richfiles/5477-03071119230013828.pdf
//   http://infoscience.epfl.ch/record/34236/files/VetterliD88.pdf
//   http://www.ucoms.org/Papers/hpec_0909.pdf
//   http://www.gweep.net/~rocko/FFT
//   http://www.ece.umassd.edu/Faculty/acosta/ICASSP/Icassp_1999/PDF/SCAN/IC992264.PDF
//   http://cr.yp.to/bib/1968/bergland-8.html
//   http://65.54.113.26/Publication/1837187/an-efficient-split-radix-fft-algorithm
//   http://www.libsou.com/pdf/01625937.pdf
//   http://wn.com/Split_Radix_Fft_Algorithm
// - real through complex
//   http://www.engineeringproductivitytools.com/stuff/T0001/PT10.HTM
//   http://www.fftguru.com/fftguru.com.tutorial2.pdf
//   http://www.katjaas.nl/realFFT/realFFT.html
//   http://www.dspguide.com/ch12/1.htm
//   http://www.ti.com/lit/an/spra291/spra291.pdf
// - pure real
//   http://scholarship.rice.edu/handle/1911/16187
//   http://www.musicdsp.org/files/rvfft.ps
//   http://cr.yp.to/bib/1968/bergland-real.html
//   http://www.springer.com/cda/content/document/cda_downloaddocument/9789048139163-c2.pdf?SGWID=0-0-45-1123959-p173942904
//   http://ieeexplore.ieee.org/xpl/freeabs_all.jsp?arnumber=1165220
//   http://www.ece.uprm.edu/~domingo/teaching/inel6050/Duhamel-Vetterli.pdf
//   http://www.gamedev.net/page/resources/_/technical/math-and-physics/fast-fourier-transforms-r1349
//   http://cnx.org/content/m16338/latest/#bid1
//   http://www-math.mit.edu/~stevenj/preprints/ShaoJo07-preprint.pdf (real conjugate-pair split-radix FFT -> MDCT)
//   http://www.amazon.com/Regularized-Fast-Hartley-Transform-Implementation/dp/9048139163
//   http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.87.3710&rep=rep1&type=pdf
//   http://classes.engr.oregonstate.edu/eecs/winter2009/ece464/AnalyticSignal_Sept1999_SPTrans.pdf
//   http://www.sciencedirect.com/science/article/pii/0169260789900801
//   http://infoscience.epfl.ch/record/33925/files/DuhamelV87.pdf
//   http://www.springerlink.com/content/m0611260519h6jv1
// - sliding DFT
//   http://dream.cs.bath.ac.uk/SDFT
//   http://lac.linuxaudio.org/2008/download/papers/10.pdf
//   http://www.convict.lu/Jeunes/ultimate_stuff/RFT.pdf
// - auto sorting (avoid bit reverse scrambling pass)
//   http://dl.acm.org/citation.cfm?id=1700455
//   http://dsp-book.narod.ru/FFTBB/0270_PDF_C06.pdf
//   http://www.adras.com/FFT-algoritms.t8918-157-5.html
//   http://books.google.hr/books?id=Ct16DS0DsjUC&pg=PA373&lpg=PA373&dq=recursive+out+of+place+fft&source=bl&ots=WxWHqyxzi3&sig=sIPHDxRGI7KkNmZGuYYRTiOzCts&hl=hr&sa=X&ei=s6xgT5m_LcGDOoqxhLQK&ved=0CHkQ6AEwCTgK#v=onepage&q=recursive%20out%20of%20place%20fft&f=false
//   http://www.dsprelated.com/showmessage/110720/1.php
// - "stereo"/two channel/signal computations per run
//   http://www.dspguide.com/ch12/5.htm
// - vectorization
//   http://www.ece.cmu.edu/~franzf/papers/ieee-si.pdf
// - multi-dimension
//   https://www.cs.drexel.edu/files/ts467/DU-CS-05-01.pdf
// - accuracy
//   http://www.dsprelated.com/showmessage/91562/1.php
//   http://www.fftw.org/accuracy/method.html
//   http://en.wikipedia.org/wiki/Fast_Fourier_transform#Accuracy_and_approximations
// - multi-dimensional
//   http://www.eurasip.org/Proceedings/Eusipco/2002/articles/paper086.pdf


// Low/hardware level tweaks/optimizations:
// - general:
//   http://www.ece.cmu.edu/~franzf/teaching/slides-18-645-simd.pdf
//   http://altdevblogaday.com/2011/05/25/instruction-level-parallelism/?replytocom=4824
//   http://www.scribd.com/doc/57793168/88/Shuffling-data
//   http://www.agner.org/optimize/instruction_tables.pdf
//   http://software.intel.com/en-us/articles/measuring-instruction-latency-and-throughput
//   http://www.tommesani.com/IntelAppNotes.html
//   http://www.agner.org/optimize/microarchitecture.pdf
//   http://vgrads.rice.edu/publications/pdfs/AEOS_Codegenerator_For_FFT.pdf
//   http://lwn.net/Articles/255364
//   http://software.intel.com/file/15529
//   http://www.intel.com/content/www/us/en/architecture-and-technology/64-ia-32-architectures-optimization-manual.html
//   http://assets.devx.com/goparallel/18027.pdf
//   http://www.drdobbs.com/go-parallel/article/212903287
// - cache usage/data access patterns
//   http://www.cs.ucc.ie/~da5/papers/cache-efficient-fft.ps.gz
//   http://www.cse.yorku.ca/~aboelaze/publication/FFT06.pdf
//   https://wiki.engr.illinois.edu/display/transformation/Patterns+for+Efficient+Data+Access
//   http://developer.amd.com/documentation/articles/pages/PerformanceOptimizationofWindowsApplicationsonAMDProcessors2.aspx
//   http://books.google.hr/books?id=dQpi46dLJ8gC&pg=PA135&lpg=PA135&dq=fft+fast+data+access+pattern&source=bl&ots=Cmr0aZNiRb&sig=4s1rYTNq-ysXgUAIcNfyPeA1p04&hl=hr&sa=X&ei=q8XpT4yDOM6L4gSfn5DrDQ&sqi=2&ved=0CEYQ6AEwAA#v=onepage&q=fft%20fast%20data%20access%20pattern&f=false
// - FFT specific
//   http://cache.freescale.com/files/32bit/doc/app_note/AN2115.pdf (AltiVec)
//   https://sites.google.com/a/istec.net/prodrig/Home/pubs (near the bottom: SIMD-FFT)
//   http://www.ams.org/journals/mcom/1993-60-201/S0025-5718-1993-1159169-0/home.html (FMA)
//   http://users.ece.cmu.edu/~franzf/papers/msc-franchetti.pdf
//   http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.49.7995
//   http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.184.5514
//   http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.39.5479
//   http://cnx.org/content/m12021/latest
//   http://www.ece.cmu.edu/~franzf/papers/diss-franchetti.pdf
//   http://repository.readscheme.org/ftp/papers/pe98-school/lawall-fft.pdf
//   http://research.ihost.com/lcpc06/final/35/35_Paper.pdf
//   http://eeeic.org/proc/papers/100.pdf
//   http://www.add.ece.ufl.edu/papers/fust.pdf
//   http://www.engr.colostate.edu/~hj/journals/24.pdf
//   http://www.ece.cmu.edu/~franzf/papers/europar03.pdf
//   http://www.ece.cmu.edu/~franzf/papers/cc04.pdf
//   http://domino.watson.ibm.com/comm/research_people.nsf/pages/pengwu.publications.html/$FILE/fp011-ren.pdf
//   http://software.intel.com/en-us/articles/using-intel-advanced-vector-extensions-to-implement-an-inverse-discrete-cosine-transform/?wapkw=AP-922
//   http://www.iseclab.org/people/pw/papers/vecpar04
//   http://www.es.isy.liu.se/publications/papers_and_reports/1999/weidongl_NorChip99.pdf
//   http://ce.et.tudelft.nl/publicationfiles/2086_674_
//   http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.38.1560&rep=rep1&type=pdf
//   http://www.ll.mit.edu/HPEC/agendas/proc07/Day2/08_Agarwal_Pres.pdf (CELL)
//   http://www.eurasip.org/Proceedings/Eusipco/Eusipco2004/defevent/papers/cr1502.pdf

// Code:
// https://bitbucket.org/jpommier/pffft
// http://www.jjj.de/fxt/#fxt
// http://star-www.rl.ac.uk/star/docs/sun194.htx/node8.html
// http://cr.yp.to/djbfft.html
// http://www.musicdsp.org/showone.php?id=79 (Sorensen pure-real)
// http://files.codes-sources.com/fichier.aspx?id=38245&f=sorensen%5cfftc.c&lang=en (Sorensen pure-real)
// http://code.google.com/p/audio-content-analysis/source/browse/trunk/src/rsrfft.cxx?r=9
// http://kaldi.sourceforge.net/srfft_8cc_source.html
// http://wwwdim.uqac.ca/~daudet/Cours/Architecture-bac/DOCUMENTS/repertoire435/MMX-et-SSE-par-Michel-Langlais/exemples-d-Intel/split%20radix%20fft (Intel AP-808)
// https://github.com/pkhuong/Napa-FFT3
// https://github.com/ahefner/bordeaux-fft
// http://staff.science.nus.edu.sg/~phywjs/CZ5101/fft.c (Cooley-Tukey + Stockham)
// http://code.google.com/p/clfft
// http://home.comcast.net/~kmbtib
// http://code.google.com/p/yafft
// http://ldesoras.free.fr/prod.html (FFTReal)
// http://www.ffte.jp
// http://code.google.com/p/in-spirit/wiki/ASFFT
// http://www.kurims.kyoto-u.ac.jp/~ooura
// http://ffmpeg.org/pipermail/ffmpeg-devel/2009-September/080247.html (AltiVec split radix)
// http://vid.ledina.org/homepage/generic_classes/fft.hpp
// https://github.com/alexbw/iPhoneFFT

/// \note
///   The Dr.Dobbs article seems to claim better performance than Intel MKL 7.0
/// even with plain C++ (scalar mode) and radix-2 algorithm, I wasn't able to
/// get even near that performance even with SSE. Even the article author's own
/// http://gfft.sourceforge.net library did not give me the stated level of
/// performance. The current split-radix SSE2 real version of this code is now
/// about as fast as the real FFT in ACML 3.6 for small transforms (128-512)
/// and about 5% to 10% slower for larger ones (up to 8192).
///   Todd Veldhuizen's implementation turned out even worse because it is too
/// much compile-time based so it instantiates an enormous amount of inlined
/// code which simply thrashes the instruction cache (the author admits this
/// problem for all except very small FFTs).
///   For these reasons the current approach is a mix of several approaches and
/// ideas tweaked through the typical trial and error procedure.
///                                           (15.02.2012.) (Domagoj Saric)

namespace detail
{
    ////////////////////////////////////////////////////////////////////////////
    //
    // Missing NT2 functionality
    //
    ////////////////////////////////////////////////////////////////////////////

    template <unsigned lower_i0, unsigned lower_i1, unsigned upper_i0, unsigned upper_i1>
    BOOST_FORCEINLINE
    __m128 shuffle( __m128 const lower, __m128 const upper )
    {
        return _mm_shuffle_ps( lower, upper, _MM_SHUFFLE( upper_i1, upper_i0, lower_i1, lower_i0 ) );
    }

    template <unsigned i0, unsigned i1, unsigned i2, unsigned i3>
    BOOST_FORCEINLINE
    __m128 shuffle( __m128 const input ) { return shuffle<i0, i1, i2, i3>( input, input ); }

    BOOST_FORCEINLINE
    __m128 reverse( __m128 const input ) { return shuffle<3, 2, 1 , 0>( input ); }

    BOOST_FORCEINLINE
    __m128 deinterleave_first ( __m128 const input0, __m128 const input1 ) { return shuffle<0, 2, 0, 2>( input0, input1 ); }
    BOOST_FORCEINLINE
    __m128 deinterleave_second( __m128 const input0, __m128 const input1 ) { return shuffle<1, 3, 1, 3>( input0, input1 ); }

    BOOST_FORCEINLINE
    __m128 interleave_first ( __m128 const input0, __m128 const input1 ) { return _mm_unpacklo_ps( input0, input1 ); }
    BOOST_FORCEINLINE
    __m128 interleave_second( __m128 const input0, __m128 const input1 ) { return _mm_unpackhi_ps( input0, input1 ); }

    BOOST_FORCEINLINE
    __m128 repeat_lower_half( __m128 const input ) { return _mm_movelh_ps( input, input ); }
    BOOST_FORCEINLINE
    __m128 repeat_upper_half( __m128 const input ) { return _mm_movehl_ps( input, input ); }

    BOOST_FORCEINLINE
    void prefetch_temporary( void const * const location )
    {
    #ifdef __GNUC__
        __builtin_prefetch( location, 0, 0 );
    #elif defined(BOOST_SIMD_HAS_SSE_SUPPORT)
        _mm_prefetch( static_cast<char const *>( location ), _MM_HINT_NTA );
    #endif
    }


    #ifdef _MSC_VER
        #define NT2_CONST_VECTOR( variable_name, ... ) vector_t variable_name; variable_name = __VA_ARGS__;
        #define NT2_VECTOR(       variable_name, ... ) vector_t variable_name; variable_name = __VA_ARGS__;
    #else
        #define NT2_CONST_VECTOR( variable_name, ... ) vector_t const variable_name = { __VA_ARGS__ };
        #define NT2_VECTOR(       variable_name, ... ) vector_t       variable_name = { __VA_ARGS__ };
    #endif // _MSC_VER


    ////////////////////////////////////////////////////////////////////////////
    //
    // sign_flipper()
    // --------------
    //
    ////////////////////////////////////////////////////////////////////////////
    /// \note sign_flipper() returns a restricted pointer (instead of a
    /// reference or simply by value) in order to make it easier for the
    /// compiler to (re)use the constant directly from its static location
    /// instead of bouncing it (or some other intermediate value) onto the
    /// stack. This becomes important in tight register allocation situations
    /// (e.g. inlined DFT16 and DFT8 routines in 32bit x86 builds).
    ///                                       (06.06.2012.) (Domagoj Saric)
    ////////////////////////////////////////////////////////////////////////////

    typedef boost::simd::native<float, BOOST_SIMD_DEFAULT_EXTENSION> flipper_vector_t;

    template <bool e0, bool e1, bool e2, bool e3>
    BOOST_FORCEINLINE
    flipper_vector_t const * BOOST_DISPATCH_RESTRICT sign_flipper()
    {
        boost::int32_t const mzero( 0x80000000 );
        /// \note MSVC10 sometimes generates wrong constants (especially when
        /// the reversed set function (_mm_setr_ps) is used.
        ///                                   (08.03.2012.) (Domagoj Saric)
        /// \note With Clang "true * -0.0f != -0.0f" even in debug builds.
        ///                                   (12.06.2012.) (Domagoj Saric)
        //static flipper_vector_t const flipper = { _mm_setr_ps( e0 * -0.0f, e1 * -0.0f, e2 * -0.0f, e3 * -0.0f ) };

        /// \note Clang generates a dynamic initializer (branching) for:
        /// static flipper_vector_t::native_type const flipper = { _mm_castsi128_ps( _mm_setr_epi32( e0 * mzero, e1 * mzero, e2 * mzero, e3 * mzero ) ) };
        ///                                   (12.06.2012.) (Domagoj Saric)

        static boost::int32_t const BOOST_SIMD_ALIGN_ON( BOOST_SIMD_ARCH_ALIGNMENT ) flipper[ flipper_vector_t::static_size ] = { e0 * mzero, e1 * mzero, e2 * mzero, e3 * mzero };
        return reinterpret_cast<flipper_vector_t const * BOOST_DISPATCH_RESTRICT>( &flipper );
    }


    ////////////////////////////////////////////////////////////////////////////
    ///
    /// \struct danielson_lanczos
    ///
    ////////////////////////////////////////////////////////////////////////////

    template <unsigned N, class Decimation, class Context, typename T, unsigned count_of_T>
    struct danielson_lanczos;


    ////////////////////////////////////////////////////////////////////////////
    ///
    /// Different decimation approaches (in time and in frequency).
    ///
    ////////////////////////////////////////////////////////////////////////////
    // The butterfly loop bodies are provided separately so that the loop can be
    // explicitly unrolled for small FFTs.
    ////////////////////////////////////////////////////////////////////////////

    struct step_decimation {};
    struct step_butterfly  {};

    struct dit //...zzz...not yet fully tested with the new split-radix algorithm...
    {
        typedef step_decimation first_step ;
        typedef step_butterfly  second_step;

        template <class Vector>
        static void BOOST_FASTCALL butterfly
        (
            Vector & r0, Vector & i0,
            Vector & r1, Vector & i1,
            Vector & r2, Vector & i2,
            Vector & r3, Vector & i3,
            split_radix_twiddles<Vector> const & w
        );

        template <typename Vector>
        static void BOOST_FASTCALL danielson_lanczos_4
        (
            Vector const & real_in ,
            Vector const & imag_in ,
            Vector       & real_out,
            Vector       & imag_out
        );
    };


    struct dif
    {
        typedef step_butterfly  first_step ;
        typedef step_decimation second_step;

        template <class Vector>
        static void BOOST_FASTCALL butterfly
        (
            Vector & r0, Vector & i0,
            Vector & r1, Vector & i1,
            Vector & r2, Vector & i2,
            Vector & r3, Vector & i3,
            split_radix_twiddles<Vector> const & w
        );

        template <typename Vector>
        static void BOOST_FASTCALL danielson_lanczos_4
        (
            Vector const & real_in ,
            Vector const & imag_in ,
            Vector       & real_out,
            Vector       & imag_out
        );

        template <typename Vector>
        static void BOOST_FASTCALL danielson_lanczos_8_in_place
        (
            Vector & real0,
            Vector & imag0,
            Vector & real1,
            Vector & imag1
        );
    };

    ////////////////////////////////////////////////////////////////////////////
    /// Contexts (different FFT implementations)
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    // - the separate() member function is used by the real FFT routine to
    //   separate/mix the real data from/to the "emulated complex domain"
    // - in order to minimize stack usage (make it easier for the compiler to
    //   pass parameters through registers) context instances are not passed
    //   "pre created" to lower recursion levels, rather the parameter0_t and
    //   parameter1_t member types (which must be fundamental types) specify the
    //   types for two parameters that hold the minimum information necessary
    //   for the creation of a context which are then passed to the lower
    //   recursion level (or the butterfly loop) which then use them to
    //   construct the context
    ////////////////////////////////////////////////////////////////////////////
    /// \note Currently only the inplace_separated_context_t (separated as
    /// opposed to interleaved real and imaginary data) is included. An
    /// unfinished/work-in-progress out-of-place pure real context was
    /// previously included but was scraped from the in-repository sources to
    /// make the rest of the (working) code more clear.
    ///                                       (06.06.2012.) (Domagoj Saric)
    ////////////////////////////////////////////////////////////////////////////

    template <typename Scalar>
    boost::simd::native<Scalar, BOOST_SIMD_DEFAULT_EXTENSION> *
    as_vector( Scalar * const p_data )
    {
        typedef boost::simd::native<Scalar, BOOST_SIMD_DEFAULT_EXTENSION> vector_t;
        BOOST_ASSERT_MSG( reinterpret_cast<std::size_t>( p_data ) % sizeof( vector_t ) == 0, "Data misaligned." );
        return reinterpret_cast<vector_t *>( p_data );
    }


    template <typename T>
    struct types
    {
        typedef T                                                           scalar_t;
        typedef boost::simd::native<scalar_t, BOOST_SIMD_DEFAULT_EXTENSION> vector_t;
        typedef typename vector_t::native_type                              native_t;

        typedef split_radix_twiddles<vector_t> twiddles;
    };

#ifdef _MSC_VER
    #pragma warning( push )
    #pragma warning( disable : 4510 ) // Default constructor could not be generated.
    #pragma warning( disable : 4512 ) // Assignment operator could not be generated.
    #pragma warning( disable : 4610 ) // Class can never be instantiated - user-defined constructor required.
#endif

    ////////////////////////////////////////////////////////////////////////////
    // \struct inplace_separated_context_t
    ////////////////////////////////////////////////////////////////////////////

    template <typename T>
    struct inplace_separated_context_t
    {
    public:
        typedef typename types<T>::scalar_t scalar_t;
        typedef typename types<T>::vector_t vector_t;
        typedef typename types<T>::native_t native_t;

        typedef typename types<T>::twiddles twiddles;

        typedef vector_t * BOOST_DISPATCH_RESTRICT parameter0_t;
        typedef vector_t * BOOST_DISPATCH_RESTRICT parameter1_t;

        typedef dif decimation_t;
      //typedef dit decimation_t; //...zzz...not yet fully tested with the new split-radix algorithm...

        template <unsigned RealP>
        struct complex_P : boost::mpl::integral_c<unsigned int, RealP - 1> {};

        BOOST_FORCEINLINE
        inplace_separated_context_t
        (
            parameter0_t const p_reals,
            parameter1_t const p_imags,
            unsigned int const N
        )
            :
            p_reals_      ( reinterpret_cast<char * BOOST_DISPATCH_RESTRICT>( p_reals ) ),
            p_imags_      ( reinterpret_cast<char * BOOST_DISPATCH_RESTRICT>( p_imags ) ),
            log2_N4_bytes_( (std::size_t)nt2::log2( N ) - boost::static_log2<4>::value + boost::static_log2<sizeof( scalar_t )>::value )
        {
            BOOST_ASSERT( boost::simd::memory::is_aligned( p_reals ) );
            BOOST_ASSERT( boost::simd::memory::is_aligned( p_imags ) );
        }

    public: // butterfly interface
        vector_t const & r( unsigned int const part ) const { return *element( p_reals_, part ); }
        vector_t const & i( unsigned int const part ) const { return *element( p_imags_, part ); }
        vector_t       & r( unsigned int const part )       { return *element( p_reals_, part ); }
        vector_t       & i( unsigned int const part )       { return *element( p_imags_, part ); }

        void operator++()
        {
            p_reals_ += sizeof( vector_t ) / sizeof( *p_reals_ );
            p_imags_ += sizeof( vector_t ) / sizeof( *p_imags_ );
        }

        template <unsigned N>
        static twiddles const * BOOST_DISPATCH_RESTRICT twiddle_factors() { return twiddles_interleaved<N, vector_t>::factors(); }

    public: // (split radix) decimation interface
        //...zzz...instead of lower/upper, left/right or even/odd names could be used...
        parameter0_t lower_parameter0       () const { return prefetched_element( p_reals_, 0 ); };
        parameter1_t lower_parameter1       () const { return prefetched_element( p_imags_, 0 ); };

        parameter0_t upper_first_parameter0 () const { return prefetched_element( p_reals_, 2 ); };
        parameter1_t upper_first_parameter1 () const { return prefetched_element( p_imags_, 2 ); };

        parameter0_t upper_second_parameter0() const { return prefetched_element( p_reals_, 3 ); };
        parameter1_t upper_second_parameter1() const { return prefetched_element( p_imags_, 3 ); };

    public: // data setup interface
        template <unsigned int valid_bits>
        static void BOOST_FASTCALL scramble( vector_t * BOOST_DISPATCH_RESTRICT p_reals, vector_t * BOOST_DISPATCH_RESTRICT p_imags )
        {
            scramble<valid_bits>( p_reals->data(), p_imags->data() );
        }

        template <unsigned N>
        static void separate( vector_t * BOOST_DISPATCH_RESTRICT const p_reals, vector_t * BOOST_DISPATCH_RESTRICT const p_imags, native_t const twiddle_sign_flipper )
        {
            separate( p_reals, p_imags, twiddle_factors<N>(), twiddle_sign_flipper, N );
        }

    private:
        template <unsigned int valid_bits>
        static void BOOST_FASTCALL scramble( scalar_t * BOOST_DISPATCH_RESTRICT p_reals, scalar_t * BOOST_DISPATCH_RESTRICT p_imags );

        static void BOOST_FASTCALL separate
        (
            vector_t           * BOOST_DISPATCH_RESTRICT p_reals,
            vector_t           * BOOST_DISPATCH_RESTRICT p_imags,
            twiddles     const * BOOST_DISPATCH_RESTRICT p_twiddle_factors,
            native_t                                     twiddle_sign_flipper,
            unsigned int                                 N
        );

        vector_t * BOOST_DISPATCH_RESTRICT reals() const { return reinterpret_cast<vector_t * BOOST_DISPATCH_RESTRICT>( p_reals_ ); }
        vector_t * BOOST_DISPATCH_RESTRICT imags() const { return reinterpret_cast<vector_t * BOOST_DISPATCH_RESTRICT>( p_imags_ ); }

        vector_t * BOOST_DISPATCH_RESTRICT element( char * BOOST_DISPATCH_RESTRICT const p_data, unsigned int const part ) const
        {
            char * BOOST_DISPATCH_RESTRICT const p_element( &p_data[ part << log2_N4_bytes_ ] );
            BOOST_ASSUME( p_element );
            return reinterpret_cast<vector_t * BOOST_DISPATCH_RESTRICT>( p_element );
        }

        vector_t * BOOST_DISPATCH_RESTRICT prefetched_element( char * BOOST_DISPATCH_RESTRICT const p_data, unsigned int const part ) const
        {
            vector_t * BOOST_DISPATCH_RESTRICT const p_element( element( p_data, part ) );
            prefetch_temporary( p_element );
            return p_element;
        }

    private:
        char * BOOST_DISPATCH_RESTRICT p_reals_;
        char * BOOST_DISPATCH_RESTRICT p_imags_;
        unsigned int const log2_N4_bytes_; ///< log2( N/4 ) * sizeof( scalar_t )
    }; // inplace_separated_context_t

#ifdef _MSC_VER
    #pragma warning( pop )
#endif

    template <class T>
    struct assert_no_default_case
    {
        typedef T result_type;
        result_type operator()( int ) const
        {
            BOOST_ASSERT_MSG( false, "unexpected default case in switch" );
            BOOST_UNREACHABLE_CODE();
        }
    };
} // namespace detail


////////////////////////////////////////////////////////////////////////////////
///
/// \class static_fft
///
////////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4510 ) // Default constructor could not be generated.
#pragma warning( disable : 4512 ) // Assignment operator could not be generated.
#pragma warning( disable : 4610 ) // Class can never be instantiated - user-defined constructor required.
#endif

template <unsigned short MinimumSize, unsigned short MaximumSize, typename T>
class static_fft
{
private:
    typedef typename detail::types<T>::scalar_t scalar_t;
    typedef typename detail::types<T>::vector_t vector_t;
    typedef typename detail::types<T>::native_t native_t;

    typedef
        boost::mpl::range_c
        <
            std::size_t,
            boost::static_log2<MinimumSize>::value,
            boost::static_log2<MaximumSize>::value + 1
        > fft_sizes_t;

#ifdef _MSC_VER
    #pragma warning( push )
    #pragma warning( disable : 4127 ) // Conditional expression is constant.
#endif

    template <class Context>
    struct transformer_t
    {
    public:
        typedef void result_type;

        typedef Context context_t;
        typedef typename context_t::parameter0_t parameter0_t;
        typedef typename context_t::parameter1_t parameter1_t;

        transformer_t( parameter0_t const param0, parameter1_t const param1 ) : context_parameter0_( param0 ), context_parameter1_( param1 ) {}

        template <typename FFTSizeExponent>
        result_type operator()( FFTSizeExponent ) const
        {
            static std::size_t const P = FFTSizeExponent::value;
            static std::size_t const N = 1 << P;

            using namespace detail;

            typedef typename context_t::decimation_t decimation_t;

            typedef danielson_lanczos
            <
                N,
                decimation_t,
                context_t,
                vector_t,
                vector_t::static_size
            > recursion;

            if ( boost::is_same<decimation_t, dit>::value )
                context_t:: template scramble<P>( context_parameter0_, context_parameter1_ );

            recursion::apply( context_parameter0_, context_parameter1_ );

            if ( boost::is_same<decimation_t, dif>::value )
                context_t:: template scramble<P>( context_parameter0_, context_parameter1_ );
        }

    protected:
        typename context_t::parameter0_t const context_parameter0_;
        typename context_t::parameter1_t const context_parameter1_;
    }; // transformer_t

#ifdef _MSC_VER
    #pragma warning( pop )
#endif

    template <class Context>
    struct forward_real_transformer_t : transformer_t<Context>
    {
        forward_real_transformer_t( typename Context::parameter0_t const param0, typename Context::parameter1_t const param1 ) : transformer_t<Context>( param0, param1 ) {}

        template <typename FFTSizeExponent>
        void operator()( FFTSizeExponent ) const
        {
            static std::size_t const P = FFTSizeExponent::value;
            static std::size_t const N = 1 << P;

            transformer_t<Context>::operator()( typename Context::template complex_P<P>() );

            Context:: template separate<N>( this->context_parameter0_, this->context_parameter1_, Zero<vector_t>() );
        }
    }; // forward_real_transformer_t

    template <class Context>
    struct backward_real_transformer_t : transformer_t<Context>
    {
        //...zzz...this class uses internal knowledge about the used Context,
        //...zzz...namely that its parameter0 and parameter1 represent pointers
        //...zzz...to real and imaginary data respectively...clean this up...

        /// \note The inverse transform is implemented with the "switch real and
        /// imaginary parts" trick. This enables the same code to be used for
        /// both the forward and inverse transform and/or without the need to
        /// pass (and xor with) the "twiddle flipper/negator/conjugator"
        /// constant.
        ///                                   (05.06.2012.) (Domagoj Saric)
        backward_real_transformer_t( typename Context::parameter0_t const p_reals, typename Context::parameter1_t const p_imags )
            : transformer_t<Context>( p_imags, p_reals ) {}

        template <typename FFTSizeExponent>
        void operator()( FFTSizeExponent ) const
        {
            static std::size_t const P = FFTSizeExponent::value;
            static std::size_t const N = 1 << P;

            /// \note The "switch real and imaginary parts" trick does not work
            /// with the separate() procedure so we must "swap back" the data
            /// pointers and pass the appropriate "twiddle conjugator" constant.
            ///                               (06.06.2012.) (Domagoj Saric)
            Context:: template separate<N>( this->context_parameter1_, this->context_parameter0_, Mzero<vector_t>() );

            transformer_t<Context>::operator()( typename Context::template complex_P<P>() );
       }
    }; // backward_real_transformer_t

public:
    static void forward_transform( T * const p_real_data, T * const p_imaginary_data, std::size_t const size )
    {
        typedef detail::inplace_separated_context_t<T> context_t;
        do_transform( transformer_t<context_t>( detail::as_vector( p_real_data ), detail::as_vector( p_imaginary_data ) ), size );
    }

    static void inverse_transform( T * const p_real_data, T * const p_imaginary_data, std::size_t const size )
    {
        /// \note The inverse transform is implemented with the "switch real and
        /// imaginary parts" trick. This enables the same code to be used for
        /// both the forward and inverse transform and/or without the need to
        /// pass (and xor with) the "twiddle flipper/negator/conjugator"
        /// constant.
        ///                                   (05.06.2012.) (Domagoj Saric)
        forward_transform( p_imaginary_data, p_real_data, size );
    }

    static void real_forward_transform
    (
        T const * const real_time_data,
        T       * const real_frequency_data,
        T       * const imag_frequency_data,
        std::size_t const size
    )
    {
        // Separate ("deinterleave") into even and odd parts ("emulated"
        // complex data):
        vector_t const *                         const p_time_data_end( detail::as_vector( const_cast<T *>( real_time_data + size ) ) );
        vector_t const * BOOST_DISPATCH_RESTRICT       p_time_data    ( detail::as_vector( const_cast<T *>( real_time_data        ) ) );
        vector_t       * BOOST_DISPATCH_RESTRICT       p_reals        ( detail::as_vector(                  real_frequency_data     ) );
        vector_t       * BOOST_DISPATCH_RESTRICT       p_imags        ( detail::as_vector(                  imag_frequency_data     ) );
        while ( p_time_data < p_time_data_end )
        {
            native_t const pair0( *p_time_data++ );
            native_t const pair1( *p_time_data++ );

            *p_reals++ = detail::deinterleave_first ( pair0, pair1 );
            *p_imags++ = detail::deinterleave_second( pair0, pair1 );
        }

        typedef detail::inplace_separated_context_t<T> context_t;
        do_transform( forward_real_transformer_t<context_t>( detail::as_vector( real_frequency_data ), detail::as_vector( imag_frequency_data ) ), size );
    }

    static void real_inverse_transform //...zzz...destroys the input data...
    (
        T /*const*/ * const real_frequency_data,
        T /*const*/ * const imag_frequency_data,
        T           * const real_time_data,
        std::size_t const size
    )
    {
        typedef detail::inplace_separated_context_t<T> context_t;
        do_transform( backward_real_transformer_t<context_t>( detail::as_vector( real_frequency_data ), detail::as_vector( imag_frequency_data ) ), size );

        vector_t const *                         const p_time_data_end( detail::as_vector( real_time_data + size ) );
        vector_t       * BOOST_DISPATCH_RESTRICT       p_time_data    ( detail::as_vector( real_time_data        ) );
        vector_t const * BOOST_DISPATCH_RESTRICT       p_reals        ( detail::as_vector( real_frequency_data   ) );
        vector_t const * BOOST_DISPATCH_RESTRICT       p_imags        ( detail::as_vector( imag_frequency_data   ) );
        while ( p_time_data < p_time_data_end )
        {
            vector_t * BOOST_DISPATCH_RESTRICT const p_data0( p_time_data++ );
            vector_t * BOOST_DISPATCH_RESTRICT const p_data1( p_time_data++ );

            native_t const reals( *p_reals++ );
            native_t const imags( *p_imags++ );

            native_t const pair0( detail::interleave_first ( reals, imags ) );
            native_t const pair1( detail::interleave_second( reals, imags ) );

            *p_data0 = pair0;
            *p_data1 = pair1;
        }
    }

    // todo: transform of two or more real sequences

private:
    template <class Trasformer>
    static void do_transform( Trasformer const & transformer, std::size_t const size )
    {
        boost::control::switch_<void>
        (
            (std::size_t)nt2::log2( size ),
            boost::control::case_<fft_sizes_t>(transformer),
            detail::assert_no_default_case<typename Trasformer::result_type>()
        );
    }
}; // class static_fft

#ifdef _MSC_VER
#pragma warning( push )
#endif

template <typename T>
T complex_fft_normalization_factor( std::size_t const size )
{
    return 1 / static_cast<T>( static_cast<std::ptrdiff_t>( size ) );
}

template <typename T>
T real_fft_normalization_factor( std::size_t const size )
{
    return 1 / static_cast<T>( static_cast<std::ptrdiff_t>( 2 * size ) );
}


namespace detail
{
    ////////////////////////////////////////////////////////////////////////////
    //
    // reverse_bits()
    // --------------
    //
    ////////////////////////////////////////////////////////////////////////////
    // Only 16 bit numbers supported.
    ////////////////////////////////////////////////////////////////////////////

    // http://graphics.stanford.edu/~seander/bithacks.html#BitReverseTable
    // http://aggregate.org/MAGIC/#Bit Reversal
    // http://stackoverflow.com/questions/932079/in-place-bit-reversed-shuffle-on-an-array
    static unsigned char const bit_reverse_table[ 256 ] =
    {
    #   define R2(n)    n ,    n + 2*64 ,    n + 1*64 ,    n + 3*64
    #   define R4(n) R2(n), R2(n + 2*16), R2(n + 1*16), R2(n + 3*16)
    #   define R6(n) R4(n), R4(n + 2*4 ), R4(n + 1*4 ), R4(n + 3*4 )
        R6(0), R6(2), R6(1), R6(3)
    };

    template <unsigned int valid_bits>
    unsigned int BOOST_FASTCALL reverse_bits( unsigned int const value, boost::mpl::int_<1> /*number of bytes*/ )
    {
        unsigned int const shift_correction( 8 - valid_bits );
        unsigned int const bit_reversed_value( bit_reverse_table[ value ] >> shift_correction );
        return bit_reversed_value;
    }

    template <unsigned int valid_bits>
    unsigned int BOOST_FASTCALL reverse_bits( unsigned int const value, boost::mpl::int_<2> /*number of bytes*/ )
    {
        unsigned int const shift_correction_lower(       16 - valid_bits   );
        unsigned int const shift_correction_upper( 8 - ( 16 - valid_bits ) );

        unsigned int const bit_reversed_value
        (
            ( bit_reverse_table[ ( value >> 0 ) & 0xFF ] << shift_correction_upper ) |
            ( bit_reverse_table[ ( value >> 8 )        ] >> shift_correction_lower )
        );
        return bit_reversed_value;
    }

    BOOST_FORCEINLINE
    unsigned int BOOST_FASTCALL reverse_bits( unsigned int const value, unsigned int const valid_bits )
    {
        // Only 16 bit numbers supported (for efficiency):
        BOOST_ASSUME( valid_bits <= 16 );

        typedef unsigned char bytes[ sizeof( value ) ];
        bytes const & input_bytes( reinterpret_cast<bytes const &>( value ) );
        #if defined( BOOST_LITTLE_ENDIAN )
            unsigned const byte0index( 0 );
            unsigned const byte1index( 1 );
            #ifndef NDEBUG
                unsigned const byte2index( 2 );
                unsigned const byte3index( 3 );
            #endif // NDEBUG
        #elif defined( BOOST_BIG_ENDIAN )
            unsigned const byte0index( 3 );
            unsigned const byte1index( 2 );
            #ifndef NDEBUG
                unsigned const byte2index( 1 );
                unsigned const byte3index( 0 );
            #endif // NDEBUG
        #endif // BOOST_LITTLE_ENDIAN
        BOOST_ASSERT( value < ( 1 << 16 )            );
        BOOST_ASSERT( input_bytes[ byte2index ] == 0 );
        BOOST_ASSERT( input_bytes[ byte3index ] == 0 );
        unsigned char const byte0         ( input_bytes[ byte0index ] );
        unsigned char const byte1         ( input_bytes[ byte1index ] );
        unsigned char const reversed_byte0( bit_reverse_table[ byte0 ] );
        unsigned char const reversed_byte1( bit_reverse_table[ byte1 ] );

        unsigned int const bit_reversed_value          ( ( reversed_byte0 << 8 ) | ( reversed_byte1 << 0 ) );
        unsigned int const valid_bits_correction_shift ( 16 - valid_bits                                   );
        unsigned int const corrected_bit_reversed_value( bit_reversed_value >> valid_bits_correction_shift );
        return corrected_bit_reversed_value;
    }


    ////////////////////////////////////////////////////////////////////////////
    //
    // scramble()
    // ----------
    //
    ////////////////////////////////////////////////////////////////////////////
    /// \brief Performs the bit reversal pass.
    /// \param data       - time domain data
    /// \param valid_bits - maximum number of bits valid in FFT bin indices
    ///                     (basically the FFT size expressed as 2^valid_bits)
    ////////////////////////////////////////////////////////////////////////////
    /// \note KissFFT doesn't seem to have a separate scrambling/bit reversal
    /// pass (investigate). See this KissFFT vs FFTW discussion
    /// http://www.dsprelated.com/showmessage/10150/1.php for notes about the
    /// scramble and separate passes.
    ///                                       (29.02.2012.) (Domagoj Saric)
    ////////////////////////////////////////////////////////////////////////////

    typedef double reim_pair_t;

    void BOOST_FORCEINLINE swap
    (
        reim_pair_t * BOOST_DISPATCH_RESTRICT const p_reim_pairs,
        unsigned int const index,
        unsigned int const mirror_index
    )
    {
        reim_pair_t * BOOST_DISPATCH_RESTRICT const p_left_reim_pair ( &p_reim_pairs[ index        ] );
        reim_pair_t * BOOST_DISPATCH_RESTRICT const p_right_reim_pair( &p_reim_pairs[ mirror_index ] );
        std::iter_swap( p_left_reim_pair, p_right_reim_pair );
    }

    void BOOST_FORCEINLINE swap
    (
        float        * BOOST_DISPATCH_RESTRICT const p_reals,
        float        * BOOST_DISPATCH_RESTRICT const p_imags,
        unsigned int                           const index,
        unsigned int                           const mirror_index
    )
    {
        float * BOOST_DISPATCH_RESTRICT const p_left_real ( &p_reals[ index        ] );
        float * BOOST_DISPATCH_RESTRICT const p_right_real( &p_reals[ mirror_index ] );
        float * BOOST_DISPATCH_RESTRICT const p_left_imag ( &p_imags[ index        ] );
        float * BOOST_DISPATCH_RESTRICT const p_right_imag( &p_imags[ mirror_index ] );
        std::iter_swap( p_left_real, p_right_real );
        std::iter_swap( p_left_imag, p_right_imag );
    }

    template <unsigned int valid_bits>
    void scramble1( float * BOOST_DISPATCH_RESTRICT const p_reals, float * BOOST_DISPATCH_RESTRICT const p_imags )
    {
        typedef boost::mpl::int_<( ( valid_bits - 1 ) / 8 ) + 1> number_of_bytes;

        unsigned int const N( 1 << valid_bits );

        unsigned int const half_N( N / 2 );
        unsigned int i( 1 );
        unsigned int j( 0 );
        while ( i < half_N )
        {
            // odd:
            j += half_N;
            BOOST_ASSERT( j == reverse_bits<valid_bits>( i, number_of_bytes() ) );
            swap( p_reals, p_imags, i, j );
            ++i;

            // even:
            j = reverse_bits<valid_bits>( i, number_of_bytes() );
            if ( i < j )
            {
                swap( p_reals, p_imags, i, j );
                BOOST_ASSERT( j < half_N );
                /// \note See
                ///  - fxtbook, chapter 2.1.3
                ///  - http://www.codeproject.com/Articles/9388/How-to-implement-the-FFT-algorithm
                ///                           (16.05.2012.) (Domagoj Saric)
                /// \note The mirrored indices can be calculated by xoring
                /// instead of subtracting (( N - 1 ) ^ i == N - 1 - i) this
                /// however seems slightly slower (MSVC10+i5).
                ///                       (01.03.2012.) (Domagoj Saric)
                swap( p_reals, p_imags, N - 1 - i, N - 1 - j );
            }
            ++i;
        }
    }

    template<class T>
    inline T lastSetBit(T t)
    {
      return nt2::ffs(t)-1;
    }

    inline
    void scramble2( float * const data, unsigned int const valid_bits )
    {
        // http://www.katjaas.nl/bitreversal/bitreversal.html
        //...zzz...doesn't work?

        reim_pair_t * BOOST_DISPATCH_RESTRICT const p_reim_pairs( reinterpret_cast<reim_pair_t *>( data ) );

        unsigned int const N     ( 1 << valid_bits );
        unsigned int const halfn ( N / 2 );            // frequently used 'constants'
        unsigned int const quartn( N / 4 );
        unsigned int const nmin1 ( N - 1 );

        unsigned int forward( halfn );         // variable initialisations
        unsigned int reverse( 1     );

        for ( unsigned int i( quartn ); i; --i )    // start of bitreversed permutation loop, N/4 iterations
        {
            // Gray code generator for even values:
            unsigned int const trailing_zeros( lastSetBit( i ) );
            forward ^= 2      << trailing_zeros;    // toggle one bit of forward
            reverse ^= quartn >> trailing_zeros;    // toggle one bit of reverse

            BOOST_ASSERT( reverse == reverse_bits( forward, valid_bits ) );

            if ( forward < reverse ) // swap even and ~even conditionally
            {
                swap( p_reim_pairs, forward, reverse );
                unsigned int const nodd   ( nmin1 ^ forward );
                unsigned int const noddrev( nmin1 ^ reverse );
                BOOST_ASSERT( noddrev == reverse_bits( nodd, valid_bits ) );
                swap( p_reim_pairs, nodd, noddrev );
            }
        }
    }

    inline
    void scramble3( float * const data, unsigned int const valid_bits )
    {
        // http://caladan.nanosoft.ca/c4/software/bitsort.php (seems slower than scramble1)
        reim_pair_t * BOOST_DISPATCH_RESTRICT const p_reim_pairs( reinterpret_cast<reim_pair_t *>( data ) );

        unsigned int const end_index( 1 << valid_bits );
        unsigned int const s        ( valid_bits - 1  );

        unsigned int forward( 0 );
        unsigned int reverse( 0 );
        unsigned int i      ( 1 );
        while ( i < end_index )
        {
            unsigned int const trailing_zeros( lastSetBit( i++ ) );
            forward ^= 1 <<       trailing_zeros  ;
            reverse ^= 1 << ( s - trailing_zeros );
            BOOST_ASSERT( reverse == reverse_bits( forward, valid_bits ) );
            if ( forward < reverse )
            {
                swap( p_reim_pairs, forward, reverse );
            }
        }
    }


    ////////////////////////////////////////////////////////////////////////////
    ///
    /// context implementations
    ///
    ////////////////////////////////////////////////////////////////////////////

    /// \todo
    ///  - better algorithm (conjugate-pair split radix, pure real...)
    ///  - out-of-place scrambless algorithm
    ///  - in-place scrambless algorithm
    ///  - use FMA, SSE3 complex math and AVX gather/scatter constructs
    ///    http://www.masm32.com/board/index.php?topic=15955.0
    ///  - vectorized scrambling if possible
    ///  - merge separate, scramble and (de/re)interleave functions/passes
    ///  - in-place complete (de)interleave algorithm
    ///    http://stackoverflow.com/questions/7780279/de-interleave-an-array-in-place
    ///    http://groups.google.com/group/comp.programming/browse_thread/thread/b335648ee7e3d065/4820cf6cd9ef6b2c?#4820cf6cd9ef6b2c
    ///    http://www.dsprelated.com/groups/audiodsp/show/700.php
    ///    http://arxiv.org/abs/0805.1598
    ///    http://stackoverflow.com/questions/1777901/array-interleaving-problem
    ///  - zero-padded FFT
    ///  - sliding FFT
    ///  - FFT with integrated/optimized-for WOLA (avoiding redundant recalculations)
    ///  - zero phase FFT (integrate Matlab's fftshift())
    ///  - polar FFT
    ///    http://www.cs.technion.ac.il/~elad/publications/journals/2004/30_PolarFFT_ACHA.pdf
    ///    http://www-user.tu-chemnitz.de/~potts/paper/polarfft.pdf
    ///    http://www.cs.tau.ac.il/~amir1/PS/Polar_Paper_New.pdf
    ///  - backend support (ACML, MKL, FFTW, KissFFT...related Eigen discussion:
    ///    http://listengine.tuxfamily.org/lists.tuxfamily.org/eigen/2012/04/msg00011.html)
    ///                                       (04.07.2012.) (Domagoj Saric)


    ////////////////////////////////////////////////////////////////////////////
    // inplace_separated_context_t
    ////////////////////////////////////////////////////////////////////////////

    template <typename T>
    template <unsigned valid_bits>
    void inplace_separated_context_t<T>::scramble( scalar_t * BOOST_DISPATCH_RESTRICT const p_reals, scalar_t * BOOST_DISPATCH_RESTRICT const p_imags )
    {
        scramble1<valid_bits>( p_reals, p_imags );
    }


    template <typename T>
    BOOST_DISPATCH_NOINLINE
    void BOOST_FASTCALL inplace_separated_context_t<T>::separate
    (
        vector_t           * BOOST_DISPATCH_RESTRICT const p_reals          , // N/2 + 1 scalars
        vector_t           * BOOST_DISPATCH_RESTRICT const p_imags          , // N/2 + 1 scalars
        twiddles     const * BOOST_DISPATCH_RESTRICT       p_twiddle_factors, // requires N/4 twiddle factors
        native_t                                     const twiddle_flipper  ,
        unsigned int                                 const N                  // power-of-two
    )
    {
        prefetch_temporary( p_twiddle_factors );

        scalar_t * BOOST_DISPATCH_RESTRICT p_lower_reals( &p_reals->data()[ 1 ] );
        scalar_t * BOOST_DISPATCH_RESTRICT p_lower_imags( &p_imags->data()[ 1 ] );
        vector_t * BOOST_DISPATCH_RESTRICT p_upper_reals( &p_reals[ N / vector_t::static_size / 2 - 1 ] );
        vector_t * BOOST_DISPATCH_RESTRICT p_upper_imags( &p_imags[ N / vector_t::static_size / 2 - 1 ] );

        NT2_CONST_VECTOR( twiddle_sign_flipper, twiddle_flipper );

        while ( p_lower_reals < p_upper_reals->data() )
        {
        /* "straight" implementation:
            ...zzz...old version where lower_* data was loaded aligned and
            ...zzz...upper_* data was loaded unaligned...
            // the following two constants go outside the loop:
            NT2_CONST_VECTOR(              half, Half<vector_t>()      );
            NT2_CONST_VECTOR( signed_half, half ^ twiddle_sign_flipper );

            NT2_CONST_VECTOR( wr, p_twiddle_factors->wr                           );
            NT2_CONST_VECTOR( wi, p_twiddle_factors->wi ^ *p_twiddle_sign_flipper );
            p_twiddle_factors++;

            NT2_CONST_VECTOR( upper_r, reverse( unaligned_load<vector_t>( p_upper_reals ) ) );
            NT2_CONST_VECTOR( upper_i, reverse( unaligned_load<vector_t>( p_upper_imags ) ) );
            NT2_CONST_VECTOR( lower_r,                                   *p_lower_reals     );
            NT2_CONST_VECTOR( lower_i,                                   *p_lower_imags     );

            NT2_CONST_VECTOR( h1r,        half * ( lower_r + upper_r ) );
            NT2_CONST_VECTOR( h1i,        half * ( lower_i - upper_i ) );
            NT2_CONST_VECTOR( h2r, signed_half * ( lower_i + upper_i ) );
            NT2_CONST_VECTOR( h2i, signed_half * ( upper_r - lower_r ) );
        */
            /// \note Alternate implementation that eliminates the signed_half
            /// constant by reordering the operations (and in the end simply
            /// flipping the real instead of the imaginary part of the twiddle
            /// factor). Scaling by 0.5 (the half constant) is also
            /// skipped/eliminated because it is/can be merged into the
            /// normalization factor.
            ///                               (27.06.2012.) (Domagoj Saric)

            NT2_CONST_VECTOR( upper_r, reverse                 ( *p_upper_reals ) /**= half*/ );
            NT2_CONST_VECTOR( upper_i, reverse                 ( *p_upper_imags ) /**= half*/ );
            NT2_CONST_VECTOR( lower_r, unaligned_load<vector_t>(  p_lower_reals ) /**= half*/ );
            NT2_CONST_VECTOR( lower_i, unaligned_load<vector_t>(  p_lower_imags ) /**= half*/ );

            NT2_CONST_VECTOR( wr, p_twiddle_factors->w0.wr ^ twiddle_sign_flipper );
            NT2_CONST_VECTOR( wi, p_twiddle_factors->w0.wi                        );
            prefetch_temporary( ++p_twiddle_factors );

            NT2_CONST_VECTOR( h1r, lower_r + upper_r );
            NT2_CONST_VECTOR( h1i, lower_i - upper_i );
            NT2_CONST_VECTOR( h2r, lower_i + upper_i );
            NT2_CONST_VECTOR( h2i, upper_r - lower_r );

            NT2_CONST_VECTOR( h_temp_r, ( wr * h2r ) - ( wi * h2i ) );
            NT2_CONST_VECTOR( h_temp_i, ( wr * h2i ) + ( wi * h2r ) );

            NT2_CONST_VECTOR( result_upper_r, reverse( h1r      - h_temp_r ) );
            NT2_CONST_VECTOR( result_lower_r,          h1r      + h_temp_r   );
            NT2_CONST_VECTOR( result_upper_i, reverse( h_temp_i - h1i      ) );
            NT2_CONST_VECTOR( result_lower_i,          h1i      + h_temp_i   );

            unaligned_store( result_lower_r, p_lower_reals );
            unaligned_store( result_lower_i, p_lower_imags );
            p_lower_reals += vector_t::static_size;
            p_lower_imags += vector_t::static_size;

            *p_upper_reals-- = result_upper_r;
            *p_upper_imags-- = result_upper_i;
        }

        /// \note Calculate the two purely real components (the first and last,
        /// DC offset and Nyquist, bins).
        ///                                   (15.02.2012.) (Domagoj Saric)
        scalar_t & dc_re     ( p_reals->data()[ 0     ] );
        scalar_t & dc_im     ( p_imags->data()[ 0     ] );
        scalar_t & nyquist_re( p_reals->data()[ N / 2 ] );
        scalar_t & nyquist_im( p_imags->data()[ N / 2 ] );
        scalar_t const real0( dc_re );
        scalar_t const imag0( dc_im );
        dc_re      = ( real0 + imag0 ) * 2; dc_im      = 0;
        nyquist_re = ( real0 - imag0 ) * 2; nyquist_im = 0;
    }


    ////////////////////////////////////////////////////////////////////////////
    // butterfly_loop
    ////////////////////////////////////////////////////////////////////////////

    template <class Decimation, class Context>
    BOOST_DISPATCH_NOINLINE
    void BOOST_FASTCALL butterfly_loop
    (
        typename Context::parameter0_t                                 const param0,
        typename Context::parameter1_t                                 const param1,
        typename Context::twiddles     const * BOOST_DISPATCH_RESTRICT       p_w,
        unsigned int                                                   const N
    )
    {
        prefetch_temporary( p_w );

        Context context( param0, param1, N );

        unsigned counter( N / 4 / Context::vector_t::static_size );
        while ( counter )
        {
            Decimation::butterfly
            (
                context.r( 0 ), context.i( 0 ),
                context.r( 1 ), context.i( 1 ),
                context.r( 2 ), context.i( 2 ),
                context.r( 3 ), context.i( 3 ),
                *p_w++
            );
            prefetch_temporary( p_w );
            ++context;
            --counter;
        }
    }


    template <class Vector>
    BOOST_FORCEINLINE
    void BOOST_FASTCALL dit::butterfly
    (
        Vector & r0_, Vector & i0_,
        Vector & r1_, Vector & i1_,
        Vector & r2_, Vector & i2_,
        Vector & r3_, Vector & i3_,
        split_radix_twiddles<Vector> const & w
    )
    {
        typedef Vector vector_t;

        NT2_CONST_VECTOR( w0r, w.w0.wr );
        NT2_CONST_VECTOR( w0i, w.w0.wi );
        NT2_CONST_VECTOR( w3r, w.w3.wr );
        NT2_CONST_VECTOR( w3i, w.w3.wi );

        NT2_CONST_VECTOR( r2, r2_ );
        NT2_CONST_VECTOR( r3, r3_ );
        NT2_CONST_VECTOR( i2, i2_ );
        NT2_CONST_VECTOR( i3, i3_ );

        NT2_CONST_VECTOR( temp_r2, ( w0r * r2 ) - ( w0i * i2 ) );
        NT2_CONST_VECTOR( temp_i2, ( w0i * r2 ) + ( w0r * i2 ) );
        NT2_CONST_VECTOR( temp_r3, ( w3r * r3 ) - ( w3i * i3 ) );
        NT2_CONST_VECTOR( temp_i3, ( w3i * r3 ) + ( w3r * i3 ) );

        NT2_CONST_VECTOR( t2p3_r, temp_r2 + temp_r3 );
        NT2_CONST_VECTOR( t2p3_i, temp_i2 + temp_i3 );
        NT2_CONST_VECTOR( t2m3_r, temp_i3 - temp_i2 );
        NT2_CONST_VECTOR( t2m3_i, temp_r2 - temp_r3 );

        NT2_CONST_VECTOR( r0, r0_ );
        NT2_CONST_VECTOR( r1, r1_ );
        r0_ = r0 + t2p3_r;
        r2_ = r0 - t2p3_r;
        r1_ = r1 - t2m3_r;
        r3_ = r1 + t2m3_r;

        NT2_CONST_VECTOR( i0, i0_ );
        NT2_CONST_VECTOR( i1, i1_ );
        i0_ = i0 + t2p3_i;
        i2_ = i0 - t2p3_i;
        i1_ = i1 - t2m3_i;
        i3_ = i1 + t2m3_i;
    }

    template <class Vector>
    BOOST_FORCEINLINE
    void BOOST_FASTCALL dif::butterfly
    (
        Vector & r0_, Vector & i0_,
        Vector & r1_, Vector & i1_,
        Vector & r2_, Vector & i2_,
        Vector & r3_, Vector & i3_,
        split_radix_twiddles<Vector> const & w
    )
    {
        typedef Vector vector_t;

        NT2_CONST_VECTOR( r0, r0_ );
        NT2_CONST_VECTOR( r1, r1_ );
        NT2_CONST_VECTOR( r2, r2_ );
        NT2_CONST_VECTOR( r3, r3_ );
        NT2_CONST_VECTOR( i0, i0_ );
        NT2_CONST_VECTOR( i1, i1_ );
        NT2_CONST_VECTOR( i2, i2_ );

                            r0_ = r0 + r2;
                            i0_ = i0 + i2;
        NT2_CONST_VECTOR( t0m2_r, r0 - r2 );
        NT2_CONST_VECTOR( t0m2_i, i0 - i2 );

        NT2_CONST_VECTOR( i3, i3_ );
                            r1_ = r1 + r3;
                            i1_ = i1 + i3;
        NT2_CONST_VECTOR( t1m3_r, r1 - r3 );
        NT2_CONST_VECTOR( t1m3_i, i1 - i3 );

        NT2_CONST_VECTOR( tpj_r, t0m2_r + t1m3_i );
        NT2_CONST_VECTOR( tpj_i, t0m2_i - t1m3_r );
        NT2_CONST_VECTOR( tmj_r, t0m2_r - t1m3_i );
        NT2_CONST_VECTOR( tmj_i, t0m2_i + t1m3_r );

        NT2_CONST_VECTOR( w0r, w.w0.wr );
        NT2_CONST_VECTOR( w0i, w.w0.wi );
        r2_ = ( w0r * tpj_r ) - ( w0i * tpj_i );
        i2_ = ( w0i * tpj_r ) + ( w0r * tpj_i );

        NT2_CONST_VECTOR( w3r, w.w3.wr );
        NT2_CONST_VECTOR( w3i, w.w3.wi );
        r3_ = ( w3r * tmj_r ) - ( w3i * tmj_i );
        i3_ = ( w3i * tmj_r ) + ( w3r * tmj_i );
    }


    template <typename Vector>
    BOOST_FORCEINLINE
    void BOOST_FASTCALL dit::danielson_lanczos_4
    (
        Vector const & real_in ,
        Vector const & imag_in ,
        Vector       & real_out,
        Vector       & imag_out
    )
    {
        //...zzz...still radix-2...:

        typedef          Vector             vector_t;
        typedef typename Vector::value_type scalar_t;

    /* "text book" version
        scalar_t r0( real_in[ 0 ] );
        scalar_t r1( real_in[ 1 ] );
        scalar_t r2( real_in[ 2 ] );
        scalar_t r3( real_in[ 3 ] );
        scalar_t i0( imag_in[ 0 ] );
        scalar_t i1( imag_in[ 1 ] );
        scalar_t i2( imag_in[ 2 ] );
        scalar_t i3( imag_in[ 3 ] );

        { // lower half (size 2)
            scalar_t const r0_( r0 );
            scalar_t const i0_( i0 );
            scalar_t const r1_( r1 );
            scalar_t const i1_( i1 );
            r0 = r0_ + r1_;
            i0 = i0_ + i1_;
            r1 = r0_ - r1_;
            i1 = i0_ - i1_;
        }
        { // upper half (size 2)
            scalar_t const r2_( r2 );
            scalar_t const i2_( i2 );
            scalar_t const r3_( r3 );
            scalar_t const i3_( i3 );
            r2 =            r2_ + r3_                        ;
            i2 =            i2_ + i3_                        ;
            r3 = flip_sign( i2_ - i3_, twiddle_sign_flipper );
            i3 = flip_sign( r3_ - r2_, twiddle_sign_flipper );
        }

        {
            scalar_t const r0_( r0 );
            scalar_t const i0_( i0 );
            scalar_t const r2_( r2 );
            scalar_t const i2_( i2 );
            r0 = r0_ + r2_;
            i0 = i0_ + i2_;
            r2 = r0_ - r2_;
            i2 = i0_ - i2_;
        }
        {
            scalar_t const r1_( r1 );
            scalar_t const i1_( i1 );
            scalar_t const r3_( r3 );
            scalar_t const i3_( i3 );
            r1 = r1_ + r3_;
            i1 = i1_ + i3_;
            r3 = r1_ - r3_;
            i3 = i1_ - i3_;
        }

        real_out[ 0 ] = r0;
        real_out[ 1 ] = r1;
        real_out[ 2 ] = r2;
        real_out[ 3 ] = r3;
        imag_out[ 0 ] = i0;
        imag_out[ 1 ] = i1;
        imag_out[ 2 ] = i2;
        imag_out[ 3 ] = i3;
    */

    /// \note The code below (unlike the text-book/cleaner versions above) tries
    /// to be more optimizer friendly so that it can avoid stack usage with the
    /// limited x86 SSE register file.
    ///                                       (07.03.2012.) (Domagoj Saric)

    //...zzz...no separate bit reversing/scrambling pass experimenting...
    unsigned int const idx0( 0 );
    unsigned int const idx1( 1 );
    unsigned int const idx2( 2 );
    unsigned int const idx3( 3 );

    #if !defined( BOOST_SIMD_HAS_SSE_SUPPORT )

        scalar_t const r2( real_in[ idx2 ] );
        scalar_t const r3( real_in[ idx3 ] );
        scalar_t const r3mr2( r3 - r2 );
        scalar_t const r2pr3( r2 + r3 );

        scalar_t const i2( imag_in[ idx2 ] );
        scalar_t const i3( imag_in[ idx3 ] );
        scalar_t const i2mi3( i2 - i3 );
        scalar_t const i2pi3( i2 + i3 );

        scalar_t const r0( real_in[ idx0 ] );
        scalar_t const r1( real_in[ idx1 ] );
        scalar_t const r0pr1( r0 + r1 );
        scalar_t const r0mr1( r0 - r1 );

        scalar_t const i0( imag_in[ idx0 ] );
        scalar_t const i1( imag_in[ idx1 ] );
        scalar_t const i0pi1( i0 + i1 );
        scalar_t const i0mi1( i0 - i1 );

        real_out[ idx0 ] = r0pr1 + r2pr3;
        real_out[ idx1 ] = r0mr1 + i2mi3;
        real_out[ idx2 ] = r0pr1 - r2pr3;
        real_out[ idx3 ] = r0mr1 - i2mi3;

        imag_out[ idx0 ] = i0pi1 + i2pi3;
        imag_out[ idx1 ] = i0mi1 + r3mr2;
        imag_out[ idx2 ] = i0pi1 - i2pi3;
        imag_out[ idx3 ] = i0mi1 - r3mr2;

    #else // BOOST_SIMD_HAS_SSE_SUPPORT

        NT2_CONST_VECTOR( odd_negate     , sign_flipper<false, true , false, true>() );
        NT2_CONST_VECTOR( negate_last_two, sign_flipper<false, false, true , true>() );

        // Real:
        NT2_CONST_VECTOR( real, real_in );

        NT2_CONST_VECTOR( r0033, shuffle<idx0, idx0, idx3, idx3>( real ) );
        NT2_CONST_VECTOR( r1122, shuffle<idx1, idx1, idx2, idx2>( real ) );

        NT2_CONST_VECTOR( r_combined, r0033 + ( r1122 ^ odd_negate ) );

        NT2_CONST_VECTOR( r_left, repeat_lower_half( r_combined ) );

        // Imaginary:
        NT2_CONST_VECTOR( imag, imag_in );

        NT2_CONST_VECTOR( i0022, shuffle<idx0, idx0, idx2, idx2>( imag ) );
        NT2_CONST_VECTOR( i1133, shuffle<idx1, idx1, idx3, idx3>( imag ) );

        NT2_CONST_VECTOR( i_combined, i0022 + ( i1133 ^ odd_negate ) );

        NT2_CONST_VECTOR( i_left, repeat_lower_half( i_combined ) );

        // Shared (because real right needs i2mi3 and imag right needs r3mr2):
        vector_t right;
        right = interleave_second( r_combined, i_combined );
        // right = [ r3pr2, i2pi3, r3mr2, i2mi3 ]
        right = shuffle<0, 3, 1, 2>( right );
        // right = [ r3pr2, i2mi3, i2pi3, r3mr2 ]

        NT2_CONST_VECTOR( r_right, repeat_lower_half( right ) );
        // r_right = [ r3pr2, i2mi3, r3pr2, i2mi3 ]
        NT2_CONST_VECTOR( i_right, repeat_upper_half( right ) );
        // i_right = [ i2pi3, r3mr2, i2pi3, r3mr2 ]

        real_out = r_left + ( r_right ^ negate_last_two );
        imag_out = i_left + ( i_right ^ negate_last_two );

    #endif // BOOST_SIMD_HAS_SSE_SUPPORT
    }

    template <typename Vector>
    BOOST_FORCEINLINE
    void BOOST_FASTCALL dif::danielson_lanczos_4
    (
        Vector const & real_in ,
        Vector const & imag_in ,
        Vector       & real_out,
        Vector       & imag_out
    )
    {
        typedef          Vector             vector_t;
        typedef typename Vector::value_type scalar_t;

        //...zzz...no separate bit reversing/scrambling pass experimenting...
        unsigned int const idx0( 0 );
        unsigned int const idx1( 1 );
        unsigned int const idx2( 2 );
        unsigned int const idx3( 3 );

    #if !defined( BOOST_SIMD_HAS_SSE_SUPPORT )

        scalar_t r0( real_in[ idx0 ] ); scalar_t i0( imag_in[ idx0 ] );
        scalar_t r1( real_in[ idx1 ] ); scalar_t i1( imag_in[ idx1 ] );
        scalar_t r2( real_in[ idx2 ] ); scalar_t i2( imag_in[ idx2 ] );
        scalar_t r3( real_in[ idx3 ] ); scalar_t i3( imag_in[ idx3 ] );

        // butterfly:
        {
            scalar_t const r0pr2( r0 + r2 ); scalar_t const i0pi2( i0 + i2 );
            scalar_t const r1pr3( r1 + r3 ); scalar_t const i1pi3( i1 + i3 );

            scalar_t const r0mr2( r0 - r2 ); scalar_t const i0mi2( i0 - i2 );
            scalar_t const r1mr3( i3 - i1 ); scalar_t const i1mi3( r1 - r3 ); // multiplication by i "merged"/included

            // merged the lower/even DFT2 calculation:
          //r0 = r0pr2; i0 = i0pi2;
          //r1 = r1pr3; i1 = i1pi3;
            r0 = r0pr2 + r1pr3; i0 = i0pi2 + i1pi3;
            r1 = r0pr2 - r1pr3; i1 = i0pi2 - i1pi3;

            r2 = r0mr2 - r1mr3; i2 = i0mi2 - i1mi3;
            r3 = r0mr2 + r1mr3; i3 = i0mi2 + i1mi3;
        }

        // store lower results (DFT2 already calculated):
        real_out[ idx0 ] = r0; imag_out[ idx0 ] = i0;
        real_out[ idx1 ] = r1; imag_out[ idx1 ] = i1;
        // store upper results (two DFT1s are NOPs):
        real_out[ idx2 ] = r2; imag_out[ idx2 ] = i2;
        real_out[ idx3 ] = r3; imag_out[ idx3 ] = i3;

    #else // BOOST_SIMD_HAS_SSE_SUPPORT
        NT2_CONST_VECTOR( real, real_in ); NT2_CONST_VECTOR( imag, imag_in );
        NT2_CONST_VECTOR( r0101, repeat_lower_half( real ) ); NT2_CONST_VECTOR( i0101, repeat_lower_half( imag ) );
        NT2_CONST_VECTOR( r2323, repeat_upper_half( real ) ); NT2_CONST_VECTOR( i2323, repeat_upper_half( imag ) );

        vector_t const * BOOST_DISPATCH_RESTRICT const p_negate_upper( sign_flipper<false, false, true, true>() );
        NT2_CONST_VECTOR( r_combined, r0101 + ( r2323 ^ *p_negate_upper ) ); NT2_CONST_VECTOR( i_combined, i0101 + ( i2323 ^ *p_negate_upper ) );

        NT2_CONST_VECTOR( r_left , shuffle<idx0, idx0, idx2, idx2>( r_combined             ) ); NT2_CONST_VECTOR( i_left , shuffle<idx0, idx0, idx2, idx2>( i_combined             ) );
        NT2_CONST_VECTOR( r_right, shuffle<idx1, idx1, idx3, idx3>( r_combined, i_combined ) ); NT2_CONST_VECTOR( i_right, shuffle<idx1, idx1, idx3, idx3>( i_combined, r_combined ) );

        real_out = r_left + ( r_right ^ *sign_flipper<false, true, false, true >()/*negate_13*/ );
        imag_out = i_left + ( i_right ^ *sign_flipper<false, true, true , false>()/*negate_12*/ );
    #endif // BOOST_SIMD_HAS_SSE_SUPPORT
    }


    template <typename Vector>
    BOOST_FORCEINLINE
    void BOOST_FASTCALL dif::danielson_lanczos_8_in_place
    (
        Vector & lower_real,
        Vector & lower_imag,
        Vector & upper_real,
        Vector & upper_imag
    )
    {
        typedef          Vector             vector_t;
        typedef typename Vector::value_type scalar_t;

    #if !defined( BOOST_SIMD_HAS_SSE_SUPPORT )
        scalar_t r0( lower_real[ 0 ] ); scalar_t i0( lower_imag[ 0 ] );
        scalar_t r1( lower_real[ 1 ] ); scalar_t i1( lower_imag[ 1 ] );
        scalar_t r2( lower_real[ 2 ] ); scalar_t i2( lower_imag[ 2 ] );
        scalar_t r3( lower_real[ 3 ] ); scalar_t i3( lower_imag[ 3 ] );
        scalar_t r4( upper_real[ 0 ] ); scalar_t i4( upper_imag[ 0 ] );
        scalar_t r5( upper_real[ 1 ] ); scalar_t i5( upper_imag[ 1 ] );
        scalar_t r6( upper_real[ 2 ] ); scalar_t i6( upper_imag[ 2 ] );
        scalar_t r7( upper_real[ 3 ] ); scalar_t i7( upper_imag[ 3 ] );

        scalar_t const r0pr4( r0 + r4 ); scalar_t const i0pi4( i0 + i4 );
        scalar_t const r1pr5( r1 + r5 ); scalar_t const i1pi5( i1 + i5 );
        scalar_t const r2pr6( r2 + r6 ); scalar_t const i2pi6( i2 + i6 );
        scalar_t const r3pr7( r3 + r7 ); scalar_t const i3pi7( i3 + i7 );

        r0 = r0pr4; i0 = i0pi4;
        r1 = r1pr5; i1 = i1pi5;
        r2 = r2pr6; i2 = i2pi6;
        r3 = r3pr7; i3 = i3pi7;

        scalar_t const r0m4( r0 - r4 ); scalar_t const i0m4( i0 - i4 );
        scalar_t const r1m5( r1 - r5 ); scalar_t const i1m5( i1 - i5 );

        scalar_t const r2m6( i6 - i2 ); scalar_t const i2m6( r2 - r6 );
        scalar_t const r3m7( i7 - i3 ); scalar_t const i3m7( r3 - r7 );

        scalar_t const r4_( r0m4 - r2m6 ); scalar_t const i4_( i0m4 - i2m6 );
        scalar_t const r5_( r1m5 - r3m7 ); scalar_t const i5_( i1m5 - i3m7 );

        scalar_t const r6_( r0m4 + r2m6 ); scalar_t const i6_( i0m4 + i2m6 );
        scalar_t const r7_( r1m5 + r3m7 ); scalar_t const i7_( i1m5 + i3m7 );

        float const sqrt2( 0.70710678118654752440084436210485f );
      //float const w0r0( 1 );
      //float const w0i0( 0 );
      //float const w3r0( 1 );
      //float const w3i0( 0 );
      //float const w0r1( +sqrt2 );
      //float const w0i1( -sqrt2 );
      //float const w3r1( -sqrt2 );
      //float const w3i1( -sqrt2 );

      //r4 = r4_ * w0r0 - i4_ * w0i0; i4 = r4_ * w0i0 + i4_ * w0r0;
      //r5 = r5_ * w0r1 - i5_ * w0i1; i5 = r5_ * w0i1 + i5_ * w0r1;

      //r6 = r6_ * w3r0 - i6_ * w3i0; i6 = r6_ * w3i0 + i6_ * w3r0;
      //r7 = r7_ * w3r1 - i7_ * w3i1; i7 = r7_ * w3i1 + i7_ * w3r1;

        r5 = ( r5_ + i5_ ) * +sqrt2; i5 = ( r5_ - i5_ ) * -sqrt2;
        r7 = ( r7_ - i7_ ) * -sqrt2; i7 = ( r7_ + i7_ ) * -sqrt2;

        {
            vector_t lower_r;
            vector_t lower_i;
            lower_r[ 0 ] = r0; lower_i[ 0 ] = i0;
            lower_r[ 1 ] = r1; lower_i[ 1 ] = i1;
            lower_r[ 2 ] = r2; lower_i[ 2 ] = i2;
            lower_r[ 3 ] = r3; lower_i[ 3 ] = i3;
            dif::danielson_lanczos_4<vector_t>
            (
                lower_r,
                lower_i,
                lower_real,
                lower_imag
            );
        }

        {
            scalar_t const r4_( r4 );
            scalar_t const i4_( i4 );
            scalar_t const r5_( r5 );
            scalar_t const i5_( i5 );
            r4 = r4_ + r5_;
            i4 = i4_ + i5_;
            r5 = r4_ - r5_;
            i5 = i4_ - i5_;
        }

        {
            scalar_t const r6_( r6 );
            scalar_t const i6_( i6 );
            scalar_t const r7_( r7 );
            scalar_t const i7_( i7 );
            r6 = r6_ + r7_;
            i6 = i6_ + i7_;
            r7 = r6_ - r7_;
            i7 = i6_ - i7_;
        }

        upper_real[ 0 ] = r4; upper_imag[ 0 ] = i4;
        upper_real[ 1 ] = r5; upper_imag[ 1 ] = i5;
        upper_real[ 2 ] = r6; upper_imag[ 2 ] = i6;
        upper_real[ 3 ] = r7; upper_imag[ 3 ] = i7;

    #else // BOOST_SIMD_HAS_SSE_SUPPORT

        NT2_CONST_VECTOR( lower_r_in, lower_real );
        NT2_CONST_VECTOR( lower_i_in, lower_imag );
        NT2_CONST_VECTOR( upper_r_in, upper_real );
        NT2_CONST_VECTOR( upper_i_in, upper_imag );

        vector_t upper_r; vector_t upper_i;

        // butterfly:
        {
            NT2_CONST_VECTOR( lower_p_upper_r, lower_r_in + upper_r_in ); NT2_CONST_VECTOR( lower_p_upper_i,  lower_i_in + upper_i_in );
            NT2_VECTOR(       lower_m_upper_r, lower_r_in - upper_r_in ); NT2_VECTOR(       lower_m_upper_i,  lower_i_in - upper_i_in );

            // we can already calculate the lower DFT4 so we do it free up
            // registers:
            vector_t const * BOOST_DISPATCH_RESTRICT const p_negate_upper( sign_flipper<false, false, true, true>() );
            {
                //...zzz...manually inlined for testing ("in search of optimal register allocation")...
                //dif::danielson_lanczos_4<vector_t>
                //(
                //    lower_p_upper_r,
                //    lower_p_upper_i,
                //    lower_real,
                //    lower_imag
                //);
                NT2_CONST_VECTOR( r0101, repeat_lower_half( lower_p_upper_r ) ); NT2_CONST_VECTOR( i0101, repeat_lower_half( lower_p_upper_i ) );
                NT2_CONST_VECTOR( r2323, repeat_upper_half( lower_p_upper_r ) ); NT2_CONST_VECTOR( i2323, repeat_upper_half( lower_p_upper_i ) );

                NT2_CONST_VECTOR( r_combined, r0101 + ( r2323 ^ *p_negate_upper ) ); NT2_CONST_VECTOR( i_combined, i0101 + ( i2323 ^ *p_negate_upper ) );

                NT2_CONST_VECTOR( r_left , shuffle<0, 0, 2, 2>( r_combined             ) ); NT2_CONST_VECTOR( i_left , shuffle<0, 0, 2, 2>( i_combined             ) );
                NT2_CONST_VECTOR( r_right, shuffle<1, 1, 3, 3>( r_combined, i_combined ) ); NT2_CONST_VECTOR( i_right, shuffle<1, 1, 3, 3>( i_combined, r_combined ) );

                lower_real = r_left + ( r_right ^ *sign_flipper<false, true, false, true >()/*negate_13*/ );
                lower_imag = i_left + ( i_right ^ *sign_flipper<false, true, true , false>()/*negate_12*/ );
            }

            {
                // multiplication by i:
                NT2_VECTOR( lower_m_upper_r_copy, lower_m_upper_r );
                lower_m_upper_r = shuffle<0, 1, 2 ,3>( lower_m_upper_r, lower_m_upper_i      );
                lower_m_upper_i = shuffle<0, 1, 2 ,3>( lower_m_upper_i, lower_m_upper_r_copy );
                lower_m_upper_r ^= *p_negate_upper;
            }

            NT2_CONST_VECTOR( r_left , repeat_lower_half( lower_m_upper_r ) ); NT2_CONST_VECTOR( i_left , repeat_lower_half( lower_m_upper_i ) );
            NT2_CONST_VECTOR( r_right, repeat_upper_half( lower_m_upper_r ) ); NT2_CONST_VECTOR( i_right, repeat_upper_half( lower_m_upper_i ) );

            upper_r = r_left - ( r_right ^ *p_negate_upper );
            upper_i = i_left - ( i_right ^ *p_negate_upper );

            // the twiddling of the 5th and 7th elements (see the scalar
            // version) is merged into the upper DFT2 calculations below:
        }

        // merged two upper DFT2s:
        {
            NT2_CONST_VECTOR( r4466, shuffle<0, 0, 2, 2>( upper_r ) ); NT2_CONST_VECTOR( i4466, shuffle<0, 0, 2, 2>( upper_i ) );
            NT2_VECTOR(       r5577, shuffle<1, 1, 3, 3>( upper_r ) ); NT2_VECTOR(       i5577, shuffle<1, 1, 3, 3>( upper_i ) );
            NT2_CONST_VECTOR( odd_negate, _mm_setr_ps( 0, -0.0f, 0, -0.0f ) );
            scalar_t const sqrt2( 0.70710678118654752440084436210485f );
            NT2_CONST_VECTOR( twiddles, _mm_setr_ps( +sqrt2, -sqrt2, -sqrt2, -sqrt2 ) );
            NT2_CONST_VECTOR( twiddled_57, ( r5577 + ( i5577 ^ odd_negate ) ) * twiddles );
            r5577 = shuffle<0, 0, 3, 3>( twiddled_57 );
            i5577 = shuffle<1, 1, 2, 2>( twiddled_57 );
            upper_real = r4466 + ( r5577 ^ odd_negate );
            upper_imag = i4466 + ( i5577 ^ odd_negate );
        }

    #endif // BOOST_SIMD_HAS_SSE_SUPPORT
    }


    ////////////////////////////////////////////////////////////////////////////
    /// danielson_lanczos shared implementation and specializations
    ////////////////////////////////////////////////////////////////////////////

    template <unsigned N, class Decimation, class Context, typename T, unsigned count_of_T>
    struct danielson_lanczos
    {
    public:
        typedef typename Context::parameter0_t parameter0_t;
        typedef typename Context::parameter1_t parameter1_t;

        BOOST_NOTHROW_NOALIAS
        static void BOOST_FASTCALL apply( parameter0_t const param0, parameter1_t const param1 )
        {
            apply( param0, param1, typename Decimation::first_step () );
            apply( param0, param1, typename Decimation::second_step() );
        }

    private:
        BOOST_NOTHROW_NOALIAS BOOST_FORCEINLINE
        static void BOOST_FASTCALL apply( parameter0_t const param0, parameter1_t const param1, step_decimation const & )
        {
            Context const context( param0, param1, N );
            typedef danielson_lanczos<N/2, Decimation, Context, T, count_of_T> lower;
            typedef danielson_lanczos<N/4, Decimation, Context, T, count_of_T> upper;
            lower::apply( context.lower_parameter0       (), context.lower_parameter1       () );
            upper::apply( context.upper_first_parameter0 (), context.upper_first_parameter1 () );
            upper::apply( context.upper_second_parameter0(), context.upper_second_parameter1() );
        }

        BOOST_NOTHROW_NOALIAS BOOST_FORCEINLINE
        static void BOOST_FASTCALL apply( typename Context::parameter0_t const param0, typename Context::parameter1_t const param1, step_butterfly const & )
        {
            butterfly_loop<Decimation, Context>
            (
                param0,
                param1,
                Context:: template twiddle_factors<N>(),
                N
            );
        }
    };


    ////////////////////////////////////////////////////////////////////////////
    /// danielson_lanczos specializations that unroll the butterfly loop body
    ///  (for size 8 there is only one pass and for size 16 there are two).
    ////////////////////////////////////////////////////////////////////////////
    //...zzz...still radix-2...
    template <class Context, typename T>
    struct danielson_lanczos<8, dit, Context, T, 4>
    {
    public:
        static unsigned const N = 8;

        BOOST_NOTHROW_NOALIAS
        static void BOOST_FASTCALL apply( typename Context::proxy const data )
        {
            typedef typename Context::vector_t vector_t;

            vector_t upper_r;
            vector_t upper_i;
            dit::danielson_lanczos_4
            (
                (vector_t const &)data.p_reals[ 4 ],
                (vector_t const &)data.p_imags[ 4 ],
                upper_r,
                upper_i
            );

            vector_t lower_r;
            vector_t lower_i;
            dit::danielson_lanczos_4
            (
                (vector_t const &)data.p_reals[ 0 ],
                (vector_t const &)data.p_imags[ 0 ],
                lower_r,
                lower_i
            );

            NT2_CONST_VECTOR( wr, Context:: template twiddles0<N>()->wr );
            NT2_CONST_VECTOR( wi, Context:: template twiddles0<N>()->wi );

            NT2_CONST_VECTOR( temp_r, ( wr * upper_r ) - ( wi * upper_i ) );
            NT2_CONST_VECTOR( temp_i, ( wi * upper_r ) + ( wr * upper_i ) );

            (vector_t &)data.p_reals[ 4 ] = lower_r - temp_r;
            (vector_t &)data.p_imags[ 4 ] = lower_i - temp_i;
            (vector_t &)data.p_reals[ 0 ] = lower_r + temp_r;
            (vector_t &)data.p_imags[ 0 ] = lower_i + temp_i;
        }
    };


    template <class Context, typename T>
    struct danielson_lanczos<8, dif, Context, T, 4>
    {
    public:
        static unsigned const N = 8;

        BOOST_NOTHROW_NOALIAS
        static void BOOST_FASTCALL apply( typename Context::parameter0_t const param0, typename Context::parameter1_t const param1 )
        {
            typedef typename Context::vector_t vector_t;

            //...zzz...uses internal knowledge about the parameter0 and
            //...zzz...parameter1 of the used Context...

            vector_t * BOOST_DISPATCH_RESTRICT const p_reals( param0 );
            vector_t * BOOST_DISPATCH_RESTRICT const p_imags( param1 );

            vector_t * BOOST_DISPATCH_RESTRICT const p_lower_r( &p_reals[ 0 ] );
            vector_t * BOOST_DISPATCH_RESTRICT const p_lower_i( &p_imags[ 0 ] );
            vector_t * BOOST_DISPATCH_RESTRICT const p_upper_r( &p_reals[ 1 ] );
            vector_t * BOOST_DISPATCH_RESTRICT const p_upper_i( &p_imags[ 1 ] );
            dif::danielson_lanczos_8_in_place( *p_lower_r, *p_lower_i, *p_upper_r, *p_upper_i );
        }
    };


    template <class Context, typename T>
    struct danielson_lanczos<16, dif, Context, T, 4>
    {
    public:
        static unsigned const N = 16;

        typedef dif Decimation;

        BOOST_NOTHROW_NOALIAS
        static void BOOST_FASTCALL apply( typename Context::parameter0_t const p_reals, typename Context::parameter1_t const p_imags )
        {
            typedef typename Context::vector_t vector_t;

            split_radix_twiddles<vector_t> const * BOOST_DISPATCH_RESTRICT const p_w( Context:: template twiddle_factors<N>() );
            prefetch_temporary( p_w );

            //...zzz...uses internal knowledge about the parameter0 and
            //...zzz...parameter1 of the used Context...

            vector_t * BOOST_DISPATCH_RESTRICT const p_r0( &p_reals[ 0 ] ); vector_t * BOOST_DISPATCH_RESTRICT const p_i0( &p_imags[ 0 ] );
            vector_t * BOOST_DISPATCH_RESTRICT const p_r1( &p_reals[ 1 ] ); vector_t * BOOST_DISPATCH_RESTRICT const p_i1( &p_imags[ 1 ] );
            vector_t * BOOST_DISPATCH_RESTRICT const p_r2( &p_reals[ 2 ] ); vector_t * BOOST_DISPATCH_RESTRICT const p_i2( &p_imags[ 2 ] );
            vector_t * BOOST_DISPATCH_RESTRICT const p_r3( &p_reals[ 3 ] ); vector_t * BOOST_DISPATCH_RESTRICT const p_i3( &p_imags[ 3 ] );

            //...zzz...manually inlined for testing ("in search of optimal register allocation")...
            //Decimation::butterfly
            //(
            //    *p_r0, *p_i0,
            //    *p_r1, *p_i1,
            //    *p_r2, *p_i2,
            //    *p_r3, *p_i3,
            //    *p_w
            //);

            NT2_CONST_VECTOR( t0p2_r, *p_r0 + *p_r2 );
            NT2_CONST_VECTOR( t0m2_r, *p_r0 - *p_r2 );
            *p_r0 = t0p2_r;
            NT2_CONST_VECTOR( t0p2_i, *p_i0 + *p_i2 );
            NT2_CONST_VECTOR( t0m2_i, *p_i0 - *p_i2 );
            *p_i0 = t0p2_i;

            NT2_CONST_VECTOR( t1p3_r, *p_r1 + *p_r3 );
            NT2_CONST_VECTOR( t1m3_r, *p_r1 - *p_r3 );
            *p_r1 = t1p3_r;
            NT2_CONST_VECTOR( t1p3_i, *p_i1 + *p_i3 );
            NT2_CONST_VECTOR( t1m3_i, *p_i1 - *p_i3 );
            *p_i1 = t1p3_i;

            NT2_CONST_VECTOR( tpj_r, t0m2_r + t1m3_i );
            NT2_CONST_VECTOR( tpj_i, t0m2_i - t1m3_r );
            NT2_CONST_VECTOR( tmj_r, t0m2_r - t1m3_i );
            NT2_CONST_VECTOR( tmj_i, t0m2_i + t1m3_r );

            Decimation::danielson_lanczos_8_in_place
            (
                *p_r0, *p_i0,
                *p_r1, *p_i1
            );

            *p_r2 = ( p_w->w0.wr * tpj_r ) - ( p_w->w0.wi * tpj_i );
            *p_i2 = ( p_w->w0.wi * tpj_r ) + ( p_w->w0.wr * tpj_i );

            *p_r3 = ( p_w->w3.wr * tmj_r ) - ( p_w->w3.wi * tmj_i );
            *p_i3 = ( p_w->w3.wi * tmj_r ) + ( p_w->w3.wr * tmj_i );

            Decimation::danielson_lanczos_4
            (
                *p_r2, *p_i2,
                *p_r2, *p_i2
            );

            Decimation::danielson_lanczos_4
            (
                *p_r3, *p_i3,
                *p_r3, *p_i3
            );
        }
    };


    ////////////////////////////////////////////////////////////////////////////
    /// twiddle-free danielson_lanczos specializations.
    ////////////////////////////////////////////////////////////////////////////

/*  ...zzz...old interface...
    template <class Impl>
    struct danielson_lanczos_dit<2, Impl>
    {
        typedef typename Impl::scalar_t scalar_t;

        static void apply( scalar_t * BOOST_DISPATCH_RESTRICT const data )
        {
            scalar_t const r0( data[ 0 ] );
            scalar_t const i0( data[ 1 ] );
            scalar_t const r1( data[ 2 ] );
            scalar_t const i1( data[ 3 ] );
            data[ 0 ] = r0 + r1;
            data[ 1 ] = i0 + i1;
            data[ 2 ] = r0 - r1;
            data[ 3 ] = i0 - i1;
        }
    };
*/

    ////////////////////////////////////////////////////////////////////////////
    /// danielson_lanczos recursion terminator.
    ////////////////////////////////////////////////////////////////////////////

    template <class Decimation, class Context, typename T, unsigned count_of_T>
    struct danielson_lanczos<1, Decimation, Context, T, count_of_T>
    {
        static void apply( typename Context::proxy )
        {
            // Clang (and probably GCC) is of course overeager to instantiate this...
            #ifndef __GNUC__
                BOOST_STATIC_ASSERT_MSG( false, "Recoursion should have been terminated before." );
            #endif // __GNUC__
        }
    };
} // namespace detail


//...zzz...investigating/testing...
#ifdef LE_SORENSEN_PURE_REAL_FFT_TEST

/////////////////////////////////////////////////////////
// Sorensen in-place split-radix FFT for real values
// data: array of doubles:
// re(0),re(1),re(2),...,re(size-1)
//
// output:
// re(0),re(1),re(2),...,re(size/2),im(size/2-1),...,im(1)
// normalized by array length
//
// Source:
// Sorensen et al: Real-Valued Fast Fourier Transform Algorithms,
// IEEE Trans. ASSP, ASSP-35, No. 6, June 1987

void realfft_split( float * BOOST_DISPATCH_RESTRICT data, unsigned const n )
{
    unsigned i5,i6,i7,i8,i0,id,i1,i2,i3,i4,n8;
    float    t1,t2,t3,t4,t5,t6;

    unsigned n4( n-1 );

    //data shuffling
    for ( unsigned i=0,j=0,n2=n/2; i < n4 ; ++i )
    {
        if (i<j){
            t1=data[j];
            data[j]=data[i];
            data[i]=t1;
        }
        unsigned k( n2 );
        while (k<=j){
            j-=k;
            k>>=1;
        }
        j+=k;
    }

    /*----------------------*/

    //length two butterflies
    i0=0;
    id=4;
    do{
        for (; i0<n4; i0+=id){
            i1=i0+1;
            t1=data[i0];
            data[i0]=t1+data[i1];
            data[i1]=t1-data[i1];
        }
        id<<=1;
        i0=id-2;
        id<<=1;
    } while ( i0<n4 );

    /*----------------------*/
    //L shaped butterflies
    float const sqrt2( std::sqrtf( 2 ) );
    for ( unsigned k=n, n2=2; k>2 ; k>>=1 )
    {
        n2<<=1;
        n4=n2>>2;
        n8=n2>>3;
        i1=0;
        id=n2<<1;
        do{
            for (; i1<n; i1+=id){
                i2=i1+n4;
                i3=i2+n4;
                i4=i3+n4;
                t1=data[i4]+data[i3];
                data[i4]-=data[i3];
                data[i3]=data[i1]-t1;
                data[i1]+=t1;
                if (n4!=1){
                    i0=i1+n8;
                    i2+=n8;
                    i3+=n8;
                    i4+=n8;
                    t1=(data[i3]+data[i4])/sqrt2;
                    t2=(data[i3]-data[i4])/sqrt2;
                    data[i4]=data[i2]-t1;
                    data[i3]=-data[i2]-t1;
                    data[i2]=data[i0]-t2;
                    data[i0]+=t2;
                }
            }
            id<<=1;
            i1=id-n2;
            id<<=1;
        } while ( i1<n );
        float const e( static_cast<float>( 2 * M_PI / static_cast<int>( n2 ) ) );
        float       a( e );
        for ( unsigned j=2; j<=n8; j++ )
        {
            float cc1, cc3;
            float const ss1( sinecosine<small>( a  , cc1 ) );
            float const ss3( sinecosine<small>( 3*a, cc3 ) );
            a = static_cast<int>( j ) * e;
            unsigned i=0;
            id=n2<<1;
            do{
                for (; i<n; i+=id){
                    i1=i+j-1;
                    i2=i1+n4;
                    i3=i2+n4;
                    i4=i3+n4;
                    i5=i+n4-j+1;
                    i6=i5+n4;
                    i7=i6+n4;
                    i8=i7+n4;
                    t1=data[i3]*cc1+data[i7]*ss1;
                    t2=data[i7]*cc1-data[i3]*ss1;
                    t3=data[i4]*cc3+data[i8]*ss3;
                    t4=data[i8]*cc3-data[i4]*ss3;
                    t5=t1+t3;
                    t6=t2+t4;
                    t3=t1-t3;
                    t4=t2-t4;
                    t2=data[i6]+t6;
                    data[i3]=t6-data[i6];
                    data[i8]=t2;
                    t2=data[i2]-t3;
                    data[i7]=-data[i2]-t3;
                    data[i4]=t2;
                    t1=data[i1]+t5;
                    data[i6]=data[i1]-t5;
                    data[i1]=t1;
                    t1=data[i5]+t4;
                    data[i5]-=t4;
                    data[i2]=t1;
                }
                id<<=1;
                i=id-n2;
                id<<=1;
            } while(i<n);
        }
    }

    //division with array length
    for( unsigned i=0; i<n; i++ ) data[i] /= static_cast<int>( n );
}


void irealfft_split(float *data,long n){

    long i,j,k,i5,i6,i7,i8,i0,id,i1,i2,i3,i4,n2,n4,n8,n1;
    float t1,t2,t3,t4,t5,a,e,sqrt2;

    sqrt2=sqrtf(2.0);

    n1=n-1;
    n2=n<<1;
    for(k=n;k>2;k>>=1){
        id=n2;
        n2>>=1;
        n4=n2>>2;
        n8=n2>>3;
        e = (float)(2*M_PI/(n2));
        i1=0;
        do{
            for (; i1<n; i1+=id){
                i2=i1+n4;
                i3=i2+n4;
                i4=i3+n4;
                t1=data[i1]-data[i3];
                data[i1]+=data[i3];
                data[i2]*=2;
                data[i3]=t1-2*data[i4];
                data[i4]=t1+2*data[i4];
                if (n4!=1){
                    i0=i1+n8;
                    i2+=n8;
                    i3+=n8;
                    i4+=n8;
                    t1=(data[i2]-data[i0])/sqrt2;
                    t2=(data[i4]+data[i3])/sqrt2;
                    data[i0]+=data[i2];
                    data[i2]=data[i4]-data[i3];
                    data[i3]=2*(-t2-t1);
                    data[i4]=2*(-t2+t1);
                }
            }
            id<<=1;
            i1=id-n2;
            id<<=1;
        } while ( i1<n1 );
        a=e;
        for (j=2; j<=n8; j++){
            float cc1, cc3;
            float const ss1( sinecosine<small>( a  , cc1 ) );
            float const ss3( sinecosine<small>( 3*a, cc3 ) );
            a=j*e;
            i=0;
            id=n2<<1;
            do{
                for (; i<n; i+=id){
                    i1=i+j-1;
                    i2=i1+n4;
                    i3=i2+n4;
                    i4=i3+n4;
                    i5=i+n4-j+1;
                    i6=i5+n4;
                    i7=i6+n4;
                    i8=i7+n4;
                    t1=data[i1]-data[i6];
                    data[i1]+=data[i6];
                    t2=data[i5]-data[i2];
                    data[i5]+=data[i2];
                    t3=data[i8]+data[i3];
                    data[i6]=data[i8]-data[i3];
                    t4=data[i4]+data[i7];
                    data[i2]=data[i4]-data[i7];
                    t5=t1-t4;
                    t1+=t4;
                    t4=t2-t3;
                    t2+=t3;
                    data[i3]=t5*cc1+t4*ss1;
                    data[i7]=-t4*cc1+t5*ss1;
                    data[i4]=t1*cc3-t2*ss3;
                    data[i8]=t2*cc3+t1*ss3;
                }
                id<<=1;
                i=id-n2;
                id<<=1;
            } while(i<n1);
        }
    }

    /*----------------------*/
    i0=0;
    id=4;
    do{
        for (; i0<n1; i0+=id){
            i1=i0+1;
            t1=data[i0];
            data[i0]=t1+data[i1];
            data[i1]=t1-data[i1];
        }
        id<<=1;
        i0=id-2;
        id<<=1;
    } while ( i0<n1 );

    /*----------------------*/

    //data shuffling
    for (i=0,j=0,n2=n/2; i<n1 ; i++){
        if (i<j){
            t1=data[j];
            data[j]=data[i];
            data[i]=t1;
        }
        k=n2;
        while (k<=j){
            j-=k;
            k>>=1;
        }
        j+=k;
    }
}
#endif // LE_SORENSEN_PURE_REAL_FFT_TEST

//------------------------------------------------------------------------------
} // namespace nt2
//------------------------------------------------------------------------------
#endif // static_fft_hpp
