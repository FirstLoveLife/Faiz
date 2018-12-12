#ifndef OBSERVER_PTR
#define OBSERVER_PTR
#include "rider/faiz/debug.hpp"
#include "rider/faiz/math/RelationAlgebra.hpp"
namespace Rider::Faiz
{
	template<typename T>
	class observer_ptr : private totally_ordered<observer_ptr<T>>,
						 private equality_comparable<observer_ptr<T>, nullptr_t>
	{
	public:
		using element_type = T;
		using pointer = add_pointer_t<element_type>;
		using reference = add_lvalue_reference_t<element_type>;

	private:
		T* ptr{};

	public:
		constexpr observer_ptr() noexcept = default;
		constexpr observer_ptr(nullptr_t) noexcept : ptr()
		{}
		explicit constexpr observer_ptr(pointer p) noexcept : ptr(p)
		{}
		template<typename Other>
		constexpr observer_ptr(observer_ptr<Other> other) noexcept
			: ptr(other.get())
		{}

		constexpr reference operator*() const noexcept
		{
			Expects(get() != nullptr);
			return *ptr;
		}

		constexpr pointer operator->() const noexcept
		{
			return ptr;
		}

		friend constexpr bool
		operator==(observer_ptr p, nullptr_t) noexcept
		{
			return !p.ptr;
		}

		explicit constexpr operator bool() const noexcept
		{
			return ptr;
		}

		explicit constexpr operator pointer() const noexcept
		{
			return ptr;
		}

		constexpr pointer
		get() const noexcept
		{
			return ptr;
		}

		constexpr pointer
		release() noexcept
		{
			const auto res(ptr);

			reset();
			return res;
		}

		constexpr void
		reset(pointer p = {}) noexcept
		{
			ptr = p;
		}

		constexpr void
		swap(observer_ptr& other) noexcept
		{
			// TODO: use Faiz::swap
			std::swap(ptr, other.ptr);
		}
	};
} // namespace Rider::Faiz
#endif
