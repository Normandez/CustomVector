#include <iostream>

#include "CVector.h"
#include <vector>

#include <gtest/gtest.h>

// ==============================< STUBS >==============================
class CInsert
{
public:
    CInsert()
    {
        std::cout << "CInsert() called" << std::endl;
        m_int_data = 0;
        m_int_ptr_data = nullptr;
        m_int_ptr_data_size = 0;
        m_std_str_data = "";
    }
    CInsert( const CInsert& other )
    {
        std::cout << "CInsert( const CInsert& other ) called" << std::endl;
        m_int_data = other.m_int_data;
        m_int_ptr_data_size = other.m_int_ptr_data_size;
        if( m_int_ptr_data_size != 0 )
        {
            m_int_ptr_data = new int [m_int_ptr_data_size];
            for( size_t count = 0; count < m_int_ptr_data_size; count++ )
            {
                m_int_ptr_data[count] = other.m_int_ptr_data[count];
            }
        }
        else
        {
            m_int_ptr_data = nullptr;
        }
        m_std_str_data = other.m_std_str_data;
    }
    CInsert( CInsert&& other )
    {
        std::cout << "CInsert( CInsert&& other ) called" << std::endl;
        m_int_data = other.m_int_data;
        m_int_ptr_data_size = other.m_int_ptr_data_size;
        m_int_ptr_data = other.m_int_ptr_data;
        m_std_str_data = other.m_std_str_data;

        other.m_int_data = 0;
        other.m_int_ptr_data_size = 0;
        delete [] other.m_int_ptr_data;
        other.m_std_str_data = "";
    }
    CInsert& operator= ( const CInsert& other )
    {
        std::cout << "operator=( const CInsert& other ) called" << std::endl;
        this->m_int_data = other.m_int_data;
        this->m_int_ptr_data_size = other.m_int_ptr_data_size;
        if( this->m_int_ptr_data_size != 0 )
        {
            this->m_int_ptr_data = new int [this->m_int_ptr_data_size];
            for( size_t count = 0; count < this->m_int_ptr_data_size; count++ )
            {
                this->m_int_ptr_data[count] = other.m_int_ptr_data[count];
            }
        }
        else
        {
            this->m_int_ptr_data = nullptr;
        }
        this->m_std_str_data = other.m_std_str_data;

        return *this;
    }
    CInsert& operator= ( CInsert&& other )
    {
        std::cout << "operator=( CInsert&& other ) called" << std::endl;
        this->m_int_data = other.m_int_data;
        this->m_int_ptr_data_size = other.m_int_ptr_data_size;
        this->m_int_ptr_data = other.m_int_ptr_data;
        this->m_std_str_data = other.m_std_str_data;

        other.m_int_data = 0;
        other.m_int_ptr_data_size = 0;
        delete [] other.m_int_ptr_data;
        other.m_std_str_data = "";

        return *this;
    }
    ~CInsert()
    {
        std::cout << "~CInsert()" << std::endl;
        m_int_data = 0;
        delete [] m_int_ptr_data;
        m_int_ptr_data = nullptr;
        m_std_str_data = "";
    }

