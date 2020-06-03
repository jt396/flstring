/*
===============================================================================

    flstring
    ===
    File    :   flstring.hpp
    Author  :   Jamie Taylor
    Desc    :   A basic fixed-length string implementation.
                Created for C++ London presentation:
                "Beyond SSO: The Merits of Fixed-Length Strings".

                TODO:
                ----
                - constexpr?
                - static_assert (use array in set_data() and have the array size be a template param?)
                - error-handling?
                - clear() should probably memset() to '\0'?
                - LOOP_UNROLLING? Go in increments of 2? Enforce string size as a multiple of two?

===============================================================================
*/
#ifndef FLSTRING_HPP
#define FLSTRING_HPP


#include <array>
#include <cassert>
#include <experimental/string_view>
#include <iostream>


namespace fl {

template<size_t string_size>
class string {
public:

                                // member types
    using value_type            = char;
    using value_traits          = std::char_traits<value_type>;
    using reference             = value_type&;
    using const_reference       = const value_type&;
    using pointer               = value_type*;
    using const_pointer         = const value_type*;
    using iterator              = pointer;
    using const_iterator        = const_pointer;
    using reverse_iterator      = std::reverse_iterator<iterator>;
    using const_reverse_iterator= std::reverse_iterator<const_iterator>;
    using size_type             = std::size_t;
    using string_view           = std::experimental::basic_string_view<value_type, value_traits>;

    static const std::size_t    npos = -1;

                                // construction and assignment
                                string();
                                string( const string& str ); // TODO: Check size (template<N>)
                                string( const_pointer s );
                                // TODO: initializer_list<> & rvalue etc.
                                ~string();

       string&                  operator=( const string& str );
       string&                  operator=( const_pointer s );
                                // TODO: initializer_list<> & rvalue etc.

                                // element access
    inline reference            operator[]( size_type pos );
    inline const_reference      operator[]( size_type pos ) const;
    inline reference            at( size_type pos );
    inline const_reference      at( size_type pos ) const;
    inline reference            back();
    inline const_reference      back() const;
    inline reference            front();
    inline const_reference      front() const;
    inline const_pointer        c_str() const noexcept;
    inline const_pointer        data() const noexcept { return &m_data[0]; }//{ return m_data.data(); }
    inline                      operator string_view() const noexcept;

                                // iterators
    inline iterator             begin();
    inline const_iterator       cbegin() const;
    inline iterator             end();
    inline const_iterator       cend() const;
    inline iterator             rbegin();
    inline const_iterator       crbegin() const;
    inline iterator             rend();
    inline const_iterator       crend() const;

                                // capacity
    inline size_type            size() const noexcept;
    inline size_type            length() const noexcept;
    inline size_type            max_size() const noexcept;
    inline size_type            available() const noexcept;
    inline bool                 empty() const noexcept;

                                // operations
    inline void                 clear() noexcept;
    inline void                 shallow_clear() noexcept;
                                template<std::size_t N>
    inline string&              operator+=( const string<N>& str );
    inline string&              operator+=( const_pointer s );
                                template<std::size_t N>
    inline int                  compare( const string<N>& str ) const;
    size_type                   copy( pointer s, size_type len, size_type pos = 0 ) const;

                                // search
    size_type                   find( const string& str, size_type pos = 0 ) const;
    size_type                   find( const_pointer s, size_type pos = 0 ) const;
    size_type                   find( const_pointer s, size_type pos, size_type n ) const;
    size_type                   find( value_type c, size_type pos = 0 ) const;
    size_type                   find( string_view sv, size_type pos = 0 ) const;
                                // ...

private:
    //std::array<value_type,
    //           string_size>      m_data;
    value_type                  m_data[string_size];

