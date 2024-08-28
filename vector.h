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

				AllocT allocator;

				if (!std::is_trivially_copyable<T>::value) {
					for (size_t index = 0; index < size; ++index) {
						std::allocator_traits<AllocT>::destroy(allocator, &data[index]);
					}
					if (newIndex != std::numeric_limits<size_t>::max()) {
						std::allocator_traits<AllocT>::destroy(allocator, &data[newIndex]);
					}
				}

				allocator.deallocate(data, capacity);
			}

			static void Emplace(T* dest, T* source, size_t size) {
				struct Emplacer {
					T* dest = nullptr;
					T* source = nullptr;
					size_t emp_size = 0;

					Emplacer(T* _dest, T* _source)
						: dest(_dest)
						, source(_source)
					{
					}

					void Emplace(T&& arg) {
						new(&dest[emp_size]) T(std::forward<T>(arg));
						//std::allocator_traits<AllocT>::
						++emp_size;
					}

					~Emplacer() {
						AllocT allocator;
						for (size_t index = 0; index < emp_size; ++index) {
							std::allocator_traits<AllocT>::destroy(allocator, &dest[index]);
						}
					}

					void Reset() {
						emp_size = 0;
					}
				};

				if (std::is_trivially_copyable<T>::value) {
					memcpy(dest, source, sizeof(T) * size);
				}
				else {
					Emplacer emplacer(dest, source);
					for (size_t index = 0; index < size; ++index) {
						emplacer.Emplace(std::forward<T>(source[index]));
					}
					emplacer.Reset();
				}
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

				Container::Emplace(tempContainer->data, _container->data, _container->size);
				tempContainer->size = _container->size;

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
