#ifndef ITERATOR
#define ITERATOR
#include "rider/faiz/char_traits.hpp"
#include "rider/faiz/cstddef.hpp"
#include "rider/faiz/initializer_list.hpp"
#include "rider/faiz/type_traits.hpp"
#include "rider/faiz/utility.hpp"

/*
p0174 http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0174r2.html#2.1
 */

#include <ostream>
namespace rider::faiz
{
	struct input_iterator_tag
	{};
	struct output_iterator_tag
	{};
	struct forward_iterator_tag : public input_iterator_tag
	{};
	struct bidirectional_iterator_tag : public forward_iterator_tag
	{};
	struct random_access_iterator_tag : public bidirectional_iterator_tag
	{};

	template<class T>
	struct has_iterator_category
	{
	private:
		struct two
		{
			char lx;
			char lxx;
		};
		template<class U>
		static two
		test(...);
		template<class U>
		static char
		test(typename U::iterator_category* = 0);

	public:
		static constexpr bool value = sizeof(test<T>(0)) == 1;
	};

	template<class Iter, bool>
	struct iterator_traits_impl
	{};

	template<class Iter>
	struct iterator_traits_impl<Iter, true>
	{
		using difference_type = typename Iter::difference_type;
		using value_type = typename Iter::value_type;
		using pointer = typename Iter::pointer;
		using reference = typename Iter::reference;
		using iterator_category = typename Iter::iterator_category;
	};

	template<class Iter, bool>
	struct iterator_traits_aux
	{};

	template<class Iter>
	struct iterator_traits_aux<Iter, true>
		: iterator_traits_impl<Iter,
			  faiz::is_convertible_v<typename Iter::iterator_category,
				  input_iterator_tag> || is_convertible_v<typename Iter::iterator_category, output_iterator_tag>>
	{};
	// `std::iterator_traits` is the type trait class that provides uniform
	// interface to the properties of Iterator types. This makes it possible to
	// implement algorithms only in terms of iterators.
	// The class defines the following types:
	//
	// * difference_type - a signed integer type that can be used to identify
	// distance between iterators
	// * value_type - the type of the values that can be obtained by
	// dereferencing the iterator. This type is void for output iterators.
	// * pointer - defines a pointer to the type iterated over (value_type)
	// * reference - defines a reference to the type iterated over (value_type)
	// * iterator_category - the category of the iterator. Must be one of
	// iterator category tags.
	//
	//  The template can be specialized for user-defined iterators so that the
	// information about the iterator can be retrieved even if the type does not
	// provide the usual typedefs.
	//
	//
	// If Iterator does not have all five member types difference_type,
	// value_type, pointer, reference, and iterator_category, then this template
	// has no members by any of those names (`std::iterator_traits` is
	// `SFINAE-friendly`)
	template<class Iter>
	struct iterator_traits
		: iterator_traits_aux<Iter, has_iterator_category<Iter>::value>
	{};


	// This type trait may be specialized for user-provided types that may be
	// used as iterators. The standard library provides two partial
	// specializations for pointer types T*, which makes it possible to use all
	// iterator-based algorithms with raw pointers.
	template<class T>
	struct iterator_traits<T*>
	{
		using difference_type = faiz::ptrdiff_t;
		using value_type = remove_cv_t<T>;
		using pointer = T*;
		using reference = T&;
		using iterator_category = random_access_iterator_tag;
	};

	template<class T,
		class U,
		bool = has_iterator_category<iterator_traits<T>>::value>
	struct has_iterator_category_convertible_to
		: public integral_constant<bool,
			  is_convertible_v<typename iterator_traits<T>::iterator_category,
				  U>>
	{};

	template<class T, class U>
	struct has_iterator_category_convertible_to<T, U, false> : public false_type
	{};

	template<class T>
	struct is_input_iterator
		: public has_iterator_category_convertible_to<T, input_iterator_tag>
	{};

	template<class T>
	struct is_forward_iterator
		: public has_iterator_category_convertible_to<T, forward_iterator_tag>
	{};

	template<class T>
	struct is_bidirectional_iterator
		: public has_iterator_category_convertible_to<T,
			  bidirectional_iterator_tag>
	{};

	template<class T>
	struct is_random_access_iterator
		: public has_iterator_category_convertible_to<T,
			  random_access_iterator_tag>
	{};

