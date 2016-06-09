#include <iostream>
#include <cstddef>
#include <stdexcept>

/*
	methods on  oaht class:
	-> oaht(capacity)
	-> oaht()
	-> oaht(const oaht<_Key, _Value>& other)
	-> iterator begin() 
	-> iterator end() 
	-> iterator rbegin() 
	-> iterator rend() 
	-> const_iterator rbegin() const
	-> const_iterator rend() const
	-> const_iterator begin() const
	-> const_iterator end() const 
	-> iterator find(_Key key) 
	-> oaht<_Key, _Value>&operator = (const oaht<_Key, _Value>&other)
	-> bool operator == (const oaht<_Key, _Value>& _Right) const 
	-> bool operator != (const oaht<_Key, _Value>& _Right) const 
	-> _Value& operator[](const _Key &key)
	-> _Value operator[](const _Key &key) const 
	-> _Value& at(const _Key &key)
	-> size_t size()
	-> size_t get_capacity()
	-> bool count(const _Key &key) const 
	-> bool count(const _Key &key, size_t& index) const
	-> bool empty() const
	-> void erase(const _Key &key)
	-> void clear() 
	-> void swap(oaht<_Key, _Value> &_Oaht)
*/

namespace open_addressing_hash_table {
	using std::size_t;

	const static char NEVER_USED = 0; 
	const static char USED = 1; 
	const static char ERASED = 2;

#pragma pack(push, 1)
	template<typename _Key, typename _Value>
	struct node {
		char state = 0;
		_Key first;
		_Value second;
	};
#pragma pack(pop)

	template<typename _Key, typename _Value, typename _Hash = std::hash<_Key> >
	class oaht {
	private:
		size_t capacity;
		size_t size_oaht;

		node<_Key, _Value> *nodes;
		_Hash h;

#ifndef __GNUC__
#pragma region
#endif
		template<typename _KeyI, typename _ValueI, typename _HashI>
		class hash_iterator
			: public std::iterator<std::input_iterator_tag,
			node<_KeyI, _ValueI> > {
		public:
			hash_iterator(const hash_iterator& it) : p(it.p) {
			}

			hash_iterator(node<_KeyI, _ValueI> *p, oaht<_KeyI, _ValueI, _HashI> * const oaht_data) : p(p), oaht_(oaht_data) {
			}

			bool operator != (hash_iterator const & other) const {
				return p != other.p;
			}

			bool operator == (hash_iterator const & other) const {
				return p == other.p;
			}

			typename hash_iterator::reference operator*() const {
				return *p;
			}

			node<_KeyI, _ValueI>* operator->() const {
				return p;
			}

			hash_iterator& operator++() {
				do {
					++p;
				} while (*this != oaht_->end() && p->state != USED);

				return *this;
			}

			hash_iterator& operator++(int) {
				do {
					++p;
				} while (*this != oaht_->end() && p->state != USED);

				return *this;
			}

			hash_iterator& operator--() {
				do {
					--p;
				} while (*this != oaht_->rend() && p->state != USED);

				return *this;
			}

			hash_iterator& operator--(int) {
				do {
					--p;
				} while (*this != oaht_->rend() && p->state != USED);

				return *this;
			}

		private:
			node<_KeyI, _ValueI> *p;
			oaht<_KeyI, _ValueI, _HashI> *oaht_;
		};
#ifndef __GNUC__
#pragma endregion
#endif

	public:
		typedef hash_iterator<_Key, _Value, _Hash> iterator;
		typedef hash_iterator< const _Key, const _Value, const _Hash> const_iterator;

		oaht(size_t capacity) : capacity(capacity), size_oaht(0) {
			nodes = new node<_Key, _Value>[capacity];
		}

		oaht() : capacity(3), size_oaht(0) {
			nodes = new node<_Key, _Value>[capacity];
			
			//std::cout << "node_state: " << sizeof(node_state) << endl;
			//std::cout << "key: " << sizeof(_Key) << endl;
			std::cout << "node: " << sizeof(node<_Key, _Value>) << endl;
		}

		oaht(const oaht<_Key, _Value>& other) {
			nodes = new node<_Key, _Value>[other.capacity];
			capacity = other.capacity;
			h = other.h;
			size_oaht = other.size_oaht;
			for (size_t i = 0; i < capacity; ++i)
			if (other.nodes[i].state == USED)
				nodes[i] = other.nodes[i];
		}

		~oaht() {
			delete[]nodes;
		}

		iterator begin() {
			if (size_oaht == 0)
				return end();
			return iterator(nodes + get_start_index(), this);
		}

		iterator end() {
			return iterator(nodes + capacity, this);
		}

		iterator rbegin() {
			return iterator(nodes + capacity - 1, this);
		}

		iterator rend() {
			return iterator(nodes - 1, this);
		}

		const_iterator rbegin() const {
			return const_iterator(nodes + capacity - 1, this);
		}

		const_iterator rend() const {
			return const_iterator(nodes - 1, this);
		}

		const_iterator begin() const {
			if (capacity == 0)
				return end();
			return const_iterator(nodes + get_start_index(), this);
		}

