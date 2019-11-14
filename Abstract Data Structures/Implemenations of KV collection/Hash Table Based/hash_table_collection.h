// Author: Hunter Ward
// File name: hash_table_collection.h
// this file is an implementation of a KV list
// with a hash table as the underlying data structure
#ifndef HASH_TABLE_COLLECTION_H
#define HASH_TABLE_COLLECTION_H

#include <vector>
#include <algorithm>
#include <functional>
#include "collection.h"

using namespace std;

template<typename K, typename V>
class HashTableCollection : public Collection<K,V>
{
public:

  // create an empty linked list
  HashTableCollection();
  
  // copy a linked list
  HashTableCollection(const HashTableCollection<K,V>& rhs);
  
  // assign a linked list
  HashTableCollection<K,V>& operator=(const HashTableCollection<K,V>& rhs);
  
  // delete a linked list
  ~HashTableCollection();
  
  // insert a key-value pair into the collection
  void insert(const K& key, const V& val);
  
  // remove a key-value pair from the collection
  void remove(const K& key);
  
  // find the value associated with the key
  bool find(const K& key, V& val) const;
  
  // find the keys associated with the range
  void find(const K& k1, const K& k2, std::vector<K>& keys) const;
  
  // return all keys in the collection
  void keys(std::vector<K>& keys) const;
  
  // return collection keys in sorted order
  void sort(std::vector<K>& keys) const;
  
  // return the number of keys in collection
  int size() const;
  
private:

  // helper to empty entire hash table
  void make_empty();
  
  // resize and rehash the hash table
  void resize_and_rehash();
  
  // linked list node structure
  struct Node {
    K key;
    V value;
    Node* next;
  };
  
  // number of k-v pairs in the collection
  int collection_size;
  
  // number of hash table buckets (default is 16)
  int table_capacity;
  
  // hash table array load factor (set at 75% before resizing)
  const double load_factor_threshold = 0.75;
  
  // hash table array
  Node** hash_table;
};


template<typename K, typename V>
HashTableCollection<K,V>::HashTableCollection() :
  collection_size(0), table_capacity(16)
{
  // dynamically allocate the hash table array
  hash_table = new Node*[table_capacity];
  // initialize the hash table chains
  for (int i = 0; i < table_capacity; i++)
    hash_table[i] = nullptr;
}

template<typename K, typename V>
void HashTableCollection<K,V>::make_empty()
{
  if (hash_table == nullptr)
    return;
  Node* ptr;
  Node* tmp;
  for (int i = 0; i < table_capacity; i++)
  {
    ptr = hash_table[i];
    while (ptr != nullptr)
    {
      tmp = ptr->next;
      delete ptr;
      ptr = tmp;
      collection_size--;
    }
  }
  ptr = nullptr;
  tmp = nullptr;
  delete hash_table;
  hash_table = nullptr;
}


template<typename K, typename V>
HashTableCollection<K,V>::~HashTableCollection()
{
  make_empty();
}


template<typename K, typename V>
HashTableCollection<K,V>::HashTableCollection(const HashTableCollection<K,V>& rhs)
  : hash_table(nullptr)
{
  *this = rhs;
}


template<typename K, typename V>
HashTableCollection<K,V>&
HashTableCollection<K,V>::operator=(const HashTableCollection<K,V>& rhs)
{
  if (this == &rhs)
    return *this;

  make_empty();
  
  //initialize new object
  table_capacity = rhs.table_capacity;
  collection_size = 0;
  hash_table = new Node*[table_capacity];
  for (int i = 0; i < table_capacity; i++)
    hash_table[i] = nullptr;
  
  Node* rhsptr;
  for (int i = 0; i < table_capacity; i++)
  {
    rhsptr = rhs.hash_table[i];
    while (rhsptr != nullptr)
    {
      // insert new keys
      insert(rhsptr->key, rhsptr->value);
      rhsptr = rhsptr->next;
    }
  }
  return *this;
}


template<typename K, typename V>
void HashTableCollection<K,V>::resize_and_rehash()
{
  
  std::hash<K> hash_fun;
  size_t temp;
  size_t index;
  int new_capacity = table_capacity*2;
  int new_size = collection_size;
  V val;
  
  Node** new_table = new Node*[new_capacity];
  
  for (int i = 0; i < new_capacity; i++)
  {
    new_table[i] = nullptr;
  }
  
  std::vector<K> ks;
  keys(ks); // get keys in current object
  
  for (int i = 0; i < ks.size(); i++)
  {
    temp = hash_fun(ks[i]);
    index = temp % new_capacity;
    // rehash
    find(ks[i],val);
    Node* new_node = new Node;
    new_node->key = ks[i];
    new_node->value = val;
    new_node->next = new_table[index];
    new_table[index] = new_node;
  }
  
  make_empty();
  
  hash_table = new_table;
  table_capacity = new_capacity;
  collection_size = new_size;
}


