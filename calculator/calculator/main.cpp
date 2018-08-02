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
			cout << result << expression() << endl;
		} catch (Error & e) {
			cout << e.what() << endl;
			clean_up();
		}
	}
}

int main() {


	cout << "Enter expression with floating number" << endl
		<< "Use: [+, -, *, /, %]" << endl
		<< "Use = for printing value" << endl
		<< "For exit print " << quit_prog << endl
		<< "-------------------------------------------" << endl;
	try {
		calculate();
	}
	catch (Error & e) {
		cout << e.what() << endl;
		system("pause");
	}
	catch (...) {
		cout << "Unexpected exeption" << endl;
		system("pause");
	}
	return 0;
}
