#include <unity.h>

#include <Angle.h>


void test_DMS_(void)
{
    //TEST_ASSERT_EQUAL_HEX8(80, AverageThreeBytes(70, 80, 90));
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_1_DMS);

    return UNITY_END();
}
