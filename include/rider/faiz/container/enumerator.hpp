#ifndef __ENUMERATOR_H_
#define __ENUMERATOR_H_
#include "rider/faiz/type_traits.hpp"
namespace Rider::Faiz::Impl_Enumerator
{
	template<typename ContainerT>
	class ConstEnumerator;

	template<typename ContainerT>
	class Enumerator : public std::iterator<std::bidirectional_iterator_tag,
						   typename ContainerT::Element>
	{
		friend ConstEnumerator<ContainerT>;

	public:
		using Element = typename ContainerT::Element;

	private:
		ContainerT* x_pContainer;
		Element* x_pElement;

	public:
		constexpr Enumerator() noexcept
			: x_pContainer(nullptr), x_pElement(nullptr)
		{}
		constexpr Enumerator(ContainerT& vContainer, Element* pElement) noexcept
			: x_pContainer(AddressOf(vContainer)), x_pElement(pElement)
		{}

	public:
		Element*
		GetElement() const noexcept
		{
			return x_pElement;
		}

	public:
		bool
		operator==(const Enumerator& enOther) const noexcept
		{
			return x_pElement == enOther.x_pElement;
		}
		bool
		operator!=(const Enumerator& enOther) const noexcept
		{
			return x_pElement != enOther.x_pElement;
		}

		Enumerator&
		operator++() noexcept
		{
			MCF_ASSERT(x_pContainer);

			if(x_pElement)
			{
				x_pElement = x_pContainer->GetNext(x_pElement);
			}
			else
			{
				x_pElement = x_pContainer->GetFirst();
			}
			return *this;
		}
		Enumerator&
		operator--() noexcept
		{
			MCF_ASSERT(x_pContainer);

			if(x_pElement)
			{
				x_pElement = x_pContainer->GetPrev(x_pElement);
			}
			else
			{
				x_pElement = x_pContainer->GetLast();
			}
			return *this;
		}

		Enumerator
		operator++(int) noexcept
		{
			auto enRet = *this;
			++(*this);
			return enRet;
		}
		Enumerator
		operator--(int) noexcept
		{
			auto enRet = *this;
			--(*this);
			return enRet;
		}

		Element& operator*() const noexcept
		{
			MCF_ASSERT(x_pElement);

			return *x_pElement;
		}
		Element* operator->() const noexcept
		{
			return x_pElement;
		}

		explicit operator bool() const noexcept
		{
			return !!x_pElement;
		}
	};

	template<typename ContainerT>
	class ConstEnumerator
		: public std::iterator<std::bidirectional_iterator_tag,
			  const typename ContainerT::Element>
	{
	public:
		using Element = const typename ContainerT::Element;

	private:
		const ContainerT* x_pContainer;
		const Element* x_pElement;

	public:
		constexpr ConstEnumerator() noexcept
			: x_pContainer(nullptr), x_pElement(nullptr)
		{}
		constexpr ConstEnumerator(
			const ContainerT& vContainer, const Element* pElement) noexcept
			: x_pContainer(AddressOf(vContainer)), x_pElement(pElement)
		{}
		constexpr ConstEnumerator(
			const Enumerator<ContainerT>& enOther) noexcept
			: x_pContainer(enOther.x_pContainer), x_pElement(enOther.x_pElement)
		{}

	public:
		const Element*
		GetElement() const noexcept
		{
			return x_pElement;
		}

	public:
		bool
		operator==(const ConstEnumerator& enOther) const noexcept
		{
			return x_pElement == enOther.x_pElement;
		}
		bool
		operator!=(const ConstEnumerator& enOther) const noexcept
		{
			return x_pElement != enOther.x_pElement;
		}

		ConstEnumerator&
		operator++() noexcept
		{
			MCF_ASSERT(x_pContainer);

			if(x_pElement)
			{
				x_pElement = x_pContainer->GetNext(x_pElement);
			}
			else
			{
				x_pElement = x_pContainer->GetFirst();
			}
			return *this;
		}
		ConstEnumerator&
		operator--() noexcept
		{
			MCF_ASSERT(x_pContainer);

			if(x_pElement)
			{
				x_pElement = x_pContainer->GetPrev(x_pElement);
			}
			else
			{
				x_pElement = x_pContainer->GetLast();
			}
			return *this;
		}

		ConstEnumerator
		operator++(int) noexcept
		{
			auto enRet = *this;
			++(*this);
			return enRet;
		}
		ConstEnumerator
		operator--(int) noexcept
		{
			auto enRet = *this;
			--(*this);
			return enRet;
		}

		const Element& operator*() const noexcept
		{
			MCF_ASSERT(x_pElement);

			return *x_pElement;
		}
		const Element* operator->() const noexcept
		{
			return x_pElement;
		}

		explicit operator bool() const noexcept
		{
			return !!x_pElement;
		}
	};

} // namespace Rider::Faiz::Impl_Enumerator

#endif
