#pragma once

#include "exceptions.h"

int factorial(int d) {
	if (d < 0) {
		throw BadArgument();
	}

	if (d == 0) {
		return 1;
	}
	else {
		return d * factorial(d - 1);
	}
}