#pragma once

namespace containers {


template<typename T> class Buffer {
protected:
	size_t size_ = 0, capacity_ = 0;
	T* data_ = nullptr;

	Buffer() = default;
	Buffer(size_t count) : capacity_(count), data_(new T[capacity_]) {}
	Buffer(const Buffer& other)
		: size_(other.size_), capacity_(other.capacity_), data_(new T[capacity_]) {
		std::copy(other.data_, other.data_ + capacity_, data_);
	}
	Buffer& operator=(const Buffer& other) {
		if (this == &other) return *this;
		delete[] data_;
		size_ = other.capacity_;
		capacity_ = other.capacity_;
		data_ = new T[capacity_];
		std::copy(other.data_, other.data_ + capacity_, data_);
		return *this;
	}
	~Buffer() { delete[] data_; }
};

template<typename T> class Vector final : private Buffer<T> {
	using Buffer<T>::size_;
	using Buffer<T>::capacity_;
	using Buffer<T>::data_;
	template <typename> class iterator;

public:
	Vector() = default;
	Vector(const Vector& other) = default;
	Vector& operator=(const Vector& other) = default;
	Vector(size_t count) : Buffer<T>(count) {}
	Vector(std::initializer_list<T> il) : Buffer<T>(il.size()) {
		size_ = il.size();
		std::copy(il.begin(), il.end(), data_);
	}
	~Vector() = default;

public: //element access
	T& operator[](size_t pos) {
		return data_[pos];
	}
	const T& operator[](size_t pos) const {
		return data_[pos];
	}

	T& front() {
		return data_[0];
	}
	const T& front() const {
		return data_[0];
	}

	T& back() {
		return data_[size_ - 1];
	}
	const T& back() const {
		return data_[size_ - 1];
	}

	T* data() {
		return data_;
	}
	const T* data() const {
		return data_;
	}

public: //capacity
	size_t capacity() const {
		return capacity_;
	}
	size_t size() const {
		return size_;
	}
	bool empty() const {
		return !size;
	}
	void reserve(size_t count) {
		if (capacity_ > count || !count) return;
		T* tmp_buf = new T[count];
		std::copy(data_, data_ + size_, tmp_buf);
		delete[] data_;
		capacity_ = count;
		data_ = tmp_buf;
	}
	void shrink_to_fit() {
		if (size_ == capacity_) return;
		T* tmp_buf = new T[size_];
		std::copy(data_, data_ + size_, tmp_buf);
		delete[] data_;
		capacity_ = size_;
		data_ = tmp_buf;
	}

public: //modifiers
	void assign(size_t count, const T& value) {
		if (count > capacity_) reserve(count);
		size_ = size_ > count ? size_ : count;
		for (size_t i = 0; i != count; ++i)
			data_[i] = value;
	}
	void clear() {
		size_ = 0;
	}
	void push_back(const T& value) {
		if (capacity_ < 4) reserve(4);
		if (size_ >= capacity_) reserve(capacity_ * 2);
		data_[size_] = value;
		++size_;
	}
	void pop_back() {
		--size_;
	}

public:
	bool equal(const Vector& other) const {
		if (this == &other) return true;
		if (size_ != other.size_) return false;
		size_t count = size_ < other.size_ ? size_ : other.size_;
		for (size_t i = 0; i != count; ++i)
			if (data_[i] != other.data_[i]) return false;
		return true;
	}
	bool less(const Vector& other) const {
		if (equal(other)) return false;
		size_t count = size_ < other.size_ ? size_ : other.size_;
		for (size_t i = 0; i != count; ++i)
			if (data_[i] > other.data_[i]) return false;
		return true;
	}

	Vector(const iterator& fst, const iterator& lst) : Buffer<T>(fst - lst) {
		std::copy(fst, lst, data_);
		size_ = fst - lst;
	}
	
	iterator begin() const {
		return data_;
	}
	iterator end() const {
		return data_ + size_;
	}
};

template <typename T> class iterator final {
	friend Vector;
	T* ptr_;

	iterator(T* ptr) noexcept : ptr_(ptr) {}

public:
	using iterator_category = std::random_access_iterator_tag;
	using value_type = T;
	using difference_type = std::ptrdiff_t;
	using pointer = T*;
	using reference = T&;

	iterator(const iterator& other) = default;
	iterator& operator=(const iterator& other) = default;

public:
	T& operator*() {
		return *ptr_;
	}
	const T& operator*() const {
		return *ptr_;
	}
	iterator& operator++() {
		++ptr_;
		return *this;
	}
	iterator operator++(int) {
		iterator old{ *this };
		++ptr_;
		return old;
	}
	iterator& operator--() {
		--ptr_;
		return *this;
	}
	iterator operator--(int) {
		iterator old{ *this };
		--ptr_;
		return old;
	}
	T& operator[](size_t pos) {
		return ptr_[pos];
	}
	const T& operator[](size_t pos) const {
		return ptr_[pos];
	}
	iterator& operator+=(size_t count) {
		ptr_ += count;
		return *this;
	}
	iterator& operator-=(size_t count) {
		ptr_ -= count;
		return *this;
	}

	auto operator<=>(const iterator&) const = default;
};

template<typename T>
bool operator!=(const Vector<T>& lhs, const Vector<T>& rhs)
{
	return !(lhs.equal(rhs));
}

template<typename T>
bool operator==(const Vector<T>& lhs, const Vector<T>& rhs)
{
	return lhs.equal(rhs);
}

template<typename T>
bool operator<(const Vector<T>& lhs, const Vector<T>& rhs)
{
	return lhs.less(rhs);
}

template<typename T>
bool operator>(const Vector<T>& lhs, const Vector<T>& rhs)
{
	return !(lhs.less(rhs)) && !(lhs.equal(rhs));
}

template<typename T>
bool operator<=(const Vector<T>& lhs, const Vector<T>& rhs)
{
	return !(lhs > rhs);
}

template<typename T>
bool operator>=(const Vector<T>& lhs, const Vector<T>& rhs)
{
	return !(lhs < rhs);
}


template<typename T>
iterator<T> operator+(
	const iterator<T>& it,
	size_t count)
{
	iterator<T> copy = it;
	copy += count;
	return copy;
}

template<typename T>
iterator<T> operator-(
	const iterator<T>& it,
	size_t count)
{
	iterator<T> copy = it;
	copy -= count;
	return copy;
}

template<typename T>
std::ptrdiff_t operator-(const iterator<T>& lhs, const iterator<T>& rhs)
{
	return ;
}

}