	template<class T>
	struct is_exactly_input_iterator
		: public integral_constant<bool,
			  has_iterator_category_convertible_to<T, input_iterator_tag>::value
				  && !has_iterator_category_convertible_to<T,
						 forward_iterator_tag>::value>
	{};


	template<class InputIter>
	constexpr void
	advance(InputIter& i,
		typename iterator_traits<InputIter>::difference_type n,
		input_iterator_tag)
	{
		for(; n > 0; --n)
			++i;
	}

	template<class BiDirIter>
	constexpr void
	advance(BiDirIter& i,
		typename iterator_traits<BiDirIter>::difference_type n,
		bidirectional_iterator_tag)
	{
		if(n >= 0)
			for(; n > 0; --n)
				++i;
		else
			for(; n < 0; ++n)
				--i;
	}

	template<class RandIter>
	constexpr void
	advance(RandIter& i,
		typename iterator_traits<RandIter>::difference_type n,
		random_access_iterator_tag)
	{
		i += n;
	}

	template<class InputIter>
	constexpr void
	advance(
		InputIter& i, typename iterator_traits<InputIter>::difference_type n)
	{
		advance(i, n, typename iterator_traits<InputIter>::iterator_category());
	}

	template<class InputIter>
	constexpr typename iterator_traits<InputIter>::difference_type
	distance(InputIter first, InputIter last, input_iterator_tag)
	{
		typename iterator_traits<InputIter>::difference_type r(0);
		for(; first != last; ++first)
			++r;
		return r;
	}

	template<class RandIter>
	constexpr typename iterator_traits<RandIter>::difference_type
	distance(RandIter first, RandIter last, random_access_iterator_tag)
	{
		return last - first;
	}

	template<class InputIter>
	constexpr typename iterator_traits<InputIter>::difference_type
	distance(InputIter first, InputIter last)
	{
		return distance(first,
			last,
			typename iterator_traits<InputIter>::iterator_category());
	}

	//-InputIt must meet the requirements of InputIterator.
	// Return the nth successor of iterator it.
	template<class InputIter>
	constexpr enable_if_t<is_input_iterator<InputIter>::value, InputIter>
	next(
		InputIter x, typename iterator_traits<InputIter>::difference_type n = 1)
	{
		faiz::advance(x, n);
		return x;
	}

	template<class BidirectionalIter>
	constexpr enable_if_t<is_bidirectional_iterator<BidirectionalIter>::value,
		BidirectionalIter>
	prev(BidirectionalIter x,
		typename iterator_traits<BidirectionalIter>::difference_type n = 1)
	{
		faiz::advance(x, -n);
		return x;
	}


	template<class T, class = void>
	struct is_stashing_iterator : false_type
	{};

	template<class T>
	struct is_stashing_iterator<T,
		faiz::void_t<typename T::stashing_iterator_tag>> : true_type
	{};

	template<class Iter>
	class reverse_iterator
	{
	private:
		static_assert(!is_stashing_iterator<Iter>::value,
			"The specified iterator type cannot be used with reverse_iterator; "
			"Using stashing iterators with reverse_iterator causes undefined "
			"behavior");

	protected:
		Iter current;

	public:
		using value_type = typename faiz::iterator_traits<Iter>::value_type;
		using difference_type =
			typename faiz::iterator_traits<Iter>::difference_type;
		using pointer = typename faiz::iterator_traits<Iter>::pointer;
		using reference = typename faiz::iterator_traits<Iter>::reference;
		using iterator_category =
			typename faiz::iterator_traits<Iter>::iterator_category;


		using iterator_type = Iter;

		constexpr reverse_iterator() : current()
		{}

		constexpr explicit reverse_iterator(Iter x) : current(x)
		{}
		template<class U>
		constexpr reverse_iterator(const reverse_iterator<U>& u)
			: current(u.base())
		{}
		template<class U>

		constexpr reverse_iterator&
		operator=(const reverse_iterator<U>& u)
		{
			current = u.base();
			return *this;
		}
		constexpr Iter
		base() const
		{
			return current;
		}
		constexpr reference operator*() const
		{
			Iter tmp = current;
			return *--tmp;
		}
		constexpr pointer operator->() const
		{
			return faiz::addressof(operator*());
		}

