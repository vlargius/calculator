#pragma once

#include <string> 
#include <vector>
#include <algorithm>

#include "exceptions.h"

using std::string;
using std::vector;

//this namespace is for variables 
namespace vars {

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

//*difficult implementation of variable list
	vector<Variable> variables;

	void set_value(string name, double value) {
		for (Variable& v : variables) {
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
		for (auto v : variables) {
			if (v.get_name() == name) {
				return v.get_value();
			}
		}
		throw MissingVariable(name);
	}

	bool is_declared(string name) {
		for (auto v : variables) {
			if (v.get_name() == name) {
				return true;
			}
		}
		return false;
	}

	double define_variable(string name, double value) {
		if (!is_declared(name)) {
			variables.push_back(Variable(name, value, false));
		}
		else {
			throw VariableError("declared twice" + name);
		}
		return value;
	}

	double define_constant(string name, double value) {
		if (!is_declared(name)) {
			variables.push_back(Variable(name, value, true));
		}
		else {
			throw VariableError("declared twice" + name);
		}
		return value;
	}

}