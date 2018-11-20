#pragma once

#include <string>
#include <iostream>

#include "service_char.h"
#include "math_func.h"
#include "token.h"
#include "variable.h"

using namespace std;

class primitives {
public:
	static SymbolTable symb_tbale;

	static double term(Tokenstream & ts);

	static double expression(Tokenstream & ts);

	static void proc(ostream & os, Tokenstream & ts);

	static double func(Tokenstream & ts);

	static double primary(Tokenstream & ts);

	static void clean_up(Tokenstream & ts);

	static double suffix_primary(Tokenstream & ts);

	static double declaration(Tokenstream & ts, bool is_const);

	static ostream & statement(ostream & os, Tokenstream & ts);
};