		constexpr reverse_iterator&
		operator++()
		{
			--current;
			return *this;
		}
		constexpr reverse_iterator
		operator++(int)
		{
			reverse_iterator tmp(*this);
			--current;
			return tmp;
		}

		constexpr reverse_iterator&
		operator--()
		{
			++current;
			return *this;
		}
		constexpr reverse_iterator
		operator--(int)
		{
			reverse_iterator tmp(*this);
			++current;
			return tmp;
		}
		constexpr reverse_iterator
		operator+(difference_type n) const
		{
			return reverse_iterator(current - n);
		}

		constexpr reverse_iterator&
		operator+=(difference_type n)
		{
			current -= n;
			return *this;
		}
		constexpr reverse_iterator
		operator-(difference_type n) const
		{
			return reverse_iterator(current + n);
		}

		constexpr reverse_iterator&
		operator-=(difference_type n)
		{
			current += n;
			return *this;
		}
		constexpr reference operator[](difference_type n) const
		{
			return *(*this + n);
		}
	};

	template<class Iter1, class Iter2>
	constexpr bool
	operator==(
		const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y)
	{
		return x.base() == y.base();
	}

	template<class Iter1, class Iter2>
	constexpr bool
	operator<(
		const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y)
	{
		return x.base() > y.base();
	}

	template<class Iter1, class Iter2>
	constexpr bool
	operator!=(
		const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y)
	{
		return x.base() != y.base();
	}

	template<class Iter1, class Iter2>
	constexpr bool
	operator>(
		const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y)
	{
		return x.base() < y.base();
	}

	template<class Iter1, class Iter2>
	constexpr bool
	operator>=(
		const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y)
	{
		return x.base() <= y.base();
	}

	template<class Iter1, class Iter2>
	constexpr bool
	operator<=(
		const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y)
	{
		return x.base() >= y.base();
	}

	template<class Iter1, class Iter2>
	constexpr auto
	operator-(const reverse_iterator<Iter1>& x,
		const reverse_iterator<Iter2>& y) -> decltype(y.base() - x.base())
	{
		return y.base() - x.base();
	}

	template<class Iter>
	constexpr reverse_iterator<Iter>
	operator+(typename reverse_iterator<Iter>::difference_type n,
		const reverse_iterator<Iter>& x)
	{
		return reverse_iterator<Iter>(x.base() - n);
	}

	template<class Iter>
	constexpr reverse_iterator<Iter>
	make_reverse_iterator(Iter i)
	{
		return reverse_iterator<Iter>(i);
	}

	// `std::back_insert_iterator` is an OutputIterator that appends to a
	// container for which it was constructed. The container's `push_back()`
	// member function is called whenever the iterator (whether dereferenced or
	// not) is assigned to. Incrementing the `std::back_insert_iterator` is a
	// *no-op*.
	template<class Container>
	class back_insert_iterator
	{
	protected:
		Container* container;

	public:
		using iterator_category = output_iterator_tag;
		using value_type = void;
		using difference_type = void;
		using pointer = void;
		using reference = void;

		using container_type = Container;

		explicit back_insert_iterator(Container& x)
			: container(faiz::addressof(x))
		{}
		back_insert_iterator&
		operator=(const typename Container::value_type& value)
		{
			container->push_back(value);
			return *this;
		}
		back_insert_iterator&
		operator=(typename Container::value_type&& value)
		{
			container->push_back(faiz::move(value));
			return *this;
		}
		back_insert_iterator& operator*()
		{
			return *this;
		}
		back_insert_iterator&
		operator++()
		{
			return *this;
		}
		back_insert_iterator
		operator++(int)
		{
			return *this;
		}
	};

	// `back_inserter` is a convenience function template that constructs a
	// `std::back_insert_iterator` for the container c with the type deduced
	// from the type of the argument.
	template<class Container>
	inline back_insert_iterator<Container>
	back_inserter(Container& x)
	{
		return back_insert_iterator<Container>(x);
	}

	// `std::front_insert_iterator` is an OutputIterator that prepends
	// elements to a container for which it was constructed. The container's
	// `push_front()` member function is called whenever the iterator (whether
	// dereferenced or not) is assigned to. Incrementing the
	// `std::front_insert_iterator` is a *no-op*.
	template<class Container>
	class front_insert_iterator
	{
	protected:
		Container* container;

