// Author: Hunter Ward
// File name: hw5_tests.cpp
// description: This is a file that tests the 
// binsearch_collection class and ensure it works.

#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include "binsearch_collection.h"
#include <cstdlib>
#include <ctime>

using namespace std;

// Test 1
TEST(BasicCollectionTest, CorrectSize) {
  BinSearchCollection<string,double> c;
  ASSERT_EQ(c.size(), 0);
  c.insert("a", 10.0);
  ASSERT_EQ(c.size(), 1);
  c.insert("b", 20.0);
  ASSERT_EQ(c.size(), 2);
}

// Test 2
TEST(BasicCollectionTest, InsertAndFind) {
  BinSearchCollection<string,double> c;
  double v;
  ASSERT_EQ(c.find("a", v), false);
  v = 10.0;
  c.insert("a", v);
  ASSERT_EQ(c.find("a",v), true);
  ASSERT_EQ(v, 10.0);
  ASSERT_EQ(c.find("b", v), false);
  v = 20.0;
  c.insert("b", v);
  ASSERT_EQ(c.find("b", v), true);
  ASSERT_EQ(v, 20.0);

  // tests for special cases in find

  BinSearchCollection<string,double> d;
  d.insert("z", 10.0);
  d.insert("a", 20.0);
  d.insert("x", 25.0);
  d.insert("h", 4.0);
  ASSERT_EQ(d.find("z", v), true);

  // test identical keys
  BinSearchCollection<string,double> e;
  e.insert("a", 10.0);
  e.insert("a", 50.0);
  e.insert("a", 15.0);
  double x;
  ASSERT_EQ(e.find("a", x), true);
  ASSERT_EQ((x), 50.0); // should default to first
  // found value, which will be 50.0, since it is inserted
  // in the middle.
  // It should be noted that the program is never intended
  // to be used with duplicate keys.

  // test a lot of keys
  BinSearchCollection<int,double> r;
  int test[250];
  for(int i = 0; i < 250; i++)
  {
    int y = rand() % 5000 + 1; 
    r.insert(y, 5.0);
    test[i] = y;
  }
  double z;
  for(int i = 0; i < 250; i++)
  {
    ASSERT_EQ(r.find(test[i],z), true);
  }
  // now that a bunch are inserted, add a boundary
  // to 
  r.insert(0, z);
  r.insert(5005, z);
  r.insert(2500, z);


  // test boundaries
  // find the first, last, middle,
  // and worst case element
  ASSERT_EQ(r.find(0, z), true);
  ASSERT_EQ(r.find(5005, z), true);
  ASSERT_EQ(r.find(2500, z), true);
  // test element out of key range
  ASSERT_EQ(r.find(5030, z), false);
}

// Test 3
TEST(BasicCollectionTest, RemoveElems) {
  BinSearchCollection<string,double> c;
  c.insert("a", 10.0);
  c.insert("b", 20.0);
  c.insert("c", 30.0);
  double v;
  c.remove("a");
  ASSERT_EQ(c.find("a", v), false);
  c.remove("b");
  ASSERT_EQ(c.find("b", v), false);
  c.remove("c");
  ASSERT_EQ(c.find("c", v), false);
  ASSERT_EQ(c.size(), 0);
  
  //test with empty list
  BinSearchCollection<string,double> d;
  double b;
  ASSERT_EQ(d.find("a", b), false);
  ASSERT_EQ(d.find("b", b), false);
  ASSERT_EQ(d.find("c", b), false);
  ASSERT_EQ(d.size(), 0);
}

// Test 4
TEST(BasicCollectionTest, GetKeys) {
  BinSearchCollection<string,double> c;
  c.insert("a", 10.0);
  c.insert("b", 20.0);
  c.insert("c", 30.0);
  vector<string> ks;
  c.keys(ks);
  vector<string>::iterator iter;
  iter = find(ks.begin(), ks.end(), "a");
  ASSERT_NE(iter, ks.end());
  iter = find(ks.begin(), ks.end(), "b");
  ASSERT_NE(iter, ks.end());
  iter = find(ks.begin(), ks.end(), "c");
  ASSERT_NE(iter, ks.end());
  iter = find(ks.begin(), ks.end(), "d");
  ASSERT_EQ(iter, ks.end());
}

// Test 5
TEST(BasicCollectionTest, GetKeyRange) {
  BinSearchCollection<string,double> c;
  c.insert("a", 10.0);
  c.insert("b", 20.0);
  c.insert("c", 30.0);
  c.insert("d", 40.0);
  c.insert("e", 50.0);
  vector<string> ks;
  c.find("b", "d", ks);
  vector<string>::iterator iter;
  iter = find(ks.begin(), ks.end(), "b");
  ASSERT_NE(iter, ks.end());
  iter = find(ks.begin(), ks.end(), "c");
  ASSERT_NE(iter, ks.end());
  iter = find(ks.begin(), ks.end(), "d");
  ASSERT_NE(iter, ks.end());
  iter = find(ks.begin(), ks.end(), "a");
  ASSERT_EQ(iter, ks.end());
  iter = find(ks.begin(), ks.end(), "e");
  ASSERT_EQ(iter, ks.end());
  
  // test if empty list
  BinSearchCollection<string,double> d;
  vector<string> keys;
  d.find("b", "d", keys);
  vector<string>::iterator itr;
  itr = find(keys.begin(), keys.end(), "b");
  ASSERT_EQ(itr, keys.end());
  itr = find(keys.begin(), keys.end(), "c");
  ASSERT_EQ(itr, keys.end());
  itr = find(keys.begin(), keys.end(), "d");
  ASSERT_EQ(itr, keys.end());
  itr = find(keys.begin(), keys.end(), "a");
  ASSERT_EQ(itr, keys.end());
  itr = find(keys.begin(), keys.end(), "e");
  ASSERT_EQ(itr, keys.end());
  // because the iterator won't change, it should == ks.end() always.
}

// Test 6
TEST(BasicCollectionTest, KeySort) {
  BinSearchCollection<string,double> c;
  c.insert("a", 10.0);
  c.insert("e", 50.0);
  c.insert("c", 30.0);
  c.insert("b", 20.0);
  c.insert("d", 40.0);
  vector<string> sorted_ks;
  c.sort(sorted_ks);
  // check if sorted order
  for (int i = 0; i < int(sorted_ks.size()) - 1; ++i)
  {
    ASSERT_LE(sorted_ks[i], sorted_ks[i+1]);
  }
  
  // test identical keys
  BinSearchCollection<string,double> d;
  d.insert("a", 10.0);
  d.insert("a", 50.0);
  d.insert("c", 30.0);
  d.insert("b", 20.0);
  d.insert("d", 40.0);
  vector<string> sorted_keys;
  d.sort(sorted_keys);
  // check if sort order
  for (int i = 0; i < int(sorted_keys.size()) - 1; ++i)
  {
    ASSERT_LE(sorted_keys[i], sorted_keys[i+1]);
  }

}

// Test sort when empty
TEST(BasicCollectionTest, KeySort_Empty) {
  BinSearchCollection<string,double> c;
  vector<string> sorted;
  c.sort(sorted);
  ASSERT_EQ(c.size(), 0);
  // this test is to see if there are any seg faults
}

int main(int argc, char** argv)
{
  srand(time(0));
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
