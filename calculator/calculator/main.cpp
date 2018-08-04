#include <iostream>
#include <vector>
#include <stdlib.h>

#include "exceptions.h"
#include "primitives.h"

/*
todo:
1) replace all magic constants +
2) add comments
3) split main function +
4) ignore all chars until printing +
*/

using std::cin;
using std::cout;
using std::endl;

void help() {
	cout << "Enter expression with floating number" << endl
		<< "________________________________________________" << endl
		<< "use: [+, -, *, /, %] signs" << endl
		<< "pow, sqrt - available" << endl
		<< declkey << " [var_name] = (value) - create variable" << endl
		<< constkey << " [const_name] = (value) - create constant" << endl
		<< "------------------------------------------------" << endl
		<< "use " << print << " for printing value, " << "for exit type " << exitkey << endl
		<< "------------------------------------------------" << endl
		<< "type h for help" << endl;
}

void calculate() {
	double val{ 0 };
	Tokenstream ts(cin);
	while (cin) {
		try {
			cout << prompt;
			Token t = ts.get();

			//print cancelation
			while (t.type == print) t = ts.get();

			//exit
			if (t.type == quit_prog) { 
				system("pause");
				return;
			}
			if (t.type == helpcall) {
				help();
				continue;
			}
			ts.put_back(t);
			cout << result << statement(ts) << endl;
		} catch (Error & e) {
			cout << e.what() << endl;
			clean_up(ts);
		}
	}
}

int main() {
	
	try {
		symb_tbale.declare_constant("pi", 3.1414);
		symb_tbale.declare_constant("e", 2.7182818);
		symb_tbale.declare_constant("k", 1000);

		help();
		calculate();
	}
	catch (Error & e) {
		cout << e.what() << endl;
		system("pause");
	}
	catch (...) {
		cout << "Unexpected exception" << endl;
		system("pause");
	}
	return 0;
}