	public:
		using iterator_category = output_iterator_tag;
		using value_type = void;
		using difference_type = void;
		using pointer = void;
		using reference = void;

		using container_type = Container;

		explicit front_insert_iterator(Container& x)
			: container(faiz::addressof(x))
		{}
		front_insert_iterator&
		operator=(const typename Container::value_type& value)
		{
			container->push_front(value);
			return *this;
		}
		front_insert_iterator&
		operator=(typename Container::value_type&& value)
		{
			container->push_front(faiz::move(value));
			return *this;
		}
		front_insert_iterator& operator*()
		{
			return *this;
		}
		front_insert_iterator&
		operator++()
		{
			return *this;
		}
		front_insert_iterator
		operator++(int)
		{
			return *this;
		}
	};

	// front_inserter is a convenience function template that constructs a
	// `std::front_insert_iterator` for the container c with the type deduced
	// from the type of the argument.
	template<class Container>
	inline front_insert_iterator<Container>
	front_inserter(Container& x)
	{
		return front_insert_iterator<Container>(x);
	}

	// `std::insert_iterator` is an OutputIterator that inserts elements into
	// a container for which it was constructed, at the position pointed to by
	// the supplied iterator. The container's `insert()` member function is
	// called whenever the iterator (whether dereferenced or not) is assigned
	// to. Incrementing the std::insert_iterator is a no-op.
	template<class Container>
	class insert_iterator
	{
	protected:
		Container* container;
		typename Container::iterator iter;

	public:
		using iterator_category = output_iterator_tag;
		using value_type = void;
		using difference_type = void;
		using pointer = void;
		using reference = void;
		using container_type = Container;


		insert_iterator(Container& x, typename Container::iterator i)
			: container(faiz::addressof(x)), iter(i)
		{}
		insert_iterator&
		operator=(const typename Container::value_type& value)
		{
			iter = container->insert(iter, value);
			++iter;
			return *this;
		}
		insert_iterator&
		operator=(typename Container::value_type&& value)
		{
			iter = container->insert(iter, faiz::move(value));
			++iter;
			return *this;
		}
		insert_iterator& operator*()
		{
			return *this;
		}
		insert_iterator&
		operator++()
		{
			return *this;
		}
		insert_iterator&
		operator++(int)
		{
			return *this;
		}
	};

	// inserter is a convenience function template that constructs a
	// std::insert_iterator for the container c and its iterator i with the type
	// deduced from the type of the argument.
	template<class Container>
	inline insert_iterator<Container>
	inserter(Container& x, typename Container::iterator i)
	{
		return insert_iterator<Container>(x, i);
	}

	// `std::istream_iterator` is an iterator for formatted extraction. For
	// instance, if you have a line of integers from a file and wish to copy
	// them to some container, you would use `std::istream_iterator<int>` which
	// internally will copy the value extracted from an int (using
	// `operator>>()`) to the container:
	//
	// ```cpp
	//  std::copy(std::istream_iterator<int>(file),
	//   std::istream_iterator<int>(),
	//  std::back_inserter(some_container));
	//  ```
	//
	template<class T,
		class CharT = char,
		class Traits = char_traits<CharT>,
		class Distance = ptrdiff_t>
	class istream_iterator
	{
	public:
		using iterator_category = input_iterator_tag;
		using value_type = T;
		using difference_type = Distance;
		using pointer = const T*;
		using reference = const T&;


		using char_type = CharT;
		using traits_type = Traits;
		using istream_type = std::basic_istream<CharT, Traits>;

	private:
		istream_type* in_stream_;
		T value;

	public:
		constexpr istream_iterator() : in_stream_(0), value()
		{}
		istream_iterator(istream_type& s) : in_stream_(faiz::addressof(s))
		{
			if(!(*in_stream_ >> value))
				in_stream_ = 0;
		}

		const T& operator*() const
		{
			return value;
		}
		const T* operator->() const
		{
			return faiz::addressof((operator*()));
		}
		istream_iterator&
		operator++()
		{
			if(!(*in_stream_ >> value))
				in_stream_ = 0;
			return *this;
		}
		istream_iterator
		operator++(int)
		{
			istream_iterator t(*this);
			++(*this);
			return t;
		}

