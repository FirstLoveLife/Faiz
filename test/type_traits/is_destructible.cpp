// #include "rider/faiz/initializer_list.hpp"
#include <catch2/catch.hpp>
#include <initializer_list>
using std::initializer_list;

#include "rider/faiz/type_traits.hpp"

namespace
{
	using namespace Rider::Faiz;
	struct E
	{};

	struct NTD1
	{
		~NTD1() = default;
	};

	struct NTD2
	{
		~NTD2();
	};

	struct NTD3
	{
		~NTD3() throw();
	};

	struct TD1
	{
		~TD1() noexcept(false);
	};

	struct TD2
	{
		~TD2();
	};

	struct Aggr
	{
		int i;
		bool b;
		E e;
	};

	struct Aggr2
	{
		int i;
		bool b;
		TD1 r;
	};

	struct Del
	{
		~Del() = delete;
	};

	struct Del2
	{
		~Del2() noexcept = delete;
	};

	struct Del3
	{
		~Del3() noexcept(false) = delete;
	};

	struct Der : Aggr
	{};

	struct Der2 : Aggr2
	{};

	union U1
	{
		int i;
		double d;
		void* p;
		TD1* pt;
	};

	union Ut
	{
		int i;
		double d;
		void* p;
		TD1 pt;
	};

	enum class En
	{
		a,
		b,
		c,
		d
	};
	enum En2
	{
		En2a,
		En2b,
		En2c,
		En2d
	};

	enum OpE : int;
	enum class OpSE : bool;

	struct Abstract1
	{
		virtual ~Abstract1() = 0;
	};

	struct AbstractDelDtor
	{
		~AbstractDelDtor() = delete;
		virtual void
		foo()
			= 0;
	};

	struct Abstract2
	{
		virtual ~Abstract2() noexcept(false) = 0;
	};

	struct Abstract3
	{
		~Abstract3() noexcept(false);
		virtual void
		foo() noexcept
			= 0;
	};

	struct Nontrivial
	{
		Nontrivial();
		Nontrivial(const Nontrivial&);
		Nontrivial&
		operator=(const Nontrivial&);
		~Nontrivial();
	};

	union NontrivialUnion
	{
		int i;
		Nontrivial n;
	};

	struct UnusualCopy
	{
		UnusualCopy(UnusualCopy&);
	};

	struct Ellipsis
	{
		Ellipsis(...)
		{}
	};

	struct DelEllipsis
	{
		DelEllipsis(...) = delete;
	};

	struct DelDef
	{
		DelDef() = delete;
	};

