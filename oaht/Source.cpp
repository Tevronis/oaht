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
	
	oaht<string, int> m1(100);
	oaht<string, int> m2(m1);
	oaht<string, int> m3;

	if (m1 == m2)
		cout << "m1 == m2" << endl;
	else
		cout << "error: m1 != m2" << endl;

	if (m1 == m3)
		cout << "error: m1 == m3" << endl;
	else
		cout << "m1 != m3" << endl;

	if (m1 != m3)
		cout << "m1 != m3" << endl;
	else
		cout << "error: m1 == m3" << endl;

	m1.at("str") = 1;

	cout << "m1.at('str') = 1. m1['str']: " << m1["str"] << endl;

	cout << "m1.size(): " << m1.size() << endl;

	cout << "m1.capacity: " << m1.get_capacity() << endl;

	if (m1.count("str"))
		cout << "m1 contain 'str'" << endl;
	else
		cout << "error: m1 not contain 'str'" << endl;

	if (m1.count("test"))
		cout << "error: m1 contain 'test'" << endl;
	else
		cout << "m1 not contain 'test'" << endl;

	if (m1.empty())
		cout << "error: m1 empty" << endl;
	else
		cout << "m1 not empty" << endl;

	if (m3.empty())
		cout << "m3 empty" << endl;
	else
		cout << "error: m3 not empty" << endl;

	m2 = m1;

	m1.clear();
	cout << "m1.clear(): m1 empty? : " << m1.empty() << endl;

	m2.swap(m1);
	cout << "m2.swap(): m1 empty? : " << m1.empty() << endl;

	m3["test"] = 1;
	m3["test2"] = 9;
	m3["test3"] = 42;
	
	cout << "m3.begin() - m3.end(): " << endl;
	for (auto it = m3.begin(); it != m3.end(); it++)
		cout << "m: "<< (*it).value << endl;

	cout << "m3.rbegin() - m3.rend(): " << endl;
	for (auto it = m3.rbegin(); it != m3.rend(); it--)
		cout << "m1: " << (*it).value << endl;

	cout << (*m1.find("str")).value;
	return 0;
}