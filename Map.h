#ifndef MAP_H
#define MAP_H
#include <string>

typedef std::string KeyType;
typedef double ValueType;

const int DEFAULT_MAX_ITEMS = 200;

class Map
{
public:
	Map();
	Map(const Map& oldMap);

	~Map();

	Map& operator=(const Map& oldMap);

	bool empty() const;
	int size() const;
	bool insert(const KeyType& key, const ValueType& value);
	bool update(const KeyType& key, const ValueType& value);
	bool insertOrUpdate(const KeyType& key, const ValueType& value);
	bool erase(const KeyType& key);
	bool contains(const KeyType& key) const;
	bool get(const KeyType& key, ValueType& value) const;
	bool get(int i, KeyType& key, ValueType& value) const;
	void swap(Map& other);
	void dump() const;

private:
	class Pair
	{
	public:
		KeyType m_key;
		ValueType m_value;

		Pair* m_next;
		Pair* m_previous;
	};

	Pair* m_head;
	Pair* m_tail;
};

bool combine(const Map& m1, const Map& m2, Map& result);
void subtract(const Map& m1, const Map& m2, Map& result);
#endif // for MAP_H
