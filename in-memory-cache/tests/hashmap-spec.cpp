#include "../db/hashmap.h"
#include <gtest/gtest.h>

Hashmap hashmapObj;
TEST(HashMapTests, HgetNill){
    ASSERT_EQ("nil", hashmapObj.h_get("key1", "field1"));
}

TEST(HashMapTests, HgetAllNill){
    ASSERT_EQ("empty list or set", hashmapObj.h_get_all("key1"));
}

TEST(HashMapTests, Hset) { 
    ASSERT_EQ(1, hashmapObj.h_set("key1", "field1", "value1"));
    ASSERT_EQ(1, hashmapObj.h_set("key1", "field2", "value1"));
    ASSERT_EQ("field1\nvalue1\nfield2\nvalue1", hashmapObj.h_get_all("key1"));
}

TEST(HashMapTests, HgetAll){
    ASSERT_EQ("field1\nvalue1\nfield2\nvalue1", hashmapObj.h_get_all("key1"));
}

TEST(HashMapTests, HgetValue){
    ASSERT_EQ("value1", hashmapObj.h_get("key1", "field1"));
    ASSERT_EQ("value1", hashmapObj.h_get("key1", "field2"));   
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}