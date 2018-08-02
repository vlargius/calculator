#pragma once
#include <string>
#include <stack>

#include "service_char.h"
#include "math_func.h"
#include "variable.h"

using std::string;
using std::cin;

struct Token {
	double value;
	char type;
	string name;

	explicit Token(char c) :
		type(c) {}
	explicit Token(double d) :
		type(number),
		value(d) {}
	explicit Token(char type, string name) :
		type(type),
		name(name) {}

	Token(const Token &) = default;
};


struct Tokenstream
{
	std::stack<Token> buff;
	bool is_free() { return buff.size() == 0; } // is buffer free 

	Tokenstream() {}

	Token get() {
		if (!is_free()) {
			Token t = buff.top();
			buff.pop();
			return t;
		}

		char ch;
		cin >> ch;

		switch (ch)
		{
		case print:
		case sep:
		case '=':
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
		case '!':
		case '(': case ')':
		case '{': case '}':
		case '<': case '>':
		{
			return Token(ch);
		}
		case '.':
		case '0': case '1': case '2':
		case '3': case '4':	case '5':
		case '6': case '7':	case '8':
		case '9': {
			cin.putback(ch); // return back digit of the number
			double val;
			cin >> val;
			return Token(val);
		}
		default:
			if (isalpha(ch)) {
				string s;
				s += ch;
				//get a full name of the variable
				while (cin.get(ch) && (isalpha(ch) || isdigit(ch) || ch == '_')) {
					s += ch;
				}
				cin.putback(ch);
				if (s == exitkey) { return Token(quit_prog); }
				if (s == declkey) {	return Token(let); }
				if (s == sqrtkey) { return Token(func_type, s); }
				if (s == powkey) { return Token(func_type, s); }
				return Token(variable, s);
			}
			if (ch == '#') {
				string s;
				s += ch;
				if (s == declkey) { return Token(let); }
			}
			throw BadToken(ch);
		}
	}

	//ignoring tokens until c
	void ignore(char c) {

		while (!is_free()) {
			buff.pop();
		}

		char ch{ 0 };
		while (cin >> ch) {
			if (ch == c) { return; }
		}
	}

	void put_back(Token t) {
		buff.push(t);
	}
} ts;


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
		return vars::get_value(t.name);
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

double declaration() {
	Token t = ts.get();
	if (t.type != variable) {
		throw BadArgument(t.name);
	}
	string var_name = t.name;
	t = ts.get();
	if (t.type != '=') {
		throw BadArgument("= sign expecded");
	}

	double d = expression();
	vars::define_variable(var_name, d);
	return d;
}

double statement() {
	Token t = ts.get();
	switch (t.type)
	{
	case let:
	{
		return declaration();
		break;
	}
	case variable:
	{
		if (!vars::is_declared(t.name)) {
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
		vars::set_value(var_name, d);
		return d;
	}
	default:
		ts.put_back(t);
		return expression();
		break;
	}
}