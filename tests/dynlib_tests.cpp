#include <gtest/gtest.h>
#include <DynLibrary.h>
#include <iostream>

TEST(dynamicLoading, LoadDynamicLibrary)
{
    DynLibrary lib;
    auto res = lib.Open("tests/test-datas/dummy", DynLibrary::DefaultDecorator);
    if (res) std::cerr << res.value() << std::endl;
    EXPECT_FALSE(res.has_value());
}

TEST(dynamicLoading, CallDynamicFunction)
{
    DynLibrary lib("tests/test-datas/dummy");
    auto func = lib.GetFunction<int(int, int)>("Addition");

    EXPECT_EQ(func(1, 3), 4);
}

TEST(dynamicLoading, GetDynamicVariable)
{
    DynLibrary lib("tests/test-datas/dummy");
    double pi = lib.GetVariable<double>("pi_value");

    EXPECT_DOUBLE_EQ(pi, 3.14159);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