		const_iterator end() const {
			return const_iterator(nodes + capacity, this);
		}

		iterator find(_Key key) {
			return iterator(nodes + get_real_index(key), this);
		}

		oaht<_Key, _Value>&operator = (const oaht<_Key, _Value>&other) {
			if (this != &other) {
				delete[]nodes;
				nodes = new node<_Key, _Value>[other.capacity];
				capacity = other.capacity;
				h = other.h;
				size_oaht = other.size_oaht;
				for (size_t i = 0; i < capacity; ++i)
				if (other.nodes[i].state == USED)
					nodes[i] = other.nodes[i];
			}
			return (*this);
		}

		bool operator == (const oaht<_Key, _Value>& _Right) const {
			if (capacity != _Right.capacity)
				return false;
			for (size_t i = 0; i < capacity; ++i)
			if (_Right.nodes[i].state == USED && nodes[i].first != _Right.nodes[i].first && nodes[i].second != _Right.nodes[i].second)
				return false;
			return true;
		}

		bool operator != (const oaht<_Key, _Value>& _Right) const {
			return !(*this == _Right);
		}

		_Value& operator[](const _Key &key) {
			rehash();

			size_t index;

			if (!count(key, index)) {
				set(key, index, nodes, capacity);
				size_oaht++;
				nodes[index].second = _Value();
			}

			return nodes[index].second;
		}

		_Value operator[](const _Key &key) const {
			size_t index = get_real_index(key);

			return nodes[index].value;
		}

		_Value& at(const _Key &key) {
			rehash();

			size_t index;
			if (!count(key, index)) {
				set(key, index, nodes, capacity);
				size_oaht++;
				nodes[index].second = _Value();
			}

			return nodes[index].second;
		}

		size_t size() {
			return size_oaht;
		}

		size_t get_capacity() {
			return capacity;
		}

		bool count(const _Key &key) const {
			size_t index = get_index(key, capacity);

			for (size_t d = 0; d < capacity; d++) {
				if (nodes[index].state == NEVER_USED)
					return false;
				if (nodes[index].state == USED && nodes[index].first == key)
					return true;
				index++;
				if (index == capacity)
					index = 0;
			}
			return false;
		}

		bool count(const _Key &key, size_t& index) const {
			index = get_index(key, capacity);

			for (size_t d = 0; d < capacity; d++) {
				if (nodes[index].state == NEVER_USED)
					return false;
				if (nodes[index].state == USED && nodes[index].first == key)
					return true;
				index++;
				if (index == capacity)
					index = 0;
			}
			return false;
		}

		bool empty() const {
			return (size_oaht == 0);
		}

		void erase(const _Key &key) {
			size_t index = get_index(key, capacity);

			for (size_t d = 0; d < capacity; d++) {
				if (nodes[index].state == NEVER_USED)
					return;
				if (nodes[index].state == USED && nodes[index].key == key) {
					nodes[index].state = ERASED;
					--size_oaht;
					return;
				}
				index++;
				if (index == capacity)
					index = 0;
			}
		}

		void clear() {
			delete[]nodes;
			capacity = 0;
			size_oaht = 0;
			nodes = new node<_Key, _Value>[capacity];
		}

		void swap(oaht<_Key, _Value> &_Oaht) {
			oaht<_Key, _Value>n_oaht(*this);
			*this = _Oaht;
			_Oaht = n_oaht;
		}

	private:
		size_t get_index(const _Key& key, size_t size_oaht) const {
			return (h(key) * 881) % size_oaht;
		}

		size_t get_start_index() {
			for (size_t i = 0; i < capacity; ++i)
			if (nodes[i].state == USED)
				return i;

			return (capacity - 1);
		}

		size_t get_real_index(const _Key& key) const {
			size_t index = get_index(key, capacity);

			for (size_t d = 0; d < capacity; d++) {
				if (nodes[index].state == NEVER_USED)
					return 0;
				if (nodes[index].state == USED && nodes[index].first == key)
					return index;

				index++;
				if (index == capacity)
					index = 0;
			}
			return 0;
		}

		void rehash() {
			if ((size_oaht << 1) <= capacity)
				return;

			size_t n_capacity = (capacity << 1);

			node<_Key, _Value> *n_nodes = new node<_Key, _Value>[n_capacity];

			for (size_t i = 0; i < capacity; ++i)
			if (nodes[i].state == USED) {
				size_t index;
				set(nodes[i].first, index, n_nodes, n_capacity);
				n_nodes[index].second = nodes[i].second;
			}

			delete[]nodes;

			nodes = n_nodes;
			capacity = n_capacity;
		}

		bool set(const _Key& key, size_t& index, node<_Key, _Value> * nodes,
			size_t nodes_length) {
			index = get_index(key, nodes_length);

			for (size_t i = 0; i < nodes_length; i++) {
				if (nodes[index].state == NEVER_USED ||
					nodes[index].state == ERASED) {
					nodes[index].first = key;
					nodes[index].state = USED;
					return true;
				}

				if (nodes[index].first == key)
					return false;

				++index;

				if (index == nodes_length)
					index = 0;
			}

			return false;
		}
	};
}