		template<class U, class _CharU, class TraitsU, class DistanceU>
		friend bool
		operator==(const istream_iterator<U, _CharU, TraitsU, DistanceU>& x,
			const istream_iterator<U, _CharU, TraitsU, DistanceU>& y);

		template<class U, class _CharU, class TraitsU, class DistanceU>
		friend bool
		operator==(const istream_iterator<U, _CharU, TraitsU, DistanceU>& x,
			const istream_iterator<U, _CharU, TraitsU, DistanceU>& y);
	};

	template<class T, class CharT, class Traits, class Distance>
	inline bool
	operator==(const istream_iterator<T, CharT, Traits, Distance>& x,
		const istream_iterator<T, CharT, Traits, Distance>& y)
	{
		return x.in_stream_ == y.in_stream_;
	}

	template<class T, class CharT, class Traits, class Distance>
	inline bool
	operator!=(const istream_iterator<T, CharT, Traits, Distance>& x,
		const istream_iterator<T, CharT, Traits, Distance>& y)
	{
		return !(x == y);
	}

	// `std::ostream_iterator` is a single-pass OutputIterator that writes
	// successive objects of type T into the `std::basic_ostream` object for
	// which it was constructed, using `operator<<`. Optional delimiter string
	// is written to the output stream after every write operation. The write
	// operation is performed when the iterator (whether dereferenced or not) is
	// assigned to. Incrementing the `std::ostream_iterator` is a *no-op*.
	//
	// In a typical implementation, the only data members of
	// std::ostream_iterator are a pointer to the associated std::basic_ostream
	// and a pointer to the first character in the delimiter string.
	//
	// When writing characters, std::ostreambuf_iterator is more efficient,
	// since it avoids the overhead of constructing and destructing the sentry
	// object once per character.
	//
	template<class T, class CharT = char, class Traits = char_traits<CharT>>
	class ostream_iterator
	{
	public:
		using iterator_category = output_iterator_tag;
		using value_type = void;
		using difference_type = void;
		using pointer = void;
		using reference = void;

		using char_type = CharT;
		using traits_type = Traits;
		using ostream_type = std::basic_ostream<CharT, Traits>;

	private:
		ostream_type* out_stream_;
		const char_type* delim_;

	public:
		ostream_iterator(ostream_type& s) noexcept
			: out_stream_(faiz::addressof(s)), delim_(0)
		{}

		ostream_iterator(ostream_type& s, const CharT* delimiter) noexcept
			: out_stream_(faiz::addressof(s)), delim_(delimiter)
		{}
		ostream_iterator&
		operator=(const T& value)
		{
			*out_stream_ << value;
			if(delim_)
				*out_stream_ << delim_;
			return *this;
		}

		ostream_iterator& operator*()
		{
			return *this;
		}
		ostream_iterator&
		operator++()
		{
			return *this;
		}
		ostream_iterator&
		operator++(int)
		{
			return *this;
		}
	};

	// `faiz::ostreambuf_iterator` is a single-pass OutputIterator that writes
	// successive characters into the `std::basic_streambuf` object for which
	// it was constructed. The actual write operation is performed when the
	// iterator (whether dereferenced or not) is assigned to. Incrementing the
	// `std::ostreambuf_iterator` is a `no-op`.
	//
	// `pos_type` is used for absolute positions in the stream
	// `off_type` is used for relative positions, `off_type` can go somewhere
	// precomputed.
	//
	// `std::istreambuf_iterator` is an iterator for unformatted extraction. It
	// works directly on the `std::streambuf` object provided through its
	// constructor. As such, if you need simply the contents of the file without
	// worrying about their format, use this iterator. For example, sometimes
	// you want to read an entire file into a string or some container. A
	// regular formatted extractor will discard leading whitespace and convert
	// extracted tokens; the buffer iterator will not:
	// ```cpp
	// std::string str(std::istreambuf_iterator<char>{file}, {});
	// ```
	template<class CharT, class Traits = faiz::char_traits<CharT>>
	class istreambuf_iterator
	{
	public:
		using iterator_category = input_iterator_tag;
		using value_type = CharT;
		using difference_type = typename Traits::off_type;
		using pointer = CharT*;
		using reference = CharT;

		using char_type = CharT;
		using traits_type = Traits;
		using int_type = typename Traits::int_type;
		using streambuf_type = std::basic_streambuf<CharT, Traits>;
		using istream_type = std::basic_istream<CharT, Traits>;

