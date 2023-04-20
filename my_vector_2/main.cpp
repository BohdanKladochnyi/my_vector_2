#include <iostream>
#include <cassert>
#include "my_vector.h"

#if 0
template <typename T>
std::ostream& dump(std::ostream& os, const containers::Vector<T>& v)
{
	for (auto& el : v)
		os << el << ' ';
	os << '\n';
	return os;
}

struct Buf {
	int x;
	Buf() = default;
	Buf(int x_) : x(x_) { std::cout << "ctor\n"; }
	Buf(const Buf& rhs) { std::cout << "copy ctor\n"; x = rhs.x; }
	Buf(Buf&& rhs) noexcept { std::cout << "move ctor\n"; x = rhs.x; }
	Buf& operator=(const Buf& rhs) { std::cout << "copy assigment\n"; x = rhs.x; return *this; };
	Buf& operator=(Buf&& rhs) noexcept { std::cout << "move assigment\n"; x = rhs.x; return *this; };
};
#endif
int main() {
	containers::Vector<int> v;
	


#if 0
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
	dump(std::cout, v4);
	v4.shrink_to_fit();
	std::cout << v4.size() << ' ' << v4.capacity() << '\n';

	containers::Vector<int> v5;
	v5.reserve(20);
	v5.assign(10, 42);
	v5[5] = 10;
	dump(std::cout, v5);
	containers::iterator<int> v5_it = v5.begin();
	std::cout << v5_it[5] << '\n';
	v5_it = v5_it + 1;
	auto v5_it2 = v5_it - v5.begin();

	containers::Vector<int> v6(v5.begin(), v5.end());
	dump(std::cout, v6);



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

	containers::Vector<Buf> v_b{};
	std::cout << "emplace\n";
	v_b.emplace(v_b.end(), 42);
	std::cout << "insert\n";
	v_b.insert(v_b.end(), 42);
	std::cout << "emplace_back\n";
	v_b.emplace_back(10);
	std::cout << "push_back\n";
	v_b.push_back(10);

	containers::Vector v7 = { 1, 2, 3 };
	containers::Vector v8rm = std::move(v7);
	v7 = std::move(v8rm);
	dump(std::cout, v7);
#endif
}