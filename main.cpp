#include <iostream>

#include "CVector.h"

#include <gtest/gtest.h>

// ==============================< STUBS >==============================
class CInsert
{
public:
    CInsert()
    {
        m_int_data = 2;
    }
    ~CInsert()
    {
        m_int_ptr_data = new int[2];
        m_int_ptr_data[0] = 2;
        m_int_ptr_data[1] = 2;
    }

    const int GetIntData() const
    {
        return m_int_data;
    }
    const int* GetIntPtrData() const
    {
        return m_int_ptr_data;
    }

private:
    int m_int_data = 0;
    int* m_int_ptr_data = nullptr;

};
// ============================================================

// ==============================< TEST FIXTURE > ==============================
class CVectorTest
    : public ::testing::Test
{
protected:
    virtual void SetUp() override
    {
        m_int_vec.assign( 2, 2 );
        m_class_vec.assign( 2, CInsert() );
    }

    virtual void TearDown() override
    {
        m_int_vec.clear();
        m_class_vec.clear();
    }

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