    void SetIntData()
    {
        m_int_data = 10;
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
};
// ============================================================

// ==============================< TESTS >==============================
TEST_F( CVectorTest, DefaultConstuct )
{
    CVector<int> int_vec;
    EXPECT_EQ( int_vec.size(), 0 );
    EXPECT_EQ( int_vec.capacity(), 0 );
    EXPECT_EQ( int_vec.data(), nullptr );

    CVector<CInsert> class_vec;
    EXPECT_EQ( int_vec.size(), 0 );
    EXPECT_EQ( int_vec.capacity(), 0 );
    EXPECT_EQ( int_vec.data(), nullptr );
}

TEST_F( CVectorTest, AssignValueConstruct )
{
    CVector<int> int_vec( 10, 10 );
    EXPECT_EQ( int_vec.size(), 10 );
    EXPECT_EQ( int_vec.capacity(), 10 );
    EXPECT_EQ( int_vec.data()[5], 10 );

    CInsert insert_obj;
    insert_obj.SetIntData();
    insert_obj.SetIntPtrData();
    insert_obj.SetStdStrData();

    CVector<CInsert> class_vec( 10, insert_obj );
    EXPECT_EQ( class_vec.size(), 10 );
    EXPECT_EQ( class_vec.capacity(), 10 );

    CInsert inserted_obj = class_vec.data()[5];
    EXPECT_EQ( inserted_obj.GetIntData(), 10 );
    EXPECT_STREQ( inserted_obj.GetStdStrData().c_str(), "Hello world!" );
    EXPECT_EQ( inserted_obj.GetIntPtrData()[4], 4 );
}

TEST_F( CVectorTest, AssignDefaultValueConstruct )
{
    CVector<int> int_vec(10);
    EXPECT_EQ( int_vec.size(), 10 );
    EXPECT_EQ( int_vec.capacity(), 10 );
    EXPECT_EQ( int_vec.data()[5], 0 );

    CVector<CInsert> class_vec(10);
    EXPECT_EQ( class_vec.size(), 10 );
    EXPECT_EQ( class_vec.capacity(), 10 );

    EXPECT_EQ( class_vec.data()[5].GetIntData(), 0 );
    EXPECT_STREQ( class_vec.data()[5].GetStdStrData().c_str(), "" );
    EXPECT_EQ( class_vec.data()[5].GetIntPtrData(), nullptr );
}

TEST_F( CVectorTest, CopyConstruct )
{
    CVector<int> int_vec1( 10, 10 );
    CVector<int> int_vec2(int_vec1);
    EXPECT_EQ( int_vec1.size(), int_vec2.size() );
    EXPECT_EQ( int_vec1.capacity(), int_vec2.capacity() );
    EXPECT_EQ( int_vec1.data()[5], int_vec2.data()[5] );

    CVector<CInsert> class_vec1(10);
    CVector<CInsert> class_vec2(class_vec1);
    EXPECT_EQ( class_vec1.size(), class_vec2.size() );
    EXPECT_EQ( class_vec1.capacity(), class_vec2.capacity() );

    EXPECT_EQ( class_vec1.data()[5].GetIntData(), class_vec2.data()[5].GetIntData() );
    EXPECT_STREQ( class_vec1.data()[5].GetStdStrData().c_str(), class_vec2.data()[5].GetStdStrData().c_str() );
    EXPECT_EQ( class_vec1.data()[5].GetIntPtrData(), class_vec2.data()[5].GetIntPtrData() );
}

TEST_F( CVectorTest, MoveConstruct )
{
    CVector<int> int_vec1( 10, 10 );
    CVector<int> int_vec2( std::move(int_vec1) );
    EXPECT_NE( int_vec1.size(), int_vec2.size() );
    EXPECT_NE( int_vec1.capacity(), int_vec2.capacity() );
	EXPECT_EQ( int_vec2.size(), 10 );
	EXPECT_EQ( int_vec2.capacity(), 10 );
	EXPECT_EQ( int_vec2.data()[5], 10 );
	EXPECT_EQ( int_vec1.size(), 0 );
	EXPECT_EQ( int_vec1.capacity(), 0 );
	EXPECT_EQ( int_vec1.data(), nullptr );

    CVector<CInsert> class_vec1(10);
    CVector<CInsert> class_vec2( std::move(class_vec1) );
    EXPECT_NE( class_vec1.size(), class_vec2.size() );
    EXPECT_NE( class_vec1.capacity(), class_vec2.capacity() );
	EXPECT_EQ( class_vec2.size(), 10 );
	EXPECT_EQ( class_vec2.capacity(), 10 );

    EXPECT_EQ( class_vec2.data()[5].GetIntData(), 0 );
    EXPECT_STREQ( class_vec2.data()[5].GetStdStrData().c_str(), "" );
    EXPECT_EQ( class_vec2.data()[5].GetIntPtrData(), nullptr );

	EXPECT_EQ( class_vec1.size(), 0 );
	EXPECT_EQ( class_vec1.capacity(), 0 );
	EXPECT_EQ( class_vec1.data(), nullptr );
}

TEST_F( CVectorTest, InitializerListConstruct )
{
	CVector<int> int_vec( { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 } );
    EXPECT_EQ( int_vec.size(), 10 );
    EXPECT_EQ( int_vec.capacity(), 10 );
    EXPECT_EQ( int_vec.data()[5], 5 );
	
	CInsert insrt;
	CVector<CInsert> class_vec( { insrt, insrt, insrt, insrt, insrt, insrt, insrt, insrt, insrt, insrt } );
    EXPECT_EQ( class_vec.size(), 10 );
    EXPECT_EQ( class_vec.capacity(), 10 );

    EXPECT_EQ( class_vec.data()[5].GetIntData(), 0 );
    EXPECT_STREQ( class_vec.data()[5].GetStdStrData().c_str(), "" );
    EXPECT_EQ( class_vec.data()[5].GetIntPtrData(), nullptr );
}
// ============================================================

int main( int argc, char* argv[] )
{
    testing::InitGoogleTest( &argc, argv );

    //CInsert insrt;
	//CVector<CInsert> vec1 = { insrt, insrt };
	//CVector<CInsert> vec2( std::move(vec1) );

    //return 0;
    return RUN_ALL_TESTS();
}