template<typename K, typename V>
void HashTableCollection<K,V>::insert(const K& key, const V& val)
{
  if ((double(collection_size)/double(table_capacity)) >= load_factor_threshold)
  {
    resize_and_rehash();
  }
  
  std::hash<K> hash_fun;
  size_t temp = hash_fun(key);
  size_t index = temp % table_capacity;
  
  // create Node and set values
  Node* ptr = new Node;
  ptr->value = val;
  ptr->key = key;
  ptr->next = nullptr;
  // check if it needs to be chained or not
  if (hash_table[index] == nullptr)
    hash_table[index] = ptr;
  else
  {
    Node* tmp = hash_table[index];
    while (tmp->next != nullptr)
    {
      tmp = tmp->next;
    }
    tmp->next = ptr;
  }
  collection_size++;
}


template<typename K, typename V>
void HashTableCollection<K,V>::remove(const K& key)
{
  V tmp;
  std::hash<K> hash_fun;
  size_t temp = hash_fun(key);
  size_t index = temp % table_capacity;
  if (collection_size == 0)
    return;
  else if (!find(key, tmp))
    return;
  else if (hash_table[index]->next == nullptr) // one element list, no loop needed
  {
    delete hash_table[index];
    hash_table[index] = nullptr;
    collection_size--;
  }
  else
  {
    Node* ptr;
    Node* prev;
    ptr = hash_table[index];
    while (ptr != nullptr) 
    {
      if (hash_table[index]->key == key and hash_table[index]->next != nullptr) 
      { // front of list
        hash_table[index] = hash_table[index]->next;
        ptr->next = nullptr;
        delete ptr;
        ptr = nullptr;
        collection_size--;
      }
      else if (ptr->key == key and ptr->next != nullptr) 
      { // middle of list (ptr iterated)
        prev = hash_table[index];
        while (prev->next != ptr) {
          prev = prev->next;
        }
        if (prev == hash_table[index]) 
        {
          hash_table[index]->next = ptr->next;
          ptr->next = nullptr;
          delete ptr;
          ptr = nullptr;
          collection_size--;
        } 
        else 
        {
          prev->next = ptr->next;
          ptr->next = nullptr;
          delete ptr;
          ptr = nullptr;
          collection_size--;
        }
      } 
      else if (ptr->key == key and ptr->next == nullptr) 
      {
        prev = hash_table[index];
        while (prev->next != ptr) 
        {
          prev = prev->next;
        }
        prev->next = nullptr;
        delete ptr;
        ptr = nullptr;
        collection_size--;
      } 
      else 
      {
        ptr = ptr->next;
      }
    }
  }
}


template<typename K, typename V>
bool HashTableCollection<K,V>::find(const K& key, V& val) const
{
  std::hash<K> hash_fun;
  size_t temp = hash_fun(key);
  size_t index = temp % table_capacity;
  if (hash_table[index] == nullptr)
  {
    return false;
  }
  Node* tmp = hash_table[index];
  while (tmp != nullptr)
  {
    if (tmp->key == key)
    {
      val = tmp->value;
      return true;
    }
    tmp = tmp->next;
  }
  return false;
}


template<typename K, typename V>
void HashTableCollection<K,V>::find(const K& k1, const K& k2, std::vector<K>& keys) const
{
  for (int i = 0; i < table_capacity; i++)
  {
    if (hash_table[i] != nullptr)
    {
      Node* tmp = hash_table[i];
      while (tmp != nullptr)
      {
        if (tmp->key >= k1 and tmp->key <= k2)
        {
          keys.push_back(tmp->key);
        }
        tmp = tmp->next;
      }
    }
  }
}

// returns all the keys in the collection into the vector that was passed in
template<typename K, typename V>
void HashTableCollection<K,V>::keys(std::vector<K>& keys) const
{
  for (int i = 0; i < table_capacity; i++)
  {
    Node* tmp = hash_table[i];
    while (tmp != nullptr)
    {
        keys.push_back(tmp->key);
        tmp = tmp->next;
    }
  }
}


template<typename K, typename V>
void HashTableCollection<K,V>::sort(std::vector<K>& ks) const
{
  keys(ks);
  std::sort(ks.begin(), ks.end());
}


template<typename K, typename V>
int HashTableCollection<K,V>::size() const
{
  return collection_size;
}


#endif
