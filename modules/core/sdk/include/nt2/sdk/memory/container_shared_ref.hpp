//==============================================================================
//         Copyright 2003 - 2011   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2011   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_SDK_MEMORY_CONTAINER_SHARED_REF_HPP_INCLUDED
#define NT2_SDK_MEMORY_CONTAINER_SHARED_REF_HPP_INCLUDED

#include <nt2/sdk/memory/container.hpp>
#include <nt2/core/container/dsl/forward.hpp>
#include <nt2/sdk/memory/adapted/container_shared_ref.hpp>
#include <boost/shared_ptr.hpp>

namespace nt2 { namespace memory
{
  //============================================================================
  /*!
   * container_shared_ref is like container_ref, but references a shared_ptr
   * to a container to keep it alive.
   *
   * \tparam Type    Value type to store in the table
   * \tparam Setting Options list describing the behavior of the container
   **/
  //============================================================================
  template<class T, class S, bool Own>
  struct container_shared_ref
  {
    typedef typename boost::remove_const<T>::type                value_type;
    typedef std::size_t                                          size_type;

    typedef typename meta::option < S
                                  , tag::semantic_
                                  , tag::table_
                                  >::type                            semantic_t;
    typedef typename meta::normalize<semantic_t,value_type,S>::type  settings_type;

    typedef typename meta::option<settings_type, tag::of_size_>::type        extent_type;
    typedef typename meta::option<settings_type, tag::storage_order_>::type  order_type;

    typedef typename specific_data< typename boost::dispatch::
                                             default_site<T>::type
                                  , value_type
                                  >::type                        specific_data_type;

    typedef typename boost::mpl::
            if_< boost::is_const<T>
               , container<value_type, S> const
               , container<value_type, S>
               >::type base_t;

    typedef typename boost::mpl::
            if_< boost::is_const<T>
               , container_base<value_type> const
               , container_base<value_type>
               >::type cbase_t;

    typedef typename boost::mpl::
            if_< boost::is_const<T>
               , typename base_t::const_reference
               , typename base_t::reference
               >::type                                           reference;
    typedef typename boost::mpl::
            if_< boost::is_const<T>
               , typename base_t::const_pointer
               , typename base_t::pointer
               >::type                                           pointer;
    typedef typename boost::mpl::
            if_< boost::is_const<T>
               , typename base_t::const_iterator
               , typename base_t::iterator
               >::type                                           iterator;
    typedef typename base_t::const_reference                     const_reference;
    typedef typename base_t::const_pointer                       const_pointer;
    typedef typename base_t::const_iterator                      const_iterator;

    container_shared_ref() : ptr()
    {
    }

    template<class U, class S2, bool Own2>
    container_shared_ref(container_shared_ref<U, S2, Own2> const& other) : ptr(other.raw()), sz(other.extent()), base_(other.base_)
    {
    }

    template<class U, class S2, bool Own2>
    container_shared_ref(container_shared_ref<U, S2, Own2> const& other, pointer p, extent_type const& sz_) : ptr(p), sz(sz_), base_(other.base_)
    {
    }

    //==========================================================================
    /**!
     * Swap the contents of two container of same type and settings
     * \param x First \c container to swap
     * \param y Second \c container to swap
     **/
    //==========================================================================
    BOOST_FORCEINLINE void swap(container_shared_ref& y)
    {
      boost::swap(*this, y);
    }

    //==========================================================================
    /*!
     * @brief Resize a container using new dimensions set
     */
    //==========================================================================
    template<class Size>
    void resize( Size const& sz_ ) const
    {
      // do nothing
    }

    //==========================================================================
    /*!
     * @brief Add element at end of container, reshape to 1D
     */
    //==========================================================================
    void push_back( value_type const& v )
    {
      // do nothing
    }

    //==========================================================================
    /*!
     * @brief Return the container dimensions set
     * @return A reference to a constant Fusion RandomAccessSequence containing
     * the size of the container over each of its dimensions.
     **/
    //==========================================================================
    BOOST_FORCEINLINE extent_type const& extent() const
    {
      return sz;
    }

