#include "rider/faiz/unique_ptr.hpp"
#include <catch2/catch.hpp>
#include <memory>

using Rider::Faiz::unique_ptr;
struct A
{};
struct B
{
	int x;
};
struct C
{
	B b;
};
struct Foo { // object to manage
    Foo() { std::cout << "Foo ctor\n"; }
    Foo(const Foo&) { std::cout << "Foo copy ctor\n"; }
    Foo(Foo&&) { std::cout << "Foo move ctor\n"; }
    ~Foo() { std::cout << "~Foo dtor\n"; }
};
struct D { // deleter
    D() {};
    D(const D&) { std::cout << "D copy ctor\n"; }
    D(D&) { std::cout << "D non-const copy ctor\n";}
    D(D&&) { std::cout << "D move ctor \n"; }
    void operator()(Foo* p) const {
        std::cout << "D is deleting a Foo\n";
        delete p;
    };
};
struct Stateful { // deleter
    Stateful() {};
    Stateful(const Stateful&) { std::cout << "D copy ctor\n"; }
    Stateful(Stateful&) { std::cout << "D non-const copy ctor\n";}
    Stateful(Stateful&&) { std::cout << "D move ctor \n"; }
    void operator()(Foo* p) const {
        std::cout << "D is deleting a Foo\n";
        delete p;
    };
  int x;
};

TEST_CASE(
	"size of Faiz::unique_ptr should not have overload when using default deleter")
{
	CHECK((sizeof(unique_ptr<int>)) == (sizeof(int*)));
	CHECK((sizeof(unique_ptr<int, D>)) == (sizeof(D*)));
	CHECK_FALSE((sizeof(unique_ptr<int, Stateful>)) == (sizeof(D*)));
	CHECK((sizeof(unique_ptr<A>)) == (sizeof(A*)));
	CHECK((sizeof(unique_ptr<C>)) == (sizeof(C*)));
}
