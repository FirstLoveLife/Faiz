#include "rider/faiz/call_traits.hpp"
#include "rider/faiz/faiz.hpp"
#include "rider/faiz/type_traits.hpp"

#include <type_traits>


namespace rider::faiz
{
	template<typename T1, typename T2>
	class compressed_pair;


	template<typename T1,
		typename T2,
		bool isSame,
		bool firstEmpty,
		bool secondEmpty>
	struct compressed_pair_switch;

#define ImplSwitch(ISSAME, FIRSTEMPTY, SECONDEMPTY, VALUE) \
	template<typename T1, typename T2> \
	struct compressed_pair_switch<T1, T2, ISSAME, FIRSTEMPTY, SECONDEMPTY> \
	{ \
		static constexpr int value = VALUE; \
	};

	// clang-format off
    ImplSwitch(false, false, false, 0)
    ImplSwitch(false, true, false, 1)
    ImplSwitch(false, false, true, 2)
    ImplSwitch(false, true, true, 3)
    ImplSwitch(true, true, true, 4)
    ImplSwitch(true, false, false, 5)
#undef ImplSwitch

    template<typename T1, typename T2, int version>
    class compressed_pair_imp;

	template<typename T>
	inline void
	cp_swap(T& t1, T& t2)
	{
		T tTemp = t1;
		t1 = t2;
		t2 = tTemp;
	}
#define ImplAlias \
using first_type = T1; \
		using second_type = T2; \
		using first_param_type = typename call_traits<first_type>::param_type; \
		using second_param_type = \
			typename call_traits<second_type>::param_type; \
		using first_reference = typename call_traits<first_type>::reference; \
		using second_reference = typename call_traits<second_type>::reference; \
		using first_const_reference = \
			typename call_traits<first_type>::const_reference; \
		using second_const_reference = \
			typename call_traits<first_type>::const_reference;

#define ImplCompressedUeqIMP(VALUE, BASE, CTOR1, CTOR2, GET1, GET2, M1, M2, SWAP) \
	template<typename T1, typename T2> \
	class compressed_pair_imp<T1, T2, VALUE> BASE \
	{ \
	public: \
		ImplAlias					  \
		compressed_pair_imp() {} \
		compressed_pair_imp(first_param_type x, second_param_type y) : CTOR1(x), CTOR2(y) {} \
		compressed_pair_imp(first_param_type x) : CTOR1(x) {} \
		compressed_pair_imp(second_param_type y) : CTOR2(y) {} \
	    first_reference first() { return GET1;}  \
		first_const_reference first() const { return GET1; } \
		second_reference second() { return GET2;} \
		second_const_reference second() const { return GET2;} \
		void swap(compressed_pair<T1, T2>& y) { SWAP } \
	private: \
		M1; \
		M2; \
	};

#define ImplCompressedEqIMP(VALUE, BASE, INITLBI, INITLUN, GET1, GET2, M1, M2, SWAP) \
	template<typename T1, typename T2> \
	class compressed_pair_imp<T1, T2, VALUE> BASE \
	{ \
	public: \
		ImplAlias					  \
		compressed_pair_imp() {} \
		compressed_pair_imp(first_param_type x, second_param_type y) : INITLBI {} \
		compressed_pair_imp(first_param_type x) : INITLUN {} \
	    first_reference first() { return GET1;}  \
		first_const_reference first() const { return GET1; } \
		second_reference second() { return GET2;} \
		second_const_reference second() const { return GET2;} \
		void swap(compressed_pair<T1, T2>& y) { SWAP } \
	private: \
		M1; \
		M2; \
	};

	ImplCompressedUeqIMP
	(0,
	 ,
	 mFirst, mSecond,
	 mFirst, mSecond,
	 first_type mFirst,
	 second_type mSecond,
	 cp_swap(mFirst COMMA y.first()) SEMICOLON
	 cp_swap(mSecond COMMA y.second()) SEMICOLON)

	ImplCompressedUeqIMP
	(1,
	 : private T1,
	 first_type, mSecond,
	 *this, mSecond,
	 ,
	 second_type mSecond,
	 cp_swap(mSecond COMMA y.second()) SEMICOLON)

