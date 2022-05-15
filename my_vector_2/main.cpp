#include <iostream>
#include <cassert>
#include "my_vector.h"

int main()
{
	containers::Vector<int> v1;
	containers::Vector<int> v2(v1);
	v2 = v1;
	containers::Vector<int> v3(10);
	v3[5] = 42;
	std::cout << v3[5] << '\n';

	containers::Vector<int> v4;
	v4.reserve(2);
	std::cout << v4.size() << ' ' << v4.capacity() << '\n';
	for (int i = 0; i != 10; ++i)
		v4.push_back(i);
	std::cout << v4.size() << ' ' << v4.capacity() << '\n';
	for (size_t i = 0; i != 8; ++i)
		std::cout << v4[i] << ' ';
	std::cout << '\n';
	v4.shrink_to_fit();
	std::cout << v4.size() << ' ' << v4.capacity() << '\n';

	{
		containers::Vector<int> v1 = { 1, 2, 3, 4, 5 };
		containers::Vector<int> v2 = v1;
		assert(v1 == v2);
		assert(v2 == v1);
		assert(!(v1 != v2));
		assert(!(v2 != v1));
		containers::Vector<int> v3 = { 1, 2, 4, 5 };
		assert(v1 < v3);
		assert(!(v1 > v3));
		assert(!(v3 < v1));
		assert(v3 > v1);
		assert(v1 <= v2);
		assert(v2 >= v1);
		assert(v1 <= v3);
		assert(v3 >= v1);
		std::cout << "PASS" << std::endl;
	}
}