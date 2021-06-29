// Author: Hunter Ward
// File name: hw7_tests.cpp
// description: This is a file that tests the 
// HashTable_collection class and ensure it works.

#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include "hash_table_collection.h"
#include <cstdlib>
#include <ctime>

using namespace std;

// Test 1
TEST(BasicCollectionTest, CorrectSize) {
  HashTableCollection<string,double> c;
  ASSERT_EQ(c.size(), 0);
  c.insert("a", 10.0);
  ASSERT_EQ(c.size(), 1);
  c.insert("b", 20.0);
  ASSERT_EQ(c.size(), 2);
  
}

// Test 2
TEST(BasicCollectionTest, InsertAndFind) {
  HashTableCollection<string,double> c;
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

  HashTableCollection<string,double> d;
  d.insert("z", 10.0);
  d.insert("a", 20.0);
  d.insert("x", 25.0);
  d.insert("h", 4.0);
  ASSERT_EQ(d.find("z", v), true);
  
  // insert a bunch more to resize it
  d.insert("d", 10.0);
  d.insert("e", 10.0);
  d.insert("f", 10.0);
  d.insert("g", 10.0);
  d.insert("i", 10.0);
  d.insert("j", 10.0);
  d.insert("k", 10.0);
  d.insert("l", 10.0);
  d.insert("m", 10.0);
  d.insert("n", 10.0);
  ASSERT_EQ(d.find("m", v), true);
  ASSERT_EQ(d.size(), 14);
  
  // test insert after rehashing a bunch
  HashTableCollection<int,int> f;
  for (int i = 0; i < 1024; i++)
    f.insert(i,i);
  ASSERT_EQ(f.size(), 1024);
}

// Test 3
TEST(BasicCollectionTest, RemoveElems) {
  HashTableCollection<int,double> c;
  // comments after insert indicate what
  // index they will be inserted at in hash table
  // these were found via "guess and check"
  c.insert(75, 10.0); // 11
  c.insert(2, 20.0); // 2
  c.insert(11, 30.0); // 11
  c.insert(31, 30.0); // 15
  c.insert(26, 30.0); // 10
  c.insert(91, 30.0); // 11
  c.insert(50, 30.0); // 2
  c.insert(991, 30.0); // 15
  c.insert(843, 30.0); // 11
  c.insert(431, 30.0); // 15
  // test with 10 elems, no need to test resize
  double v;
  c.remove(75);
  ASSERT_EQ(c.find(75, v), false);
  c.remove(2);
  ASSERT_EQ(c.find(2, v), false);
  c.remove(11);
  ASSERT_EQ(c.find(11, v), false);
  c.remove(31);
  ASSERT_EQ(c.find(31, v), false);
  c.remove(26);
  ASSERT_EQ(c.find(26, v), false);
  c.remove(91);
  ASSERT_EQ(c.find(91, v), false);
  c.remove(50);
  ASSERT_EQ(c.find(50, v), false);
  c.remove(991);
  ASSERT_EQ(c.find(991, v), false);
  c.remove(843);
  ASSERT_EQ(c.find(843, v), false);
  c.remove(431);
  ASSERT_EQ(c.find(431, v), false);
  c.remove(1023); // does not exist
  ASSERT_EQ(c.find(1023, v), false);
  ASSERT_EQ(c.size(), 0);
  
  
  // now test with resize
  for (int i = 0; i < 100; i++)
  {
    c.insert(i, 5.0);
  }
  
  for (int i = 0; i < 100; i++)
  {
    c.remove(i);
  }
  ASSERT_EQ(c.size(), 0);
  
  
  //test with empty list
  HashTableCollection<string,double> d;
  double b;
  ASSERT_EQ(d.find("a", b), false);
  ASSERT_EQ(d.find("b", b), false);
  ASSERT_EQ(d.find("c", b), false);
  ASSERT_EQ(d.size(), 0);
}


// Test 4
TEST(BasicCollectionTest, GetKeys) {
  HashTableCollection<string,double> c;
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
  HashTableCollection<string,double> c;
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
  HashTableCollection<string,double> d;
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
  HashTableCollection<string,double> c;
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
  // not intended to be used with identical keys
  HashTableCollection<string,double> d;
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
  HashTableCollection<string,double> c;
  vector<string> sorted;
  c.sort(sorted);
  ASSERT_EQ(c.size(), 0);
  // this test is to see if there are any seg faults
}

// test copy constructor and equals operator
TEST(BasicCollectionTest, Equivalency) {
  HashTableCollection<string, double> c;
  c.insert("a", 40.0);
  c.insert("b", 20.0);
  c.insert("c", 30.0);
  c.insert("d", 40.0);
  c.insert("e", 50.0);
  
  HashTableCollection<string, double> d;
  d = c;
  ASSERT_EQ(d.size(), c.size());
  vector<string> ks_c;
  vector<string> ks_d;
  d.keys(ks_d);
  c.keys(ks_c);
  for (int i = 0; i < ks_c.size(); i++)
    ASSERT_EQ(ks_c[i], ks_d[i]);
  
  HashTableCollection<string, double> e(d);
  ASSERT_EQ(d.size(), e.size());
  vector<string> ks_e;
  e.keys(ks_e);
  for (int i = 0; i < ks_e.size(); i++)
    ASSERT_EQ(ks_e[i], ks_d[i]);
}

int main(int argc, char** argv)
{
  srand(time(0));
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
