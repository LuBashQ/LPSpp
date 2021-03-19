#pragma once
#include "Instruction.h"
#include "Util.h"
#include <stdexcept>

class Vm {
public:
	Vm();
	~Vm() {};
	void execute_program(const std::string&& file);
private:
	std::vector<int> stack;
	std::vector<int> reg;
	std::vector<Instruction> operations;
	unsigned sp;
	bool halted;

	void parse(const std::string& file);
	void execute_instruction(Instruction& op);
};