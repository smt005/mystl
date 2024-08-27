#pragma once

#include <memory>

namespace mystd {
	template <typename T, typename AllocT = std::allocator<T>>
	class vector final {
	private:
		struct Container {
			T* data = nullptr;
			size_t capacity = 0;
			size_t size = 0;
			size_t newIndex = std::numeric_limits<size_t>::max();

			Container() = default;
			Container(size_t _capacity)
				: capacity(_capacity) {
				AllocT allocator;
				data = allocator.allocate(capacity);
			}
			~Container() {
				if (!data) {
					return;
				}

				if (!std::is_trivially_copyable<T>::value) {
					for (size_t index = 0; index < size; ++index) {
						data[index].~T();
					}
					if (newIndex != std::numeric_limits<size_t>::max()) {
						data[newIndex].~T();
					}
				}

				AllocT allocator;
				allocator.deallocate(data, capacity);

				data = nullptr;
				capacity = 0;
				size = 0;
				newIndex = std::numeric_limits<size_t>::max();
			}
		};

	public:
		template<typename U>
		T& emplace_back(U&& arg)
		{
			if (!_container) {
				_container = std::make_unique<Container>();
			}
			if (_container->size == _container->capacity) {
				size_t newCapacity = static_cast<size_t>(static_cast<float>(_container->size) * 1.3f) + 1;
				std::unique_ptr<Container> tempContainer = std::make_unique<Container>(newCapacity);

				new(&tempContainer->data[_container->size]) T(arg);
				tempContainer->newIndex = _container->size;

				if (std::is_trivially_copyable<T>::value) {
					memcpy(tempContainer->data, _container->data, sizeof(T) * _container->size);
					tempContainer->size = _container->size;
				}
				else if (std::is_nothrow_move_constructible<T>::value) {
					for (size_t index = 0; index < _container->size; ++index) {
						new(&tempContainer->data[index]) T(std::forward<T&&>(_container->data[index]));
						++tempContainer->size;
					}
				}
				else {
					for (size_t index = 0; index < _container->size; ++index) {
						new(&tempContainer->data[index]) T(_container->data[index]);
						++tempContainer->size;
					}
				}

				tempContainer->size = tempContainer->newIndex + 1;
				tempContainer->newIndex = std::numeric_limits<size_t>::max();
				std::swap(_container, tempContainer);
			}
			else {
				new(&_container->data[_container->size]) T(std::forward<U>(arg));
				++_container->size;
			}

			return _container->data[_container->size - 1];
		}

		T& operator[](size_t index)
		{
			return _container->data[index];
		}

		const T& operator[](size_t index) const 
		{
			return _container->data[index];
		}

		size_t size() const
		{
			return _container->size;
		}

	private:
		std::unique_ptr<Container> _container;
	};
}
