#include "../db/list.h"
#include <gtest/gtest.h>

List listObj;
TEST(ListTests, LpopNill){
    ASSERT_EQ("nil", listObj.l_pop("key1"));
}

TEST(ListTests, LlenZero){
    ASSERT_EQ(0, listObj.l_len("key1"));
}

TEST(ListTests, LrangeEmptyList){
    ASSERT_EQ("empty list or set", listObj.l_range("key1", 0, 2));
}

TEST(ListTests, Lpush) { 
    ASSERT_EQ(1, listObj.l_push("key1", "value1"));
    ASSERT_EQ(2, listObj.l_push("key1", "value2"));
}

TEST(ListTests, LrangeList){
    ASSERT_EQ("value2\nvalue1", listObj.l_range("key1", 0, 1));
    ASSERT_EQ("value2\nvalue1", listObj.l_range("key1", 0, 2));
    ASSERT_EQ("value2\nvalue1", listObj.l_range("key1", 0, -1));
    ASSERT_EQ("empty list or set", listObj.l_range("key1", 0, -3));
    ASSERT_EQ("value2", listObj.l_range("key1", 0, 0));
}

TEST(ListTests, LlenValue){
    ASSERT_EQ(2, listObj.l_len("key1"));
}

TEST(ListTests, LpopValue){
    ASSERT_EQ("value2", listObj.l_pop("key1"));
    ASSERT_EQ("value1", listObj.l_pop("key1"));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}