	private:
		mutable streambuf_type* sbuf;

		class proxy
		{
			char_type keep;
			streambuf_type* sbuf;

			proxy(char_type c, streambuf_type* s) : keep(c), sbuf(s)
			{}
			friend class istreambuf_iterator;

		public:
			char_type operator*() const
			{
				return keep;
			}
		};


		bool
		test_for_eof() const
		{
			if(sbuf
				&& traits_type::eq_int_type(sbuf->sgetc(), traits_type::eof()))
				sbuf = 0;
			return sbuf == 0;
		}

	public:
		constexpr istreambuf_iterator() noexcept : sbuf(0)
		{}

		istreambuf_iterator(istream_type& s) noexcept : sbuf(s.rdbuf())
		{}

		istreambuf_iterator(streambuf_type* s) noexcept : sbuf(s)
		{}

		istreambuf_iterator(const proxy& p) noexcept : sbuf(p.sbuf)
		{}

		char_type operator*() const
		{
			return static_cast<char_type>(sbuf->sgetc());
		}
		istreambuf_iterator&
		operator++()
		{
			sbuf->sbumpc();
			return *this;
		}
		proxy
		operator++(int)
		{
			return proxy(sbuf->sbumpc(), sbuf);
		}

		bool
		equal(const istreambuf_iterator& b) const
		{
			return test_for_eof() == b.test_for_eof();
		}
	};

	template<class CharT, class Traits>
	inline bool
	operator==(const istreambuf_iterator<CharT, Traits>& a,
		const istreambuf_iterator<CharT, Traits>& b)
	{
		return a.equal(b);
	}

	template<class CharT, class Traits>
	inline bool
	operator!=(const istreambuf_iterator<CharT, Traits>& a,
		const istreambuf_iterator<CharT, Traits>& b)
	{
		return !a.equal(b);
	}

	// `std::ostreambuf_iterator` is a single-pass OutputIterator that writes
	// successive characters into the `std::basic_streambuf` object for which it
	// was constructed. The actual write operation is performed when the
	// iterator (whether dereferenced or not) is assigned to. Incrementing the
	// `std::ostreambuf_iterator` is a *no-op*.
	//
	// In a typical implementation, the only data members of
	// `std::ostreambuf_iterator` are a pointer to the associated
	// `std::basic_streambuf` and a boolean flag indicating if the the end of
	// file condition has been reached.
	template<class CharT, class Traits>
	class ostreambuf_iterator
	{
	public:
		using iterator_category = output_iterator_tag;

		using char_type = CharT;
		using traits_type = Traits;
		using streambuf_type = std::basic_streambuf<CharT, Traits>;
		using ostream_type = std::basic_ostream<CharT, Traits>;

	private:
		streambuf_type* sbuf;

	public:
		ostreambuf_iterator(ostream_type& s) noexcept : sbuf(s.rdbuf())
		{}

		ostreambuf_iterator(streambuf_type* s) noexcept : sbuf(s)
		{}
		ostreambuf_iterator&
		operator=(CharT c)
		{
			if(sbuf
				&& traits_type::eq_int_type(sbuf->sputc(c), traits_type::eof()))
				sbuf = 0;
			return *this;
		}
		ostreambuf_iterator& operator*()
		{
			return *this;
		}
		ostreambuf_iterator&
		operator++()
		{
			return *this;
		}
		ostreambuf_iterator&
		operator++(int)
		{
			return *this;
		}
		bool
		failed() const noexcept
		{
			return sbuf == 0;
		}
	};

	// `std::move_iterator` is an iterator adaptor which behaves exactly like
	// the underlying iterator (which must be at least an InputIterator), except
	// that dereferencing converts the value returned by the underlying iterator
	// into an rvalue. If this iterator is used as an input iterator, the effect
	// is that the values are moved from, rather than copied from.
	template<class Iter>
	class move_iterator
	{
	private:
		Iter i;

	public:
		using iterator_type = Iter;

		using iterator_category =
			typename iterator_traits<iterator_type>::iterator_category;

		using value_type = typename iterator_traits<iterator_type>::value_type;

		using difference_type =
			typename iterator_traits<iterator_type>::difference_type;

