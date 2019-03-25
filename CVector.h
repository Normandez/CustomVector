#ifndef CVECTOR_H
#define CVECTOR_H

#include <memory>
#include <stdexcept>
#include <utility>
#include <initializer_list>
#include <algorithm>

template<class T>
class CVector
{
public:
    using value_type = T;
    using size_type = size_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    using allocator_type = std::allocator<value_type>;

private:
    pointer m_data = nullptr;
    allocator_type m_allocator;
    size_type m_size = 0;
    size_type m_capacity = 0;

// =============< Memory managment >=============

    // Alloc underlying data
    void _alloc( bool _construct )
    {
        if(!m_data)
        {
            m_data = m_allocator.allocate(m_capacity);
            if (_construct) __construct();
        }
    }

    // Unalloc underlying data
    void _unalloc( bool _destroy )
    {
        if(m_data)
        {
            if(_destroy) __destroy();
            m_allocator.deallocate( m_data, m_capacity );
            m_data = nullptr;
        }
    }

    // Capacity change reallocation (call after 'm_capacity' changed)
    void _realloc( size_type _new_cap )
    {
        size_type old_cap = m_capacity;
        m_capacity = _new_cap;
        if( m_capacity == 0 ) m_capacity = 1;

        pointer buf_data = m_data;
        m_data = nullptr;
        _alloc(false);
		for( size_type it = 0; it < m_size; it++ )
		{
			m_allocator.construct( ( m_data + it ), buf_data[it] );
			m_allocator.destroy( ( buf_data + it ) );
		}
		m_allocator.deallocate( buf_data, old_cap );
    }

    // Destroy all constructed underlying values
    void __destroy()
    {
        for( size_t it = 0 ; it < m_size; it++ )
        {
            m_allocator.destroy( m_data + it );
        }
    }

    // Default construct underlying values
    void __construct()
    {
        for( size_type it = 0; it < m_size; it++ )
        {
            m_allocator.construct( m_data + it );
        }
    }

// =============

//=============< Utils >=============

    // Internal cleanupper
    void _clear()
    {
        _unalloc(true);
        m_size = 0;
        m_capacity = 0;
    }

    // Internal copy 'other' logic
    void _copy_other( const CVector& _other )
    {
		if( m_capacity != 0 )
		{
			_clear();
		}

        m_size = _other.m_size;
        m_capacity = _other.m_capacity;
        _alloc(false);
        for( size_type it = 0; it < m_size; it++ )
        {
            m_allocator.construct( ( m_data + it ), _other.m_data[it] );
        }
    }

    // Internal move 'other' logic
    void _move_other( const CVector& _other )
    {
        if( m_capacity != 0 )
		{
			_clear();
		}

    	m_size = _other.m_size;
    	m_capacity = _other.m_capacity;
    	m_data = _other.m_data;
    }

    // Internal assigner by default constructor
    void _assign( size_type _count )
    {
        if( m_capacity != 0 )
		{
			_clear();
		}

        m_size = _count;
        m_capacity = m_size;
        _alloc(false);
        for( size_type it = 0; it < m_size; it++ )
        {
            m_allocator.construct( ( m_data + it ) );
        }
    }

    // Internal assigner by value
    void _assign( size_type _count, const_reference _value )
    {
        if( m_capacity != 0 )
		{
			_clear();
		}

        m_size = _count;
        m_capacity = m_size;
        _alloc(false);
        for( size_type it = 0; it < m_size; it++ )
        {
            m_allocator.construct( (m_data + it), _value );
        }
    }

    // Internal inserter (call only after capacity chk)
    void _insert( size_type _pos, const_reference _value )
    {
        if( _pos == 0 )
        {
            _push_front(_value);
            return;
        }

		size_type data_buf_length = m_size - _pos;
		pointer data_buf = m_allocator.allocate(data_buf_length);
		for( size_type it = 0; it < data_buf_length; it++ )
		{
			m_allocator.construct( ( data_buf + it ), m_data[ _pos + it ] );
		}
		m_allocator.construct( ( m_data + _pos ), _value );
        m_size++;
		for ( size_type it = _pos + 1; it < m_size; it++ )
		{
			m_allocator.construct( ( m_data + it ), data_buf[it - _pos - 1] );
			m_allocator.destroy( ( data_buf + it - _pos - 1 ) );
		}
		m_allocator.deallocate( data_buf, data_buf_length );
    }

