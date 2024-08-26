#pragma once

namespace mystd {
	template <typename T>
	class vector final {
	public:
		vector() = default;

		T& emplace_back(T&& value)
		{
			if (_size == _capacity) {
				size_t newCapacity = (_size * 2) + 1;
				T* tempData = new T[newCapacity];

				if (std::is_integral<T>()) {
					memcpy(tempData, _data, sizeof(T) * _size);
				}
				else {
					for (int i = 0; i < _size; ++i) {
						tempData[i] = _data[i];
					}
				}

				std::swap(_data, tempData);
				delete[] tempData;

				_capacity = newCapacity;
			}

			size_t lastIndex = _size++;
			_data[lastIndex] = value;			
			return _data[lastIndex];
		}

		T& operator[](size_t index)
		{
			return _data[index];
		}

		const T& operator[](size_t index) const 
		{
			return _data[index];
		}

		size_t size() const
		{
			return _size;
		}

	private:
		T* _data = nullptr;
		size_t _size = 0;
		size_t _capacity = 0;
	};
}
