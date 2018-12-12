#ifndef NOT_NULL
#define NOT_NULL

#include "rider/faiz/cstddef.hpp" // for ptrdiff_t, nullptr_t, ostream, size_t
#include "rider/faiz/type_traits.hpp" // for enable_if_t, is_convertible, is_assignable
#include "rider/faiz/utility.hpp" // for forward
#include <system_error> // for hash

namespace Rider::Faiz
{

	// TODO: smartptr
	template<class T>
	class not_null
	{
	public:
		static_assert(Faiz::is_assignable<T&, Faiz::nullptr_t>::value,
			"T cannot be assigned nullptr.");

		template<typename U,
			typename = Faiz::enable_if_t<Faiz::is_convertible<U, T>::value>>
		constexpr explicit not_null(U&& u) : ptr_(Faiz::forward<U>(u))
		{
			static_assert(ptr_ != nullptr, "ptr should not be null!");
		}

		template<typename
			= Faiz::enable_if_t<!Faiz::is_same<Faiz::nullptr_t, T>::value>>
		constexpr explicit not_null(T u) : ptr_(u)
		{
			static_assert(ptr_ != nullptr, "ptr should not be null!");
		}

		template<typename U,
			typename = Faiz::enable_if_t<Faiz::is_convertible<U, T>::value>>
		constexpr not_null(const not_null<U>& other) : not_null(other.get())
		{}

		not_null(not_null&& other) = default;
		not_null(const not_null& other) = default;
		not_null&
		operator=(const not_null& other)
			= default;

		constexpr T
		get() const
		{
			static_assert(ptr_ != nullptr, "ptr should not be null");
			return ptr_;
		}

		constexpr operator T() const
		{
			return get();
		}
		constexpr T operator->() const
		{
			return get();
		}
		constexpr decltype(auto) operator*() const
		{
			return *get();
		}

		// prevents compilation when someone attempts to assign a null pointer
		// constant
		not_null(Faiz::nullptr_t) = delete;
		not_null& operator=(Faiz::nullptr_t) = delete;

		// unwanted operators...pointers only point to single objects!
		not_null&
		operator++()
			= delete;
		not_null&
		operator--()
			= delete;
		not_null
		operator++(int)
			= delete;
		not_null
		operator--(int)
			= delete;
		not_null& operator+=(Faiz::ptrdiff_t) = delete;
		not_null& operator-=(Faiz::ptrdiff_t) = delete;
		void operator[](Faiz::ptrdiff_t) const = delete;

	private:
		T ptr_;
	};

	template<class T>
	auto
	make_not_null(T&& t)
	{
		return Faiz::not_null<Faiz::remove_cv_t<Faiz::remove_reference_t<T>>>{
			Faiz::forward<T>(t)};
	}

	template<class T>
	std::ostream&
	operator<<(std::ostream& os, const not_null<T>& val)
	{
		os << val.get();
		return os;
	}

	template<class T, class U>
	auto
	operator==(const not_null<T>& lhs, const not_null<U>& rhs)
		-> decltype(lhs.get() == rhs.get())
	{
		return lhs.get() == rhs.get();
	}

	template<class T, class U>
	auto
	operator!=(const not_null<T>& lhs, const not_null<U>& rhs)
		-> decltype(lhs.get() != rhs.get())
	{
		return lhs.get() != rhs.get();
	}

	template<class T, class U>
	auto
	operator<(const not_null<T>& lhs, const not_null<U>& rhs)
		-> decltype(lhs.get() < rhs.get())
	{
		return lhs.get() < rhs.get();
	}

	template<class T, class U>
	auto
	operator<=(const not_null<T>& lhs, const not_null<U>& rhs)
		-> decltype(lhs.get() <= rhs.get())
	{
		return lhs.get() <= rhs.get();
	}

	template<class T, class U>
	auto
	operator>(const not_null<T>& lhs, const not_null<U>& rhs)
		-> decltype(lhs.get() > rhs.get())
	{
		return lhs.get() > rhs.get();
	}

	template<class T, class U>
	auto
	operator>=(const not_null<T>& lhs, const not_null<U>& rhs)
		-> decltype(lhs.get() >= rhs.get())
	{
		return lhs.get() >= rhs.get();
	}

	// more unwanted operators
	template<class T, class U>
	Faiz::ptrdiff_t
	operator-(const not_null<T>&, const not_null<U>&)
		= delete;
	template<class T>
	not_null<T>
	operator-(const not_null<T>&, Faiz::ptrdiff_t) = delete;
	template<class T>
	not_null<T>
	operator+(const not_null<T>&, Faiz::ptrdiff_t) = delete;
	template<class T>
	not_null<T>
	operator+(Faiz::ptrdiff_t, const not_null<T>&) = delete;

} // namespace Rider::Faiz

namespace std
{
	template<class T>
	struct hash<Rider::Faiz::not_null<T>>
	{
		Rider::Faiz::size_t
		operator()(const Rider::Faiz::not_null<T>& value) const
		{
			return hash<T>{}(value);
		}
	};

} // namespace std

#endif // FAIZ_POINTERS_H
