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

public:
	Vector() = default;
	Vector(const Vector& other) = default;
	Vector& operator=(const Vector& other) = default;
	Vector(size_t count) : Buffer<T>(count) {}
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
	void assign() {

	}
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
	class iterator {
		T* ptr_;

		iterator(T* ptr) : ptr_(ptr) {}

	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using reference = T&;

		iterator(const iterator& other) = default;
		iterator& operator=(const iterator& other) = default;

	public:
		T& operator*() const {
			return *ptr_;
		}
		iterator& operator++() {
			++ptr_;
			return *this;
		}
		iterator operator++(int) {
			iterator old{ *this };
			++(*this);
			return old;
		}
		iterator& operator--() {
			--ptr_;
			return *this;
		}
		iterator operator--(int) {
			iterator old{ *this };
			--(*this);
			return old;
		}
		iterator& operator[](size_t pos) {

		}
	};
};

}