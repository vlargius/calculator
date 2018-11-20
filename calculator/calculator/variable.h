#pragma once

#include <string> 
#include <vector>
#include <algorithm>

#include "exceptions.h"

using namespace std;

class SymbolTable {

	class Variable {
	public:
		Variable(string name, double value, bool is_constant):
		name(name),
		value(value),
		is_constant_(is_constant){}

		string get_name() const { return name; }
		double get_value() const { return value; }
		bool is_constant() const { return is_constant_; }

		void set_value(double value) { this->value = value; }

	private:
		string name;
		double value;	
		bool is_constant_;
	};


	vector<Variable> var_table;

	double declare(string name, double value, bool is_constant) {
		if (!is_declared(name)) {
			var_table.push_back(Variable(name, value, is_constant));
		}
		else {
			throw VariableError("declared twice: " + name);
		}
		return value;
	}

public:
	void set_value(string name, double value) {
		for (Variable& v : var_table) {
			if (v.get_name() == name) {
				if (!v.is_constant()) {
					v.set_value(value);
					return;
				}
				else {
					throw ConstAssigning(name);
				}
			}
		}
		throw MissingVariable(name);
	}

	double get_value(string name) {
		for (auto v : var_table) {
			if (v.get_name() == name) {
				return v.get_value();
			}
		}
		throw MissingVariable(name);
	}

	bool is_declared(string name) {
		for (auto v : var_table) {
			if (v.get_name() == name) {
				return true;
			}
		}
		return false;
	}

	double declare_variable(string name, double value) {		
		return declare(name, value, false);;
	}

	double declare_constant(string name, double value) {
		return declare(name, value, true);;
	}
};