	struct DelCopy
	{
		DelCopy(const DelCopy&) = delete;
	};

} // namespace
TEST_CASE("is_destructible: ")
{
	STATIC_REQUIRE(is_destructible<int>::value);
	STATIC_REQUIRE(is_destructible<const int>::value);
	STATIC_REQUIRE(is_destructible<bool>::value);
	STATIC_REQUIRE(is_destructible<const bool>::value);
	STATIC_REQUIRE(is_destructible<int*>::value);
	STATIC_REQUIRE(is_destructible<void*>::value);
	STATIC_REQUIRE(is_destructible<int Der::*>::value);
	STATIC_REQUIRE(is_destructible<const int Der::*>::value);
	STATIC_REQUIRE(is_destructible<void (Der::*)() const>::value);
	STATIC_REQUIRE(is_destructible<void (*)()>::value);
	STATIC_REQUIRE(is_destructible<En>::value);
	STATIC_REQUIRE(is_destructible<const En>::value);
	STATIC_REQUIRE(is_destructible<En2>::value);
	STATIC_REQUIRE(is_destructible<const En2>::value);
	STATIC_REQUIRE(is_destructible<OpE>::value);
	STATIC_REQUIRE(is_destructible<const OpE>::value);
	STATIC_REQUIRE(is_destructible<OpSE>::value);
	STATIC_REQUIRE(is_destructible<const OpSE>::value);
	STATIC_REQUIRE(is_destructible<nullptr_t>::value);
	STATIC_REQUIRE(is_destructible<const nullptr_t>::value);
	STATIC_REQUIRE(is_destructible<Der>::value);
	STATIC_REQUIRE(is_destructible<const Der>::value);
	STATIC_REQUIRE(is_destructible<Aggr>::value);
	STATIC_REQUIRE(is_destructible<const Aggr>::value);
	STATIC_REQUIRE(is_destructible<E>::value);
	STATIC_REQUIRE(is_destructible<const E>::value);
	STATIC_REQUIRE(is_destructible<U1>::value);
	STATIC_REQUIRE(is_destructible<const U1>::value);
	STATIC_REQUIRE(is_destructible<Abstract1>::value);
	STATIC_REQUIRE(is_destructible<const Abstract1>::value);

	STATIC_REQUIRE(is_destructible<int[1]>::value);
	STATIC_REQUIRE(is_destructible<const int[1]>::value);
	STATIC_REQUIRE(is_destructible<int[1][2]>::value);
	STATIC_REQUIRE(is_destructible<const int[1][2]>::value);
	STATIC_REQUIRE(is_destructible<int&>::value);
	STATIC_REQUIRE(is_destructible<int&&>::value);
	STATIC_REQUIRE(is_destructible<int(&)[1]>::value);
	STATIC_REQUIRE(is_destructible<const int(&)[1]>::value);
	STATIC_REQUIRE(is_destructible<void (&)()>::value);

	STATIC_REQUIRE(is_destructible<Ellipsis>::value);
	STATIC_REQUIRE(is_destructible<const Ellipsis>::value);
	STATIC_REQUIRE(is_destructible<Abstract2>::value);
	STATIC_REQUIRE(is_destructible<const Abstract2>::value);
	STATIC_REQUIRE(is_destructible<Aggr2>::value);
	STATIC_REQUIRE(is_destructible<const Aggr2>::value);
	STATIC_REQUIRE(is_destructible<DelDef>::value);
	STATIC_REQUIRE(is_destructible<const DelDef>::value);
	STATIC_REQUIRE(is_destructible<DelCopy>::value);
	STATIC_REQUIRE(is_destructible<const DelCopy>::value);
	STATIC_REQUIRE(is_destructible<DelEllipsis>::value);
	STATIC_REQUIRE(is_destructible<const DelEllipsis>::value);
	STATIC_REQUIRE(is_destructible<initializer_list<int>>::value);
	STATIC_REQUIRE(is_destructible<const initializer_list<int>>::value);
	STATIC_REQUIRE(is_destructible<initializer_list<Del>>::value);
	STATIC_REQUIRE(!is_destructible<void>::value);
	STATIC_REQUIRE(!is_destructible<const void>::value);
	STATIC_REQUIRE(!is_destructible<void()>::value);
	STATIC_REQUIRE(!is_destructible<void() const>::value);
	STATIC_REQUIRE(!is_destructible<int[]>::value);
	STATIC_REQUIRE(!is_destructible<const int[]>::value);
	STATIC_REQUIRE(!is_destructible<Del>::value);
	STATIC_REQUIRE(!is_destructible<const Del>::value);
	STATIC_REQUIRE(!is_destructible<AbstractDelDtor>::value);
	STATIC_REQUIRE(!is_destructible<const AbstractDelDtor>::value);
	STATIC_REQUIRE(!is_destructible<int[][1]>::value);
	STATIC_REQUIRE(!is_destructible<const int[][1]>::value);
	STATIC_REQUIRE(!is_destructible<Del[1]>::value);
	STATIC_REQUIRE(!is_destructible<const Del[1]>::value);
	STATIC_REQUIRE(!is_destructible<Del[]>::value);
	STATIC_REQUIRE(!is_destructible<const Del[]>::value);
	// Deleted members in unions with non-trivial members:
	STATIC_REQUIRE(!is_destructible<NontrivialUnion>::value);
	// Unusual copy:
	STATIC_REQUIRE(is_destructible<UnusualCopy>::value);
}
