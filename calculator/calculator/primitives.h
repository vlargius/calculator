#pragma once
#include <string>

#include "service_char.h"
#include "math_func.h"
#include "token.h"
#include "variable.h"

using std::string;
using std::cin;

RomanInt term(Tokenstream & ts);

RomanInt expression(Tokenstream & ts) {
	RomanInt left = term(ts);
	Token t = ts.get();
	while (true) {
		switch (t.type) {
		case '+':
		{
			left += term(ts);
			break;
		}
		case '-':
		{
			left -= term(ts);
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

RomanInt func(Tokenstream & ts) {
	Token t = ts.get();
	if (t.name == sqrtkey) {
		string name = t.name;
		t = ts.get();
		if (t.type != '(') { throw NoOpenBracket(); }
		double d = expression(ts);
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
		int x =(int) expression(ts);
		t = ts.get();
		if (t.type != ',') { throw BadArgument("comma missed"); }
		double i = expression(ts);
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
		double d = expression(ts);
		t = ts.get();
		if (t.type != ')') { throw NoOpenBracket(); }
		return sin(d);
	}

	
	throw MissingFunction(t.name);
}

double primary(Tokenstream & ts) {
	Token t = ts.get();
	switch (t.type)
	{
	case '(':
	{
		double v = expression(ts);
		t = ts.get();
		if (t.type != ')') {
			throw NoCloseBracket();
		}
		return v;
	}
	case '{':
	{
		double v = expression(ts);
		t = ts.get();
		if (t.type != '}') {
			throw NoCloseBracket();
		}
		return v;
	}
	case '<':
	{
		double v = expression(ts);
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
		return func(ts);
	}
	case '-':
	{
		return -primary(ts);
	}
	case '+':
	{
		return primary(ts);
	}
	default:
		throw PrimaryExpressionExpected();
		break;
	}
}

RomanInt suffix_primary(Tokenstream & ts) {
	RomanInt val = primary(ts);
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


RomanInt term(Tokenstream & ts) {
	RomanInt left = suffix_primary(ts);
	Token t = ts.get();
	while (true) {
		switch (t.type) {
		case '*':
		{
			left *= suffix_primary(ts);
			break;
		}
		case '/':
		{
			left /= suffix_primary(ts);
			break;
		}
		case '%':
		{
			int i1 = int(left);
			if (i1 != left)
				throw BadArgument("left argument is not integer");
			double d = term(ts);
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

void clean_up(Tokenstream & ts) {
	ts.ignore(print);
}

double declaration(Tokenstream & ts, bool is_const) {
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
		d = expression(ts);
		symb_tbale.declare_constant(var_name, d);
	}
	else {
		t = ts.get();
		if (t.type == '=') {
			d = expression(ts);
		}		
		symb_tbale.declare_constant(var_name, d);
	}
	return d;
}

RomanInt statement(Tokenstream & ts) {
	Token t = ts.get();
	switch (t.type)
	{
	case let:
	{
		return declaration(ts, false);
		break;
	}
	case const_tok:
	{
		return declaration(ts, true);
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
			return expression(ts);
		}
		double d = expression(ts);
		symb_tbale.set_value(var_name, d);
		return d;
	}
	default:
		ts.put_back(t);
		return expression(ts);
		break;
	}
}