#pragma once

#include <iostream>
#include <stack>
#include <string>
#include <memory>

using std::istream;
using std::string;
using std::cin;

struct Token {
	double value;
	char type;
	std::string name;

	Token(const Token &) = default;

	explicit Token(char c);
	explicit Token(double d);
	explicit Token(char type, std::string name);
};


class Tokenstream
{
public:
	Tokenstream(istream & in = cin) :
		in(&in) {}

	void set_stream(istream* stream) { 
		in = stream;
	}
	void set_default_stream() {
		in = default_strem;
	}
	istream & get_stream() { return *in; }

	Token get();
	void put_back(Token t);
	//ignoring tokens until c
	void ignore(char c);	
	bool is_free() { return buff.size() == 0; } // is buffer free 

private:
	std::stack<Token> buff;
	istream * in;

	istream * const default_strem = &cin;

};
