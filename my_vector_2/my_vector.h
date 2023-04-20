#pragma once

namespace containers {


template <typename T>
void destroy(T* p) {
	p->~T();
}

template <typename FwdIt>
void destroy(FwdIt first, FwdIt last) {
	while (first != last) {
		destroy(&*first);
		first += 1;
	}
}

template <typename T1, typename T2>
void construct(T1* p, const T2& elem) {
	new (p) T2(elem);
}

template<typename T> class Buffer {
protected:
	T* data_;
	size_t size_, used_ = 0;

	Buffer(const Buffer&) = delete;
	Buffer& operator=(const Buffer& other) = delete;

	Buffer(size_t size = 0)
		: data_((size == 0) ? nullptr : static_cast<T*>(::operator new(sizeof(T) * size))), size_(size) {}
	
	Buffer(Buffer&& other) noexcept
		: data_(other.data_), size_(other.size_), used_(other.used_) {
		other.data_ = nullptr;
		other.size_ = 0;
		other.used_ = 0;
	}
	Buffer& operator=(Buffer&& other) noexcept {
		std::swap(data_, other.data_);
		std::swap(size_, other.size_);
		std::swap(used_, other.used_);
		return *this;
	}
	~Buffer() {
		destroy(data_, data_ + used_);
		::operator delete(data_);
	}
};

template <typename> class iterator;

template<typename T>
class Vector final : private Buffer<T> {
	using Buffer<T>::data_;
	using Buffer<T>::size_;
	using Buffer<T>::used_;
public:
	Vector(Vector&&) = default;
	Vector& operator=(Vector&&) = default;
	~Vector() = default;

	explicit Vector(size_t size = 0) : Buffer<T>(size) {}

	Vector(const Vector& other) : Buffer<T>(other.used_) {
		while (used_ != other.used_) {
			construct(data_ + used_, other.data_[used_]);
			used_ += 1;
		}
	}
	Vector& operator=(const Vector& other) {
		Vector new_vec(other);
		std::swap(*this, new_vec);
		return *this;
	}



public: //element access
	T& back() {
		if (empty())
			throw std::runtime_error("Vector is empty");
		return data_[used_ - 1];
	}
	const T& back() const {
		if (empty())
			throw std::runtime_error("Vector is empty");
		return data_[used_ - 1];
	}

	T& front() {
		if (empty())
			throw std::runtime_error("Vector is empty");
		return data_[0];
	}
	const T& front() const {
		if (empty())
			throw std::runtime_error("Vector is empty");
		return data_[0];
	}

public: //capacity
	size_t size() const {
		return used_;
	}
	size_t capacity() const {
		return size_;
	}
	bool empty() const {
		return (used_ == 0);
	}

public: //modifiers
	void push_back(const T& value) {
		if (used_ == size_) {
			Vector new_vec(size_ * 2 + 1);
			while (new_vec.used_ < used_) {
				new_vec.push_back(data_[new_vec.used_]);
			}
			new_vec.push_back(value);
			std::swap(*this, new_vec);
		} else {
			construct(data_ + used_, value);
			used_ += 1;
		}
	}

	void pop_back() {
		if (empty())
			throw std::runtime_error("Vector is empty");
		used_ -= 1;
		destroy(data_ + used_);
	}
#if 0

	Vector(size_t count, const T& value) : Buffer<T>(count) {
		assign(count, value);
	}

	Vector(std::initializer_list<T> init) : Buffer<T>(init.size()) {
		size_ = init.size();
		std::copy(init.begin(), init.end(), data_);
	}
	template <typename InputIt>
	Vector(const InputIt& first, const InputIt& last) : Buffer<T>(last - first) {
		std::copy(first, last, data_);
		size_ = last - first;
	}
	~Vector() = default;

public: //element access
	T& operator[](size_t pos) {
		return data_[pos];
	}
	const T& operator[](size_t pos) const {
		return data_[pos];
	}

	T* data() {
		return data_;
	}
	const T* data() const {
		return data_;
	}

public: //capacity
	void reserve(size_t count) {
		if (used_ > count || !count) return;
		T* tmp_buf = new T[count];
		std::copy(data_, data_ + size_, tmp_buf);
		delete[] data_;
		used_ = count;
		data_ = tmp_buf;
	}
	void shrink_to_fit() {
		if (size_ == used_) return;
		T* tmp_buf = new T[size_];
		std::copy(data_, data_ + size_, tmp_buf);
		delete[] data_;
		used_ = size_;
		data_ = tmp_buf;
	}

public: //modifiers
	void assign(size_t count, const T& value) {
		if (count > used_) reserve(count);
		size_ = size_ > count ? size_ : count;
		for (size_t i = 0; i != count; ++i)
			data_[i] = value;
	}
	void clear() {
		size_ = 0;
	}
	void push_back(const T& value) {
		emplace_back(value);
	}
	template <typename Arg>
	void emplace_back(Arg&& arg) {
		if (used_ < 4) reserve(4);
		if (size_ >= used_) reserve(used_ * 2);
		new (data_ + size_) T(std::forward<Arg>(arg));
		++size_;
	}
	iterator<T> insert(iterator<T> pos, const T& value) {
		return emplace(pos, value);
	}
	template <typename Arg>
	iterator<T> emplace(iterator<T> pos, Arg&& arg) {
		ptrdiff_t pos_ = pos - begin();
		if (pos == end()) emplace_back(std::forward<Arg>(arg));
		else new (data_ + pos_) T(std::forward<Arg>(arg));
		return  data_ + pos_;

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
	
	iterator<T> begin() const {
		return data_;
	}
	iterator<T> end() const {
		return data_ + size_;
	}
};

template <typename T> class iterator final {
	template <typename> friend class Vector;
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

	std::ptrdiff_t difference(const iterator& other) const {
		return ptr_ - other.ptr_;
	}
#endif
};
#if 0
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
iterator<T> operator+(const iterator<T>& it, size_t count)
{
	iterator<T> copy = it;
	copy += count;
	return copy;
}

template<typename T>
iterator<T> operator-(const iterator<T>& it, size_t count)
{
	iterator<T> copy = it;
	copy -= count;
	return copy;
}

template<typename T>
std::ptrdiff_t operator-(const iterator<T>& lhs, const iterator<T>& rhs)
{
	return lhs.difference(rhs);
}
#endif
}