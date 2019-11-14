#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include "bst_collection.h"
#include "algorithm"

using namespace std;

// Test 1
TEST (BasicCollectionTest, CorrectSizeAndHeight) {
  BSTCollection <string, double> c ;
  ASSERT_EQ(c.size(), 0);
  c.insert ("c", 10.0);
  ASSERT_EQ(c.size(), 1);
  c.insert ("a", 20.0);
  ASSERT_EQ(c.size(), 2);
  
  // test height
  c.insert("e", 50.0);
  c.insert("b", 30.0);
  c.insert("d", 40.0);
  c.insert("g", 10.0);
  c.insert("f", 15.0);
  ASSERT_EQ(c.height(), 4);
}

// Test 2
TEST (BasicCollectionTest, InsertAndFind) {
  BSTCollection <string, double> c;
  double v;
  ASSERT_EQ (c.find("a", v), false);
  c.insert("a", 10.0);
  ASSERT_EQ (c.find("a", v), true);
  ASSERT_EQ (v , 10.0);
  ASSERT_EQ (c.find ("b", v), false);
  c.insert ("b", 20.0);
  ASSERT_EQ (c.find("b", v), true);
  ASSERT_EQ (v, 20.0);
}
 
// Test 3 -- THIS TEST SHOULD FAIL FOR HW8
TEST (BasicCollectionTest, RemoveElems) {
  BSTCollection <string, double> c;
  c.insert("a", 10.0);
  c.insert ("b", 20.0);
  c.insert ("c", 30.0);
  double v;
  c.remove ("a");
  ASSERT_EQ (c.find("a", v), false);
  ASSERT_EQ (c.find("b", v), true);
  ASSERT_EQ (c.find("c", v), true);
  c.remove ("b");
  ASSERT_EQ (c.find("b", v), false);
  ASSERT_EQ (c.find("c", v), true);
  c.remove ("c");
  ASSERT_EQ (c.find("c", v), false);
  ASSERT_EQ (c.size(), 0);
}

// Test 4
TEST (BasicCollectionTest, GetKeys) {
  BSTCollection <string, double> c ;
  c.insert("e", 10.0);
  c.insert("b", 20.0);
  c.insert("d", 30.0);
  c.insert("a", 30.0);
  c.insert("i", 30.0);
  c.insert("g", 30.0);
  c.insert("f", 30.0);
  c.insert("k", 30.0);
  vector <string> ks;
  c.keys(ks);
  vector <string>:: iterator iter;
  iter = find(ks.begin(), ks.end(), "a");
  ASSERT_NE(iter, ks.end());
  iter = find(ks.begin(), ks.end(), "b");
  ASSERT_NE(iter, ks.end());
  iter = find(ks.begin(), ks.end(), "g");
  ASSERT_NE(iter, ks.end());
  iter = find(ks.begin(), ks.end(), "m");
  ASSERT_EQ(iter, ks.end());
  for (int i = 0; i < ks.size() -1; i++)
  {
    ASSERT_LE(ks[i], ks[i+1]);
    // make sure keys are sorted order
  }
}

// Test 5
TEST (BasicCollectionTest, GetKeyRange) {
  BSTCollection <string, double> c ;
  c.insert("a", 10.0);
  c.insert("b", 20.0);
  c.insert("c", 30.0);
  c.insert("d", 40.0);
  c.insert("e", 50.0);
  vector <string> ks;
  c.find ("b", "d", ks);
  vector <string>::iterator iter;
  iter = find(ks.begin(), ks.end(), "b");
  ASSERT_NE (iter, ks.end());
  iter = find(ks.begin(), ks.end(), "c");
  ASSERT_NE (iter, ks.end());
  iter = find (ks.begin(), ks.end(), "d");
  ASSERT_NE (iter, ks.end());
  iter = find (ks.begin(), ks.end(), "a");
  ASSERT_EQ (iter, ks.end());
  iter = find (ks.begin(), ks.end(), "e");
  ASSERT_EQ (iter, ks.end());
}

// Test 6
TEST (BasicCollectionTest, KeySort) {
  BSTCollection <string, double> c;
  c.insert("a", 10.0);
  c.insert("e", 50.0);
  c.insert("c", 30.0);
  c.insert("b", 20.0);
  c.insert("d", 40.0);
  vector <string> sorted_ks;
  c.sort(sorted_ks);
  ASSERT_EQ (sorted_ks.size(), c.size());
  for (int i = 0; i < int ( sorted_ks.size()) - 1; ++i) {
    ASSERT_LE(sorted_ks[i], sorted_ks[i +1]);
  }
}

// Test 7
TEST (TreeCollectionTest, AssignOpTest) {
  BSTCollection <string,int> c1;
  c1.insert("c", 10);
  c1.insert("b", 15);
  c1.insert("d", 20);
  c1.insert("a", 20);
  BSTCollection <string, int> c2;
  c2 = c1;
  ASSERT_EQ(c1.size(), c2.size());
  ASSERT_EQ(c1.height(), c2.height());
}

int main (int argc, char** argv)
{
    testing::InitGoogleTest (&argc, argv);
    return RUN_ALL_TESTS();
}
