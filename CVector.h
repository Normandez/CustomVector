#ifndef CVECTOR_H
#define CVECTOR_H

#include <memory>

template<class T>
class CVector
{
public:
    using value_type = T;
    using size_type = size_t;
    using reference = T&;
    using pointer = T*;
    using const_pointer = const T*;

private:
    pointer m_data;
    size_type m_data_size;

public:
    // Construct empty vector
    explicit CVector() noexcept
    {
        m_data_size = 0;
        m_data = nullptr;
    }

    // Constuct vector filled with 'count' of 'value'
    explicit CVector( size_type count, const T& value = T() )
    {
        m_data_size = count;
        m_data = new value_type[count];
        for( size_type i = 0; i < count; i++ )
        {
            m_data[i] = value;
        }
        
    }
    
    // Construct vector filled with 'count' of default 'T()'
    explicit CVector( size_type count )
    {
        m_data_size = count;
        for( size_type i = 0; i < count; i++ )
        {
            m_data[i] = T();
        }

    }

    // Copy constructor
    explicit CVector( const CVector<value_type>& other )
    {
        m_data_size = other.size();
        m_data = new value_type[m_data_size];
        for( size_type i = 0; i < m_data_size; i++ )
        {
            m_data[i] = other.m_data[i];
        }

    }

    // Move constructor
    explicit CVector( CVector<value_type>&& other )
    {
        m_data_size = other.size();
        m_data = new value_type[m_data_size];
        for( size_type i = 0; i < m_data_size; i++ )
        {
            m_data[i] = other.m_data[i];
        }

        other.m_data_size = 0;
        delete [] other.m_data;
        other.m_data = nullptr;
    }

    // Initializer constructor
    explicit CVector( std::initializer_list<value_type> initl )
    {
        m_data_size = initl.size();
        m_data = new value_type[m_data_size];
        for( auto it = initl.begin(), size_type i = 0; it != initl.end(); it++, size_type++ )
        {
            m_data[i] = *it;
        }

    }

    // Copy assignment
    void operator=( CVector<value_type>& other )
    {
        m_data_size = other.size();
        m_data = new value_type[m_data_size];
        for( size_type i = 0; i < m_data_size; i++ )
        {
            m_data[i] = other.m_data[i];
        }

    }

    // Move assignment
    void operator=( CVector<value_type>&& other )
    {
        m_data_size = other.size();
        m_data = new value_type[m_data_size];
        for( size_type i = 0; i < m_data_size; i++ )
        {
            m_data[i] = other.m_data[i];
        }

        other.m_data_size = 0;
        delete [] other.m_data;
        other.m_data = nullptr;
    }

    ~CVector()
    {
        m_data_size = 0;
        delete [] m_data_size;
        m_data_size = nullptr;
    }

    const size_type size() noexcept
    {
        return m_data_size;
    }

    const_pointer data() noexcept
    {
        return m_data;
    }
    
};

#endif //CVECTOR_H