    // Internal front pusher (call only after capacity chk)
    void _push_front( const_reference _value )
    {
		pointer data_buf = m_allocator.allocate( m_size );
		for ( size_type it = 0; it < m_size; it++ )
		{
			m_allocator.construct( ( data_buf + it ), m_data[it] );
		}
		m_allocator.construct( m_data, _value );
		m_size++;
		for ( size_type it = 1; it < m_size; it++ )
		{
			m_allocator.construct( ( m_data + it ), data_buf[it - 1] );
			m_allocator.destroy( data_buf + it - 1 );
		}
		m_allocator.deallocate( data_buf, ( m_size - 1 ) );
    }

    // Internal back pusher (call only after capacity chk)
    void _push_back( const_reference _value )
    {
        m_allocator.construct( ( m_data + m_size ), _value );
        m_size++;
    }

	// Internal emplacer (call only after capacity chk)
	template<class... Args>
	void _emplace( size_type _pos, Args&&... _args )
	{
		if( _pos == 0 )
        {
            _emplace_front( std::forward<Args&&>(_args)... );
            return;
        }

        size_type data_buf_length = m_size - _pos;
        pointer data_buf = m_allocator.allocate(data_buf_length);
        for( size_type it = 0; it < data_buf_length; it++ )
        {
            m_allocator.construct( ( data_buf + it ), m_data[_pos + it] );
        }
        m_allocator.construct( ( m_data + _pos ), std::forward<Args&&>(_args)... );
        m_size++;
		for ( size_type it = _pos + 1; it < m_size; it++ ) 
		{
			m_allocator.construct( ( m_data + it ), data_buf[it - _pos - 1] );
			m_allocator.destroy( data_buf + it - _pos - 1 );
		}
        m_allocator.deallocate( data_buf, data_buf_length );
	}

	// Internal front emplacer (call only after capacity chk)
	template<class... Args>
	void _emplace_front( Args&&... _args )
	{
		pointer data_buf = m_allocator.allocate(m_size);
        for ( size_type it = 0; it < m_size; it++ )
        {
            m_allocator.construct( ( data_buf + it ), m_data[it] );
        }
		m_allocator.construct( m_data, std::forward<Args&&>(_args)... );
        m_size++;
        for ( size_type it = 1; it < m_size; it++ )
        {
            m_allocator.construct( ( m_data + it ), data_buf[it-1] );
			m_allocator.destroy( data_buf + it - 1 );
        }
        m_allocator.deallocate( data_buf, ( m_size - 1 ) );
	}

	// Internal back emplacer (call only after capacity chk)
	template<class... Args>
    void _emplace_back( Args&&... _args )
    {
        m_allocator.construct( ( m_data + m_size ), std::forward<Args&&>(_args)... );
        m_size++;
    }

    // Internal remover at specified position
    void _remove_at( size_type _pos )
    {
        m_allocator.destroy( ( m_data + _pos ) );
		if( _pos == m_size )	// Last element case
		{
			return;
		}

		for( size_type it = _pos; it < m_size; it++ )
		{
			m_allocator.destroy( m_data + it );
			m_allocator.construct( ( m_data + it ), m_data[it + 1] );
		}
    }

    // Internal in range remover [_first; _last)
    void _remove_in_range( size_type _first, size_type _last )
    {
        for( size_type it = _first; it < _last; it++ ) m_allocator.destroy( ( m_data + it ) );
        
		size_type remove_dif = _last - _first;
		for( size_type it = _first; it < m_size; it++ )
		{
			m_allocator.destroy( m_data + it );
			m_allocator.construct( ( m_data + it ), m_data[it + remove_dif] );
		}
    }

