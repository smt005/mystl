#pragma once

#include <algorithm>
#include <ostream>
#include <cstdint>

// ¬–≈Ã≈ÕÕŒ => œŒ—“ŒﬂÕÕŒ
#include <string>

namespace mystd {
	template <typename T>
	class string_iterator {
	public:
		string_iterator(T* ptr) : _ptr(ptr) {
		}

		string_iterator(const string_iterator& it) : _ptr(it._ptr) {
		}

		string_iterator(string_iterator&& it) : noexcept {
			std::swap(_ptr, it._ptr);
		}

		string_iterator& operator = (const string_iterator& it) {
			_ptr = it._ptr;
		}

		string_iterator& operator = (string_iterator&& it) {
			std::swap(_ptr, it._ptr);
		}

		T& operator * () {
			return *_ptr;
		}

		string_iterator& operator ++ () {
			++_ptr;
			return *this;
		}

		string_iterator& operator -- () {
			--_ptr;
			return *this;
		}

		string_iterator& operator ++ (int) {
			string_iterator temp(*this);
			++_ptr;
			return temp;
		}

		string_iterator& operator -- (int) {
			string_iterator temp(*this);
			--_ptr;
			return temp;
		}

		bool operator == (const string_iterator& it) const {
			return _ptr == it._ptr;
		}

		bool operator != (const string_iterator& it) const {
			return _ptr != it._ptr;
		}

	private:
		T* _ptr;
	};

	template <typename T>
	class string_base final {
	public:
		using iterator = string_iterator<T>;
		string_base()
			: _data(nullptr)
			, _size(0)
			, _capacity(0)
		{
			Log("string_base");
		}

		~string_base() {
			Log("~string_base " + (_data ? std::string(_data) : "EMPTY"));
		}

		string_base(const T* chars) {
			Log("string_base(const T *): " + (chars ? std::string(chars) : "EMPTY"));

			const size_t size = strlen(chars);
			if (size == 0) {
				_size = 0;
				_capacity = 0;
				return;
			}

			_data = new T[size + 1];
			if (!_data) {
				_size = 0;
				_capacity = 0;
				return;
			}

			if (errno_t err = strcpy_s(_data, size + 1, chars); err == 0) {
				_size = size;
				_capacity = size + 1;
			}
		}

		string_base(const string_base& str) {
			Log("string_base(const string_base &): " + (str._data ? std::string(str._data) : "EMPTY"));

			const size_t size = str._size;
			if (size == 0) {
				_size = 0;
				_capacity = 0;
				return;
			}

			_data = new T[size + 1];
			if (!_data) {
				_size = 0;
				_capacity = 0;
				return;
			}

			if (errno_t err = strcpy_s(_data, size + 1, str._data); err == 0) {
				_size = size;
				_capacity = size + 1;
			}
		}

		string_base(string_base&& str) noexcept {
			Log("string_base(string_base &&): " + std::string(str._data));

			std::swap(_data, str._data);
			std::swap(_size, str._size);
			std::swap(_capacity, str._capacity);
		}

		size_t size() const noexcept {
			return _size;
		}

		size_t length() const noexcept {
			return _size;
		}

		bool empty() const noexcept {
			return _size == 0;
		}

		T& operator[](const size_t index) noexcept {
			return _data[index];
		}

		const T& operator[](const size_t index) const noexcept {
			return _data[index];
		}

		string_base& operator = (const string_base& str) {
			Log("operator = (const string_base &): " + (str._data ? std::string(str._data) : "EMPTY"));

			if (!str._data) {
				return *this;
			}

			T* data = new T[str._capacity];
			if (!data) {
				return *this;
			}

			if (errno_t err = strcpy_s(data, str._capacity, str._data); err == 0) {
				_data = data;
				_size = str._size;
				_capacity = str._capacity;
			}

			return *this;
		}

		string_base& operator = (string_base&& str) noexcept {
			Log("operator = (string_base &&): " + (str._data ? std::string(str._data) : "EMPTY"));

			std::swap(_data, str._data);
			std::swap(_size, str._size);
			std::swap(_capacity, str._capacity);

			return *this;
		}

		string_base& operator + (const string_base& str) {
			Log("operator + (const string_base &): " + (_data ? std::string(_data) : "EMPTY") + " + " + (str._data ? std::string(str._data) : "EMPTY"));

			size_t size = _size + str._size;
			if ((size + 1) <= _capacity) {
				if (errno_t err = strcpy_s(_data + _size, (str._size + 1), str._data); err == 0) {
					_size = size;
				}
			}
			else {
				T* data = new T[size + 1];
				if (!data) {
					return *this;
				}


				if (errno_t err = strcpy_s(data, (_size + 1), _data); err == 0) {
					if (errno_t err = strcpy_s(data + _size, (str._size + 1), str._data); err == 0) {
						_data = data;
						_size = size;
						_capacity = size + 1;
					}
				}
			}

			return *this;
		}

		string_base& operator + (string_base&& str) {
			Log("operator + (string_base &&): " + (_data ? std::string(_data) : "EMPTY") + " + " + (str._data ? std::string(str._data) : "EMPTY"));

			size_t size = _size + str._size;
			if ((size + 1) <= _capacity) {
				if (errno_t err = strcpy_s(_data + _size, (str._size + 1), str._data); err == 0) {
					_size = size;
				}
			}
			else {
				T* data = new T[size + 1];
				if (!data) {
					return *this;
				}


				if (errno_t err = strcpy_s(data, (_size + 1), _data); err == 0) {
					if (errno_t err = strcpy_s(data + _size, (str._size + 1), str._data); err == 0) {
						_data = data;
						_size = size;
						_capacity = size + 1;
					}
				}
			}

			return *this;
		}

		T* data() {
			return _data;
		}

		bool operator == (const string_base& str) const {
			if (_size != str._size) {
				return false;
			}

			for (size_t index = 0; index < _size; ++index) {
				if (_data[index] != str._data[index]) {
					return false;
				}
			}

			return true;
		}

		bool operator != (const string_base& str) const {
			if (_size == str._size) {
				return false;
			}

			for (size_t index = 0; index < _size; ++index) {
				if (_data[index] != str._data[index]) {
					return true;
				}
			}

			return false;
		}

		friend
			std::ostream& operator << (std::ostream& out, const string_base& str) {
			if (str._enableLog) {
				if (str._data && str._size > 0) {
					out << "string_base [" << &str << "]: << " << str._data;
				}
				else {
					out << "string_base [" << &str << "]: << EMPTY";
				}
			}
			else {
				out << str._data;
			}
			return out;
		}

		iterator begin() {
			return iterator(&_data[0]);
		}

		iterator end() {
			return iterator(&_data[_size]);
		}

	private:
		T* _data;
		size_t _size;
		size_t _capacity;

	private:
		bool _enableLog = false;

		void Log(const std::string& text) {
			if (_enableLog) {
				std::cout << "string_base [" << this << "]: " << text << std::endl;
#ifdef _DEBUG
				std::uintptr_t ptr = reinterpret_cast<std::uintptr_t>(this);
				std::string logText = "string_base [" + std::to_string(ptr) + "]: " + text;
				_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, "LOG: %s\n", logText.c_str());
#endif
			}
		}
	};

	// Õ‡·Ó ÍÎ‡ÒÒÓ‚
	typedef string_base<char> string;
}