	ImplCompressedUeqIMP
	(2,
	 : private T2,
	 mFirst, second_type,
	 mFirst, *this,
	 first_type mFirst,
	 ,
	 cp_swap(mFirst COMMA y.first()) SEMICOLON)

	ImplCompressedUeqIMP
	(3,
	 : private T1 COMMA private T2,
	 first_type, second_type,
	 *this, *this,
	 ,
	 ,
	 )


	ImplCompressedEqIMP
	(4,
	 : private T1,
	 first_type(x), first_type(x),
	 *this, *this,
	 ,
	 ,
	 )

	ImplCompressedEqIMP
	(5,
	 ,
	 mFirst(x) COMMA mSecond(y),
	 mFirst(x) COMMA mSecond(x),
	 mFirst, mSecond,
	 first_type mFirst,
	 second_type mSecond,
	 cp_swap(mFirst COMMA y.first()) SEMICOLON
	 cp_swap(mSecond COMMA y.second()) SEMICOLON)


	template<typename T1, typename T2>
	class compressed_pair : private compressed_pair_imp<T1,
								T2,
								compressed_pair_switch<T1,
									T2,
									is_same_v<remove_cv_t<T1>, remove_cv_t<T2>>,
									is_empty_v<T1>,
									is_empty_v<T2>>::value>
	{
	private:
		using base = compressed_pair_imp<T1,
			T2,
			compressed_pair_switch<T1,
				T2,
				is_same_v<remove_cv_t<T1>, remove_cv_t<T2>>,
				is_empty_v<T1>,
				is_empty_v<T2>>::value>;

	public:
		ImplAlias

		compressed_pair() : base()
		{}
		compressed_pair(first_param_type x, second_param_type y) : base(x, y)
		{}
		explicit compressed_pair(first_param_type x) : base(x)
		{}
		explicit compressed_pair(second_param_type y) : base(y)
		{}

		first_reference
		first()
		{
			return base::first();
		}
		first_const_reference
		first() const
		{
			return base::first();
		}

		second_reference
		second()
		{
			return base::second();
		}
		second_const_reference
		second() const
		{
			return base::second();
		}

		void
		swap(compressed_pair& y)
		{
			base::swap(y);
		}
	};

	// Partial specialisation for case where T1 == T2:
	template<typename T>
	class compressed_pair<T, T>
		: private compressed_pair_imp<T,
			  T,
			  compressed_pair_switch<T,
				  T,
				  is_same_v<remove_cv_t<T>, remove_cv_t<T>>,
				  is_empty_v<T>,
				  is_empty_v<T>>::value>
	{
	private:
		using base = compressed_pair_imp<T,
			T,
			compressed_pair_switch<T,
				T,
				is_same_v<remove_cv_t<T>, remove_cv_t<T>>,
				is_empty_v<T>,
				is_empty_v<T>>::value>;

	public:
		using first_type = T;
		using second_type = T;
		using first_param_type = typename call_traits<first_type>::param_type;
		using second_param_type = typename call_traits<second_type>::param_type;
		using first_reference = typename call_traits<first_type>::reference;
		using second_reference = typename call_traits<second_type>::reference;
		using first_const_reference =
			typename call_traits<first_type>::const_reference;
		using second_const_reference =
			typename call_traits<second_type>::const_reference;

		compressed_pair() : base()
		{}
		compressed_pair(first_param_type x, second_param_type y) : base(x, y)
		{}
		explicit compressed_pair(first_param_type x) : base(x)
		{}

		first_reference
		first()
		{
			return base::first();
		}
		first_const_reference
		first() const
		{
			return base::first();
		}

		second_reference
		second()
		{
			return base::second();
		}
		second_const_reference
		second() const
		{
			return base::second();
		}

		void
		swap(compressed_pair<T, T>& y)
		{
			base::swap(y);
		}
	};


	template<typename T1, typename T2>
	inline void
	swap(compressed_pair<T1, T2>& x, compressed_pair<T1, T2>& y)
	{
		x.swap(y);
	}
} // namespace rider::faiz

// clang-format on
