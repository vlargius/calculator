#pragma once

#include <string>

#include "service_char.h"
#include "math_func.h"
#include "token.h"
#include "variable.h"

using std::string;
using std::cin;

class primitives {
public:
	static SymbolTable symb_tbale;

	static double term(Tokenstream & ts);

	static double expression(Tokenstream & ts);

	static double func(Tokenstream & ts);

	static double primary(Tokenstream & ts);

	static void clean_up(Tokenstream & ts);

	static double suffix_primary(Tokenstream & ts);

	static double declaration(Tokenstream & ts, bool is_const);

	static double statement(Tokenstream & ts);
};