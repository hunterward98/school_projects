This was an assignment for my abstract data structures and algotrithms class.
We first implemented a KV collection with a vector that had a linear find
and used a sorting algorithm to sort the collection. This made insertions
fast, as all we had to do was use push_back() to add the item to the collection.
However, this implementation uses binary search to find where the item should be
inserted, so the KV collection is always sorted, and thus, we don't have to
use any sorting algorithms.