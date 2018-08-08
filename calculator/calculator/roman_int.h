#pragma once

#include <iostream>
#include <string>

using namespace std;

class RomanInt {
public:

	RomanInt();
	RomanInt(const RomanInt&) = default;
	RomanInt & operator=(const RomanInt&) = default;

	RomanInt(int val);
	RomanInt(const string& str);

	operator int() const { return i_val; }
	operator string() const { return s_val;	}

	bool is_pos() const { return is_positive; }

	RomanInt operator+(const RomanInt& a);
	RomanInt operator-(const RomanInt& a);
	RomanInt operator/(const RomanInt& a);
	RomanInt operator*(const RomanInt& a);

	RomanInt& operator+=(const RomanInt& a);
	RomanInt& operator-=(const RomanInt& a);
	RomanInt& operator/=(const RomanInt& a);
	RomanInt& operator*=(const RomanInt& a);

private:
	string s_val;
	int i_val;
	bool is_positive;
};

ostream& operator<<(ostream& os, const RomanInt& ri);

istream& operator>>(istream& is, RomanInt& ri);