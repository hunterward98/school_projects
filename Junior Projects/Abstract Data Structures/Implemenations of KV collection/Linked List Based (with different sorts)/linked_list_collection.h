// Author: Hunter Ward
// File name: linked_list_collection.h

#ifndef LINKED_LIST_COLLECTION_H
#define LINKED_LIST_COLLECTION_H

#include <vector>
#include <algorithm>
#include "collection.h"


template<typename K, typename V>
class LinkedListCollection : public Collection<K, V>
{
public:

	// create an empty linked list
	LinkedListCollection();

	// copy a linked list
	LinkedListCollection(const LinkedListCollection<K, V>& rhs);

	// assign a linked list
	LinkedListCollection<K, V>& operator=(const LinkedListCollection<K, V>& rhs);

	// delete a linked list
	~LinkedListCollection();

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

	// different types of sorts for the list
	void insertion_sort();
	void merge_sort();
	void quick_sort();

private:
	// linked list node structure
	struct Node {
		K key;
		V value;
		Node* next;
	};
	Node* head;   // pointer to first list node
	Node* tail;   // pointer to last list node
	int length;   // number of linked list nodes in list

	Node* merge_sort(Node* left, int len); // helper for merge_sort

	Node* quick_sort(Node* start, int len); // helper for quick_sort

	void remove_nodes(Node* TBD); // deletes a node
};

// create empty linked list
template<typename K, typename V>
LinkedListCollection<K, V>::LinkedListCollection()
{
	length = 0;
	head = nullptr;
	tail = nullptr;
}

// copy a linked list
template<typename K, typename V>
LinkedListCollection<K, V>::LinkedListCollection(const LinkedListCollection<K, V>& rhs)
{
	length = 0;
	head = nullptr;
	tail = nullptr;
	*this = rhs;
}

// assign a linked list
template<typename K, typename V>
LinkedListCollection<K, V>& LinkedListCollection<K, V>::
operator=(const LinkedListCollection<K, V>& rhs)
{
	Node* ptr = rhs.head;
	if (this != &rhs)
	{
		remove_nodes(head);
		while (ptr != nullptr)
		{
			insert(ptr->key, ptr->value);
			ptr = ptr->next;
		}
	}
	return *this;
}

// removes a list given the address of the first Node
template<typename K, typename V>
void LinkedListCollection<K, V>::remove_nodes(Node* TBD)
{
	Node* ptr = TBD;
	Node* iter;

	while (length > 0)
	{
		iter = ptr->next;
		delete ptr;
		ptr = iter;
		length--;
	}
	head = nullptr;
	tail = nullptr;
}

// delete a linked list
template<typename K, typename V>
LinkedListCollection<K, V>::~LinkedListCollection()
{
	remove_nodes(head);
}

template <typename K, typename V>
void LinkedListCollection<K, V>::insertion_sort()
{
	if (length <= 1)
	{
		return;
	}
	else
	{
		Node* sort = head;
		Node* unsort = sort->next;
		Node* ptr = unsort;
		Node* tmp = head;
		for (int i = 0; i < size() - 1; i++)
		{
			ptr = unsort;
			if ((ptr->key > sort->key))
			{ // for when unsorted portion > sorted
				sort = sort->next;
				unsort = unsort->next;
			}

			else if ((ptr->key < sort->key) and (ptr->key < head->key))
			{ // unsort is the smallest element found
				unsort = unsort->next;
				sort->next = ptr->next;
				ptr->next = head;
				head = ptr;
			}

			else if ((ptr->key < sort->key) and (ptr->key > head->key))
			{ // need to insert in middle
				tmp = head;
				while ((tmp->key < ptr->key) and (tmp->next->key < ptr->key)
					and (tmp->next != sort))
				{
					tmp = tmp->next;
				}
				unsort = unsort->next;
				sort->next = ptr->next;
				ptr->next = tmp->next;
				tmp->next = ptr;
			}
		}
	}
}



template<typename K, typename V>
typename LinkedListCollection<K, V>::Node*
LinkedListCollection<K, V>::merge_sort(Node* left, int len)
{
	int mid = int(len / 2);

	Node* right = left;
	Node* end_left = left; // used for splitting

	if (len <= 1)
		return left;

	for (int i = 0; i < mid; i++)
	{
		right = right->next;
	}

	while (end_left->next != right)
	{
		end_left = end_left->next;
	}
	end_left->next = nullptr; // splits list

	left = merge_sort(left, mid);
	right = merge_sort(right, len - mid);

	if (left == nullptr)
		return right;
	if (right == nullptr)
		return left;

	// build new list
	Node* leftptr = left;
	Node* rightptr = right;
	Node* tmpptr;
	Node* temp = nullptr;

	if (leftptr->key < rightptr->key)
	{
		temp = leftptr;
		leftptr = leftptr->next;
	}
	else
	{
		temp = rightptr;
		rightptr = rightptr->next;
	}
	tmpptr = temp;

	while (leftptr != nullptr and rightptr != nullptr) //for inserting everything else into temp
	{
		if (leftptr->key < rightptr->key)
		{
			tmpptr->next = leftptr;
			leftptr = leftptr->next;
		}
		else
		{
			tmpptr->next = rightptr;
			rightptr = rightptr->next;
		}
		tmpptr = tmpptr->next;
	}
	if (leftptr == nullptr)
		tmpptr->next = rightptr;
	else if (rightptr == nullptr)
		tmpptr->next = leftptr;

	return temp;
}

