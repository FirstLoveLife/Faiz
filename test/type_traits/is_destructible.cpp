#include "rider/faiz/initializer_list.hpp"
#include "rider/faiz/type_traits.hpp"
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

int
main()
{
	static_assert(is_destructible<int>::value, "Error");
	static_assert(is_destructible<const int>::value, "Error");
	static_assert(is_destructible<bool>::value, "Error");
	static_assert(is_destructible<const bool>::value, "Error");
	static_assert(is_destructible<int*>::value, "Error");
	static_assert(is_destructible<void*>::value, "Error");
	static_assert(is_destructible<int Der::*>::value, "Error");
	static_assert(is_destructible<const int Der::*>::value, "Error");
	static_assert(is_destructible<void (Der::*)() const>::value, "Error");
	static_assert(is_destructible<void (*)()>::value, "Error");
	static_assert(is_destructible<En>::value, "Error");
	static_assert(is_destructible<const En>::value, "Error");
	static_assert(is_destructible<En2>::value, "Error");
	static_assert(is_destructible<const En2>::value, "Error");
	static_assert(is_destructible<OpE>::value, "Error");
	static_assert(is_destructible<const OpE>::value, "Error");
	static_assert(is_destructible<OpSE>::value, "Error");
	static_assert(is_destructible<const OpSE>::value, "Error");
	static_assert(is_destructible<nullptr_t>::value, "Error");
	static_assert(is_destructible<const nullptr_t>::value, "Error");
	static_assert(is_destructible<Der>::value, "Error");
	static_assert(is_destructible<const Der>::value, "Error");
	static_assert(is_destructible<Aggr>::value, "Error");
	static_assert(is_destructible<const Aggr>::value, "Error");
	static_assert(is_destructible<E>::value, "Error");
	static_assert(is_destructible<const E>::value, "Error");
	static_assert(is_destructible<U1>::value, "Error");
	static_assert(is_destructible<const U1>::value, "Error");
	static_assert(is_destructible<Abstract1>::value, "Error");
	static_assert(is_destructible<const Abstract1>::value, "Error");

	static_assert(is_destructible<int[1]>::value, "Error");
	static_assert(is_destructible<const int[1]>::value, "Error");
	static_assert(is_destructible<int[1][2]>::value, "Error");
	static_assert(is_destructible<const int[1][2]>::value, "Error");
	static_assert(is_destructible<int&>::value, "Error");
	static_assert(is_destructible<int&&>::value, "Error");
	static_assert(is_destructible<int(&)[1]>::value, "Error");
	static_assert(is_destructible<const int(&)[1]>::value, "Error");
	static_assert(is_destructible<void (&)()>::value, "Error");

	static_assert(is_destructible<Ellipsis>::value, "Error");
	static_assert(is_destructible<const Ellipsis>::value, "Error");
	static_assert(is_destructible<Abstract2>::value, "Error");
	static_assert(is_destructible<const Abstract2>::value, "Error");
	static_assert(is_destructible<Aggr2>::value, "Error");
	static_assert(is_destructible<const Aggr2>::value, "Error");
	static_assert(is_destructible<DelDef>::value, "Error");
	static_assert(is_destructible<const DelDef>::value, "Error");
	static_assert(is_destructible<DelCopy>::value, "Error");
	static_assert(is_destructible<const DelCopy>::value, "Error");
	static_assert(is_destructible<DelEllipsis>::value, "Error");
	static_assert(is_destructible<const DelEllipsis>::value, "Error");
	static_assert(is_destructible<initializer_list<int>>::value, "Error");
	static_assert(is_destructible<const initializer_list<int>>::value, "Error");
	static_assert(is_destructible<initializer_list<Del>>::value, "Error");
	static_assert(!is_destructible<void>::value, "Error");
	static_assert(!is_destructible<const void>::value, "Error");
	static_assert(!is_destructible<void()>::value, "Error");
	static_assert(!is_destructible<void() const>::value, "Error");
	static_assert(!is_destructible<int[]>::value, "Error");
	static_assert(!is_destructible<const int[]>::value, "Error");
	static_assert(!is_destructible<Del>::value, "Error");
	static_assert(!is_destructible<const Del>::value, "Error");
	static_assert(!is_destructible<AbstractDelDtor>::value, "Error");
	static_assert(!is_destructible<const AbstractDelDtor>::value, "Error");
	static_assert(!is_destructible<int[][1]>::value, "Error");
	static_assert(!is_destructible<const int[][1]>::value, "Error");
	static_assert(!is_destructible<Del[1]>::value, "Error");
	static_assert(!is_destructible<const Del[1]>::value, "Error");
	static_assert(!is_destructible<Del[]>::value, "Error");
	static_assert(!is_destructible<const Del[]>::value, "Error");
	// Deleted members in unions with non-trivial members:
	static_assert(!is_destructible<NontrivialUnion>::value, "Error");
	// Unusual copy:
	static_assert(is_destructible<UnusualCopy>::value, "Error");
}
