#ifndef COLLECTION_H
#define COLLECTION_H

#include <vector>

using namespace std;

template<typename K, typename V>
class Collection
{
public:

	// insert a key-value pair into the collection
	virtual void insert(const K& key, const V& val) = 0;

	// remove a key-value pair from the collection
	virtual void remove(const K& key) = 0;

	// find and return the value associated with the key
	virtual bool find(const K& key, V& val) const = 0;

	// find and return the list of keys >= k1 and <= k2
	virtual void find(const K& k1, const K& k2, vector<K>& keys) const = 0;

	// return all of the keys in the collection
	virtual void keys(vector<K>& keys) const = 0;

	// return all of the keys in ascending (sorted) order
	virtual void sort(vector<K>& keys) const = 0;

	// return the number of keys in the collection
	virtual int size() const = 0;

};

#endif