#ifndef STRING
#define STRING
#include "rider/faiz/iosfwd.hpp"
#include "rider/faiz/macros.hpp"
namespace Rider::Faiz
{
	tpl<class StateT> class fpos
	{
	private:
		StateT st_;
		streamoff off_;

	public:
		fpos(streamoff off = streamoff()) : st_(), off_(off)
		{}

		operator streamoff() const
		{
			return off_;
		}

		StateT
		state() const
		{
			return st_;
		}
		void
		state(StateT st)
		{
			st_ = st;
		}

		fpos&
		operator+=(streamoff off)
		{
			off_ += off;
			return *this;
		}
		fpos
		operator+(streamoff off) const
		{
			fpos t(*this);
			t += off;
			return t;
		}
		fpos&
		operator-=(streamoff off)
		{
			off_ -= off;
			return *this;
		}
		fpos
		operator-(streamoff off) const
		{
			fpos t(*this);
			t -= off;
			return t;
		}
	};

} // namespace Rider::Faiz
#endif
