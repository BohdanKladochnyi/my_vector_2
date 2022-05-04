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
	T& operator[](size_t pos) { return data_[pos]; }
	const T& operator[](size_t pos) const { return data_[pos]; }

	T& front() { return data_[0]; }
	const T& front() const { return data_[0]; }

	T& back() { return data_[size_ - 1]; }
	const T& back() const { return data_[size_ - 1]; }

	T* data() { return data_; }
	const T* data() const { return data_; }

public: //capacity
	void assign() {

	}
	size_t capacity() const { return capacity_; }
	size_t size() const { return size_; }
	bool empty() const { return !size; }
};

}