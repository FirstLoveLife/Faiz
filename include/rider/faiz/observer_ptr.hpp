#ifndef OBSERVER_PTR
#define OBSERVER_PTR
#include "rider/faiz/debug.hpp"
#include "rider/faiz/macros.hpp"
#include "rider/faiz/math/RelationAlgebra.hpp"
namespace Rider::Faiz
{
	tpl<typ T> class observer_ptr
		: private totally_ordered<observer_ptr<T>>,
		  private equality_comparable<observer_ptr<T>, nullptr_t>
	{
	public:
		using element_type = T;
		using pointer = add_pointer_t<element_type>;
		using reference = add_lvalue_reference_t<element_type>;

	private:
		T* ptr{};

	public:
		cexp
		observer_ptr() noexcept
			= default;
		cexp observer_ptr(nullptr_t) noexcept : ptr()
		{}
		explicit cexp
		observer_ptr(pointer p) noexcept
			: ptr(p)
		{}
		tpl<typ Other> cexp
		observer_ptr(observer_ptr<Other> other) noexcept
			: ptr(other.get())
		{}

		cexp reference operator*() const noexcept
		{
			Expects(get() != nullptr);
			return *ptr;
		}

		cexp pointer operator->() const noexcept
		{
			return ptr;
		}

		friend cexp bool
		operator==(observer_ptr p, nullptr_t) noexcept
		{
			return !p.ptr;
		}

		explicit cexp operator bool() const noexcept
		{
			return ptr;
		}

		explicit cexp operator pointer() const noexcept
		{
			return ptr;
		}

		cexp pointer
		get() const noexcept
		{
			return ptr;
		}

		cexp pointer
		release() noexcept
		{
			const auto res(ptr);

			reset();
			return res;
		}

		cexp void
		reset(pointer p = {}) noexcept
		{
			ptr = p;
		}

		cexp void
		swap(observer_ptr& other) noexcept
		{
			// TODO: use Faiz::swap
			std::swap(ptr, other.ptr);
		}
	};
} // namespace Rider::Faiz
#endif
