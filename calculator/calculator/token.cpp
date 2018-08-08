#include "token.h"

#include <iostream>
#include <string>


#include "exceptions.h"
#include "service_char.h"


using std::string;

Token::Token(char c) :
	type(c) {}
Token::Token(double d) :
	type(number),
	value(d) {}
Token::Token(char type, string name) :
	type(type),
	name(name) {}

Token Tokenstream::get() {
	if (!is_free()) {
		Token t = buff.top();
		buff.pop();
		return t;
	}

	if (in->peek() == EOF) {
		set_default_stream();
	}

	char ch;
	while (in->get(ch) && isspace(ch)) {}

	switch (ch)
	{
	case next_str:
	{
		return Token(print);
	}
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
		in->putback(ch); // return back digit of the number
		double val;
		*in >> val;
		return Token(val);
	}
	default:
		if (isalpha(ch)) {
			string s;
			s += ch;
			//get a full name of the variable
			while (in->get(ch) && (isalpha(ch) || isdigit(ch) || ch == '_')) {
				s += ch;
			}
			in->putback(ch);
			if (s == exitkey) { return Token(quit_prog); }
			if (s == helpkey) { return Token(helpcall); }
			if (s == fromkey) { return Token(fromcall); }
			if (s == tokey) { return Token(tocall); }

			if (s == declkey) { return Token(let); }
			if (s == constkey) { return Token(const_tok); }
			if (s == sqrtkey) { return Token(func_type, s); }
			if (s == powkey) { return Token(func_type, s); }
			if (s == sinkey) { return Token(func_type, s); }
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
void Tokenstream::ignore(char c) {

	while (!is_free()) {
		if (buff.top().type == c) { return; }
		buff.pop();
	}

	char ch{ 0 };
	while (in->get(ch) && ch != next_str) {
		if (ch == c) { return; }
	}
}

void Tokenstream::put_back(Token t) {
	buff.push(t);
}
