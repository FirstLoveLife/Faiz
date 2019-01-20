#ifndef OPTIONAL
// TODO: implement ***::insert with optional, instead of pair
//
#	include "rider/faiz/macros.hpp"
namespace Rider::Faiz
{
	// `Faiz::nullopt_t` is an empty class type used to indicate optional type
	// with uninitialized state. In particular, `Faiz::optional` has a
	// constructor with nullopt_t as a single argument, which creates an
	// optional that does not contain a value.
	//
	// Faiz::nullopt_t must be a non-aggregate LiteralType and cannot have a
	// default constructor or an initializer-list constructor.
	//
	// It must have a cexp constructor that takes some
	// implementation-defined literal type.
	//
	// The constraints on nullopt_t's constructors exist to support both `op =
	// {};` and `op = nullopt;` as the syntax for disengaging an optional
	// object.
	struct nullopt_t
	{
		struct secret_tag
		{
			inline explicit secret_tag() = default;
		};
		cexp explicit nullopt_t(secret_tag, secret_tag) noexcept
		{}
	};

	// `Faiz::nullopt` is a constant of type `Faiz::nullopt_t` that is used to
	// indicate optional type with uninitialized state.
	inline cexp nullopt_t nullopt{
		nullopt_t::secret_tag{}, nullopt_t::secret_tag{}};

} // namespace Rider::Faiz
#	define OPTIONAL
#endif
