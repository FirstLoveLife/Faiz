#include "rider/faiz/type_traits.hpp"
#include <iostream>
template<typename T>
class list;
template<typename T>
class Node
{
	Node* prev;
	Node* next;
	T value;

public:
	Node(T v) : value{v}, next{nullptr}, prev{nullptr}
	{}
	friend class list<T>;
};
template<typename T>
class list
{
public:
	// friend class Node<T>;
	Node<T>* head;
	Node<T>* tail;
	size_t length;
	template<typename... U>
	list(U... vTs) : tail{nullptr}, length{0}
	{
		auto len = sizeof...(vTs);
		auto first = len;
		int x[] = {(push_back(vTs), 0)...};
	}
	void
	push_back(T const& a)
	{
		length++;
		auto temp = new Node(a);
		if(length == 1)
		{
			tail = head = temp;
			head->prev = tail->prev = nullptr;
		}
		else
		{
			auto prev = tail;
			tail = temp;
			prev->next = tail;
			tail->prev = prev;
		}
	}
	void
	push_front(T const& a)
	{
		length++;
		auto temp = new Node(a);
		if(length == 1)
		{
			tail = head = temp;
			head->prev = tail->prev = nullptr;
		}
		else
		{
			auto next = head;
			head = temp;
			next->prev = head;
			head->next = next;
		}
	}
	void
	output()
	{
		auto current = head;
		while(current != nullptr)
		{
			std::cout << current->value << " ";
			current = current->next;
		}
		std::cout << '\n';
	}

	void
	reverseOutput()
	{
		auto current = tail;
		while(current != nullptr)
		{
			std::cout << current->value << " ";
			current = current->prev;
		}
		std::cout << '\n';
	}


	~list()
	{
		auto current = head;
		while(current != nullptr)
		{
			auto next = current->next;
			delete current;
			current = next;
		}
	}
};

Tpl<Typ T, Typ... U>
list(T, U...)
	->list<Rider::Faiz::enable_if_t<(Rider::Faiz::are_same_v<T, U...>), T>>;

int
main()
{
	list l(1, 2, 3);
	l.reverseOutput();
	l.push_front(8);
	l.reverseOutput();
	l.output();
	l.reverseOutput();
	l.push_back(4);
	l.reverseOutput();
	l.output();
}
