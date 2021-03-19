#pragma once
#include <string>
#include <iostream>
#include <climits>
#include <map>
#include <string>

enum OPERATION_CLASS {
	STACK = 1,
	IMMEDIATE = 2,
	REGISTER = 4,
	JUMP = 8,
	CONDITIONAL = 16,
	SPECIAL = 32
};

enum OPERATION {
	PUSH = 0,
	POP,
	PRINT,
	HLT,
	ADD,
	SUB,
	MUL,
	DIV,
	MOVE
};

static const std::map<std::string, OPERATION> m = {
	{"PUSH", PUSH},
	{"POP", POP},
	{"PRINT", PRINT},
	{"HLT", HLT},
	{"ADD", ADD},
	{"SUB", SUB},
	{"MUL", MUL},
	{"DIV", DIV},
	{"MOVE", MOVE},
};

static int get_operation_class(OPERATION o) {
	switch (o)
	{
	case PUSH:
		return STACK | IMMEDIATE;
	case POP:
	case PRINT:
		return REGISTER;
	case HLT:
		return SPECIAL;
	case ADD:
	case SUB:
	case MUL:
	case DIV:
	case MOVE:
		return REGISTER | IMMEDIATE;
	default:
		throw std::invalid_argument("Operation not supported");
	}
}

class Instruction
{
public:
	OPERATION type;
	int op_class;
	int immediate_value;
	unsigned first_reg;
	unsigned second_reg;
	int jump_to_relative_line;

	Instruction(int cls, OPERATION type); // SPECIAL
	Instruction(int cls, OPERATION type, int immediate); // STACK & IMMEDIATE
	Instruction(int cls, OPERATION type, unsigned x); // REGISTER & STACK
	Instruction(int cls, OPERATION type, int immediate, unsigned x); // REGISTER & IMMEDIATE
	Instruction(int cls, OPERATION type, unsigned x, unsigned y); // REGISTER
	Instruction() = delete;
	~Instruction() {}

	friend std::ostream& operator<<(std::ostream& out, Instruction& o) {
		out << "[OP: " << o.type << ", I: " << o.immediate_value << "]" << std::endl;
		return out;
	}
};