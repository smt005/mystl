#pragma once
#include <iostream>

namespace mystd
{
	template <typename T>
	class shared_ptr final {
	public:
		shared_ptr()
			: _ptr(nullptr)
			, _counterPtr(nullptr)
		{}

		shared_ptr(T* ptr)
			: _ptr(ptr)
			, _counterPtr(_ptr ? new unsigned int(1) : nullptr)
		{}

		shared_ptr(const shared_ptr& ph)
			: _ptr(ph._ptr)
		{
			_counterPtr = ph._counterPtr;
			++(*_counterPtr);
		}

		shared_ptr(shared_ptr&& ph) noexcept {
			std::swap(_ptr, ph._ptr);
			std::swap(_counterPtr, ph._counterPtr);
		}

		~shared_ptr() {
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

		operator bool() const {
			return _ptr;
		}

		T& operator * () const {
			return *_ptr;
		}

		bool operator == (const shared_ptr& ph) const {
			return _ptr == ph._ptr;
		}

		bool operator == (const shared_ptr&& ph) const noexcept {
			return _ptr == ph._ptr;
		}

		shared_ptr& operator = (const shared_ptr& ph) {
			if (this == &ph || _ptr == ph._ptr) {
				return *this;
			}

			shared_ptr();

			if (ph._counterPtr && ph._ptr) {
				_ptr = ph._ptr;
				_counterPtr = ph._counterPtr;
				++(*_counterPtr);
			}
		}

		shared_ptr& operator = (shared_ptr&& ph) noexcept {
			std::swap(_ptr, ph._ptr);
			std::swap(_counterPtr, ph._counterPtr);
		}

		template <typename... Args>
		static shared_ptr make(Args&&... args) {
			return shared_ptr(new T(std::forward<Args>(args)...));
		}

		unsigned int Counters() {
			return _counterPtr ? *_counterPtr : 0;
		}

	private:
		T* _ptr = nullptr;
		unsigned int* _counterPtr = nullptr;
	};
}
