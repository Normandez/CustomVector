#ifndef CVECTOR_H
#define CVECTOR_H

#include <memory>
#include <stdexcept>

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

private:
    pointer m_data;
    size_type m_size;
    size_type m_capacity;

//=============< Memory managment >=============
    // Alloc underlying data
    void _alloc()
    {
        if(!m_data)
        {
            m_data = ::operator new(m_capacity);
        }
    }

    // Unalloc underlying data
    void _unalloc()
    {
        if(m_data)
        {
            delete[] m_data;
            m_data = nullptr;
        }
    }

    // Capacity change reallocation
    void _realloc()
    {
        pointer buf_data = m_data;
        m_data = nullptr;
        _alloc();

        for( size_type it = 0; it < m_size; it++ )
        {
            m_data[it] = buf_data[it];
        }

        delete [] buf_data;
    }
//=============

//=============< Utils >=============
    // Internal cleanupper (use only when internal data not empty and valid)
    void _clear()
    {
        m_size = 0;
        m_capacity = 0;
        _unalloc();
    }

    // Internal copy 'other' logic
    void _copy_other( const CVector& _other )
    {
        m_size = _other.m_size;
        m_capacity = _other.m_capacity;
        _unalloc();
        _alloc();
        for( size_type it = 0; it < m_size; it++ )
        {
            m_data[it] = _other.m_data[it];
        }
    }

    // Internal assigner (use only after cleanup)
    void _assign( size_type _count, const_reference _value = value_type() )
    {
        m_size = _count;
        m_capacity = m_size;
        _unalloc();
        _alloc();
        for( size_type it = 0; it < m_size; it++ )
        {
            m_data[it] = _value;
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

        size_type r_buf_length = m_size - _pos;
        value_type r_buf [r_buf_length];
        for( size_type it = 0; it < r_buf_length; it++ ) r_buf[it] = m_data[_pos + it];
        m_data[_pos] = _value;
        m_size++;
        for ( size_type it = _pos + 1; it < m_size; it++ ) m_data[it] = r_buf[it - _pos - 1];
    }

    // Internal front pusher (call only after capacity chk)
    void _push_front( const_reference _value )
    {
        value_type data_buf [m_size];
        for ( size_type it = 0; it < m_size; it++ ) data_buf[it] = m_data[it];
        m_data[0] = _value;
        m_size++;
        for ( size_type it = 1; it < m_size; it++ ) m_data[it] = data_buf[it-1];
    }

    // Internal back pusher (call only after capacity chk)
    void _push_back( const_reference _value )
    {
        m_data[m_size] = _value;
        m_size++;
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
    explicit CVector( size_type count, const T& value = T() )
    {
        _assign( count, value );
    }
    
    // Construct vector filled with 'count' of default 'T()'
    explicit CVector( size_type count )
    {
        _assign(count);
    }

    // Copy constructor
    explicit CVector( const CVector& other )
    {
        _copy_other(other);
    }

    // Move constructor
    explicit CVector( CVector&& other )
    {
        _copy_other( static_cast< const CVector& > (other) );

        other.m_size = 0;
        other.m_capacity = 0;
        other._unalloc();
    }

    // Initializer constructor
    explicit CVector( std::initializer_list<T> initl )
    {
        m_size = initl.size();
        m_capacity = m_size;
        _unalloc();
        _alloc();
        for( auto it = initl.begin(), size_type i = 0; it != initl.end(); it++, size_type++ )
        {
            m_data[i] = *it;
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
        _copy_other( static_cast< const CVector& > (other) );

        other.m_size = 0;
        other.m_capacity = 0;
        other._unalloc();
    }

    // Custom vector assignment
    void assign( size_type size, const T& value )
    {
        _clear();
        _assign( size, value );
    }
//=============

//=============< Element access >=============
    // Get value by reference via method
    reference at( size_type pos )
    {
        if( pos > m_size )
        {
            throw std::out_of_range("'pos' out of range");
        }
        return m_data[pos];
    }

    // Get value by const reference via method
    const_reference at( size_type pos ) const
    {
        if( pos > m_size )
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
        return m_data[0]
    }

    // Get last element of array by reference
    reference back() noexcept
    {
        return m_data[ m_size - 1 ];
    }

    // Get last element of array by const reference
    const_reference back() const noexcept
    {
        return m_data[ m_size - 1 ];
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
        return (bool) m_size;
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
        if( mew_cap <= m_size ) return;
        if( new_cap > max_size() ) throw std::length_error("'new_cap' more than max_size()");
        m_capacity = new_cap;
        _realloc();
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
            m_capacity = m_size;
            _realloc();
        }
    }
//=============

//=============< Modifiers >=============
    // Clear vector (capacity not changed)
    void clear() noexcept
    {
        _clear();
    }

    // Inserts value 'value' in position 'pos' by const reference
    reference insert( size_type pos, const T& value )
    {
        if( pos <= m_size )
        {
            if( ( m_size + 1 ) > m_capacity )
            {
                m_capacity = m_capacity * 2;
                _realloc();
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
        if( pos <= m_size )
        {
            if( ( m_size + 1 ) > m_capacity )
            {
                m_capacity = m_capacity * 2;
                _realloc();
            }

            _insert( pos, static_cast<const T& value> (value) );
        }
        else
        {
            throw std::length_error("'pos' is out of range");
        }

        return m_data[pos];
    }

    // Inserts 'count' copies of 'value' in position 'pos' by const reference
    reference insert( size_type pos, size_type count, const T& value )
    {

    }
//=============
};

#endif //CVECTOR_H
