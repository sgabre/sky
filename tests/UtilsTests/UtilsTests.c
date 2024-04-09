#include <unity.h>

#include <Angle.h>


void test_1_Degres(void)
{
    //TEST_ASSERT_EQUAL_HEX8(80, AverageThreeBytes(70, 80, 90));
    TEST_ASSERT_DOUBLE_WITHIN(1.234, 1.234, 1);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_1_Degres);

    return UNITY_END();
}
