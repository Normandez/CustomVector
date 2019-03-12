#include <iostream>

#include "CVector.h"

#include <gtest/gtest.h>

// ==============================< STUBS >==============================
class CInsert
{
public:
    CInsert()
    {
        m_int_data = 0;
        m_int_ptr_data = nullptr;
        m_int_ptr_data_size = 0;
        m_std_str_data = "";
    }
    CInsert( const CInsert& other )
    {
        m_int_data = other.m_int_data;
        m_int_ptr_data_size = other.m_int_ptr_data_size;
        m_int_ptr_data = new int [m_int_ptr_data_size];
        for( size_t count = 0; count < m_int_ptr_data_size; count++ )
        {
            m_int_ptr_data[count] = other.m_int_ptr_data[count];
        }
        m_std_str_data = other.m_std_str_data;
    }
    CInsert( CInsert&& other )
    {
        m_int_data = other.m_int_data;
        m_int_ptr_data_size = other.m_int_ptr_data_size;
        m_int_ptr_data = other.m_int_ptr_data;
        m_std_str_data = other.m_std_str_data;

        other.m_int_data = 0;
        other.m_int_ptr_data_size = 0;
        delete [] other.m_int_ptr_data;
        other.m_std_str_data = "";
    }
    void operator= ( const CInsert& other )
    {
        m_int_data = other.m_int_data;
        m_int_ptr_data_size = other.m_int_ptr_data_size;
        m_int_ptr_data = new int [m_int_ptr_data_size];
        for( size_t count = 0; count < m_int_ptr_data_size; count++ )
        {
            m_int_ptr_data[count] = other.m_int_ptr_data[count];
        }
        m_std_str_data = other.m_std_str_data;
    }
    void operator= ( CInsert&& other )
    {
        m_int_data = other.m_int_data;
        m_int_ptr_data_size = other.m_int_ptr_data_size;
        m_int_ptr_data = other.m_int_ptr_data;
        m_std_str_data = other.m_std_str_data;

        other.m_int_data = 0;
        other.m_int_ptr_data_size = 0;
        delete [] other.m_int_ptr_data;
        other.m_std_str_data = "";
    }
    ~CInsert()
    {
        m_int_data = 0;
        delete [] m_int_ptr_data;
        m_int_ptr_data = nullptr;
        m_std_str_data = "";
    }

    void SetIntData()
    {
        m_int_data = 0;
    }

    void SetIntPtrData()
    {
        m_int_ptr_data_size = 5;
        m_int_ptr_data = new int [m_int_ptr_data_size];
        for( size_t count = 0; count < m_int_ptr_data_size; count++ )
        {
            m_int_ptr_data[count] = static_cast<int> (count);
        }
    }

    void SetStdStrData()
    {
        m_std_str_data = "Hello world!";
    }

    const int GetIntData() const
    {
        return m_int_data;
    }
    const int* GetIntPtrData() const
    {
        return m_int_ptr_data;
    }
    const std::string GetStdStrData() const
    {
        return m_std_str_data;
    }

private:
    int m_int_data = 0;
    int* m_int_ptr_data = nullptr;
    size_t m_int_ptr_data_size = 0;
    std::string m_std_str_data = "";

};
// ============================================================

// ==============================< TEST FIXTURE > ==============================
class CVectorTest
    : public ::testing::Test
{
protected:
    virtual void SetUp() override
    {
        
    }

    virtual void TearDown() override
    {
        m_int_vec.clear();
        m_class_vec.clear();
    }

private:
    CVector<int> m_int_vec;
    CVector<CInsert> m_class_vec;

};
// ============================================================

// ==============================< TESTS >==============================
TEST_F( CVectorTest, Construct )
{
    
    SUCCEED();
}
// ============================================================

int main( int argc, char* argv[] )
{
    testing::InitGoogleTest( &argc, argv );

    return RUN_ALL_TESTS();
}
