#include <iostream>
#include "my_vector.h"

int main()
{
	containers::Vector<int> v1;
	containers::Vector<int> v2(v1);
	v2 = v1;
	containers::Vector<int> v3(10);
	v3[5] = 42;
	std::cout << v3[5] << '\n';

	containers::Vector<float> v4;
	v4.reserve(12);
}