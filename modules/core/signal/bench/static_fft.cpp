//==============================================================================
//         Copyright 2003 - 2011   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2011   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//         Copyright 2012          Domagoj Saric, Little Endian Ltd.
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#define NT2_BENCH_MODULE "nt2 signal processing with Boost.SIMD - test"

#include <nt2/signal/static_fft.hpp>

#include <nt2/sdk/unit/tests.hpp>
#include <nt2/sdk/unit/module.hpp>
#include <nt2/sdk/unit/details/helpers.hpp>
#include <nt2/sdk/unit/perform_benchmark.hpp>

#include <boost/simd/sdk/memory/allocator.hpp>
#include <boost/simd/include/functions/scalar/ilog2.hpp>

#include <boost/array.hpp>
#include <boost/assert.hpp>
#include <boost/foreach.hpp>
#include <boost/noncopyable.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>

#ifdef __APPLE__
    // FIXME: this requires "-framework Accelerate" to be added to linker flags
    #include "Accelerate/Accelerate.h" //vDSP.h
#endif // __APPLE__

#include <vector>

namespace bench
{
    namespace constants
    {
        static std::size_t const minimum_dft_size = 128 ;
        static std::size_t const maximum_dft_size = 8192;
        static std::size_t const test_dft_size    = 4096;

        static int const test_data_range_minimum = -1;
        static int const test_data_range_maximum = +1;
    }

    static std::size_t const N = constants::test_dft_size;
    #if defined( BOOST_SIMD_HAS_LRB_SUPPORT ) || defined( BOOST_SIMD_HAS_AVX_SUPPORT )
        typedef double T;
    #else //...zzz...cardinal-must-be-4 limitation...
        typedef float T;
    #endif // BOOST_SIMD_HAS_LRB_SUPPORT || BOOST_SIMD_HAS_AVX_SUPPORT

    typedef BOOST_SIMD_ALIGN_ON( BOOST_SIMD_ARCH_ALIGNMENT ) boost::array<T, N      > aligned_array;
    typedef BOOST_SIMD_ALIGN_ON( BOOST_SIMD_ARCH_ALIGNMENT ) boost::array<T, N/2 + 1> aligned_half_complex_array;

    typedef std::vector<T, boost::simd::memory::allocator<T> > dynamic_aligned_array;

    typedef nt2::static_fft<constants::minimum_dft_size, constants::maximum_dft_size, T> FFT;

    template <class Range>
    void randomize( Range & data )
    {
        /// \note Portably reproducible pseudo "random" values.
        ///                                   (24.07.2012.) (Domagoj Saric)
        boost::random::mt19937                            prng        ( 42                                                                     );
        boost::random::uniform_real_distribution<T> const distribution( constants::test_data_range_minimum, constants::test_data_range_maximum );
        BOOST_FOREACH( T & scalar, data )
            scalar = distribution( prng );
    }


    class complex_fft_test : boost::noncopyable
    {
    public:
        void reset()
        {
            randomize( real_data_ );
            randomize( imag_data_ );
        }

        std::size_t size() const
        {
            BOOST_ASSERT( real_data_.size() == imag_data_.size() );
            return real_data_.size();
        }

    protected:
        complex_fft_test( std::size_t const length )
            :
            real_data_( length ),
            imag_data_( length )
        {
            reset();
        }

        ~complex_fft_test() {}

    protected:
        mutable dynamic_aligned_array real_data_;
        mutable dynamic_aligned_array imag_data_;
    }; // class complex_fft_test


    struct test_fft_forward : complex_fft_test
    {
        test_fft_forward( std::size_t const length ) : complex_fft_test( length ) {}

        void operator()() const { FFT::forward_transform( &real_data_[ 0 ], &imag_data_[ 0 ], size() ); }
    };

    struct test_fft_inverse : complex_fft_test
    {
        test_fft_inverse( std::size_t const length ) : complex_fft_test( length ) {}

        void operator()() const { FFT::inverse_transform( &real_data_[ 0 ], &imag_data_[ 0 ], size() ); }
    };


    class real_fft_test : boost::noncopyable
    {
    public:
        std::size_t size() const { return real_time_data_.size(); }

    protected:
        real_fft_test( std::size_t const length )
            :
            real_time_data_     ( length         ),
            real_frequency_data_( length / 2 + 1 ),
            imag_frequency_data_( length / 2 + 1 )
        {}

        ~real_fft_test() {}

    protected:
        /// \note The forward transform uses the frequency data as
        /// output/scratch buffers and the inverse transform uses the
        /// real_time_data as output/scratch buffer, in addition
        /// FFT::real_inverse_transform destroys input data (so it has
        /// to be regenerated). For the said reasons all of the arrays have to
        /// declared mutable
        ///                                   (10.10.2012.) (Domagoj Saric)
        mutable dynamic_aligned_array real_time_data_     ;
        mutable dynamic_aligned_array real_frequency_data_;
        mutable dynamic_aligned_array imag_frequency_data_;
    }; // class real_fft_test


    struct test_fft_real_forward : real_fft_test
    {
        test_fft_real_forward( std::size_t const length ) : real_fft_test( length )
        {
            randomize( real_time_data_ );
        }

        void operator()() const
        { 
            FFT::real_forward_transform( &real_time_data_[ 0 ], &real_frequency_data_[ 0 ], &imag_frequency_data_[ 0 ], size() );
        }

        static void reset() {}
    }; // class test_fft_real_forward


    struct test_fft_real_inverse : real_fft_test
    {
        test_fft_real_inverse( std::size_t const length ) : real_fft_test( length ) { reset(); }