    // Internal resizer
    void _resize( size_type _count, const_reference _value = value_type )
    {
        if( _count < m_size )
        {
            for( size_type it = _count; it < m_size; it++ )
            {
                m_allocator.destroy( m_data + it );
                m_allocator.construct( m_data + it );
                m_data[it] = _value;
            }
        }
        else if( _count > m_size )
        {
            if( _count > m_capacity )
            {
                _realloc( m_capacity * 2 );
            }

            for( size_type it = m_size; it <= _count; it++ )
            {
                m_allocator.construct( m_data + it );
                m_data[it] = _value;
            }
        }
        m_size = _count;
    }

//=============

public:
//=============< Initializers >=============

    // Construct empty vector
    explicit CVector() noexcept
    {
        m_size = 0;
        m_capacity = 0;
        m_data = nullptr;
    }

    // Constuct vector filled with 'count' of 'value'
    explicit CVector( size_type count, const T& value )
    {
        _assign( count, value );
    }
    
    // Construct vector filled with 'count' of default 'T()'
    explicit CVector( size_type count )
    {
        _assign(count);
    }

    // Copy constructor
    CVector( const CVector& other )
    {
        _copy_other(other);
    }

    // Move constructor
    CVector( CVector&& other )
    {
    	_move_other(other);

		other.m_size = 0;
		other.m_capacity = 0;
		other.m_data = nullptr;
    }

    // Initializer constructor
    CVector( std::initializer_list<T> initl )
    {
        _clear();
        m_size = initl.size();
        m_capacity = m_size;
        _alloc(false);

        size_type i = 0;
        for( auto it = initl.begin(); it != initl.end(); it++, i++ )
        {
			m_allocator.construct( ( m_data + i ), *it );
        }
    }

    ~CVector()
    {
        _clear();
    }

    // Copy assignment
    void operator=( const CVector& other )
    {
        _copy_other(other);
    }

    // Move assignment
    void operator=( CVector&& other )
    {
        _move_other(other);
        
		other.m_size = 0;
		other.m_capacity = 0;
		other.m_data = nullptr;
    }

    // Custom vector assignment
    void assign( size_type size, const T& value )
    {
        _assign( size, value );
    }

//=============

//=============< Element access >=============

    // Get value by reference via method
    reference at( size_type pos )
    {
        if( pos >= m_size )
        {
            throw std::out_of_range("'pos' out of range");
        }
        return m_data[pos];
    }

    // Get value by const reference via method
    const_reference at( size_type pos ) const
    {
        if( pos >= m_size )
        {
            throw std::out_of_range("'pos' out of range");
        }
        return m_data[pos];
    }

    // Get value by reference via operator[]
    reference operator[] ( size_type pos ) noexcept
    {
        return m_data[pos];
    }

    // Get value by const reference via operator[]
    const_reference operator[] ( size_type pos ) const noexcept
    {
        return m_data[pos];
    }

    // Get first element of array by reference
    reference front() noexcept
    {
        return m_data[0];
    }

    // Get first element of array by cosnt reference
    const_reference front() const noexcept
    {
        return m_data[0];
    }

    // Get last element of array by reference
    reference back() noexcept
    {
        return m_data[m_size - 1];
    }

    // Get last element of array by const reference
    const_reference back() const noexcept
    {
        return m_data[m_size - 1];
    }

    // Get underlying array by pointer
    pointer data() noexcept
    {
        return m_data;
    }

    // Get underlying array by const pointer
    const_pointer data() const noexcept
    {
        return m_data;
    }

//=============

//=============< Capacity >=============

    // Check is vector empty
    bool empty() const noexcept
    {
        return ( m_size == 0 );
    }

    // Get size of vector
    size_type size() const noexcept
    {
        return m_size;
    }

    // Returns theoretical max size of vector
    size_type max_size() const noexcept
    {
        return std::numeric_limits<size_type>::max();
    }

    // Increase the capacity of vector
    void reserve( size_type new_cap )
    {
        if( new_cap <= m_size ) return;
        if( new_cap > max_size() ) throw std::length_error("'new_cap' more than max_size()");
        _realloc(new_cap);
    }

    // Returns current capacity
    size_type capacity() const noexcept
    {
        return m_capacity;
    }

    // Removed unused capacity
    void shrink_to_fit()
    {
        if( m_capacity > m_size )
        {
            _realloc(m_size);
        }
    }

//=============

//=============< Modifiers >=============

