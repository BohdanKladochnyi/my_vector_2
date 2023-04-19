#include "pch.h"
#include "../my_vector_2/my_vector.h"


TEST(VectorCtorTest, DefaultCtor) {
	containers::Vector<int> v;
	EXPECT_EQ(v.empty(), true);
	EXPECT_EQ(v.size(), 0);
}

TEST(VectorCtorTest, CopyCtor) {
	containers::Vector<int> v;

	EXPECT_EQ(v.empty(), true);
	EXPECT_EQ(v.size(), 0);
}

TEST(VectorElementAccessTest, PushBack) {
	containers::Vector<int> v;
	//v.push_back(1);

}