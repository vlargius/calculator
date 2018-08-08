#include <sstream>
#include <map>
#include <set>

#include "roman_int.h"
#include "exceptions.h"

using namespace std;

template<class T>
bool me_zero(const T& a) {
	return a >= 0;
}

bool is_roman(char c) {
	set<char> chars { 'I', 'V', 'X', 'L', 'C', 'D', 'M' };
	return chars.find(c) != chars.end();
}

int r2i(char c) {
	map<char, int> conv{
		{ 'I' , 1},
		{ 'V' , 5 },
		{ 'X' , 10 },
		{ 'L' , 50 },
		{ 'C' , 100 },
		{ 'D' , 500 },
		{ 'M' , 1000 }
	};

	auto it = conv.find(c);
	if (it != conv.end()) {
		return it->second;
	}
	else {
		error("cant cast roman to int");
	}
}

char i2r(int i) {
	map<int, char> conv{
		{ 1, 'I' },
		{ 5, 'V'  },
		{ 10, 'X' },
		{ 50, 'L' },
		{ 100, 'C' },
		{ 500, 'D' },
		{ 1000, 'M' }
	};

	auto it = conv.find(i);
	if (it != conv.end()) {
		return it->second;
	}
	else {
		error("cant cast roman to int");
	}
}

string cast_int2rom(int val) {
	val = abs(val);

	stringstream res;
	int div = 1;
	while (val >= div) {
		div *= 10;
	}

	div /= 10;

	while (val) {
		int last = val / div;
		if (last <= 3) {
			res << string(last, i2r(div));
		}
		if (last == 4) {
			res << i2r(div) << i2r(div * 5);
		}
		if (last >= 5 && last <= 8 ) {
			res << i2r(div * 5) << string(last - 5, i2r(div));
			
		}
		if (last == 9) {
			res << i2r(div) << i2r(div * 10);
		}
		val %= div;
		div /= 10;
	}

	return res.str();
}

RomanInt::RomanInt() :
	is_positive(true) {
}

RomanInt::RomanInt(int val) :
	i_val(val),
	s_val(cast_int2rom(val)),
	is_positive(me_zero(val)){

}

RomanInt::RomanInt(const string& str) :
		s_val(str) {
		int prev = r2i(str.back());
		i_val = prev;
		for (auto i = str.rbegin(); i != str.rend(); ++i) {
			if (i == str.rbegin()) continue;
			int curr = r2i(*i);
			while (curr < prev) {
				i_val -= curr;
				++i;
				prev = curr;
				if (i != str.rend()) {
					curr = r2i(*i);
				}
				else {
					return;
				}
			}

			i_val += curr;
			prev = curr;
		}
	}

RomanInt RomanInt::operator+(const RomanInt & a)
{	
	return RomanInt((int)*this + (int)a);
}

RomanInt RomanInt::operator-(const RomanInt & a)
{
	return RomanInt((int)*this - (int)a);
}

RomanInt RomanInt::operator/(const RomanInt & a)
{
	return RomanInt((int)*this / (int)a);
}

RomanInt RomanInt::operator*(const RomanInt & a)
{
	return RomanInt((int)*this *(int)a);
}

RomanInt& RomanInt::operator+=(const RomanInt & a)
{
	*this = *this + a;
	return *this;
}

RomanInt& RomanInt::operator-=(const RomanInt & a)
{
	*this = *this - a;
	return *this;
}

RomanInt& RomanInt::operator/=(const RomanInt & a)
{
	*this = *this / a;
	return *this;
}

RomanInt& RomanInt::operator*=(const RomanInt & a)
{
	*this = *this * a;
	return *this;
}

ostream& operator<<(ostream& os, const RomanInt& ri) {
	return os << (ri.is_pos() ? "" : "-") << (string)ri;
}

istream& operator>>(istream& is, RomanInt& ri) {
	string s;
	char c;
	while (is >> c) {
		if (is_roman(c)) {
			s += c;
		}
		else {
			is.putback(c);
			break;
		}
	}
	ri = RomanInt(s);
	return is;
}