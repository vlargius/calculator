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


void calculate() {
	double val{ 0 };
	while (cin) {
		try {
			cout << prompt;
			Token t = ts.get();
			while (t.type == print) t = ts.get();//print canselation
			if (t.type == quit_prog) { //exit
				system("pause");
				return;
			}
			ts.put_back(t);
			cout << result << statement() << endl;
		} catch (Error & e) {
			cout << e.what() << endl;
			clean_up();
		}
	}
}

int main() {


	cout << "Enter expression with floating number" << endl
		<< "Use: [+, -, *, /, %]" << endl
		<< "pow, sqrt - available" << endl
		<< "Use = for printing value" << endl
		<< "For exit print " << quit_prog << endl
		<< "-------------------------------------------" << endl;
	try {
		vars::define_constant("pi", 3.1414);
		vars::define_constant("e", 2.7182818);
		vars::define_constant("k", 1000);
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
