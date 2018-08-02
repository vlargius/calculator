#pragma once
#include <string>

struct Error {
	std::string msg;

	Error(std::string msg) :
		msg(msg.c_str()) {}

	virtual std::string what() { return msg; }
};

struct NoCloseBracket : Error {
	NoCloseBracket() :
		Error("closing bracket expected") {}
};

struct BadToken : Error {
	BadToken(char ch) :
		Error("bad token: " + std::string(1, ch)) {}
};

struct ZeroDivision : Error {
	ZeroDivision() :
		Error("zero division") {}
};

struct PrimaryExpressionExpected : Error {
	PrimaryExpressionExpected() :
		Error("primary expression expected") {}
};

struct BadArgument : Error {
	BadArgument(std::string user_msg = "") :
		Error("bad argument: " + user_msg) {}
};

