#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cassert>
#include <map>
#include <random>
#include <unordered_map>
#include <hash_map>
#include "oaht.h"

using namespace std;

typedef unsigned long long ull;

mt19937_64 g;

inline ull rnd()
{
	/*
	ull result = 0;
	result = (result << 8) ^ (rand() & 255);
	result = (result << 8) ^ (rand() & 255);
	result = (result << 8) ^ (rand() & 255);
	result = (result << 8) ^ (rand() & 255);
	result = (result << 8) ^ (rand() & 255);
	result = (result << 8) ^ (rand() & 255);
	result = (result << 8) ^ (rand() & 255);
	result = (result << 8) ^ (rand() & 255);
	*/
	return g();
}

const int N = 10000;

void testStdMap()
{
	g.seed(1);
	//srand(0);
	size_t s = clock();
	ull sum = 0;
	map<ull, ull> m;
	for (int i = 0; i < N; i++)
	{
		ull item = rnd();
		sum += item;
		m[rnd()] = item;
	}
	int r = 0;
	for (int i = 0; i < N; i++)
	{
		if (m.count(rnd()))
			r++;
	}
	for (map<ull, ull>::iterator it = m.begin(); it != m.end(); it++)
		sum -= it->second;
	// assert(sum == 0);
	cout << "std::map: " << (clock() - s) << " " << sum << " " << r << endl;
}

void testStdUnorderedMap()
{
	g.seed(1);
	//srand(0);
	size_t s = clock();
	ull sum = 0;
	unordered_map<ull, ull> m;
	for (int i = 0; i < N; i++)
	{
		ull item = rnd();
		sum += item;
		m[rnd()] = item;
	}
	int r = 0;
	for (int i = 0; i < N; i++)
	{
		if (m.count(rnd()))
			r++;
	}
	for (unordered_map<ull, ull>::iterator it = m.begin(); it != m.end(); it++)
		sum -= it->second;
	// assert(sum == 0);
	cout << "std::unordered_map: " << (clock() - s) << " " << sum << " " << r << endl;
}

struct hash_ull
{
	size_t operator()(const ull& x) const
	{
		return size_t(x);
	}
};

void testOaht()
{
	g.seed(1);
	//srand(0);
	size_t s = clock();
	ull sum = 0;
	open_addressing_hash_table::oaht<ull, ull, hash_ull> m;
	for (int i = 0; i < N; i++)
	{
		ull item = rnd();
		sum += item;
		m[rnd()] = item;
	}
	int r = 0;
	for (int i = 0; i < N; i++)
	{
		if (m.count(rnd()))
			r++;
	}
	for (auto it = m.begin(); it != m.end(); it++)
		sum -= it->second;
	// assert(sum == 0);
	cout << "oaht: " << (clock() - s) << " " << sum << " " << r << endl;
}

int main()
{
	//testStdMap();
	//testStdUnorderedMap();
	testOaht();
}