//==============================================================================
//         Copyright 2003 - 2011   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2011   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_CORE_CONTAINER_MEMORY_ILIFFE_BUFFER_HPP_INCLUDED
#define NT2_CORE_CONTAINER_MEMORY_ILIFFE_BUFFER_HPP_INCLUDED

//==============================================================================
/**
  * \file
  * \brief Defines and implements the \c nt2::memory::iliffe_buffer class
  **/
//==============================================================================
#include <boost/fusion/include/at.hpp>
#include <boost/fusion/include/fold.hpp>
#include <boost/fusion/include/copy.hpp>
#include <nt2/include/functions/multiplies.hpp>
#include <nt2/sdk/memory/adapted/iliffe_buffer.hpp>

namespace nt2 { namespace memory
{
  //============================================================================
  /**
    * \brief Multi-dimensionnal memory buffer adaptor.
    *
    * iliffe_buffer is a Buffer adaptor that turns a 1D Buffer into a nD Buffer
    * by using the Iliffe vector allocation scheme to allow multi-dimensionnal
    * access by precomputing index tables of nD pointers.
    * 
    * \tparam Dimensions    Number of dimensions stored in this Buffer
    * \tparam Data          1D Buffer to remodel as data storage
    * \tparam Index         1D Buffer to remodel as indices storage
   **/
  //============================================================================
  template<typename Dimensions, typename Data, typename Index>
  struct iliffe_buffer
  {
    //==========================================================================
    /** Type of the allocator used in current buffer                          */
    //==========================================================================
    typedef typename Data::allocator_type               allocator_type;
    typedef typename Index::allocator_type              index_allocator_type;

    //==========================================================================
    /** Type of the value stored in current buffer                            */
    //==========================================================================
    typedef typename Data::value_type                   value_type;

    //==========================================================================
    /** Type of the iterator to the iliffe_buffer values                      */
    //==========================================================================
    typedef typename Data::iterator                     iterator;

    //==========================================================================
    /** Type of the const_iterator to the iliffe_buffer values                */
    //==========================================================================
    typedef typename Data::const_iterator               const_iterator;

    //==========================================================================
    /** Type of reference to a value                                          */
    //==========================================================================
    typedef typename Data::reference                    reference;

    //==========================================================================
    /** Type of reference to a constant value                                 */
    //==========================================================================
    typedef typename Data::const_reference              const_reference;

    //==========================================================================
    /** Type representing an amount of values                                 */
    //==========================================================================
    typedef typename Data::size_type                    size_type;

    //==========================================================================
    /** Type representing an offset between values                            */
    //==========================================================================
    typedef typename Data::difference_type              difference_type;
    
    //==========================================================================
    /**
     *  Default constructor builds an empty iliffe buffer
     *
     * /param alloc Optional allocator object to pass to the underlying
     *              buffers
     **/
    //==========================================================================
    iliffe_buffer ( allocator_type const& a = allocator_type() ) : data_(a) {}

    //==========================================================================
    /**
      * Initializes a Iliffe buffer from a dimensions and bases set.
      * 
      * \param sz FusionRandomAccessSequence containing the dimensions set 
      *           of the buffer.
      * \param bs FusionRandomAccessSequence containing the base indices set
      *           of the buffer.
     **/
    //==========================================================================
    template<typename Sizes, typename Bases>
    iliffe_buffer( Sizes const& sz, Bases const& bs )
      : data_ ( data_size(sz), boost::fusion::at_c<0>(bs)  )
      , index_( index_size(sz), boost::fusion::at_c<1>(bs) )
    {
      boost::fusion::copy(sz,sizes_);
      boost::fusion::copy(bs,bases_);
      make_links();
    }

    //==========================================================================
    /**
      * Initializes a Iliffe buffer from a dimensions and bases set.
      * 
      * \param sz FusionRandomAccessSequence containing the dimensions set 
      *           of the buffer.
      * \param bs FusionRandomAccessSequence containing the base indices set
      *           of the buffer.
      * \param a  Allocator to pass to Data buffer.
      * 
     **/
    //==========================================================================
    template<typename Sizes, typename Bases>
    iliffe_buffer ( Sizes const& sz
                  , Bases const& bs
                  , allocator_type const& a
                  )
      : data_ ( data_size(sz) , boost::fusion::at_c<0>(bs), a )
      , index_( index_size(sz), boost::fusion::at_c<1>(bs) )
    {
      boost::fusion::copy(sz,sizes_);
      boost::fusion::copy(bs,bases_);
      make_links();
    }

    //==========================================================================
    /**
      * Give access to the beginning of the values stored in the buffer.
      * begin() and end() are both returning un-biased iterator, making them
      * suitable arguments for standard algorithms. Note that such iterators
      * dont walk over padding.
      *
      * \return A pointer pointing to the beginning of the buffer segment
      * holding values.
     **/
    //==========================================================================
    iterator        begin()       { return data_.begin(); }
    const_iterator  begin() const { return data_.begin(); }

