#pragma once

#include <iostream>
#include <cstddef>
#include <stdexcept>


using namespace std;

namespace open_addressing_hash_table
{
	template<typename _Key, typename _Value, typename _Hash = std::hash<_Key>>
	class oaht;

	using std::size_t;

	enum node_state {
		NEVER_USED,
		USED,
		ERASED
	};

	template<typename _Key, typename _Value>
	struct node {
		_Key key;
		_Value value;
		node_state state;
	};

	template<typename _Key, typename _Value, typename _Hash = std::hash<_Key>>
	class hash_iterator : public std::iterator<std::input_iterator_tag, node<_Key, _Value>>
	{
	public:
		hash_iterator(const hash_iterator& it) : p(it.p) {
		}

		hash_iterator(node<_Key, _Value> *p, oaht<_Key, _Value> *const oaht_data) : p(p), oaht_(oaht_data) {
		}

		bool operator!=(hash_iterator const& other) const {
			return p != other.p;
		}

		bool operator==(hash_iterator const& other) const {
			return p == other.p;
		}

		typename hash_iterator::reference operator*() const {
			return *p;
		}

		hash_iterator& operator++() {
			do {
				++p;
			} while (*this != oaht_->end() && p->state != node_state::USED);

			return *this;
		}

		hash_iterator& operator--() {
			do {
				--p;
			} while (*this != oaht_->rend() && p->state != node_state::USED);

			return *this;
		}
	private:
		node<_Key, _Value>* p;
		oaht<_Key, _Value>* oaht_;
	};

	template<typename _Key, typename _Value, typename _Hash = std::hash<_Key>>
	class oaht {
	private:
		size_t capacity;
		size_t size_oaht;

		node<_Key, _Value>* nodes;
		_Hash h;

		size_t get_start_index() {
			for (size_t i = 0; i < capacity; ++i)
			if (nodes[i].state == node_state::USED)
				return i;

			return (capacity - 1);
		}
	public:
		typedef hash_iterator<_Key, _Value> iterator;
		typedef hash_iterator<const _Key, const _Value> const_iterator;

		oaht(size_t capacity) : capacity(capacity), size_oaht(0) {
			nodes = new node<_Key, _Value>[capacity];

			for (size_t i = 0; i < capacity; i++)
				nodes[i] = node<_Key, _Value>();
		}

		oaht() : capacity(3), size_oaht(0) {
			nodes = new node<_Key, _Value>[capacity];

			for (size_t i = 0; i < capacity; i++)
				nodes[i] = node<_Key, _Value>();
		}

		~oaht() {
			delete[] nodes;
		}

		iterator begin() {
			if (capacity == 0)
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

		_Value& operator[](const _Key &key) {
			rehash();

			size_t index;
			bool result = set(key, index, nodes, capacity);
			if (result) {
				size_oaht++;
				nodes[index].value = _Value();
			}

			return nodes[index].value;
		}

		_Value operator[](const _Key &key) const {
			return nodes[get_index(key, capacity)].value;
		}

		size_t size() {
			return size_oaht;
		}

		size_t count(const _Key &key) const {
			size_t index = get_index(key, capacity);

			for (size_t d = 0; d < capacity; d++) {
				if (nodes[index].state == node_state::NEVER_USED)
					return 0;
				if (nodes[index].state == node_state::USED && nodes[index].key == key)
					return 1;
				index++;
				if (index == capacity)
					index = 0;
			}
			return 0;
		}

		bool empty() const {
			return (size_oaht == 0);
		}

		void erase(const _Key &key) {
			size_t index = get_index(key, capacity);

			for (size_t d = 0; d < capacity; d++) {
				if (nodes[index].state == node_state::NEVER_USED)
					return;
				if (nodes[index].state == node_state::USED && nodes[index].key == key) {
					nodes[index].state = node_state::ERASED;
					--size_oaht;
					return;
				}
				index++;
				if (index == capacity)
					index = 0;
			}
		}

		_Value& at(const _Key& _Keyval) {	// find element matching _Keyval
			rehash();

			size_t index;
			bool result = set(key, index, nodes, capacity);
			if (result) {
				size_oaht++;
				nodes[index].value = _Value();
			}

			return nodes[index].value;
		}

		void clear() {
			delete[] nodes;
			capacity = 0;
			size_oaht = 0;
			nodes = new node<_Key, _Value>[capacity];

			for (size_t i = 0; i < capacity; i++)
				nodes[i] = node<_Key, _Value>();
		}

		void swap(const oaht<_Key, _Value> & _Oaht) {
			oaht<_Key, _Value> n_oaht = this;
			this = _Oaht;
			oaht_ = n_oaht;
		}

	private:
		size_t get_index(const _Key& key, size_t size_oaht) const {
			return (h(key) * 881) % size_oaht;
		}

		void rehash() {
			if ((size_oaht << 1) <= capacity)
				return;

			size_t n_capacity = (capacity << 1);	// capacity * 2

			node<_Key, _Value>* n_nodes = new node<_Key, _Value>[n_capacity];	// create new buckets
			
			for (size_t i = 0; i < n_capacity; ++i)
				n_nodes[i] = node<_Key, _Value>();

			for (size_t i = 0; i < capacity; ++i)
			if (nodes[i].state == node_state::USED) {
				size_t index;
				set(nodes[i].key, index, n_nodes, n_capacity);
				n_nodes[index].value = nodes[i].value;
			}

			delete[] nodes;

			nodes = n_nodes;
			capacity = n_capacity;
		}

		bool set(const _Key& key, size_t& index, node<_Key, _Value>* nodes, size_t nodes_length) {
			index = get_index(key, nodes_length);

			for (size_t i = 0; i < nodes_length; i++) {
				if (nodes[index].state == node_state::NEVER_USED || nodes[index].state == node_state::ERASED) {
					nodes[index].key = key;
					nodes[index].state = node_state::USED;
					return true;
				}

				if (nodes[index].key == key)
					return false;

				++index;

				if (index == nodes_length)
					index = 0;
			}

			return false;
		}
	};
}