#ifndef KOALA__UTILITY__INSTRUCTION
#define KOALA__UTILITY__INSTRUCTION

namespace Koala::Utility {

enum class Instruction : unsigned char
{
	// System
	nop = 0x00,
	halt,

	// Stack
	push8 = 0x10,
	push64,
	dup8,
	dup64,
	drop8,
	drop64,
	swap8,
	swap64,

	// Math
	add8 = 0x30,
	add64,
	add64i,
	sub8,
	sub64,
	sub64i,
	mul,
	div,
	mod,
	sqrt,
	pow,

	// I/O
	printchar = 0x50,
	readchar,
	printnum,
	readnum,
	printstr,
	readstr,
	printref,

	// Variable
	reserve = 0x70,
	setlocal,
	getlocal,
	getconst,

	// Memory
	ref = 0x80,
	deref,
	assignref,
	alloc,
	dealloc,

	// Conversions
	d2i = 0xA0,
	i2d,
	s2i,
	i2s,
	s2d,
	d2s,

	// Logic
	equal8 = 0xB0,
	equal64,
	greater8,
	greater64,
	and,
	or,
	not,

	// Branch
	jmp = 0xD0,
	jmpcond,
	call,
	ret,

	// Reserved
	NONE = 0xE0,
};

} // namespace Koala::Utility

#endif // KOALA__UTILITY__INSTRUCTION
