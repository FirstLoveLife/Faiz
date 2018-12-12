#ifndef CHECKD_DELETER
#define CHECKD_DELETER
namespace Rider::Faiz
{
	// std::shared_ptr alternative: https://stackoverflow.com/a/22091803/6949852
	// https://stackoverflow.com/a/6802060/6949852:
	// > > * What's the result of applying sizeof to an incomplete type?
	// >
	// > A compile-time error, unless the compiler chooses to return 0 as a
	// > nonstandard extension.
	// >
	// > > * Why is sizeof called twice?
	// >
	// > The second sizeof is a workaround for a Metrowerks CodeWarrior bug in
	// > which  the first typeof is never instantiated unless used.
	// >
	// > > * Why is the result of sizeof cast to void? What exactly does that
	// > line
	// > > do?
	// >
	// > Silences a compiler warning.

	template<class T>
	void
	checked_delete(T* x)
	{
		// intentionally complex - simplification causes regressions
		using type_must_be_complete = char[sizeof(T) ? 1 : -1];
		(void)sizeof(type_must_be_complete);
		delete x;
	}

	template<class T>
	void
	checked_array_delete(T* x)
	{
		using type_must_be_complete = char[sizeof(T) ? 1 : -1];
		(void)sizeof(type_must_be_complete);
		delete[] x;
	}

	template<class T>
	struct checked_deleter
	{
		using result_type = void ;
		using argument_type =  T*;

		void
		operator()(T* x) const
		{
			// adl is shit.
			Faiz::checked_delete(x);
		}
	};

	template<class T>
	struct checked_array_deleter
	{
		using result_type = void;
		using argument_type = T*;

		void
		operator()(T* x) const
		{
			Faiz::checked_array_delete(x);
		}
	};

} // namespace Rider::Faiz
#endif
