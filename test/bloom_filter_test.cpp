#include <bloom_filter.hpp>
#include <bloom_filter/basic.hpp>
#include <bloom_filter/counting.hpp>
#include <chrono>
#include <gtest/gtest.h>
#include <iostream>
TEST(BloomFilterTest, BasicBloomFilterTest) {
  bf::basic_bloom_filter b(bf::make_hasher(5), 1000000, 16);
  b.clear();
  b.add("foo");
  b.add(32);
  b.add("foo");
  b.add(33);
  b.add("foo");
  EXPECT_TRUE(b.lookup(32));
  EXPECT_TRUE(b.lookup(33));
  EXPECT_TRUE(b.lookup("foo"));
  EXPECT_FALSE(b.lookup("fo"));
}

TEST(BloomFilterTest, CountingBloomFilterTest) {
  bf::counting_bloom_filter b(bf::make_hasher(5), 1000000, 16);
  b.clear();
  b.add("foo");
  b.add(32);
  b.add("foo");
  b.add(33);
  b.add("foo");
  EXPECT_EQ(b.lookup(32), 1);
  EXPECT_EQ(b.lookup(33), 1);
  EXPECT_EQ(b.lookup("foo"), 3);
  EXPECT_EQ(b.lookup("fo"), 0);
}