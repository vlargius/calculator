#pragma once
#include <string>

#include "service_char.h"
#include "math_func.h"
#include "token.h"
#include "variable.h"

using std::string;
using std::cin;

double term();

double expression() {
	double left = term();
	Token t = ts.get();
	while (true) {
		switch (t.type) {
		case '+':
		{
			left += term();
			break;
		}
		case '-':
		{
			left -= term();
			break;
		}
		default:
			ts.put_back(t);
			return left;
		}
		t = ts.get();
	}
	return left;
}

double func() {
	Token t = ts.get();
	if (t.name == sqrtkey) {
		string name = t.name;
		t = ts.get();
		if (t.type != '(') { throw NoOpenBracket(); }
		double d = expression();
		if (d < 0) {
			throw BadArgument("sqrt argument must be more than zero");
		}
		t = ts.get();
		if (t.type != ')') { throw NoOpenBracket(); }
		return sqrt(d);
	}

	if (t.name == powkey) {
		string name = t.name;
		t = ts.get();
		if (t.type != '(') { throw NoOpenBracket(); }
		double x = expression();
		t = ts.get();
		if (t.type != ',') { throw BadArgument("comma missed"); }
		double i = expression();
		if (i != int(i)) {
			throw BadArgument("pow(x, i) - i must be integer");
		}
		t = ts.get();
		if (t.type != ')') { throw NoOpenBracket(); }
		return power(x, i);
	}

	if (t.name == sinkey) {
		string name = t.name;
		t = ts.get();
		if (t.type != '(') { throw NoOpenBracket(); }
		double d = expression();
		t = ts.get();
		if (t.type != ')') { throw NoOpenBracket(); }
		return sin(d);
	}

	
	throw MissingFunction(t.name);
}

double primary() {
	Token t = ts.get();
	switch (t.type)
	{
	case '(':
	{
		double v = expression();
		t = ts.get();
		if (t.type != ')') {
			throw NoCloseBracket();
		}
		return v;
	}
	case '{':
	{
		double v = expression();
		t = ts.get();
		if (t.type != '}') {
			throw NoCloseBracket();
		}
		return v;
	}
	case '<':
	{
		double v = expression();
		t = ts.get();
		if (t.type != '>') {
			throw NoCloseBracket();
		}
		return v;
	}
	case number:
	{
		return t.value;
	}
	case variable:
	{
		return symb_tbale.get_value(t.name);
	}
	case func_type:
	{
		ts.put_back(t);
		return func();
	}
	case '-':
	{
		return -primary();
	}
	case '+':
	{
		return primary();
	}
	default:
		throw PrimaryExpressionExpected();
		break;
	}
}

double post_primary() {
	double val = primary();
	Token n = ts.get();
	switch (n.type)
	{
	case '!':
	{
		int d = val; 
		if (d != val)
			throw BadArgument("factorial argument must be integer");
		return factorial(d);
	}
	default:
		ts.put_back(n);
		return val;
		break;
	}
}


double term() {
	double left = post_primary();
	Token t = ts.get();
	while (true) {
		switch (t.type) {
		case '*':
		{
			left *= post_primary();
			break;
		}
		case '/':
		{
			left /= post_primary();
			break;
		}
		case '%':
		{
			int i1 = int(left);
			if (i1 != left)
				throw BadArgument("left argument is not integer");
			double d = term();
			int i2 = int(d);
			if (i2 != d)
				throw BadArgument("right argument is not integer");
			if (i2 == 0)
				throw ZeroDivision();
			left = i1 % i2;
			break;
		}
		default:
			ts.put_back(t); // put current token back to stream

			return left;
		}
		t = ts.get();
	}
	return left;
}

void clean_up() {
	ts.ignore(print);
}

double declaration(bool is_const) {
	Token t = ts.get();
	if (t.type != variable) {
		throw BadArgument(t.name);
	}
	string var_name = t.name;
	double d{ 0 };
	if (is_const) {
		t = ts.get();
		if (t.type != '=') {
			throw BadArgument("= sign expecded");
		}
		d = expression();
		symb_tbale.declare_constant(var_name, d);
	}
	else {
		t = ts.get();
		if (t.type == '=') {
			d = expression();
		}		
		symb_tbale.declare_constant(var_name, d);
	}
	return d;
}

double statement() {
	Token t = ts.get();
	switch (t.type)
	{
	case let:
	{
		return declaration(false);
		break;
	}
	case const_tok:
	{
		return declaration(true);
		break;
	}
	case variable:
	{
		if (!symb_tbale.is_declared(t.name)) {
			throw MissingVariable(t.name);
		}
		string var_name = t.name;
		Token m = ts.get();
		if (m.type != '=') {
			ts.put_back(m);
			ts.put_back(t);
			return expression();
		}
		double d = expression();
		symb_tbale.set_value(var_name, d);
		return d;
	}
	default:
		ts.put_back(t);
		return expression();
		break;
	}
}