        void operator()() const
        {
            FFT::real_inverse_transform( &real_frequency_data_[ 0 ], &imag_frequency_data_[ 0 ], &real_time_data_[ 0 ], size() );
        }

        void reset()
        {
            /// \note FFT::real_inverse_transform destroys input data so it has
            /// to be regenerated.
            ///                               (10.10.2012.) (Domagoj Saric)
            randomize( real_frequency_data_ );
            randomize( imag_frequency_data_ );
        }
    }; // test_fft_real_inverse


#ifdef __APPLE__
    class apple_fft_test : boost::noncopyable
    {
    protected:
        apple_fft_test( std::size_t const length )
            :
            log2length_  ( boost::simd::ilog2( length )                      ),
            fft_instance_( ::vDSP_create_fftsetup( log2length_, kFFTRadix2 ) )
        {
            if ( !fft_instance_ )
                BOOST_THROW_EXCEPTION( std::bad_alloc() );
        }

        ~apple_fft_test() { ::vDSP_destroy_fftsetup( fft_instance_ ); }

        FFTSetup     instance  () const { return fft_instance_; }
        unsigned int log2length() const { return log2length_  ; }

    private:
        unsigned int const log2length_  ;
        FFTSetup     const fft_instance_;
    }; // class apple_fft_test


    template <FFTDirection direction>
    class apple_complex_fft_test
        :
        public apple_fft_test,
        public complex_fft_test
    {
    public:
        apple_complex_fft_test( std::size_t const length ) : apple_fft_test( length ), complex_fft_test( length ) {}

        void operator()() const
        {
            DSPSplitComplex complex_data( split_data() );
            ::vDSP_fft_zip( instance(), &complex_data, 1, log2length(), direction );
        }
        
        DSPSplitComplex split_data() const
        {
            DSPSplitComplex complex_data = { &real_data_[ 0 ], &imag_data_[ 0 ] };
            return complex_data;
        }
    }; // class apple_complex_fft_test


    class apple_real_fft_test
        :
        public apple_fft_test,
        public real_fft_test
    {
    protected:
        apple_real_fft_test( std::size_t const length ) : apple_fft_test( length ), real_fft_test( length ) {}

        DSPSplitComplex split_data() const
        {
            DSPSplitComplex complex_data = { &real_frequency_data_[ 0 ], &imag_frequency_data_[ 0 ] };
            return complex_data;
        }
    }; // class apple_real_fft_test

    class apple_real_forward_fft_test : public apple_real_fft_test
    {
    public:
        apple_real_forward_fft_test( std::size_t const length ) : apple_real_fft_test( length ) { randomize( real_time_data_ ); }

        void operator()() const
        {
            DSPSplitComplex split_real_data( split_data() );
            vDSP_ctoz    ( reinterpret_cast<DSPComplex const *>( &real_time_data_[ 0 ] ), 2, &split_real_data, 1, size() / 2 );
            vDSP_fft_zrip( instance(), &split_real_data, 1, log2length(), FFT_FORWARD );
        }

    public:
        static void reset() {}
    }; // class apple_real_forward_fft_test


    class apple_real_inverse_fft_test : public apple_real_fft_test
    {
    public:
        apple_real_inverse_fft_test( std::size_t const length ) : apple_real_fft_test( length ) { reset(); }

        void operator()() const
        {
            DSPSplitComplex split_real_data( split_data() );
            vDSP_fft_zrip( instance(), &split_real_data, 1, log2length(), FFT_INVERSE );
            vDSP_ztoc    ( &split_real_data, 1, reinterpret_cast<DSPComplex *>( &real_time_data_[ 0 ] ), 2, size() / 2 );
        }

    public:
        void reset()
        {
            randomize( real_frequency_data_ );
            randomize( imag_frequency_data_ );
        }
    }; // class apple_real_fft_test
#endif // __APPLE__

    template <class Benchmark>
    void do_perform_benchmark( char const * const benchmark_name )
    {
        std::cout << benchmark_name <<"\n";

        nt2::unit::benchmark_result<nt2::details::cycles_t> dv;
        nt2::unit::benchmark_result<double                > tv;
        Benchmark benchmark( N );
        nt2::unit::perform_benchmark( benchmark, 1.0, dv );
        nt2::unit::perform_benchmark( benchmark, 1.0, tv );

        std::cout << std::scientific << dv.median / benchmark.size() << "\t";
        std::cout << std::scientific << tv.median << "\n";
    }

    void do_test()
    {
        std::cout.precision( 3 );
        std::cout << "test_dft_size : " << N <<"\n";

        do_perform_benchmark<test_fft_forward     >( "complex forward transform" );
        do_perform_benchmark<test_fft_inverse     >( "complex inverse transform" );
        do_perform_benchmark<test_fft_real_forward>( "real forward transform"    );
        do_perform_benchmark<test_fft_real_inverse>( "real inverse transform"    );

    #ifdef __APPLE__
        do_perform_benchmark<apple_complex_fft_test<FFT_FORWARD> >( "(apple) complex forward transform" );
        do_perform_benchmark<apple_complex_fft_test<FFT_INVERSE> >( "(apple) complex inverse transform" );
        do_perform_benchmark<apple_real_forward_fft_test         >( "(apple) real forward transform"    );
        do_perform_benchmark<apple_real_inverse_fft_test         >( "(apple) real inverse transform"    );
    #endif // __APPLE__
    }
}

NT2_TEST_CASE( test_fft )
{
    bench::do_test();
}
