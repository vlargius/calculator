#pragma once

#include "exceptions.h"

template<class ToType, class FromType>
int check(FromType val) {
	ToType v = (ToType)val;
	if (v != val)
		throw BadArgument("wrong data type, it must me " + string(typeid(ToType).name()));
	return v;
}