    // Clear vector (capacity not changed)
    void clear()
    {
		if( m_capacity != 0 )
		{
			_clear();
		}
    }

    // Inserts value 'value' in position 'pos' by const reference
    reference insert( size_type pos, const T& value )
    {
        if( pos < m_size )
        {
            if( ( m_size + 1 ) > m_capacity )
            {
                _realloc( m_capacity * 2 );
            }
            _insert( pos, value );
        }
        else
        {
            throw std::length_error("'pos' is out of range");
        }
        return m_data[pos];
    }

    // Inserts value 'value' in position 'pos' by rvalue reference
    reference insert( size_type pos, T&& value )
    {
        if( pos < m_size )
        {
            if( ( m_size + 1 ) > m_capacity )
            {
                _realloc( m_capacity * 2 );
            }
            _insert( pos, value );
        }
        else
        {
            throw std::length_error("'pos' is out of range");
        }
        return m_data[pos];
    }

    // Inserts 'count' copies of 'value' at position 'pos' by const reference
    reference insert( size_type pos, size_type count, const T& value )
    {
        if( pos < m_size )
        {
            while( ( m_size + count ) > m_capacity )
            {
                _realloc( m_capacity * 2 );
            }
            while( count > 0 )
            {
                _insert( pos, value );
                count--;
            }
        }
        else
        {
            throw std::length_error("'pos' is out of range");
        }
        return m_data[pos];
    }

    // Inserts values from 'ilist' at position 'pos'
    reference insert( size_type pos, std::initializer_list<T> ilist )
    {
        if( pos < m_size )
        {
            while( ( m_size + ilist.size() ) > m_capacity )
            {
                _realloc( m_capacity * 2 );
            }
            for( auto it = ilist.end() - 1; it != ilist.begin() - 1; it-- )
            {
                _insert( pos, *it );
            }
        }
        else
        {
            throw std::length_error("'pos' is out of range");
        }
        return m_data[pos];
    }

    // Inserts element before 'pos' via T(... args) constructor
    template<class... Args>
    reference emplace( size_type pos, Args&&... args )
    {
        if( pos < m_size )
        {
            if( ( m_size + 1 ) > m_capacity )
            {
                _realloc( m_capacity * 2 );
            }
            _emplace( pos, std::forward<Args&&>(args)... );
        }
        else
        {
            throw std::length_error("'pos' is out of range");
        }
        return m_data[pos];
    }

    // Removes element in position 'pos' and returns reference on element after removed
    reference erase( size_type pos )
    {
        if( pos < m_size )
        {
            m_size--;
            _remove_at(pos);
        }
        else
        {
            throw std::length_error("'pos' is out of range");
        }

		if ( pos == m_size )
		{
			return m_data[pos - 1];
		}
		else
		{
			return m_data[pos];
		}
    }

    // Removes element in range [first; last) and returns reference on element after removed
    reference erase( size_type first, size_type last )
    {
        if( first < m_size && last < m_size && last >= first )
        {
            m_size -= (last - first);
            _remove_in_range( first, last );
        }
        else
        {
            throw std::length_error("'pos' is out of range");
        }

		return m_data[last - ( last - first )];
    }

    // Appends the given element value to the end of the container by const reference
    void push_back( const T& value )
    {
        if( ( m_size + 1 ) > m_capacity )
        {
            _realloc( m_capacity * 2 );
        }
        _push_back(value);
    }

    // Appends the given element value to the end of the container via move semantics
    void push_back( T&& value )
    {
        if( ( m_size + 1 ) > m_capacity )
        {
            _realloc( m_capacity * 2 );
        }
        _push_back(value);

        m_allocator.destroy(&value);
    }

	// Inserts element in the end of the container 'pos' via T() constructor
    template<class... Args>
    void emplace_back( Args&&... args )
    {
        if( ( m_size + 1 ) > m_capacity )
        {
            _realloc( m_capacity * 2 );
        }
		_emplace_back( std::forward<Args&&>(args)... );
    }

	// Removes the last element of the container
	void pop_back()
	{
		m_allocator.destroy( m_data + m_size - 1 );
		m_size--;
	}

