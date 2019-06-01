#ifndef VECTOR__
#define VECTOR__
#include "rider/faiz/check/arithmetic.hpp"
#include "rider/faiz/container/enumerator.hpp"
#include "rider/faiz/debug.hpp"
#include "rider/faiz/macros.hpp"
#include "rider/faiz/memory/ConstructorDestructor.hpp"
#include "rider/faiz/memory/allocator.hpp"
#include "rider/faiz/type_traits.hpp"
#include "rider/faiz/utility.hpp"
/*
TODO: add test for vector
TODO: deduction guide for vector:
Tpl<Typ Iter>
vector(Iter begin, Iter end) -> vector<Typ
Faiz::iterator_traits<Iter>::value_type>;
 */

#include "rider/faiz/macros.hpp"
namespace Rider::Faiz
{
	Tpl<Typ tElement, Typ tAllocator = DefaultAllocator> class vector
	{
	public:
		using Element = tElement;
		using Allocator = tAllocator;
		using ConstEnumerator = Impl_Enumerator::ConstEnumerator<vector>;
		using Enumerator = Impl_Enumerator::Enumerator<vector>;


	private:
		Element* pElem;
		size_t size;
		size_t capacity;

	public:
		fn
		Swap(vector& vOther) noexcept
		{
			using std::swap;
			swap(pElem, vOther.elem);
			swap(size, vOther.size);
			swap(capacity, vOther.capacity);
		}

		cfn
		getSize() const noexcept->size_t
		{
			return size;
		}

		vector() : pElem{nullptr}, size{0}, capacity{0}
		{}

		Tpl<Typ... tParams>
		vector(size_t size, const tParams&... vParams) : vector()
		{
			append(size, vParams...);
		}

		Tpl<Typ... ParamsT> cfn
		uncheckedPush(ParamsT&&... vParams) noexcept(
			is_nothrow_constructible_v<Element, ParamsT&&...>)
			->Element&
		{
			Except(getCapacity() - getSize() > 0);

			const auto pElement = pElem + size;
			DefaultConstruct(pElement, Faiz::forward<ParamsT>(vParams)...);
			++size;

			return *pElement;
		}


		fn
		getCapacity() const noexcept->size_t
		{
			return capacity;
		}

		fn
		reserve(size_t uNewCapacity)
		{
			const auto uOldCapacity = getCapacity();
			if(uNewCapacity <= uOldCapacity)
			{
				return;
			}

			auto uElementsToAlloc = uOldCapacity + 1;
			uElementsToAlloc += (uElementsToAlloc >> 1);
			uElementsToAlloc = (uElementsToAlloc + 0x0F) & (size_t)-0x10;
			if(uElementsToAlloc < uNewCapacity)
			{
				uElementsToAlloc = uNewCapacity;
			}

			// std::length_error if new_cap > max_size().
			const auto uBytesToAlloc
				= Check::Mul(sizeof(Element), uElementsToAlloc);
			const auto pNewElem
				= static_cast<Element*>(Allocator()(uBytesToAlloc));
			const auto pOldElem = pElem;
			auto pWrite = pNewElem;
			try
			{
				for(size_t uIndex = 0; uIndex < getSize(); ++uIndex)
				{
					Construct(pWrite, move_if_noexcept(pOldElem[uIndex]));
					++pWrite;
				}
			}
			catch(...)
			{
				while(pWrite != pNewElem)
				{
					--pWrite;
					Destruct(pWrite);
				}
				Allocator()(static_cast<void*>(pNewElem));
				throw;
			}
			for(size_t uIndex = getSize(); uIndex > 0; --uIndex)
			{
				Destruct(pOldElem + uIndex - 1);
			}
			Allocator()(static_cast<void*>(pOldElem));

			pElem = pNewElem;
			capacity = uElementsToAlloc;
		}

		fn
		pop(size_t uCount = 1) noexcept
		{
			Expects(uCount <= getSize());

			for(size_t uIndex = 0; uIndex < uCount; ++uIndex)
			{
				Destruct(pElem + size - 1 - uIndex);
			}
			size -= uCount;
		}

		fn
		operator=(const vector& vOther)->vector&
		{
			if(is_nothrow_copy_constructible_v<Element> or isEmpty())
			{
				reserve(vOther.getSize());
				try
				{
					for(size_t uIndex = 0; uIndex < vOther.getSize(); ++uIndex)
					{
						uncheckedPush(vOther.elem[uIndex]);
					}
				}
				catch(...)
				{
					Clear();
					throw;
				}
			}
			else
			{
				vector(vOther).Swap(*this);
			}
			return *this;
		}

		// Move-assignment should be simple and efficient: Just swap().
		fn
		operator=(vector&& vOther) noexcept->vector&
		{
			vOther.Swap(*this);
			return *this;
		}
		~vector()
		{
			Clear();
			Allocator()(static_cast<void*>(pElem));
#ifndef NDEBUG
			__builtin_memset(&pElem, 0xEF, sizeof(pElem));
#endif
		}

		cfn
		isEmpty() const noexcept->bool
		{
			return size == 0;
		}
		cfn
		Clear() noexcept
		{
			pop(getSize());
		}

	private:
		cfn
		X_PrepareForinsertion(size_t uPos, size_t uDeltaSize)
		{
			Except(is_nothrow_move_constructible_v<Element>);
			Except(not isEmpty());
			Except(uPos <= size);

			reserveMore(uDeltaSize);
			for(size_t uIndex = size; uIndex > uPos; --uIndex)
			{
				Construct(
					pElem + uIndex - 1 + uDeltaSize, move(pElem[uIndex - 1]));
				Destruct(pElem + uIndex - 1);
			}
		}
		cfn
		X_UndoPreparation(size_t uPos, size_t uDeltaSize) noexcept
		{
			Except(is_nothrow_move_constructible_v<Element>);
			Except(not isEmpty());
			Except(uPos <= size);
			Except(uDeltaSize <= size - uPos);

			for(size_t uIndex = uPos; uIndex < size; ++uIndex)
			{
				Construct(pElem + uIndex, move(pElem[uIndex + uDeltaSize]));
				Destruct(pElem + uIndex + uDeltaSize);
			}
		}

		fn
		reserveMore(size_t uDeltaCapacity)
		{
			const auto uNewCapacity = Check::Add(uDeltaCapacity, getSize());
			reserve(uNewCapacity);
		}


	public:
		template<Typ OutputIteratorT>
		OutputIteratorT
		extract(OutputIteratorT itOutput)
		{
			try
			{
				for(std::size_t uIndex = 0; uIndex < size; ++uIndex)
				{
					*itOutput = std::move(pElem[uIndex]);
					++itOutput;
				}
			}
			catch(...)
			{
				Clear();
				throw;
			}
			Clear();
			return itOutput;
		}

		cfn
		getFirst() const noexcept->const Element*
		{
			if(isEmpty())
			{
				return nullptr;
			}
			return getBegin();
		}

		cfn
		getFirst() noexcept->Element*
		{
			if(isEmpty())
			{
				return nullptr;
			}
			return getBegin();
		}
		cfn
		getConstFirst() const noexcept->const Element*
		{
			return getFirst();
		}
		cfn
		getLast() const noexcept->const Element*
		{
			if(isEmpty())
			{
				return nullptr;
			}
			return getEnd() - 1;
		}

		cfn
		getLast() noexcept->Element*
		{
			if(isEmpty())
			{
				return nullptr;
			}
			return getEnd() - 1;
		}

		cfn
		getConstLast() const noexcept->const Element*
		{
			return getLast();
		}

		cfn
		getPrev(const Element* pPos) const noexcept->const Element*
		{
			Except(pPos);

			const auto pBegin = getBegin();
			auto uOffset = static_cast<std::size_t>(pPos - pBegin);
			if(uOffset == 0)
			{
				return nullptr;
			}
			--uOffset;
			return pBegin + uOffset;
		}

		cfn
		getPrev(Element* pPos) noexcept->Element*
		{
			Except(pPos);

			const auto pBegin = getBegin();
			auto uOffset = static_cast<std::size_t>(pPos - pBegin);
			if(uOffset == 0)
			{
				return nullptr;
			}
			--uOffset;
			return pBegin + uOffset;
		}

		cfn
		getNext(const Element* pPos) const noexcept->const Element*
		{
			Except(pPos);

			const auto pBegin = getBegin();
			auto uOffset = static_cast<std::size_t>(pPos - pBegin);
			++uOffset;
			if(uOffset == getSize())
			{
				return nullptr;
			}
			return pBegin + uOffset;
		}

		cfn
		getNext(Element* pPos) noexcept->Element*
		{
			Except(pPos);

			const auto pBegin = getBegin();
			auto uOffset = static_cast<std::size_t>(pPos - pBegin);
			++uOffset;
			if(uOffset == getSize())
			{
				return nullptr;
			}
			return pBegin + uOffset;
		}

		cfn
		enumerateFirst() const noexcept->ConstEnumerator
		{
			return ConstEnumerator(*this, getFirst());
		}

		cfn
		enumerateFirst() noexcept->Enumerator
		{
			return Enumerator(*this, getFirst());
		}

		cfn
		enumerateConstFirst() const noexcept->ConstEnumerator
		{
			return enumerateFirst();
		}

		cfn
		enumerateLast() const noexcept->ConstEnumerator
		{
			return ConstEnumerator(*this, getLast());
		}

		cfn
		enumerateLast() noexcept->Enumerator
		{
			return Enumerator(*this, getLast());
		}

		cfn
		enumerateConstLast() const noexcept->ConstEnumerator
		{
			return enumerateLast();
		}

		cfn
		enumerateSingular() const noexcept->ConstEnumerator
		{
			return ConstEnumerator(*this, nullptr);
		}

		cfn
		enumerateSingular() noexcept->Enumerator
		{
			return Enumerator(*this, nullptr);
		}
		cfn
		enumerateConstSingular() const noexcept->ConstEnumerator
		{
			return enumerateSingular();
		}

		cfn
		getData() const noexcept->const Element*
		{
			return pElem;
		}
		cfn
		getData() noexcept->Element*
		{
			return pElem;
		}
		cfn
		getConstData() const noexcept->const Element*
		{
			return getData();
		}

		cfn
		getCapacityRemaining() const noexcept->size_t
		{
			return getCapacity() - getSize();
		}

		cfn
		getBegin() const noexcept->const Element*
		{
			return pElem;
		}

		cfn
		getBegin() noexcept->Element*
		{
			return pElem;
		}

		cfn
		getConstBegin() const noexcept->const Element*
		{
			return getBegin();
		}

		cfn
		getEnd() const noexcept->const Element*
		{
			return pElem + size;
		}

		cfn
		getEnd() noexcept->Element*
		{
			return pElem + size;
		}

		cfn
		getConstEnd() const noexcept->const Element*
		{
			return getEnd();
		}

		cfn
		Get(size_t uIndex) const->const Element&
		{
			if(uIndex >= getSize())
			{
				throw std::out_of_range("vector: index is out of range");
			}
			return uncheckedGet(uIndex);
		}

		cfn
		Get(size_t uIndex)->Element&
		{
			if(uIndex >= getSize())
			{
				throw std::out_of_range("vector: index is out of range");
			}
			return uncheckedGet(uIndex);
		}

		cfn
		uncheckedGet(size_t uIndex) const noexcept->const Element&
		{
			Except(uIndex < getSize());

			return pElem[uIndex];
		}

		cfn
		uncheckedGet(size_t uIndex) noexcept->Element&
		{
			Except(uIndex < getSize());

			return pElem[uIndex];
		}

		Tpl<Typ... ParamsT> cfn
		resize(size_t uSize, const ParamsT&... vParams)->Element*
		{
			const auto uOldSize = getSize();
			if(uSize > uOldSize)
			{
				append(uSize - uOldSize, vParams...);
			}
			else
			{
				pop(uOldSize - uSize);
			}
			return getData();
		}
		Tpl<Typ... ParamsT> cfn
		resizeMore(size_t uDeltaSize, const ParamsT&... vParams)->Element*
		{
			const auto uOldSize = getSize();
			append(uDeltaSize, vParams...);
			return getData() + uOldSize;
		}

		cfn
		resizeToCapacity() noexcept->std::pair<Element*, size_t>
		{
			const auto uOldSize = getSize();
			const auto uNewSize = getCapacity();
			const auto uDeltaSize = uNewSize - uOldSize;
			uncheckedAppend(uDeltaSize);
			return std::make_pair(getData() + uOldSize, uDeltaSize);
		}

		Tpl<Typ... ParamsT> cfn
		Push(ParamsT&&... vParams)->Element&
		{
			reserveMore(1);
			return uncheckedPush(Faiz::forward<ParamsT>(vParams)...);
		}


		Tpl<Typ... ParamsT> cfn
		append(size_t uDeltaSize, const ParamsT&... vParams)
		{
			reserveMore(uDeltaSize);

			size_t uElementsPushed = 0;
			try
			{
				for(size_t uIndex = 0; uIndex < uDeltaSize; ++uIndex)
				{
					uncheckedPush(vParams...);
					++uElementsPushed;
				}
			}
			catch(...)
			{
				pop(uElementsPushed);
				throw;
			}
		}
		Tpl<Typ tIterator,
			enable_if_t<is_base_of<std::input_iterator_tag,
							Typ std::iterator_traits<
								tIterator>::iterator_category>::value,
				int> = 0> void
		append(tIterator itBegin, size_t uDeltaSize)
		{
			reserveMore(uDeltaSize);

			size_t uElementsPushed = 0;
			try
			{
				for(size_t uIndex = 0; uIndex < uDeltaSize; ++uIndex)
				{
					uncheckedPush(*itBegin);
					++itBegin;
					++uElementsPushed;
				}
			}
			catch(...)
			{
				pop(uElementsPushed);
				throw;
			}
		}
		Tpl<Typ tIterator,
			enable_if_t<is_base_of<std::input_iterator_tag,
							Typ std::iterator_traits<
								tIterator>::iterator_category>::value,
				int> = 0>
			fn
		append(tIterator itBegin, std::common_type_t<tIterator> itEnd)
		{
			constexpr bool kHasDeltaSizeHint = is_base_of<
				std::forward_iterator_tag,
				Typ std::iterator_traits<tIterator>::iterator_category>::value;

			if(kHasDeltaSizeHint)
			{
				const auto uDeltaSize
					= static_cast<size_t>(std::distance(itBegin, itEnd));
				reserveMore(uDeltaSize);
			}

			size_t uElementsPushed = 0;
			try
			{
				for(auto itCur = itBegin; itCur != itEnd; ++itCur)
				{
					if(kHasDeltaSizeHint)
					{
						uncheckedPush(*itCur);
					}
					else
					{
						Push(*itCur);
					}
					++uElementsPushed;
				}
			}
			catch(...)
			{
				pop(uElementsPushed);
				throw;
			}
		}
		fn
		append(std::initializer_list<Element> ilElements)
		{
			append(ilElements.begin(), ilElements.end());
		}

		Tpl<Typ... ParamsT> fn
		uncheckedAppend(size_t uDeltaSize, const ParamsT&... vParams)
		{
			size_t uElementsPushed = 0;
			try
			{
				for(size_t uIndex = 0; uIndex < uDeltaSize; ++uIndex)
				{
					uncheckedPush(vParams...);
					++uElementsPushed;
				}
			}
			catch(...)
			{
				pop(uElementsPushed);
				throw;
			}
		}
		Tpl<Typ tIterator,
			enable_if_t<is_base_of<std::input_iterator_tag,
							Typ std::iterator_traits<
								tIterator>::iterator_category>::value,
				int> = 0>
			fn
		uncheckedAppend(tIterator itBegin, size_t uDeltaSize)
		{
			size_t uElementsPushed = 0;
			try
			{
				for(size_t uIndex = 0; uIndex < uDeltaSize; ++uIndex)
				{
					uncheckedPush(*itBegin);
					++itBegin;
					++uElementsPushed;
				}
			}
			catch(...)
			{
				pop(uElementsPushed);
				throw;
			}
		}
		Tpl<Typ tIterator,
			enable_if_t<is_base_of<std::input_iterator_tag,
							Typ std::iterator_traits<
								tIterator>::iterator_category>::value,
				int> = 0>
			fn
		uncheckedAppend(tIterator itBegin, std::common_type_t<tIterator> itEnd)
		{
			size_t uElementsPushed = 0;
			try
			{
				for(auto itCur = itBegin; itCur != itEnd; ++itCur)
				{
					uncheckedPush(*itCur);
					++uElementsPushed;
				}
			}
			catch(...)
			{
				pop(uElementsPushed);
				throw;
			}
		}
		fn
		uncheckedAppend(std::initializer_list<Element> ilElements)
		{
			uncheckedAppend(ilElements.begin(), ilElements.end());
		}

		Tpl<Typ... ParamsT> fn
		emplace(const Element* pPos, ParamsT&&... vParams)->Element*
		{
			size_t uOffset;
			if(pPos)
			{
				uOffset = static_cast<size_t>(pPos - pElem);
			}
			else
			{
				uOffset = size;
			}

			if(is_nothrow_move_constructible_v<Element>)
			{
				X_PrepareForinsertion(uOffset, 1);
				auto uWrite = uOffset;
				try
				{
					DefaultConstruct(
						pElem + uWrite, Faiz::forward<ParamsT>(vParams)...);
				}
				catch(...)
				{
					X_UndoPreparation(uOffset, 1);
					throw;
				}
				size += 1;
			}
			else
			{
				auto uNewCapacity = Check::Add(1, size);
				const auto uCapacity = getCapacity();
				if(uNewCapacity < uCapacity)
				{
					uNewCapacity = uCapacity;
				}
				vector vecTemp;
				vecTemp.reserve(uNewCapacity);
				for(size_t uIndex = 0; uIndex < uOffset; ++uIndex)
				{
					vecTemp.uncheckedPush(pElem[uIndex]);
				}
				vecTemp.uncheckedPush(Faiz::forward<ParamsT>(vParams)...);
				for(size_t uIndex = uOffset; uIndex < size; ++uIndex)
				{
					vecTemp.uncheckedPush(pElem[uIndex]);
				}
				*this = move(vecTemp);
			}

			return pElem + uOffset;
		}

		Tpl<Typ... ParamsT> fn
		insert(
			const Element* pPos, size_t uDeltaSize, const ParamsT&... vParams)
			->Element*
		{
			size_t uOffset;
			if(pPos)
			{
				uOffset = static_cast<size_t>(pPos - pElem);
			}
			else
			{
				uOffset = size;
			}

			if(uDeltaSize != 0)
			{
				if(is_nothrow_move_constructible_v<Element>)
				{
					X_PrepareForinsertion(uOffset, uDeltaSize);
					auto uWrite = uOffset;
					try
					{
						for(size_t uIndex = 0; uIndex < uDeltaSize; ++uIndex)
						{
							DefaultConstruct(pElem + uWrite, vParams...);
							++uWrite;
						}
					}
					catch(...)
					{
						while(uWrite != uOffset)
						{
							--uWrite;
							Destruct(pElem + uWrite);
						}
						X_UndoPreparation(uOffset, uDeltaSize);
						throw;
					}
					size += uDeltaSize;
				}
				else
				{
					auto uNewCapacity = Check::Add(uDeltaSize, size);
					const auto uCapacity = getCapacity();
					if(uNewCapacity < uCapacity)
					{
						uNewCapacity = uCapacity;
					}
					vector vecTemp;
					vecTemp.reserve(uNewCapacity);
					for(size_t uIndex = 0; uIndex < uOffset; ++uIndex)
					{
						vecTemp.uncheckedPush(pElem[uIndex]);
					}
					for(size_t uIndex = 0; uIndex < uDeltaSize; ++uIndex)
					{
						vecTemp.uncheckedPush(vParams...);
					}
					for(size_t uIndex = uOffset; uIndex < size; ++uIndex)
					{
						vecTemp.uncheckedPush(pElem[uIndex]);
					}
					*this = move(vecTemp);
				}
			}

			return pElem + uOffset;
		}
		Tpl<Typ tIterator,
			enable_if_t<is_base_of<std::input_iterator_tag,
							Typ std::iterator_traits<
								tIterator>::iterator_category>::value,
				int> = 0>
			fn
		insert(const Element* pPos,
			tIterator itBegin,
			std::common_type_t<tIterator> itEnd)
			->Element*
		{
			size_t uOffset;
			if(pPos)
			{
				uOffset = static_cast<size_t>(pPos - pElem);
			}
			else
			{
				uOffset = size;
			}

			if(itBegin != itEnd)
			{
				if(is_base_of<std::forward_iterator_tag,
					   Typ std::iterator_traits<tIterator>::iterator_category>::
						value)
				{
					const auto uDeltaSize
						= static_cast<size_t>(std::distance(itBegin, itEnd));
					if(is_nothrow_move_constructible_v<Element>)
					{
						X_PrepareForinsertion(uOffset, uDeltaSize);
						auto uWrite = uOffset;
						try
						{
							for(auto itCur = itBegin; itCur != itEnd; ++itCur)
							{
								DefaultConstruct(pElem + uWrite, *itCur);
								++uWrite;
							}
						}
						catch(...)
						{
							while(uWrite != uOffset)
							{
								--uWrite;
								Destruct(pElem + uWrite);
							}
							X_UndoPreparation(uOffset, uDeltaSize);
							throw;
						}
						size += uDeltaSize;
					}
					else
					{
						auto uNewCapacity = Check::Add(uDeltaSize, size);
						const auto uCapacity = getCapacity();
						if(uNewCapacity < uCapacity)
						{
							uNewCapacity = uCapacity;
						}
						vector vecTemp;
						vecTemp.reserve(uNewCapacity);
						for(size_t uIndex = 0; uIndex < uOffset; ++uIndex)
						{
							vecTemp.uncheckedPush(pElem[uIndex]);
						}
						for(auto itCur = itBegin; itCur != itEnd; ++itCur)
						{
							vecTemp.uncheckedPush(*itCur);
						}
						for(size_t uIndex = uOffset; uIndex < size; ++uIndex)
						{
							vecTemp.uncheckedPush(pElem[uIndex]);
						}
						*this = move(vecTemp);
					}
				}
				else
				{
					vector vecTemp;
					const auto uCapacity = getCapacity();
					vecTemp.reserve(uCapacity);
					for(size_t uIndex = 0; uIndex < uOffset; ++uIndex)
					{
						vecTemp.uncheckedPush(pElem[uIndex]);
					}
					for(auto itCur = itBegin; itCur != itEnd; ++itCur)
					{
						vecTemp.Push(*itCur);
					}
					for(size_t uIndex = uOffset; uIndex < size; ++uIndex)
					{
						vecTemp.Push(pElem[uIndex]);
					}
					*this = move(vecTemp);
				}
			}

			return pElem + uOffset;
		}
		fn
		insert(const Element* pPos, std::initializer_list<Element> ilElements)
			->Element*
		{
			return insert(pPos, ilElements.begin(), ilElements.end());
		}

		fn
		erase(const Element* pBegin, const Element* pEnd) noexcept(
			is_nothrow_move_constructible_v<Element>)
			->Element*
		{
			size_t uOffsetBegin, uOffsetEnd;
			if(pBegin)
			{
				uOffsetBegin = static_cast<size_t>(pBegin - pElem);
			}
			else
			{
				uOffsetBegin = size;
			}
			if(pEnd)
			{
				uOffsetEnd = static_cast<size_t>(pEnd - pElem);
			}
			else
			{
				uOffsetEnd = size;
			}

			if(uOffsetBegin != uOffsetEnd)
			{
				if(uOffsetEnd == size)
				{
					const auto uDeltaSize = uOffsetEnd - uOffsetBegin;
					pop(uDeltaSize);
				}
				else if(is_nothrow_move_constructible_v<Element>)
				{
					const auto uDeltaSize = uOffsetEnd - uOffsetBegin;
					for(size_t uIndex = uOffsetBegin; uIndex < uOffsetEnd;
						++uIndex)
					{
						Destruct(pElem + uIndex);
					}
					for(size_t uIndex = uOffsetEnd; uIndex < size; ++uIndex)
					{
						Construct(
							pElem + uIndex - uDeltaSize, move(pElem[uIndex]));
						Destruct(pElem + uIndex);
					}
					size -= uDeltaSize;
				}
				else
				{
					vector vecTemp;
					const auto uCapacity = getCapacity();
					vecTemp.reserve(uCapacity);
					for(size_t uIndex = 0; uIndex < uOffsetBegin; ++uIndex)
					{
						vecTemp.uncheckedPush(pElem[uIndex]);
					}
					for(size_t uIndex = uOffsetEnd; uIndex < size; ++uIndex)
					{
						vecTemp.uncheckedPush(pElem[uIndex]);
					}
					*this = move(vecTemp);
				}
			}

			return pElem + uOffsetBegin;
		}
		fn
		erase(const Element* pPos) noexcept(
			noexcept(declval<vector&>().erase(pPos, pPos)))
			->Element*
		{
			Expects(pPos);

			return erase(pPos, pPos + 1);
		}

		fn
		operator[](size_t uIndex) const noexcept->const Element&
		{
			return uncheckedGet(uIndex);
		}
		fn
		operator[](size_t uIndex) noexcept->Element&
		{
			return uncheckedGet(uIndex);
		}

		friend cfn
		swap(vector& vSelf, vector& vOther) noexcept
		{
			vSelf.Swap(vOther);
		}

		friend cdfn
		begin(const vector& vOther) noexcept
		{
			return vOther.enumerateFirst();
		}
		friend cdfn
		begin(vector& vOther) noexcept
		{
			return vOther.enumerateFirst();
		}
		friend cdfn
		cbegin(const vector& vOther) noexcept
		{
			return begin(vOther);
		}
		friend cdfn
		end(const vector& vOther) noexcept
		{
			return vOther.enumerateSingular();
		}
		friend cdfn
		end(vector& vOther) noexcept
		{
			return vOther.enumerateSingular();
		}
		friend cdfn
		cend(const vector& vOther) noexcept
		{
			return end(vOther);
		}
	};

} // namespace Rider::Faiz
#endif
