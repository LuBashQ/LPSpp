#include "Instruction.h"

Instruction::Instruction(int cls, Operation t)
{
	op_class = cls;
	type = t;
	immediate_value = 0;
	first_reg = 0;
	second_reg = 0;
	jump_to_relative_line = 0;
}

Instruction::Instruction(int cls, Operation type, int immediate)
	: Instruction(cls, type)
{
	immediate_value = immediate;
}

Instruction::Instruction(int cls, Operation type, unsigned x)
	: Instruction(cls, type)
{
	first_reg = x;
}

Instruction::Instruction(int cls, Operation type, int immediate, unsigned x)
	: Instruction(cls, type, immediate)
{
	first_reg = x;
}

Instruction::Instruction(int cls, Operation type, unsigned x, unsigned y)
	: Instruction(cls, type, x)
{
	second_reg = y;
}
