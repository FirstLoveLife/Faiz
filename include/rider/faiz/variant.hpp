#ifndef VARIANT
#define VARIANT
/*
http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0472r0.html
 */
namespace rider::faiz
{

	// Unit type intended for use as a well-behaved empty alternative in
	// `faiz::variant`. In particular, a variant of non-default-constructible
	// types may list `faiz::monostate` as its first alternative: this makes the
	// variant itself default-constructible.
	struct monostate
	{};
} // namespace rider::faiz
#endif
