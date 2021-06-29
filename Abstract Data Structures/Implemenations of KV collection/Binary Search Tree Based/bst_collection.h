// Author: Hunter Ward

#ifndef BST_COLLECTION_H
#define BST_COLLECTION_H

#include <vector>
#include "collection.h"

template <typename K, typename V>
class BSTCollection : public Collection <K,V>
{
public :
  // create an empty linked list
  BSTCollection();

  // copy a linked list
  BSTCollection(const BSTCollection<K,V>& rhs);

  // assign a linked list
  BSTCollection <K,V>& operator=(const BSTCollection<K,V>& rhs);

  // delete a linked list
  ~BSTCollection();

  // insert a key - value pair into the collection
  void insert (const K& key, const V& val);

  // remove a key - value pair from the collection
  void remove(const K& key);

  // find the value associated with the key
  bool find(const K& key, V& val) const;

  // find the keys associated with the range
  void find(const K& k1, const K& k2, std::vector <K>& keys) const;

  // return all keys in the collection
  void keys(std::vector <K>& keys ) const;

  // return collection keys in sorted order
  void sort (std::vector <K>& keys ) const;

  // return the number of keys in collection
  int size() const;

  // return the height of the tree
  int height() const;
  
private :

  // binary search tree node structure
  struct Node {
    K key;
    V value;
    Node* left;
    Node* right;
  };

  // root node of the search tree
  Node* root;

  // number of k-v pairs in the collection
  int collection_size;

  // helper to recursively empty search tree
  void make_empty (Node* subtree_root);

  // helper to recursively build sorted list of keys
  void inorder(const Node* subtree, std::vector <K>& keys) const;

  // helper to recursively build sorted list of keys
  void preorder(const Node* subtree, std::vector <K>& keys) const;

  // helper to recursively find range of keys
  void range_search(const Node* subtree, const K& k1, const K& k2,
  std::vector <K>& keys) const;

  // return the height of the tree rooted at subtree_root
  int height(const Node* subtree_root) const;

};

template<typename K, typename V>
BSTCollection <K,V>::BSTCollection() :
  collection_size (0), root (nullptr)
{ }

template<typename K, typename V>
void BSTCollection <K,V>:: make_empty (Node* subtree_root)
{
  if (subtree_root == nullptr)
    return;
  make_empty(subtree_root->left);
  delete subtree_root;
  make_empty(subtree_root->right);
  collection_size = 0;
  root = nullptr;
}

template<typename K, typename V>
BSTCollection <K,V>::~BSTCollection()
{
  make_empty(root);
}

template<typename K, typename V>
BSTCollection <K,V>::BSTCollection (const BSTCollection <K,V>& rhs)
: collection_size(0), root(nullptr)
{
  *this = rhs;
}

template<typename K, typename V>
BSTCollection <K,V>& BSTCollection <K,V>::operator=(const BSTCollection <K,V>&rhs)
{
  if (this == &rhs)
    return *this;
  // delete current
  make_empty(root);
  // build tree
  std::vector <K> ks;
  preorder(rhs.root, ks);
  for (int i = 0; i < ks.size(); i++)
  {
    V tmp;
    find(ks[i], tmp);
    insert(ks[i], tmp);
  }
  return *this;
}

template<typename K, typename V>
void BSTCollection <K,V>::insert(const K& key, const V& val)
{
  Node* ptr = new Node;
  ptr->key = key;
  ptr->value = val;
  ptr->left = nullptr;
  ptr->right = nullptr;
  
  if (collection_size == 0)
    root = ptr;
  else
  { // need to find where it should go
    Node* tmp = root;
    while (tmp != nullptr)
    {
      if (tmp->key > key and tmp->left == nullptr)
      { // value being inserted is less than current key
        tmp->left = ptr;
        tmp = nullptr;
      }
      else if (tmp->key < key and tmp->right == nullptr)
      { // value being inserting is greater than current key
        tmp->right = ptr;
        tmp = nullptr;
      }
      else if (tmp->key > key and tmp->left != nullptr)
        tmp = tmp->left;
      else if (tmp->key < key and tmp->right != nullptr)
        tmp = tmp->right;
    }
  }
  collection_size++;
}

template<typename K, typename V>
void BSTCollection <K,V>::remove(const K& key)
{
  // ... Leave empty for now ...
  // ... SAVE FOR HW 9 ...
}

template<typename K, typename V>
bool BSTCollection<K,V>::find(const K& key, V& val) const
{
  Node* ptr = root;
  if (collection_size == 0)
    return false;
  while (ptr != nullptr)
  {
    if (ptr->key == key)
    {
      val = ptr->value;
      return true; // found
    }
    else if (ptr->key > key)
      ptr = ptr->left;
    else if (ptr->key < key)
      ptr = ptr->right;
  }
  return false;
}

template<typename K, typename V> void
BSTCollection <K,V>::
range_search(const Node* subtree, const K& k1, const K& k2, std::vector <K>& ks) const
{
  if (subtree == nullptr)
    return;
  if (subtree->key >= k1 and subtree->key <= k2)
    ks.push_back(subtree->key);
  range_search(subtree->left, k1, k2, ks);
  range_search(subtree->right, k1, k2, ks);
}

template<typename K, typename V> void
BSTCollection <K,V>::find(const K& k1, const K& k2, std::vector <K>& ks) const
{
  // defer to the range search ( recursive ) helper function
  range_search(root, k1, k2, ks);
}

template<typename K, typename V>
void BSTCollection <K,V>::inorder(const Node* subtree, std::vector <K>& ks) const
{
  if (subtree == nullptr)
    return; // base case
  inorder(subtree->left, ks);
  ks.push_back(subtree->key);
  inorder(subtree->right, ks);
}

template<typename K, typename V>
void BSTCollection <K,V>::preorder(const Node* subtree, std::vector <K>& ks) const
{
  if (subtree == nullptr)
    return; // base case
  ks.push_back(subtree->key);
  preorder(subtree->left, ks);
  preorder(subtree->right, ks);
}

template<typename K , typename V >
void BSTCollection <K,V>::keys(std::vector <K>& ks) const
{
  // defer to the inorder ( recursive ) helper function
  inorder(root, ks);
}

template<typename K, typename V>
void BSTCollection <K,V>::sort(std::vector <K>& ks) const
{
  // defer to the inorder ( recursive ) helper function
  inorder(root, ks);
}

template<typename K, typename V>
int BSTCollection <K,V>::size () const
{
  return collection_size;
}

template<typename K, typename V>
int BSTCollection <K,V>::height(const Node* subtree_root) const
{
  if (subtree_root == nullptr)
    return 0; // at a leaf, return
  int track_left = 0, track_right = 0;
  track_left = height(subtree_root->left); // go left and track height
  track_right = height(subtree_root->right); // go right and track height
  return track_left > track_right ? track_left + 1 : track_right + 1;
  // return the largest value
}

template<typename K, typename V>
int BSTCollection<K,V>::height() const
{
  // defer to the height ( recursive ) helper function
  return height(root);
}

#endif