    //==========================================================================
    /*!
     * @brief Return the container number of element
     * @return The number of logical element stored in the buffer.
     **/
    //==========================================================================
    BOOST_FORCEINLINE size_type size() const
    {
      return numel(extent());
    }

    //==========================================================================
    /*!
     * @brief Return the container number of element along the main dimension
     *
     * leading_size retrieves the number of element of the leading dimension in
     * a settings independant way.
     *
     * @return The number of elements stored on the main dimension
     */
    //==========================================================================
    BOOST_FORCEINLINE size_type leading_size() const
    {
      typedef typename boost::mpl
                            ::apply < order_type
                                    , boost::mpl::size_t<extent_type::static_size>
                                    , boost::mpl::size_t<0U>
                                    >::type                     dim_t;
      return extent()[dim_t::value];
    }

    //==========================================================================
    /*!
     * @brief Check for container emptyness
     * @return A boolean that evaluates to \c true if the container stores no
     * value, \c false otherwise.
     */
    //==========================================================================
    BOOST_FORCEINLINE bool empty() const { return size() == 0u; }

    //==========================================================================
    /*!
     * Return the begin of the raw memory
     */
    //==========================================================================
    BOOST_FORCEINLINE pointer        raw() const { return ptr; }

    //==========================================================================
    /*!
     * Return the begin of the data
     */
    //==========================================================================
    BOOST_FORCEINLINE iterator       begin() const { return iterator(ptr); }

    //==========================================================================
    /*!
     * Return the end of the data
     */
    //==========================================================================
    BOOST_FORCEINLINE iterator       end() const   { return begin() + size(); }

    //==========================================================================
    // Linear Random Access
    //==========================================================================
    BOOST_FORCEINLINE reference operator[](size_type i) const
    {
      return begin()[i];
    }

    //==========================================================================
    /*!
     * @brief Access to the architecture specific container data
     * As the inner structure of a container may change with the hardware
     * configuration, a specific data segment is provided to gather informations
     * that may be required for proper operation on said configuration.
     *
     * @return A reference to the specific data of the container.
     **/
    //==========================================================================
    specific_data_type&  specifics() const { BOOST_ASSERT_MSG(0, "unimplemented"); }

    //==========================================================================
    // Check if a position is safely R/W in the current container
    //==========================================================================
    BOOST_FORCEINLINE bool is_safe(size_type p) const { return p == 0u || p < size(); }

    boost::shared_ptr<cbase_t> base() const { return base_; }

  private:
    template<class U, class S2, bool Own2>
    friend struct container_shared_ref;

    pointer                                  ptr;
    extent_type                              sz;
    boost::shared_ptr<cbase_t>               base_;
  };

  // this is really just a container adaptor with shallow copying
  template<class T, class S>
  struct container_shared_ref<T, S, true>
  {
    typedef T                                                    value_type;
    typedef std::size_t                                          size_type;

    typedef typename meta::option < S
                                  , tag::semantic_
                                  , tag::table_
                                  >::type                            semantic_t;
    typedef typename meta::normalize<semantic_t,value_type,S>::type  settings_type;

    typedef typename meta::option<settings_type, tag::of_size_>::type        extent_type;
    typedef typename meta::option<settings_type, tag::storage_order_>::type  order_type;

    typedef typename specific_data< typename boost::dispatch::
                                             default_site<T>::type
                                  , value_type
                                  >::type                        specific_data_type;

    typedef container<value_type, S>                             base_t;

    typedef typename base_t::reference                           reference;
    typedef typename base_t::const_reference                     const_reference;
    typedef typename base_t::pointer                             pointer;
    typedef typename base_t::const_pointer                       const_pointer;
    typedef typename base_t::iterator                            iterator;
    typedef typename base_t::const_iterator                      const_iterator;

    container_shared_ref() : base_(boost::make_shared<base_t>())
    {
    }

    //==========================================================================
    /**!
     * Swap the contents of two container of same type and settings
     * \param x First \c container to swap
     * \param y Second \c container to swap
     **/
    //==========================================================================
    BOOST_FORCEINLINE void swap(container_shared_ref& y)
    {
      boost::swap(*this, y);
    }

