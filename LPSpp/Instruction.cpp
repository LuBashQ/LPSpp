#include "Instruction.h"

Instruction::Instruction(std::vector<Operation_Class>&& cls, Operation t)
{
	classes = std::move(cls);
	type = t;
	immediate_value = 0;
	first_reg = 0;
	second_reg = 0;
}

Instruction::Instruction(std::vector<Operation_Class>&& cls, Operation type, int immediate)
	: Instruction(std::move(cls), type)
{
	immediate_value = immediate;
}

Instruction::Instruction(std::vector<Operation_Class>&& cls, Operation type, unsigned x)
	: Instruction(std::move(cls), type)
{
	first_reg = x;
}

Instruction::Instruction(std::vector<Operation_Class>&& cls, Operation type, int immediate, unsigned x)
	: Instruction(std::move(cls), type, immediate)
{
	first_reg = x;
}

Instruction::Instruction(std::vector<Operation_Class>&& cls, Operation type, unsigned x, unsigned y)
	: Instruction(std::move(cls), type, x)
{
	second_reg = y;
}
