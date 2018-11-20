#pragma once
#include <string>

#include "service_char.h"

using std::string;

struct Error {
	string msg;

	Error(const string msg) :
		msg(msg.c_str()) {}

	virtual string what() { return msg; }
};

struct NoCloseBracket : Error {
	NoCloseBracket() :
		Error("closing bracket expected") {}
};

struct NoOpenBracket : Error {
	NoOpenBracket() :
		Error("open bracket expected") {}
};

struct NoTerminator : Error {
	NoTerminator() :
		Error(print + " expected") {}
};

struct BadToken : Error {
	BadToken(char ch) :
		Error("bad token: " + string(1, ch)) {}
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
	BadArgument(const string user_msg = "") :
		Error("bad argument: " + user_msg) {}
};

struct VariableError: Error {
	VariableError(const string msg) :
		Error(msg) {}
};

struct ConstAssigning : VariableError {
	ConstAssigning(const string msg) :
		VariableError("cant set cosntant: " + msg) {}
};

struct MissingVariable : VariableError {
	MissingVariable(const string name) :
		VariableError("missing variable: " + name) {}
};

struct MissingFunction : Error {
	MissingFunction(const string name) :
		Error("missing function: " + name) {}
};
