#include "Map.h"
#include <iostream>
using namespace std;

Map::Map()
{
	m_head = nullptr; // An empty map's head and tail pointers do not point to anything.
	m_tail = nullptr; 
}

Map::Map(const Map& otherMap)
{
	m_head = nullptr; 
	m_tail = nullptr;

	for (Pair *p = otherMap.m_head; p != nullptr; p = p->m_next) { // We want to create an identical node for each node in the map passed in.
		Pair *q = new Pair();
		q->m_key = p->m_key;
		q->m_value = p->m_value;
		q->m_next = nullptr;
		q->m_previous = m_tail;

		if (m_head == nullptr) // If this is the first node, the head pointer should point to it.
			m_head = q;
		else // Otherwise, the previous node should point to this one.
			m_tail->m_next = q;

		m_tail = q; // Since we're inserting at the end, we always need to update the tail pointer.
	}
}

Map::~Map()
{
	for (Pair *p = m_head; p != nullptr; p = p->m_next)
		delete p->m_previous; // For each node, we want to delete the one before it. This is okay for the first node, since deleting nullptr does nothing.
	delete m_tail; // We have to remember to delete the very last node.
}

Map& Map::operator=(const Map& otherMap)
{
	if (&otherMap == this) // In case of aliasing
		return *this;

	for (Pair *p = m_head; p != nullptr; p = p->m_next) // We first must deallocate all of the nodes in the current map.
		delete p->m_previous;
	delete m_tail;
	
	m_head = nullptr;
	m_tail = nullptr;
	for (Pair *p = otherMap.m_head; p != nullptr; p = p->m_next) { // We create new nodes and copy the contents of the other map into the current one.
		Pair *q = new Pair();
		q->m_key = p->m_key;
		q->m_value = p->m_value;
		q->m_next = nullptr;
		q->m_previous = m_tail;

		if (m_head == nullptr)
			m_head = q;
		else
			m_tail->m_next = q;

		m_tail = q;
	}

	return *this;
}

bool Map::empty() const
{
	return m_head == nullptr; // The map is empty if the head pointer doesn't point to anything.
}

int Map::size() const
{
	int size = 0;
	for (Pair *p = m_head; p != nullptr; p = p->m_next) // We increment a counter for every node in our map.
		size++;

	return size;
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
	if (contains(key)) // We don't want to insert a key if one already exists.
		return false;

	Pair *p = new Pair(); // If one doesn't already exist, we allocate a new node and attach it to the end of our list.
	p->m_key = key;
	p->m_value = value;
	p->m_next = nullptr;
	p->m_previous = m_tail;

	if (m_head == nullptr)
		m_head = p;
	else
		m_tail->m_next = p;

	m_tail = p;

	return true;
}

bool Map::update(const KeyType& key, const ValueType& value)
{
	for (Pair *p = m_head; p != nullptr; p = p->m_next) { //If the key can be found, we want to change the value of the corresponding node.
		if (p->m_key == key) {
			p->m_value = value;
			return true;
		}
	}
	
	return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
	return update(key, value) || insert(key, value); // We either insert a new key or update an existing one.
}

bool Map::erase(const KeyType& key)
{
	for (Pair *p = m_tail; p != nullptr; p = p->m_previous) { // Traversing from the end backward will be helpful for the combine and subtract functions.
		if (p->m_key == key) { // If we find the node to delete, there are multiple scenarios to consider.
			if (m_head == m_tail) { // If there's only one node in the map, the head and tail pointers become nullptr.
				m_head = nullptr;
				m_tail = nullptr;
			}
			else if (p->m_previous == nullptr) { // If our node is at the very front, we want to make the next node the new head.
				m_head = p->m_next;
				m_head->m_previous = nullptr;
			}
			else if (p->m_next == nullptr) { // If our node is at the very end, we want to make the previous node the new tail.
				m_tail = p->m_previous;
				m_tail->m_next = nullptr;
			}
			else { // If we get to this case, we are guaranteed that there is a node before and after this one, and we connect the two.
				p->m_previous->m_next = p->m_next;
				p->m_next->m_previous = p->m_previous;
			}
			delete p; // Now that we've sorted out our pointers, we can now delete the node.
			
			return true;
		}
	}

	return false;
}

bool Map::contains(const KeyType& key) const
{
	for (Pair *p = m_head; p != nullptr; p = p->m_next) { // We check if the key is in the current map.
		if (p->m_key == key)
			return true;
	}

	return false;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
	for (Pair *p = m_head; p != nullptr; p = p->m_next) { // If we there's a node with the corresponding key, we want to store its value in the parameter.
		if (p->m_key == key) {
			value = p->m_value;
			return true;
		}
	}

	return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
	Pair *p = m_head;
	for (; i > 0; i--, p = p->m_next) { // This ensures we get the i + 1 th node in the list.
		if (p == nullptr)
			return false;
	}
	
	key = p->m_key;
	value = p->m_value;
	return true;
}

void Map::swap(Map& other)
{
	Pair *temp = m_head; // We only need to swap the head and tail pointers, every other node is accessed through these two.
	m_head = other.m_head;
	other.m_head = temp;

	temp = m_tail;
	m_tail = other.m_tail;
	other.m_tail = temp;
}

void Map::dump() const
{
	cerr.setf(ios::fixed);
	cerr.setf(ios::showpoint);
	cerr.precision(2);

	cerr << "-----\n";

	for (Pair *p = m_head; p != nullptr; p = p->m_next)
		cerr << p->m_key << " " << p->m_value << endl;
}

bool combine(const Map& m1, const Map& m2, Map& result)
{
	result = m1; // We will combine the two maps by taking m1 and selectively adding nodes from m2.,
	
	KeyType k;
	ValueType v;
	bool toReturn = true;
	for (int i = m2.size() - 1; i >= 0; i--) {
		m2.get(i, k, v);
		if (result.contains(k)) {
			ValueType v2;
			m1.get(k, v2);
			if (v == v2) // If the key already exists in result, we need to compare the two values.
				continue; // If the two nodes are identical, we simply don't add the same key twice.
			else {
				result.erase(k); // If the two nodes differ in value, then we shouldn't include either, so we need to remove the one in result.
				toReturn = false;
			}
		}
	}

	return toReturn;
}

void subtract(const Map& m1, const Map& m2, Map& result)
{
	KeyType k;
	ValueType v;
	for (int i = result.size() - 1; i >= 0; i--) { // We need to start by emptying result using the public interface.
		result.get(i, k, v);
		result.erase(k);
	}
	
	for (int i = m1.size() - 1; i >= 0; i--) {
		m1.get(i, k, v);
		if (!m2.contains(k)) // We only want to add to result the nodes in m1 that aren't in m2.
			result.insert(k, v);
	}
}
