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

struct NoOpenBracket : Error {
	NoOpenBracket() :
		Error("open bracket expected") {}
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

struct VariableError: Error {
	VariableError(std::string msg) :
		Error(msg) {}
};

struct ConstAssigning : VariableError {
	ConstAssigning(std::string msg) :
		VariableError("cant set cosntant: " + msg) {}
};

struct MissingVariable : VariableError {
	MissingVariable(std::string name) :
		VariableError("missing variable: " + name) {}
};

struct MissingFunction : Error {
	MissingFunction(std::string name) :
		Error("missing function: " + name) {}
};

inline void error(const std::string& msg) {
	throw Error(msg);
}


