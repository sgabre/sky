#include <unity.h>

#include <Angle.h>

void test_Radians_0(void)
{
    TEST_ASSERT_DOUBLE_WITHIN(1.234, 1.234, 3);
}

void test_Radians_30(void)
{
    TEST_ASSERT_DOUBLE_WITHIN(1.234, 1.234, 3);

}

void test_Radians_45(void)
{
    TEST_ASSERT_DOUBLE_WITHIN(1.234, 1.234, 3);
    
}

void test_Radians_60(void)
{
    TEST_ASSERT_DOUBLE_WITHIN(1.234, 1.234, 3);
    
}

void test_Radians_90(void)
{
    TEST_ASSERT_DOUBLE_WITHIN(1.234, 1.234, 3);
    
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_Radians_0);
    RUN_TEST(test_Radians_30);
    RUN_TEST(test_Radians_45);
    RUN_TEST(test_Radians_60);
    RUN_TEST(test_Radians_90);

    return UNITY_END();
}
