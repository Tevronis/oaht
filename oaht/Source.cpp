#include "oaht.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <map>
#include <ctime>
#include <cassert>
#include <cstdint>
#include <string>

using namespace std;
using namespace open_addressing_hash_table;

const int N = 50;

int64_t mas[N];
string s[N];

void main_functional() 
{
	oaht<int64_t, int64_t, std::hash<int64_t>> m;
	for (size_t i = 0; i < N; i++)
		m[mas[i]] = i;

	
	/*oaht<string, string> m;
	for (size_t i = 0; i < N; i++)
		m[s[i]] = s[i] + "test";*/
	

	cout << "all element [a.begin(); a.end()]:" << endl;
	for (auto it = m.begin(); it != m.end(); ++it)
		cout << (*it).value << endl;

	cout << "all element [a.rbegin(); a.rend()]:" << endl;
	for (auto it = m.rbegin(); it != m.rend(); --it)
		cout << (*it).value << endl;

	cout << "a.begin() -> key:" << endl;
	auto iter = m.begin();
	cout << (*iter).key << endl;

	cout << "a.begin() -> value:" << endl;
	iter = m.begin();
	cout << (*iter).value << endl;

	cout << "--a.end() -> key:" << endl;
	iter = m.end();
	cout << (*(--iter)).key << endl;

	cout << "--a.end() -> value:" << endl;
	iter = m.end();
	cout << (*(--iter)).value << endl;

	cout << "map.size(): " << endl;
	cout << m.size() << endl;

	cout << "map.empty(): " << endl;
	cout << m.empty() << endl;

	cout << "map.clear(): " << endl;
	m.clear(); //m.begin == m.end; 0 iterations
	for (auto it = m.begin(); it != m.end(); ++it)
		cout << (*it).value << endl;

	cout << "map.size(): " << endl;
	cout << m.size() << endl;

	cout << "map.empty(): " << endl;
	cout << m.empty() << endl;
}

int main() {

	for (size_t i = 0; i < N; i++)
		mas[i] = rand() * rand();
	s[0] = "oa_";
	for (size_t i = 1; i < N; i++)
		s[i] = s[i - 1] + "tst";

	main_functional();
	
	return 0;
}