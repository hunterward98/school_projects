// Author: Hunter Ward
// File: binsearch_collection.h
// This file is an implementation of a
// key-value pair collection that utilizes binary search

#ifndef BINSEARCH_COLLECTION_H
#define BINSEARCH_COLLECTION_H

#include <vector>
#include <algorithm>
#include "collection.h"


template<typename K, typename V>
class BinSearchCollection : public Collection<K, V>
{
public:
  
  // insert a key-value pair into the collection
  void insert(const K& key, const V& val);
  
  // remove a key-value pair from the collection
  void remove(const K& key);
  
  // find the key and return true if found
  bool find(const K& key, V& val) const;
  
  // find the keys associated with the range
  void find(const K& k1, const K& k2, std::vector<K>& keys) const;
  
  // return all keys in the collection
  void keys(std::vector<K>& keys) const;
  
  // return collection keys in sorted order
  void sort(std::vector<K>& keys) const;
  
  // return the number of keys in the collection
  int size() const;
  
private:
  
  // helper function for binary search
  bool binsearch(const K& key, int& index) const;
  
  // vector storage
  std::vector<std::pair<K,V>> kv_list;
};

// helper function for binary search
// index is only to be used returning
template<typename K, typename V>
bool BinSearchCollection<K,V>::binsearch(const K& key, int& index) const
{
  bool isFound = false;
  int top = kv_list.size() - 1;
  int bottom = 0;
  int mid = kv_list.size() / 2;
  index = -1; // if this value is returned, then it was not found
  
  if (kv_list.size() == 0)
  {
    index = 0;
    return false;
  }
  
  while (top >= bottom)
  {
    mid = (top + bottom) / 2;
    
    if (kv_list[mid].first == key)
    {
      index = mid;
      return true;
    }
    else if (kv_list[mid].first > key)
    {
      top = mid - 1;
    }
    else
    {
      bottom = mid + 1;
    }
  }
  index = mid;
  return false;
}

// insert a key-value pair into the collection
template<typename K, typename V>
void BinSearchCollection<K,V>::insert(const K& key, const V& val)
{
  int index = 0;
  bool tmp = binsearch(key, index);
  std::pair<K,V> p;
  p.first = key;
  p.second = val;
  
  if (kv_list.size() == 0)
  {
    kv_list.insert(kv_list.begin(), p);
  }
  else if (key > kv_list[index].first)
  {
    kv_list.insert(kv_list.begin() + index + 1, p);
  }
  else if (key <= kv_list[index].first)
  {
    kv_list.insert(kv_list.begin() + index, p);
  }
}

// remove a key-value pair from the collection
template<typename K, typename V>
void BinSearchCollection<K,V>::remove(const K& key)
{
  int index = 0;
  if (binsearch(key, index))
  {
    kv_list.erase(kv_list.begin() + index);
  }
}

// find value associated with the key
template<typename K, typename V>
bool BinSearchCollection<K,V>::find(const K& key, V& val) const
{
  int index = 0;
  bool found = binsearch(key, index);
  if (found)
  {
    val = kv_list[index].second;
  }
  return found;
}

// find the keys associated with the range
template<typename K, typename V>
void BinSearchCollection<K,V>::
find(const K& k1, const K& k2, std::vector<K>& keys) const
{
  for (int i = 0; i < kv_list.size(); i++)
  {
    if (kv_list[i].first >= k1 and kv_list[i].first <= k2)
      keys.push_back(kv_list[i].first);
  }
}

// return all the keys in the collection
template<typename K, typename V>
void BinSearchCollection<K,V>::keys(std::vector<K>& keys) const
{
  for (int i = 0; i < kv_list.size(); i++)
  {
    keys.push_back(kv_list[i].first);
  }
}

// return collection keys in sorted order
template<typename K, typename V>
void BinSearchCollection<K,V>::sort(std::vector<K>& keys) const
{
  for (int i = 0; i < kv_list.size(); i++)
  {
    keys.push_back(kv_list[i].first);
  }
}

// return number of keys in collection
template<typename K, typename V>
int BinSearchCollection<K,V>::size() const
{
  return kv_list.size();
}

#endif
