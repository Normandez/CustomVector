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
    explicit CVector() noexcept
    {
        m_data_size = 0;
        m_data = nullptr;
    }

    explicit CVector( size_type count, const T& value = T() )
    {
        m_data_size = count;
        m_data = new value_type[count];
        for( size_type i = 0; i < count; i++ )
        {
            m_data[i] = value;
        }
        
    }
    
    explicit CVector( size_type count )
    {
        m_data_size = count;
        for( size_type i = 0; i < count; i++ )
        {
            m_data[i] = T();
        }

    }

    explicit CVector( const CVector& other )
    {
        m_data_size = other.size();
        m_data = new value_type[m_data_size];
        for( size_type i = 0; i < m_data_size; i++ )
        {
            m_data[i] = other.data()[i];
        }

    }

    explicit CVector( CVector&& other )
    {
        m_data_size = other.size();
        m_data = std::move( other.data() );
    }

    explicit CVector( std::initializer_list<value_type> initl )
    {
        m_data_size = initl.size();
        m_data = new value_type[m_data_size];
        for( auto it = initl.begin(), size_type i = 0; it != initl.end(); it++, size_type++ )
        {
            m_data[i] = *it;
        }

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
