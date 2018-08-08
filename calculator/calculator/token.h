#pragma once

#include <iostream>
#include <stack>
#include <string>

#include "roman_int.h"


struct Token {
	RomanInt value;
	char type;
	std::string name;

	Token(const Token &) = default;

	explicit Token(char c);
	explicit Token(RomanInt d);
	explicit Token(char type, std::string name);
};


class Tokenstream
{
public:
	Tokenstream(std::istream & in);

	Token get();
	void put_back(Token t);
	//ignoring tokens until c
	void ignore(char c);	
	bool is_free() { return buff.size() == 0; } // is buffer free 

private:
	std::stack<Token> buff;
	std::istream & in;

};
