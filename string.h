#pragma once

#include <algorithm>
#include <ostream>
#include <string>
#include <cstdint>

namespace mystl {
	template <typename T>
	class string_base final {
	public:
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
			Log("string_base(string_base &&): " + str);

			std::swap(_data, str._data);
			std::swap(_size, str._size);
			std::swap(_capacity, str._capacity);
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

	private:
		T* _data;
		size_t _size;
		size_t _capacity;

	private:
		bool _enableLog = true;

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

	// ����� �������
	typedef string_base<char> string;
}