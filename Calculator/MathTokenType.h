#pragma once

enum class MathTokenType
{
	Invalid,      // Value = null
	Invisible,    // Value = null

	String,       // Value = string
	Number,       // Value = double
	OpenBracket,  // Value = null
	CloseBracket, // Value = null
	Add,          // Value = null
	Sub,          // Value = null
	Div,          // Value = null
	Mul,          // Value = null
	Equ,          // Value = null
	Pow,          // Value = null
	Fact,         // Value = null

	Comma,        // Value = null
};