template<typename K, typename V>
void LinkedListCollection<K, V>::merge_sort()
{
	head = merge_sort(head, length);
	tail = head;
	while (tail->next != nullptr)
		tail = tail->next;
}

template<typename K, typename V>
typename LinkedListCollection<K, V>::Node*
LinkedListCollection<K, V>::quick_sort(Node* start, int len)
{
	Node* tmp;
	Node* left = nullptr;
	Node* right = nullptr;
	if (len <= 1)
		return start;
	if (len == 2)
	{
		// swap if needed
		if (start->next->key < start->key)
		{
			tmp = start->next;
			tmp->next = start;
			start->next = nullptr;
			start = tmp;
			tmp = nullptr;
		}
		return start;
	}

	Node* pivot = start; // separate pivot
	start = start->next;
	pivot->next = nullptr;

	Node* leftptr;
	Node* rightptr;
	int length_left = 0, length_right = 0;

	while (start != nullptr)
	{
		if (start->key < pivot->key)
		{
			if (left == nullptr)
			{
				left = start;
				leftptr = left;
			}
			else
			{
				leftptr->next = start;
				leftptr = leftptr->next;
			}
			start = start->next;
			leftptr->next = nullptr;
			length_left++;
		}
		else
		{
			if (right == nullptr)
			{
				right = start;
				rightptr = right;
			}
			else
			{
				rightptr->next = start;
				rightptr = rightptr->next;
			}
			start = start->next;
			rightptr->next = nullptr;
			length_right++;
		}
	}

	left = quick_sort(left, length_left);
	right = quick_sort(right, length_right);

	if (left != nullptr)
	{
		tmp = left;
		while (tmp->next != nullptr)
			tmp = tmp->next;
		tmp->next = pivot;
		tmp->next->next = right;
	}
	else
	{
		left = pivot;
		left->next = right;
	}

	pivot = nullptr;
	right = nullptr;
	return left;

}

template<typename K, typename V>
void LinkedListCollection<K, V>::quick_sort()
{
	head = quick_sort(head, length);
	tail = head;
	while (tail->next != nullptr)
		tail = tail->next;
}

// insert a key-value pair into the collection
template<typename K, typename V>
void LinkedListCollection<K, V>::insert(const K& key, const V& val)
{
	Node* cur = new Node;
	cur->key = key;
	cur->value = val;
	cur->next = nullptr;
	if (length == 0)
	{
		head = cur;
		tail = head;
	}
	else
	{
		tail->next = cur;
		tail = cur;
	}
	length++;
}

// remove a key-value pair from the collection
template<typename K, typename V>
void LinkedListCollection<K, V>::remove(const K& key)
{
	if (length == 0) // empty, nothing to delete
		return;
	else if (length == 1) // single item
	{
		delete head;
		head = nullptr;
		tail = nullptr;
		length--;
		return;
	}
	Node* prevptr = head;
	Node* delptr = head->next;

	while (delptr != nullptr)
	{
		if (prevptr->key == key and prevptr == head)
		{
			head = prevptr->next;
			prevptr->next = nullptr;
			delete prevptr;
			length--;
			return;
		}
		else if (prevptr->next->key == key and delptr != tail)
		{
			prevptr->next = delptr->next;
			delptr->next = nullptr; // maybe redundant
			delete delptr;
			length--;
			return;
		}
		else if (delptr->key == key and delptr == tail)
		{
			tail = prevptr;
			tail->next = nullptr;
			delete delptr;
			length--;
			return;
		}
		else
		{
			delptr = delptr->next;
			prevptr = prevptr->next;
		}
	}
}

// find value associated with the key
template<typename K, typename V>
bool LinkedListCollection<K, V>::find(const K& key, V& val) const
{
	Node* ptr = head;
	bool is_found = false;
	while (ptr != nullptr and !is_found)
	{
		if (ptr->key == key)
		{
			val = ptr->value;
			is_found = true;
		}
		ptr = ptr->next;
	}
	return is_found;
}

// find the keys associated with the range
template<typename K, typename V>
void LinkedListCollection<K, V>::
find(const K& k1, const K& k2, std::vector<K>& keys) const
{
	V tempVal;
	Node* ptr = head;
	for (int i = 0; i < length; i++)
	{
		if (ptr->key >= k1 and ptr->key <= k2)
		{
			keys.push_back(ptr->key);
		}
		ptr = ptr->next;
	}
}

// return all the keys in the collection
template<typename K, typename V>
void LinkedListCollection<K, V>::keys(std::vector<K>& keys) const
{
	Node* ptr = head;
	while (ptr != nullptr)
	{
		keys.push_back(ptr->key);
		ptr = ptr->next;
	}
}

// return collection keys in sorted order
template<typename K, typename V>
void LinkedListCollection<K, V>::sort(std::vector<K>& keys) const
{
	Node* ptr = head;
	while (ptr != nullptr)
	{
		keys.push_back(ptr->key);
		ptr = ptr->next;
	}
	std::sort(keys.begin(), keys.end());
}

// return number of keys in collection
template<typename K, typename V>
int LinkedListCollection<K, V>::size() const
{
	return length;
}

#endif