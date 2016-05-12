#include "oaht.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <map>
#include <ctime>
#include <cassert>
#include <cstdint>
#include <string>
#include <vector>

using namespace std;
using namespace open_addressing_hash_table;

const int N = 1000000;

int64_t mas[N];
string s[N];


vector<int> initialization() {
	vector<int> result;

	for (size_t i = 0; i < N; i++)
		result.push_back(rand() * rand());

	return result;
}

template<typename Map, typename _Key, typename _Value>
void test(string name) {
	vector<_Key> vec = initialization();

	clock_t start_clock = clock();

	Map m;

	for (int i = 0; i < N; i++) {
		m[vec[i]] = vec[i];
	}

	cout << name << " " << ((clock() - start_clock) * 1000.0 / CLOCKS_PER_SEC) << endl;
}

int main() {

	//test<oaht<int, int>, int, int>("oaht");
	//test<map<int, int>, int, int>("map");
	
	/*map<int, int> n;
	n[10] = 1;
	map<int, int> n1;
	cout << n1[10];*/

	oaht<int, int> m;
	m[10] = 1;
	m[100] = 9;
	m[5] = 42;
	oaht<int, int> m1;

	//m1 = m;

	if (m1 != m)
		cout << "cool!" << endl;
	
	for (auto it = m.begin(); it != m.end(); it++)
		cout << "m: "<< (*it).value << endl;
	for (auto it = m1.begin(); it != m1.end(); it++)
		cout << "m1: " << (*it).value << endl;


	return 0;
}