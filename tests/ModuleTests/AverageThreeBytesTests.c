#include <unity.h>

#include <module.h>

void test_AverageThreeBytes_should_AverageMidRangeValues(void)
{
    TEST_ASSERT_EQUAL_HEX8(40, AverageThreeBytes(30, 40, 50));
    TEST_ASSERT_EQUAL_HEX8(40, AverageThreeBytes(10, 70, 40));
    TEST_ASSERT_EQUAL_HEX8(33, AverageThreeBytes(33, 33, 33));
}

void test_AverageThreeBytes_should_AverageHighValues(void)
{
    TEST_ASSERT_EQUAL_HEX8(80, AverageThreeBytes(70, 80, 90));
    TEST_ASSERT_EQUAL_HEX8(127, AverageThreeBytes(127, 127, 127));
    TEST_ASSERT_EQUAL_HEX8(84, AverageThreeBytes(0, 126, 126));
}

void test_AverageThreeBytes_0(void)
{
    TEST_ASSERT_EQUAL_HEX8(80, AverageThreeBytes(70, 80, 90));
}

void test_AverageThreeBytes_30(void)
{
    TEST_ASSERT_EQUAL_HEX8(80, AverageThreeBytes(70, 80, 90));
}

void test_AverageThreeBytes_45(void)
{
    TEST_ASSERT_EQUAL_HEX8(80, AverageThreeBytes(70, 80, 90));
}


int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_AverageThreeBytes_should_AverageMidRangeValues);
    RUN_TEST(test_AverageThreeBytes_should_AverageHighValues);
    RUN_TEST(test_AverageThreeBytes_0);
    RUN_TEST(test_AverageThreeBytes_30);
    RUN_TEST(test_AverageThreeBytes_45);

    return UNITY_END();
}