    //==========================================================================
    /**
      * Give access to the end of the values stored in the buffer.
      * begin() and end() are both returning un-biased iterator, making them
      * suitable arguments for standard algorithms. Note that such iterators
      * dont walk over padding.
      * 
      * \return A pointer pointing past the last element of the buffer segment
      * holding values.
     **/
    //==========================================================================
    iterator        end()         { return data_.end(); }
    const_iterator  end() const   { return data_.end(); }

    //==========================================================================
    /**
      * Reallocate a iliffe_bufer to a new size.
     **/
    //==========================================================================
    template<typename Sizes> inline void resize( Sizes const& szs )
    {
      data_.resize(data_size(szs));
      index_.resize(index_size(szs));
      boost::fusion::copy(szs,sizes_);
      
      make_links();
    }

    //==========================================================================
    /**
      * Rebase an iliffe_bufer to a new base set.
     **/
    //==========================================================================
    template<typename Bases> inline void rebase( Bases const& bss )
    {
      data_.rebase(boost::fusion::at_c<0>(bss));
      index_.rebase(boost::fusion::at_c<1>(bss));
      boost::fusion::copy(bss,bases_);
      make_links();
    }

    //==========================================================================
    /**
      * Resize and rebase a iliffe_bufer to a new size/base set.
     **/
    //==========================================================================
    template<typename Sizes, typename Bases> inline void
    restructure( Sizes const& szs, Bases const& bss )
    {
      data_.resize(data_size(szs));
      data_.rebase(boost::fusion::at_c<0>(bss));
      
      index_.resize(index_size(szs));
      index_.rebase(boost::fusion::at_c<1>(bss));

      boost::fusion::copy(szs,sizes_);
      boost::fusion::copy(bss,bases_);
      
      make_links();
    }

    //==========================================================================
    /**
      * Access to a given position through the iliffe_buffer
     **/
    //==========================================================================
    template<class Position> BOOST_FORCEINLINE
    reference operator[]( Position const& pos )
    {
      return access ( meta::as_sequence(pos)
                    , boost::fusion::size(meta::as_sequence(pos))
                    );
    }
    
    template<class Position> BOOST_FORCEINLINE
    const_reference operator[]( Position const& pos ) const
    {
      return access ( meta::as_sequence(pos)
                    , boost::fusion::size(meta::as_sequence(pos))
                    );
    }
    
    protected:
    //==========================================================================
    /*
     * data_size computes the number of elements required to store the data 
     * table of the Iliffe vector
     */
    //==========================================================================
    template<class Seq> inline std::size_t
    data_size(Seq const& s) const
    {
      return boost::fusion::fold( s
                                , boost::mpl::size_t<1>()
                                , boost::dispatch::functor<tag::multiplies_>() 
                                );
    }

    //==========================================================================
    /*
     * index_size computes the number of elements required to store the index 
     * table of the Iliffe vector
     */
    //==========================================================================
    template<class Seq> inline std::size_t
    index_size(Seq const& s) const
    {
      return boost::fusion::fold( boost::fusion::pop_front(s)
                                , boost::mpl::size_t<1>()
                                , boost::dispatch::functor<tag::multiplies_>() 
                                );
    }

    //==========================================================================
    // Access for 1D Position
    //==========================================================================
    template<class Position> BOOST_FORCEINLINE
    reference access(Position const& p, boost::mpl::int_<1> const& )
    {
      return data_[boost::fusion::at_c<0>(p)];      
    }

    template<class Position> BOOST_FORCEINLINE
    const_reference access(Position const& p, boost::mpl::int_<1> const& ) const
    {
      return data_[boost::fusion::at_c<0>(p)];
    }

    //==========================================================================
    // Access for 2D Position
    //==========================================================================
    template<class Position> BOOST_FORCEINLINE
    reference access(Position const& p, boost::mpl::int_<2> const& )
    {
      return index_[boost::fusion::at_c<1>(p)][boost::fusion::at_c<0>(p)];
    }

    template<class Position> BOOST_FORCEINLINE
    const_reference access(Position const& p, boost::mpl::int_<2> const& ) const
    {
      return index_[boost::fusion::at_c<1>(p)][boost::fusion::at_c<0>(p)];
    }
    
    //==========================================================================
    /*
     * link performs the indexing of the Iliffe vector
     */
    //==========================================================================
    void make_links()
    {
      typename Index::difference_type i = index_.lower();
      typename Index::difference_type u = index_.upper();

      // Points to the beginning of the data block
      index_[i++] = data_.origin();

      // Fill out the remaining indices
      std::size_t local_size = boost::fusion::at_c<0>(sizes_);
      for(i; i <= u; ++i) index_[i] = index_[i-1] + local_size;        
    }

    private:
    Data    data_;
    Index   index_;
    boost::array<std::size_t,Dimensions::value>     sizes_;
    boost::array<std::ptrdiff_t,Dimensions::value>  bases_;
  };
} }

#include <nt2/sdk/memory/details/iliffe_buffer_1d.hpp>
#include <nt2/sdk/memory/details/iliffe_buffer_2d.hpp>

#endif
