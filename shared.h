#pragma once
#include <iostream>

namespace mystd {

template <typename T>
class shared final {
public:
	shared()
		: _ptr(nullptr)
		, _counterPtr(nullptr)
	{
		std::cout << "shared" << std::endl;
	}

	shared(T* ptr)
		: _ptr(ptr)
		, _counterPtr(_ptr ? new unsigned int(1) : nullptr)
	{
		std::cout << "shared(T* )" << std::endl;
	}

	shared(const shared& ph)
		: _ptr(ph._ptr)
	{
		std::cout << "shared(const shared & )" << std::endl;

		_counterPtr = ph._counterPtr;
		++(*_counterPtr);
	}

	shared(shared&& ph) noexcept {
		std::cout << "shared(shared && )" << std::endl;

		std::swap(_ptr, ph._ptr);
		std::swap(_counterPtr, ph._counterPtr);
	}

	~shared() {
		if (!_counterPtr) {
			return;
		}

		--(*_counterPtr);
		
		if (*_counterPtr <= 0) {
			delete _ptr;
			delete _counterPtr;
		}
	}

	T& operator * () {
		return *_ptr;
	}

	T* operator -> () {
		return _ptr;
	}

	template <typename... Args>
	T& Ref(Args&&... defaultArgs) {
		if (_counterPtr && *_counterPtr > 0 && _ptr) {
			return *_ptr;
		}

		_ptr = new T(std::forward<Args>(defaultArgs)...);
		_counterPtr = new unsigned int(1);
		return *_ptr;
	}

	operator bool () const {
		return _ptr;
	}

	bool operator == (const shared& ph) const {
		return _ptr == ph._ptr;
	}

	bool operator == (const shared&& ph) const noexcept {
		return _ptr == ph._ptr;
	}

	shared& operator = (const shared& ph) {
		std::cout << "shared = (shared & )" << std::endl;

		if (this == &ph || _ptr == ph._ptr) {
			return *this;
		}

		shared();

		_ptr = ph._ptr;
		_counterPtr = ph.GetCounter();
	}

	shared& operator = (shared&& ph) noexcept {
		std::cout << "shared = (shared && )" << std::endl;

		std::swap(_ptr, ph._ptr);
		std::swap(_counterPtr, ph._counterPtr);
	}

	template <typename... Args>
	static shared make(Args&&... args) {
		return shared(new T(std::forward<Args>(args)...));
	}

	unsigned int Counters() {
		return _counterPtr ? *_counterPtr : 0;
	}

private:
	T* _ptr = nullptr;
	unsigned int* _counterPtr = nullptr;
};

}