    void                        set_data( string_view sv );
    size_type                   do_find( string_view sv, size_type pos, size_type n ) const;
    string&                     do_concat( string_view sv );
};

// construction and assignment
template<std::size_t string_size>
string<string_size>::string() {
    m_data[string_size-1] = static_cast<value_type>( string_size-1 );
}
template<std::size_t string_size>
string<string_size>::string( const string& str ) {
    set_data( str.c_str() );
}
template<std::size_t string_size>
string<string_size>::string( const_pointer s ) {
    set_data( s );
}
template<std::size_t string_size>
string<string_size>::~string() {
    // do nothing
}
template<std::size_t string_size>
string<string_size>& string<string_size>::operator=( const string& str ) {
    set_data( str );
    return *this;
}
template<std::size_t string_size>
string<string_size>& string<string_size>::operator=( const_pointer s ) {
    set_data( string_view( s, value_traits::length( s ) ) );
    return *this;
}

// element access
template<std::size_t string_size>
inline typename string<string_size>::reference string<string_size>::operator[]( size_type pos ) { 
    return m_data[pos];
}
template<std::size_t string_size>
inline typename string<string_size>::const_reference string<string_size>::operator[]( size_type pos ) const { 
    return m_data[pos];
}
template<std::size_t string_size>
inline typename string<string_size>::reference string<string_size>::at( size_type pos ) {
    return m_data[pos];
}
template<std::size_t string_size>
inline typename string<string_size>::const_reference string<string_size>::at( size_type pos ) const { 
    return m_data[pos];
}
template<std::size_t string_size>
inline typename string<string_size>::reference string<string_size>::back() { 
    // subtract the remaining capacity in the string' last byte from the string length
    // to get the 'back'/last character (+1 to handle the null-terminator)
    return m_data[string_size-1 - static_cast<size_type>( m_data[string_size-1] + 1 )];
}
template<std::size_t string_size>
inline typename string<string_size>::const_reference string<string_size>::back() const {
    // subtract the remaining capacity in the string' last byte from the string length
    // to get the 'back'/last character (+1 to handle the null-terminator)
    return m_data[string_size-1 - static_cast<size_type>( m_data[string_size-1] + 1 )];
}
template<std::size_t string_size>
inline typename string<string_size>::reference string<string_size>::front() { 
    return m_data[0];
}
template<std::size_t string_size>
inline typename string<string_size>::const_reference string<string_size>::front() const { 
    return m_data[0];
}
template<std::size_t string_size>
inline typename string<string_size>::const_pointer string<string_size>::c_str() const noexcept {
    return &m_data[0];//m_data.data();
}
#if 0
template<std::size_t string_size>
inline typename string<string_size>::const_pointer string<string_size>::data() const noexcept {
    return m_data.data();
}
#endif
template<std::size_t string_size>
inline string<string_size>::operator string_view() const noexcept {
    return string_view( &m_data[0]/*m_data.data()*/, string_size-1 - available() ); 
}

// iterators
template<std::size_t string_size>
inline typename string<string_size>::iterator string<string_size>::begin() {
    return &m_data[0];
}
template<std::size_t string_size>
inline typename string<string_size>::const_iterator string<string_size>::cbegin() const {
    return const_cast<const_iterator>( &m_data[0] );
}
template<std::size_t string_size>
inline typename string<string_size>::iterator string<string_size>::end() {
    return &m_data[string_size];
}
template<std::size_t string_size>
inline typename string<string_size>::const_iterator string<string_size>::cend() const {
    return const_cast<const_iterator>( &m_data[string_size] );
}
template<std::size_t string_size>
inline typename string<string_size>::iterator string<string_size>::rbegin() {
    return &m_data[length()-1];
}
template<std::size_t string_size>
inline typename string<string_size>::const_iterator string<string_size>::crbegin() const {
    return const_cast<const_iterator>( &m_data[length()-1] );
}
template<std::size_t string_size>
inline typename string<string_size>::iterator string<string_size>::rend() {
    return &m_data[0] - sizeof( value_type );
}
template<std::size_t string_size>
inline typename string<string_size>::const_iterator string<string_size>::crend() const {
    return const_cast<const_iterator>( &m_data[0] - sizeof( value_type ) );
}

// capacity
template<std::size_t string_size>
inline typename string<string_size>::size_type string<string_size>::size() const noexcept { 
    return string_size-1 - static_cast<size_type>( m_data[string_size-1] );
}
template<std::size_t string_size>
inline typename string<string_size>::size_type string<string_size>::length() const noexcept { 
    return string_size-1 - static_cast<size_type>( m_data[string_size-1] );
}
template<std::size_t string_size>
inline typename string<string_size>::size_type string<string_size>::max_size() const noexcept {
    return string_size;
}
template<std::size_t string_size>
inline typename string<string_size>::size_type string<string_size>::available() const noexcept {
    return static_cast<size_type>( m_data[string_size-1] );
}
template<std::size_t string_size>
inline bool string<string_size>::empty() const noexcept {
    return m_data[string_size-1] == static_cast<value_type>( string_size-1 ); 
}

// operations
template<std::size_t string_size>
inline void string<string_size>::clear() noexcept {
    //memset( (void*)m_data.data(), 0x00, string_size-2 );
    m_data[0] = '\0';
    m_data[string_size-1] = static_cast<value_type>( string_size-1 );
    //std::cout << "clear(): m_data[0] = " << m_data[0] << ", m_data[string_size-1] = " << (int)m_data[string_size-1] << std::endl;
}
template<std::size_t string_size>
inline void string<string_size>::shallow_clear() noexcept {
    m_data[0] = '\0';
    m_data[string_size-1] = static_cast<value_type>( string_size-1 );
}
template<std::size_t string_size>
template<std::size_t N>
inline string<string_size>& string<string_size>::operator+=( const string<N>& str ) {
    return do_concat( str );
}
template<std::size_t string_size>
inline string<string_size>& string<string_size>::operator+=( const_pointer ch ) {
    return do_concat( string_view( ch, value_traits::length( ch ) ) );
}
template<std::size_t string_size>
template<std::size_t N>
inline int string<string_size>::compare( const string<N>& str ) const {
    const size_type len = length();
    const size_type strlen = str.length();
    
    int result = value_traits::compare( data(), str.data(), std::min( len, strlen ) );
    if( ( len != strlen ) && ( result == 0 ) ) {
            result = len < strlen ? -1 : 1;
    }

    return result;
}
template<std::size_t string_size>
typename string<string_size>::size_type string<string_size>::copy( pointer s, size_type len, size_type pos ) const {
    //assert( len <= string_size );
    //assert( ( pos + len ) <= string_size );

    size_type copied = 0;

    for( auto i=pos; i<pos+len; ++i ) {
        *s++ = m_data[i];
        ++copied;
    }

    return copied;
}

// search
template<std::size_t string_size>
typename string<string_size>::size_type string<string_size>:: find( const string& str, size_type pos ) const {
    return do_find( str, pos, str.length() );
}
template<std::size_t string_size>
typename string<string_size>::size_type string<string_size>::find( const_pointer s, size_type pos ) const {
    const string_view sv( s, value_traits::length( s ) );
    return do_find( sv, pos, sv.length() );
}
template<std::size_t string_size>
typename string<string_size>::size_type string<string_size>::find( const_pointer s, size_type pos, size_type n ) const {
    const string_view sv( s, value_traits::length( s ) );
    return do_find( sv, pos, n );
}
template<std::size_t string_size>
typename string<string_size>::size_type string<string_size>::find( value_type c, size_type pos ) const {
    size_type index = string::npos;

    const size_type length_to_search = string_size - pos;

    for( size_type i=pos; i<length_to_search; ++i ) {
        if( m_data[i] == c ) {
            index = i;
            break;
        }
    }

    return index;
}
template<std::size_t string_size>
typename string<string_size>::size_type string<string_size>::find( string_view sv, size_type pos ) const {
    return do_find( sv, pos, sv.length() );
}

// private functions
template<std::size_t string_size>
void string<string_size>::set_data( string_view sv ) {
    // TODO: use static_assert here
    //assert( length < string_size );//, "flstring::set_data() please allocate more space." );

    const_pointer strdata = sv.data();
    pointer data = &m_data[0];

    while( *strdata ) { 
        *data++ = *strdata++;
    }
    *data = '\0';

    // store the remaining capacity in the last element if non-zero,
    // otherwise the already inserted null-terminator(above) will 
    // be doubling-up as a remaining capacity of zero
    if( data != &m_data[string_size-1] ) {
        m_data[string_size-1] = static_cast<value_type>( string_size-1 - sv.length() );
    }
}
template<std::size_t string_size>
typename string<string_size>::size_type    string<string_size>::do_find( string_view sv, size_type pos, size_type n ) const {
    //const size_type target_size = sv.length() - n;
    //assert( target_size <= string_size );
    //assert( ( pos + target_size ) <= string_size );

    size_type index = string::npos;

    const size_type length_to_search = string_size - pos;
    const_pointer target_string = sv.data();

    for( size_type i=pos; i<length_to_search; ++i ) {
        if( m_data[i] == *target_string ) {
            auto j = i;
            auto ptr = target_string;
            do {
                ++j;
                ++ptr;
            } while( ( *ptr != '\0' ) && ( m_data[j] == *ptr ) );

            // a null pointer (null terminator) would indicate
            // that we've matched the entire string we were
            // looking for, so we're safe to return this index
            if( *ptr == '\0' ) {
                index = i;
                break;
            }
        }
    }

    return index;
}
template<std::size_t string_size>
string<string_size>& string<string_size>::do_concat( string_view sv ) {
    //assert( ( length() + strlen ) <= max_size() );

    const_pointer strdata = sv.data();
    pointer data = &m_data[length()];
    while( *strdata ) { 
        *data++ = *strdata++;
    }
    *data = '\0';

    if( data != &m_data[string_size-1] ) {
        m_data[string_size-1] -= static_cast<value_type>( sv.length() );
    }

    return *this;
}

// added for hash-map testing
template<std::size_t lhs_size, std::size_t rhs_size>
bool operator<( const string<lhs_size>& lhs, const string<rhs_size>& rhs ) {
    return ( lhs.compare( rhs ) == -1 ) && ( lhs.length() < rhs.length() );
}


} // namespace fl


#endif // FLSTRING_HPP
