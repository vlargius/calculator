#include <iostream>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <memory>

#include "exceptions.h"
#include "primitives.h"

using namespace std;

void help() {
	cout << "Enter expression with floating number" << endl
		<< "_________________________________________________" << endl
		<< "use: [+, -, *, /, %] signs" << endl
		<< "pow, sqrt - available" << endl
		<< declkey << " [var_name] = (value) - create variable" << endl
		<< constkey << " [const_name] = (value) - create constant" << endl
		<< "-------------------------------------------------" << endl
		<< "to set input file print \"" << fromkey << " [file_name] \""<< endl
		<< "to set output file print \"" << tokey << " [file_name] \"" << endl
		<< "use keyword \"" << stdiotoken << "\" to set standart input or output" << endl
		<< "-------------------------------------------------" << endl
		<< "use " << print << " for printing value, " << "for exit type " << exitkey << endl
		<< "-------------------------------------------------" << endl
		<< "type h for help" << endl;
}

void calculate() {
	ostream & default_out = cout;
	istream & default_in = cin;
	double val{ 0 };
	unique_ptr<ifstream> in_s;	
	ostream * out_s = &default_out;
	Tokenstream ts;	

	while (ts.get_stream()) {
		try {
			if (&ts.get_stream() == &default_in) {
				default_out << prompt;
			}
			Token t = ts.get();

			//print cancelation
			while (t.type == print) t = ts.get();

			//exit
			if (t.type == quit_prog) { 
				system("pause");
				return;
			}
			//set in stream
			if (t.type == fromcall) {
				t = ts.get();
				if (t.name == stdiotoken) {
					ts.set_default_stream();
					continue;
				}

				in_s = make_unique<ifstream>(ifstream(t.name.c_str()));
				if (!in_s->is_open()) {
					throw Error("no such file");
				}
				ts.set_stream(in_s.get());
				continue;
			}
			//set out stream
			if (t.type == tocall) {
				t = ts.get();
				if (t.name == stdiotoken) {
					out_s = &default_out;
					continue;
				}
				if (out_s != &default_out) { delete out_s; }
				
				out_s = new ofstream(t.name.c_str());
				continue;
			}
			//help
			if (t.type == helpcall) {
				help();
				continue;
			}
			ts.put_back(t);
			*out_s << result;
			primitives::statement(*out_s, ts) << endl;
		} catch (Error & e) {
			*out_s << e.what() << endl;
			primitives::clean_up(ts);
		}
	}
}

int main() {
	
	try {	
		primitives::symb_tbale.declare_constant("pi", 3.1414);
		primitives::symb_tbale.declare_constant("e", 2.7182818);
		primitives::symb_tbale.declare_constant("k", 1000);
		
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
