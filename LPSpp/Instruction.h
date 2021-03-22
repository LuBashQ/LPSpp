#pragma once
#include <iostream>
#include <map>
#include <string>
#include <vector>

enum class Operation_Class {
	STACK,	
	IMMEDIATE,
	REGISTER,
	JUMP,
	CONDITIONAL,
	SPECIAL
};

enum class Operation {
	PUSH,
	POP,
	PRINT,
	HLT,
	ADD,
	SUB,
	MUL,
	DIV,
	MOVE,
	JMP,
	JMPC,
	GEQ,
	LEQ,
	GT,
	LT,
	CALL
};

static const std::map<std::string, Operation> op_table = {
	{"PUSH",	Operation::PUSH},
	{"POP",		Operation::POP},
	{"PRINT",	Operation::PRINT},
	{"HLT",		Operation::HLT},
	{"ADD",		Operation::ADD},
	{"SUB",		Operation::SUB},
	{"MUL",		Operation::MUL},
	{"DIV",		Operation::DIV},
	{"MOVE",	Operation::MOVE},
	{"JMP",		Operation::JMP},
	{"JMPC",	Operation::JMPC},
	{"GEQ",		Operation::GEQ},
	{"LEQ",		Operation::LEQ},
	{"GT",		Operation::GT},
	{"LT",		Operation::LT},
	{"CALL",	Operation::CALL},
};

class Instruction
{
public:
	Operation type;
	std::vector<Operation_Class> classes;
	int immediate_value;
	unsigned first_reg;
	unsigned second_reg;

	Instruction(std::vector<Operation_Class>&& cls, Operation type); // SPECIAL
	Instruction(std::vector<Operation_Class>&& cls, Operation type, int immediate); // STACK & IMMEDIATE & JUMP
	Instruction(std::vector<Operation_Class>&& cls, Operation type, unsigned x); // REGISTER & STACK & JUMP
	Instruction(std::vector<Operation_Class>&& cls, Operation type, int immediate, unsigned x); // REGISTER & IMMEDIATE
	Instruction(std::vector<Operation_Class>&& cls, Operation type, unsigned x, unsigned y); // REGISTER
	Instruction() = delete;
	~Instruction() = default;
};