		using pointer = iterator_type;

		using reference_aux =
			typename iterator_traits<iterator_type>::reference;

		using reference = conditional_t<is_reference_v<reference_aux>,
			remove_reference_t<reference_aux>&&,
			reference_aux>;

		constexpr move_iterator() : i()
		{}

		constexpr explicit move_iterator(Iter x) : i(x)
		{}
		template<class U>
		constexpr move_iterator(const move_iterator<U>& u) : i(u.base())
		{}
		constexpr Iter
		base() const
		{
			return i;
		}
		constexpr reference operator*() const
		{
			return static_cast<reference>(*i);
		}
		constexpr pointer operator->() const
		{
			return i;
		}
		constexpr move_iterator&
		operator++()
		{
			++i;
			return *this;
		}
		constexpr move_iterator
		operator++(int)
		{
			move_iterator tmp(*this);
			++i;
			return tmp;
		}
		constexpr move_iterator&
		operator--()
		{
			--i;
			return *this;
		}
		constexpr move_iterator
		operator--(int)
		{
			move_iterator tmp(*this);
			--i;
			return tmp;
		}
		constexpr move_iterator
		operator+(difference_type n) const
		{
			return move_iterator(i + n);
		}
		constexpr move_iterator&
		operator+=(difference_type n)
		{
			i += n;
			return *this;
		}
		constexpr move_iterator
		operator-(difference_type n) const
		{
			return move_iterator(i - n);
		}
		constexpr move_iterator&
		operator-=(difference_type n)
		{
			i -= n;
			return *this;
		}
		constexpr reference operator[](difference_type n) const
		{
			return static_cast<reference>(i[n]);
		}
	};

	template<class Iter1, class Iter2>
	constexpr bool
	operator==(const move_iterator<Iter1>& x, const move_iterator<Iter2>& y)
	{
		return x.base() == y.base();
	}

	template<class Iter1, class Iter2>
	constexpr bool
	operator<(const move_iterator<Iter1>& x, const move_iterator<Iter2>& y)
	{
		return x.base() < y.base();
	}

	template<class Iter1, class Iter2>
	constexpr bool
	operator!=(const move_iterator<Iter1>& x, const move_iterator<Iter2>& y)
	{
		return x.base() != y.base();
	}

	template<class Iter1, class Iter2>
	constexpr bool
	operator>(const move_iterator<Iter1>& x, const move_iterator<Iter2>& y)
	{
		return x.base() > y.base();
	}

	template<class Iter1, class Iter2>
	constexpr bool
	operator>=(const move_iterator<Iter1>& x, const move_iterator<Iter2>& y)
	{
		return x.base() >= y.base();
	}

	template<class Iter1, class Iter2>
	constexpr bool
	operator<=(const move_iterator<Iter1>& x, const move_iterator<Iter2>& y)
	{
		return x.base() <= y.base();
	}

	template<class Iter1, class Iter2>
	constexpr auto
	operator-(const move_iterator<Iter1>& x, const move_iterator<Iter2>& y)
		-> decltype(x.base() - y.base())
	{
		return x.base() - y.base();
	}

	template<class Iter>
	constexpr move_iterator<Iter>
	operator+(typename move_iterator<Iter>::difference_type n,
		const move_iterator<Iter>& x)
	{
		return move_iterator<Iter>(x.base() + n);
	}

	template<class Iter>
	constexpr move_iterator<Iter>
	make_move_iterator(Iter i)
	{
		return move_iterator<Iter>(i);
	}

	// In addition to being included in <iterator>, std::begin and std::cbegin
	// are guaranteed to become available if any of the following headers are
	// included: <array>, <deque>, <forward_list>, <list>, <map>, <regex>,
	// <set>, <span> (since C++20), <string>, <string_view> (since C++17),
	// <unordered_map>, <unordered_set>, and <vector>.


	template<class T, size_t Np>
	constexpr T* begin(T (&array)[Np])
	{
		return array;
	}

	template<class T, size_t Np>
	constexpr T* end(T (&array)[Np]) noexcept
	{
		return array + Np;
	}


	template<class Cp>
	constexpr auto
	begin(Cp& c) -> decltype(c.begin())
	{
		return c.begin();
	}

	template<class Cp>
	constexpr auto
	begin(const Cp& c) -> decltype(c.begin())
	{
		return c.begin();
	}

