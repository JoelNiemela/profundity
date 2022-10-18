#ifndef COMPTIME_H
#define COMPTIME_H

#include "values.h"
#include "ast.h"

class ComptimeState {
private:
public:
	ComptimeState();

	Value* run_exp(Exp* exp);
private:
};

#endif
