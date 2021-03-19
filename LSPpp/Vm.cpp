#include "Vm.h"

void Vm::execute_instruction(Instruction& op)
{
	switch (op.type)
	{
	case Operation::PUSH:
		if (op.op_class & IMMEDIATE)
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
		if (op.op_class & REGISTER)
			reg[op.second_reg] = reg[op.first_reg];
		else
			reg[op.first_reg] = op.immediate_value;
		break;
	case Operation::ADD:
		if (op.op_class & REGISTER)
			reg[op.second_reg] += reg[op.first_reg];
		else
			reg[op.first_reg] += op.immediate_value;
		break;
	case Operation::SUB:
		if (op.op_class & REGISTER)
			reg[op.second_reg] -= reg[op.first_reg];
		else
			reg[op.first_reg] -= op.immediate_value;
		break;
	case Operation::MUL:
		if (op.op_class & REGISTER)
			reg[op.second_reg] *= reg[op.first_reg];
		else
			reg[op.first_reg] *= op.immediate_value;
		break;
	case Operation::DIV:
		if (op.op_class & REGISTER)
			reg[op.second_reg] /= reg[op.first_reg];
		else
			reg[op.first_reg] /= op.immediate_value;
		break;
	default:
		throw std::invalid_argument("Operation type not supported");
	}
}

Vm::Vm()
{
	reg.resize(8);
	sp = -1;
	halted = false;
	stack.resize(4096);
}

void Vm::execute_program(const std::string&& file)
{
	parse(file);
	for (auto& op : operations)
		if (!halted)
			execute_instruction(op);
		else
			break;
}

void Vm::parse(const std::string& file)
{
	auto tokens = get_tokens(file);
	for (auto& line : tokens)
	{
		switch (line.size())
		{
		case 0: // empty line
			break;
		case 1:
			operations.emplace_back(SPECIAL, m.find(line[0])->second);
			break;
		case 2:
			if (line[1][0] == '@') // register
				operations.emplace_back(STACK | REGISTER, m.find(line[0])->second, (unsigned)std::atoi(line[1].substr(1).c_str()));
			else //immediate
				operations.emplace_back(STACK | IMMEDIATE, m.find(line[0])->second, std::atoi(line[1].c_str()));
			break;
		case 3:
			if (line[1][0] == '@') //register
				operations.emplace_back(REGISTER, m.find(line[0])->second,
					(unsigned)std::atoi(line[1].substr(1).c_str()),
					(unsigned)std::atoi(line[2].substr(1).c_str()));
			else //immediate
				operations.emplace_back(IMMEDIATE, m.find(line[0])->second,
					std::atoi(line[1].c_str()),
					(unsigned)std::atoi(line[2].substr(1).c_str()));
			break;
		default:
			throw std::invalid_argument("Instruction has too many arguments");
			break;
		}
	}
}