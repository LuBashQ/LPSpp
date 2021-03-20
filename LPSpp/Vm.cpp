#include "Vm.h"

void Vm::excecute_comparison_operation(const Instruction& op, std::function<bool(int, int)> cmp) {
	if (std::find(op.classes.begin(), op.classes.end(), Operation_Class::IMMEDIATE) != op.classes.end())
		if (cmp(op.immediate_value, reg[op.first_reg]))
			reg[8] = 1;
		else reg[8] = 0;
	else
		if (cmp(reg[op.first_reg], reg[op.second_reg]))
			reg[8] = 1;
		else reg[8] = 0;
}

void Vm::execute_arithmetic_operation(const Instruction& op, std::function<int(int, int)> f) {
	if (std::find(op.classes.begin(), op.classes.end(), Operation_Class::REGISTER) != op.classes.end())
		reg[op.second_reg] = f(reg[op.second_reg], reg[op.first_reg]);
	else
		reg[op.first_reg] = f(reg[op.first_reg], op.immediate_value);
}

void Vm::execute_jump_operation(const Instruction& op, std::function<bool(int, int)> cond)
{
	if (std::find(op.classes.begin(), op.classes.end(), Operation_Class::IMMEDIATE) != op.classes.end())
		if (cond(reg[8], 1)) std::advance(current_instruction, op.immediate_value - 1);
	else
		if (cond(reg[8], 1)) std::advance(current_instruction, reg[op.first_reg] - 1);
}

void Vm::execute_instruction(const Instruction& op)
{
	switch (op.type)
	{
	case Operation::PUSH:
		if (std::find(op.classes.begin(), op.classes.end(), Operation_Class::IMMEDIATE) != op.classes.end())
			stack[++sp] = op.immediate_value;
		else
			stack[++sp] = reg[op.first_reg];
		break;
	case Operation::POP:
		reg[op.first_reg] = stack[sp--];
		break;
	case Operation::PRINT:
		std::cout << reg[op.first_reg] << std::endl;
		break;
	case Operation::HLT:
		halted = true;
		break;
	case Operation::MOVE:
		if (std::find(op.classes.begin(), op.classes.end(), Operation_Class::REGISTER) != op.classes.end())
			reg[op.second_reg] = reg[op.first_reg];
		else
			reg[op.first_reg] = op.immediate_value;
		break;
	case Operation::ADD:
		execute_arithmetic_operation(op, [](int a, int b) {return a + b; });
		break;
	case Operation::SUB:
		execute_arithmetic_operation(op, [](int a, int b) {return a - b; });
		break;
	case Operation::MUL:
		execute_arithmetic_operation(op, [](int a, int b) {return a * b; });
		break;
	case Operation::DIV:
		execute_arithmetic_operation(op, [](int a, int b) {return a / b; });
		break;
	case Operation::JMP:
		execute_jump_operation(op);
		break;
	case Operation::JMPC:
		execute_jump_operation(op, [](int a, int b) {return a == b; });
		break;
	case Operation::GEQ:
		excecute_comparison_operation(op, [](int a, int b) {return a >= b; });
		break;
	case Operation::LEQ:
		excecute_comparison_operation(op, [](int a, int b) {return a <= b; });
		break;
	case Operation::GT:
		excecute_comparison_operation(op, [](int a, int b) {return a > b; });
		break;
	case Operation::LT:
		excecute_comparison_operation(op, [](int a, int b) {return a < b; });
		break;
	default:
		throw std::invalid_argument("Operation type not supported");
	}
}

Vm::Vm()
{
	reg.resize(9);
	sp = -1;
	halted = false;
	stack.resize(4096);
}

void Vm::execute_program(const std::string&& file)
{
	parse(file);
	for (current_instruction = operations.begin(); current_instruction != operations.end(); current_instruction++)
		if (halted) 
			break;
		else 
			execute_instruction(*current_instruction);
}

void Vm::parse(const std::string& file)
{
	auto tokens = get_tokens(file);
	for (auto& line : tokens)
	{
		if (line.size() == 0) continue;
		const Operation op = op_table.find(line[0])->second;

		switch (line.size())
		{
		case 1:
			operations.emplace_back(std::vector<Operation_Class>({ Operation_Class::SPECIAL }), op);
			break;
		case 2:
			if (line[1][0] == '@') // register
				operations.emplace_back(std::vector<Operation_Class>({ Operation_Class::STACK, Operation_Class::REGISTER }),
					op,
					static_cast<unsigned>(std::atoi(line[1].substr(1).c_str())));
			else if (line[1][0] == ':') // jump
				if (line[1][1] == '@') // jump register
					operations.emplace_back(std::vector<Operation_Class>({ Operation_Class::REGISTER, Operation_Class::JUMP }),
						op,
						static_cast<unsigned>(std::atoi(line[1].substr(2).c_str())));
				else // jump immediate
					operations.emplace_back(std::vector<Operation_Class>({ Operation_Class::STACK, Operation_Class::JUMP, Operation_Class::IMMEDIATE }),
						op,
						std::atoi(line[1].substr(1).c_str()));
			else // immediate
				operations.emplace_back(std::vector<Operation_Class>({ Operation_Class::STACK, Operation_Class::IMMEDIATE }),
					op,
					std::atoi(line[1].c_str()));
			break;
		case 3:
			if (line[1][0] == '@') //register
				operations.emplace_back(std::vector<Operation_Class>({ Operation_Class::REGISTER }),
					op,
					static_cast<unsigned>(std::atoi(line[1].substr(1).c_str())),
					static_cast<unsigned>(std::atoi(line[2].substr(1).c_str())));
			else //immediate
				operations.emplace_back(std::vector<Operation_Class>({ Operation_Class::IMMEDIATE }),
					op,
					std::atoi(line[1].c_str()),
					static_cast<unsigned>(std::atoi(line[2].substr(1).c_str())));
			break;
		default:
			throw std::invalid_argument("Instruction has too many arguments");
			break;
		}
	}
}