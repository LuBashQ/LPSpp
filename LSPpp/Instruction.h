#pragma once
#include <string>
#include <iostream>
#include <climits>
#include <map>
#include <string>

enum Operation_Class {
	STACK = 1,
	IMMEDIATE = 2,
	REGISTER = 4,
	JUMP = 8,
	CONDITIONAL = 16,
	SPECIAL = 32
};

enum class Operation {
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

static const std::map<std::string, Operation> m = {
	{"PUSH", Operation::PUSH},
	{"POP", Operation::POP},
	{"PRINT", Operation::PRINT},
	{"HLT", Operation::HLT},
	{"ADD", Operation::ADD},
	{"SUB", Operation::SUB},
	{"MUL", Operation::MUL},
	{"DIV", Operation::DIV},
	{"MOVE", Operation::MOVE},
};

static const int get_operation_class(Operation o) {
	switch (o)
	{
	case Operation::PUSH:
		return STACK | IMMEDIATE;
	case Operation::POP:
		return STACK | REGISTER;
	case Operation::PRINT:
		return REGISTER;
	case Operation::HLT:
		return SPECIAL;
	case Operation::ADD:
	case Operation::SUB:
	case Operation::MUL:
	case Operation::DIV:
	case Operation::MOVE:
		return REGISTER | IMMEDIATE;
	default:
		throw std::invalid_argument("Operation not supported");
	}
}


class Instruction
{
public:
	Operation type;
	int op_class;
	int immediate_value;
	unsigned first_reg;
	unsigned second_reg;
	int jump_to_relative_line;

	Instruction(int cls, Operation type); // SPECIAL
	Instruction(int cls, Operation type, int immediate); // STACK & IMMEDIATE
	Instruction(int cls, Operation type, unsigned x); // REGISTER & STACK
	Instruction(int cls, Operation type, int immediate, unsigned x); // REGISTER & IMMEDIATE
	Instruction(int cls, Operation type, unsigned x, unsigned y); // REGISTER
	Instruction() = delete;
	~Instruction() {}
};