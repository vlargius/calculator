#pragma once

#include <iostream>
#include <stack>
#include <string>
#include <memory>

using namespace std;

struct Token {
	double value;
	char type;
	string name;

	Token(const Token &) = default;

	explicit Token(char c);
	explicit Token(double d);
	explicit Token(char type, string name);
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
	stack<Token> buff;
	istream * in;

	istream * const default_strem = &cin;

};
