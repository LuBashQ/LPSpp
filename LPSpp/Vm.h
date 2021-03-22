#pragma once
#include "Instruction.h"
#include "Util.h"
#include <stdexcept>
#include <functional>

class Vm {
public:
	Vm();
	~Vm() {};
	void execute_program(const std::string&& file);
private:
	std::vector<int> stack;
	std::vector<int> reg;
	std::vector<Instruction> operations;
	std::vector<Instruction>::iterator current_instruction;
	unsigned sp;
	bool halted;

	void parse(const std::string& file);
	void execute_instruction(const Instruction& op);
	void execute_comparison_operation(const Instruction& op, std::function<bool(int, int)>&& cmp);
	void execute_arithmetic_operation(const Instruction& op, std::function<int(int, int)>&& f);
	void execute_jump_operation(const Instruction& op, const std::function<bool(int, int)>&& cond = [](int a, int b) {return true; });
};