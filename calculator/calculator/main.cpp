#include <iostream>
#include <vector>
#include <stdlib.h>
#ifdef _MBCS
#include <Windows.h>
#endif // WIN32

using std::cin;
using std::cout;

struct Error {
	const char* msg;

	Error(const char* msg) :
		msg(msg) {}

	virtual const char* what() { return msg; }
};

struct NoCloseBracket : Error {
	NoCloseBracket():
		Error("closing bracket expected"){}
};

struct BadToken : Error {
	BadToken() :
		Error("bad token") {}
};


int factorial(double d) {
	if (d == 0) {
		return 1;
	}
	else {
		return d * factorial(d - 1);
	}
}



struct Token {
	double value;
	char type;

	Token(char c) :
		type(c) {}
	Token(double d):
		type('8'),
		value(d){}

	Token(const Token &) = default;
};



struct Tokenstream
{
	Token buff;
	bool is_free;

	Tokenstream() :
		is_free(true),
		buff('0') {		
	}

	Token get() {
		if (!is_free) {
			is_free = true;
			return buff;
		}

		char ch;

		std::cin >> ch;

		switch (ch)
		{
		case 'x': case '+': case '-': case '*': case '/': case '%': case '=': case '!':
		case ')': case '(': case '{': case '}': case '<': case '>':
		{
			return Token(ch);
		}
		case '.': case '0': case '1':
		case '2': case '3': case '4':
		case '5': case '6': case '7':
		case '8': case '9': {
			std::cin.putback(ch);
			double val;
			std::cin >> val;
			return Token(val);
		}
		default:
			throw BadToken();
		}

	}

	void put_back(Token t) {
		if (!is_free) throw Error("Buffer is not free");
		buff = t;
		is_free = false;
	}
} ts;

double term();

double expression() {
	double left = term();
	Token t = ts.get();
	while(true) {
		switch(t.type) {
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
	case '8':
	{
		return t.value;
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
		ts.put_back(t);
		break;
	}
}

double fact() {
	double val = primary();
	Token n = ts.get();
	switch (n.type)
	{
	case '!':
	{
		return factorial(val);
	}
	default:
		ts.put_back(n);
		return val;
		break;
	}
}


double term() {
	double left = fact();
	Token t = ts.get();
	while (true) {
		switch (t.type) {		
		case '*':
		{
			left *= fact();
			break;
		}
		case '/':
		{
			left /= fact();
			break;
		}
		case '%':
		{
			left = (int) left % (int) t.value;
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


int main() {


	std::cout << "Enter expression with floating number" << std::endl
		<< "You can use: +, -, *, /, % and = for printing value" << std::endl
		<< "For exit print x" << std::endl
		<< "---------------------------------------------------" << std::endl;
	try {
		double val{ 0 };
		while (std::cin) {
			cout << "> ";
			Token t = ts.get();
			while(t.kind == ';') t = ts.get();
			if(t.kind == 'q') {
				return 0;
			}
			ts.putback(t);
			cout << "= " << expression() << endl;
		}
	}
	catch (Error & e) {
		std::cout << e.what() << std::endl;
		system("pause");
	}
	catch (...) {
		std::cout << "Unexpected exeption" << std::endl;
		system("pause");
	}
	return 0;
}