    template<class S2>
    BOOST_FORCEINLINE void swap(container<T, S2>& x) const
    {
      base_->swap(x);
    }

    //==========================================================================
    /*!
     * @brief Resize a container using new dimensions set
     */
    //==========================================================================
    template<class Size>
    void resize( Size const& sz_ ) const
    {
      base_->resize(sz_);
    }

    //==========================================================================
    /*!
     * @brief Add element at end of container, reshape to 1D
     */
    //==========================================================================
    void push_back( value_type const& v )
    {
      base_->push_back(v);
    }

    //==========================================================================
    /*!
     * @brief Return the container dimensions set
     * @return A reference to a constant Fusion RandomAccessSequence containing
     * the size of the container over each of its dimensions.
     **/
    //==========================================================================
    BOOST_FORCEINLINE extent_type const& extent() const
    {
      return base_->extent();
    }

    //==========================================================================
    /*!
     * @brief Return the container number of element
     * @return The number of logical element stored in the buffer.
     **/
    //==========================================================================
    BOOST_FORCEINLINE size_type size() const
    {
      return numel(extent());
    }

    //==========================================================================
    /*!
     * @brief Return the container number of element along the main dimension
     *
     * leading_size retrieves the number of element of the leading dimension in
     * a settings independant way.
     *
     * @return The number of elements stored on the main dimension
     */
    //==========================================================================
    BOOST_FORCEINLINE size_type leading_size() const
    {
      typedef typename boost::mpl
                            ::apply < order_type
                                    , boost::mpl::size_t<extent_type::static_size>
                                    , boost::mpl::size_t<0U>
                                    >::type                     dim_t;
      return extent()[dim_t::value];
    }

    //==========================================================================
    /*!
     * @brief Check for container emptyness
     * @return A boolean that evaluates to \c true if the container stores no
     * value, \c false otherwise.
     */
    //==========================================================================
    BOOST_FORCEINLINE bool empty() const { return size() == 0u; }

    //==========================================================================
    /*!
     * Return the begin of the raw memory
     */
    //==========================================================================
    BOOST_FORCEINLINE pointer        raw() const { return base_->raw(); }

    //==========================================================================
    /*!
     * Return the begin of the data
     */
    //==========================================================================
    BOOST_FORCEINLINE iterator       begin() const { return base_->begin(); }

    //==========================================================================
    /*!
     * Return the end of the data
     */
    //==========================================================================
    BOOST_FORCEINLINE iterator       end() const   { return begin() + size(); }

    //==========================================================================
    // Linear Random Access
    //==========================================================================
    BOOST_FORCEINLINE reference operator[](size_type i) const
    {
      return begin()[i];
    }

    //==========================================================================
    /*!
     * @brief Access to the architecture specific container data
     * As the inner structure of a container may change with the hardware
     * configuration, a specific data segment is provided to gather informations
     * that may be required for proper operation on said configuration.
     *
     * @return A reference to the specific data of the container.
     **/
    //==========================================================================
    specific_data_type&  specifics() const { base_->specifics(); }

    //==========================================================================
    // Check if a position is safely R/W in the current container
    //==========================================================================
    BOOST_FORCEINLINE bool is_safe(size_type p) const { return p == 0u || p < size(); }

  private:
    template<class U, class S2, bool Own2>
    friend struct container_shared_ref;

    boost::shared_ptr<base_t>                base_;
  };

  //============================================================================
  /**!
   * Swap the contents of two container of same type and settings
   * \param x First \c container to swap
   * \param y Second \c container to swap
   **/
  //============================================================================
  template<class T, class S, bool Own> inline
  void swap(container_shared_ref<T, S, Own>& x, container_shared_ref<T, S, Own>& y)  { x.swap(y); }

  template<class T, class S1, class S2> inline
  void swap(container_shared_ref<T, S1, true> const& x, container<T, S2>& y)  { x.swap(y); }

  template<class T, class S1, class S2> inline
  void swap(container<T, S1>& x, container_shared_ref<T, S2, true> const& y)  { y.swap(x); }
} }

#endif