	template<class Cp>
	constexpr auto
	end(Cp& c) -> decltype(c.end())
	{
		return c.end();
	}

	template<class Cp>
	constexpr auto
	end(const Cp& c) -> decltype(c.end())
	{
		return c.end();
	}


	template<class T, size_t Np>
	constexpr reverse_iterator<T*> rbegin(T (&array)[Np])
	{
		return reverse_iterator<T*>(array + Np);
	}

	template<class T, size_t Np>
	constexpr reverse_iterator<T*> rend(T (&array)[Np])
	{
		return reverse_iterator<T*>(array);
	}

	template<class Ep>
	constexpr reverse_iterator<const Ep*>
	rbegin(initializer_list<Ep> il)
	{
		return reverse_iterator<const Ep*>(il.end());
	}

	template<class Ep>
	constexpr reverse_iterator<const Ep*>
	rend(initializer_list<Ep> il)
	{
		return reverse_iterator<const Ep*>(il.begin());
	}

	// Returns exactly faiz::begin(c), with c always treated as const-qualified.
	// If Cp is a standard Container, this always returns C::const_iterator.
	template<class Cp>
	constexpr auto
	cbegin(const Cp& c) noexcept(noexcept(faiz::begin(c)))
		-> decltype(faiz::begin(c))

	{
		return faiz::begin(c);
	}
	// Returns exactly faiz::end(c), with c always treated as const-qualified.
	// If Cp is a standard Container, this always returns a C::const_iterator.
	template<class Cp>
	constexpr auto
	cend(const Cp& c) noexcept(noexcept(faiz::end(c))) -> decltype(faiz::end(c))
	{
		return faiz::end(c);
	}

	template<class Cp>
	constexpr auto
	rbegin(Cp& c) -> decltype(c.rbegin())
	{
		return c.rbegin();
	}

	template<class Cp>
	constexpr auto
	rbegin(const Cp& c) -> decltype(c.rbegin())
	{
		return c.rbegin();
	}

	template<class Cp>
	constexpr auto
	rend(Cp& c) -> decltype(c.rend())
	{
		return c.rend();
	}

	template<class Cp>
	constexpr auto
	rend(const Cp& c) -> decltype(c.rend())
	{
		return c.rend();
	}

	template<class Cp>
	constexpr auto
	crbegin(const Cp& c) -> decltype(faiz::rbegin(c))
	{
		return faiz::rbegin(c);
	}

	template<class Cp>
	constexpr auto
	crend(const Cp& c) -> decltype(faiz::rend(c))
	{
		return faiz::rend(c);
	}


	template<class Cont>
	constexpr auto
	size(const Cont& c) noexcept(noexcept(c.size())) -> decltype(c.size())
	{
		return c.size();
	}

	template<class T, size_t Sz>
	constexpr size_t
	size(const T (&)[Sz]) noexcept
	{
		return Sz;
	}

	template<class Cont>
	constexpr auto
	empty(const Cont& c) noexcept(noexcept(c.empty())) -> decltype(c.empty())
	{
		return c.empty();
	}

	template<class T, size_t Sz>
	constexpr bool
	empty(const T (&)[Sz]) noexcept
	{
		return false;
	}

	template<class Ep>
	constexpr bool
	empty(initializer_list<Ep> il) noexcept
	{
		return il.size() == 0;
	}

	// Returns a pointer to the block of memory containing the elements of the
	// container.
	template<class Cont>
	constexpr inline auto
	data(Cont& c) noexcept(noexcept(c.data())) -> decltype(c.data())
	{
		return c.data();
	}

	// Returns a pointer to the block of memory containing the elements of the
	// container.
	template<class Cont>
	constexpr inline auto
	data(const Cont& c) noexcept(noexcept(c.data())) -> decltype(c.data())
	{
		return c.data();
	}

	// Returns a pointer to the block of memory containing the elements of the
	// container.
	template<class T, size_t Sz>
	constexpr T* data(T (&array)[Sz]) noexcept
	{
		return array;
	}

	// Returns a pointer to the block of memory containing the elements of the
	// container.
	template<class Ep>
	constexpr const Ep*
	data(initializer_list<Ep> il) noexcept
	{
		return il.begin();
	}

} // namespace rider::faiz
#endif
