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
	v.push_back(-1);
	EXPECT_EQ(v.back(), -1);

	v.push_back(1);
	EXPECT_EQ(v.back(), 1);

	v.push_back(2);
	EXPECT_EQ(v.back(), 2);

	EXPECT_EQ(v.size(), 3);
}

TEST(VectorElementAccessTest, TopPopBack) {
	containers::Vector<int> v;
	EXPECT_ANY_THROW(v.pop_back());
	v.push_back(1);
	EXPECT_EQ(v.back(), 1);
	v.pop_back();
	EXPECT_TRUE(v.empty());
}