    // Resizes the container to contain count elements (init by default)
    void resize( size_type count )
    {
        if( count != m_size )
        {
            _resize(count);
        }
    }

    // Resizes the container to contain count elements (init by 'value')
    void resize( size_type count, const value_type& value )
    {
        if( count != m_size )
        {
            _resize( count, value );
        }
    }

    // Exchanges the contents of the container with those of other
    void swap( CVector& other )
    {
        pointer buf_data = m_data;
        allocator_type buf_allocator = m_allocator;
        size_type buf_size = m_size;
        size_type buf_capacity = m_capacity;

        m_data = other.m_data;
        m_allocator = other.m_allocator;
        m_size = other.m_size;
        m_capacity = other.m_capacity;

        other.m_data = buf_data;
        other.m_allocator = buf_allocator;
        other.m_size = buf_size;
        other.m_capacity = buf_capacity;
    }

//=============

// =============< PUBLIC NON-MEMBERS >=============

    // Overloaded operators
    template< class T >
    friend bool operator==( const CVector<T>& lhs, const CVector<T>& rhs ) noexcept;
    template< class T >
    friend bool operator!=( const CVector<T>& lhs, const CVector<T>& rhs ) noexcept;
    template< class T >
    friend bool operator<( const CVector<T>& lhs, const CVector<T>& rhs ) noexcept;
    template< class T >
    friend bool operator<=( const CVector<T>& lhs, const CVector<T>& rhs ) noexcept;
    template< class T >
    friend bool operator>( const CVector<T>& lhs, const CVector<T>& rhs ) noexcept;
    template< class T >
    friend bool operator>=( const CVector<T>& lhs, const CVector<T>& rhs ) noexcept;

    // Non-member swap
    template< class T >
    friend void swap( CVector<T>& lhs, CVector<T>& rhs );

// =============
};

// =============< NON-MEMBERS >=============

// Checks if the contents of 'lhs' and 'rhs' are equal
template< class T >
bool operator==( const CVector<T>& lhs, const CVector<T>& rhs ) noexcept
{
    if( lhs.m_size != rhs.m_size )
    {
        return false;
    }
    else
    {
        for( size_t it = 0; it < lhs.m_size; it++ )
        {
            if( lhs.m_data[it] != rhs.m_data[it] )
            {
                return false;
            }
        }
    }
    return true;
}

// Checks if the contents of 'lhs' and 'rhs'
template< class T >
bool operator!=( const CVector<T>& lhs, const CVector<T>& rhs ) noexcept
{
    return !( operator==( lhs, rhs ) );
}

// Checks if the contents of 'lhs' lowest than 'rhs'
template< class T >
bool operator<( const CVector<T>& lhs, const CVector<T>& rhs ) noexcept
{
    return std::lexicographical_compare( lhs.m_data, lhs.m_data + lhs.m_size,
                                         rhs.m_data, rhs.m_data + rhs.m_size );
}

// Checks if the contents of 'lhs' lowest than 'rhs' or equeals
template< class T >
bool operator<=( const CVector<T>& lhs, const CVector<T>& rhs ) noexcept
{
    if( lhs == rhs )
    {
        return true;
    }
    else
    {
        return std::lexicographical_compare( lhs.m_data, lhs.m_data + lhs.m_size,
                                             rhs.m_data, rhs.m_data + rhs.m_size );
    }
}

// Checks if the contents of 'lhs' higher than 'rhs'
template< class T >
bool operator>( const CVector<T>& lhs, const CVector<T>& rhs ) noexcept
{
    return !( lhs < rhs );
}

// Checks if the contents of 'lhs' higher than 'rhs' or equeals
template< class T >
bool operator>=( const CVector<T>& lhs, const CVector<T>& rhs ) noexcept
{
    if( lhs == rhs )
    {
        return true;
    }
    else
    {
        return !std::lexicographical_compare( lhs.m_data, lhs.m_data + lhs.m_size,
                                             rhs.m_data, rhs.m_data + rhs.m_size );
    }
}

// Swaping vectors (via 'swap()' method)
template< class T >
void swap( CVector<T>& lhs, CVector<T>& rhs )
{
    lhs.swap(rhs);
}
// =============

#endif //CVECTOR_H
