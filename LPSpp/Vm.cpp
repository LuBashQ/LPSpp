#include "Vm.h"

void Vm::execute_comparison_operation(const Instruction &op, const std::function<bool(int, int)> &&cmp) {
    if (std::find(op.classes.begin(), op.classes.end(), Operation_Class::IMMEDIATE) != op.classes.end())
        if (cmp(op.immediate_value, reg[op.first_reg]))
            reg[8] = 1;
        else reg[8] = 0;
    else if (cmp(reg[op.first_reg], reg[op.second_reg]))
        reg[8] = 1;
    else reg[8] = 0;
}

void Vm::execute_arithmetic_operation(const Instruction &op, const std::function<int(int, int)> &&f) {
    if (std::find(op.classes.begin(), op.classes.end(), Operation_Class::REGISTER) != op.classes.end())
        reg[op.second_reg] = f(reg[op.second_reg], reg[op.first_reg]);
    else
        reg[op.first_reg] = f(reg[op.first_reg], op.immediate_value);
}

void Vm::execute_jump_operation(const Instruction &op, const std::function<bool(int, int)> &&cond) {
    if (std::find(op.classes.begin(), op.classes.end(), Operation_Class::IMMEDIATE) != op.classes.end())
        if (cond(reg[8], 1)) std::advance(current_instruction, op.immediate_value - 1);
        else if (cond(reg[8], 1)) std::advance(current_instruction, reg[op.first_reg] - 1);
}

void Vm::execute_instruction(const Instruction &op) {
    switch (op.type) {
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
            execute_arithmetic_operation(op, std::plus<>());
            break;
        case Operation::SUB:
            execute_arithmetic_operation(op, std::minus<>());
            break;
        case Operation::MUL:
            execute_arithmetic_operation(op, std::multiplies<>());
            break;
        case Operation::DIV:
            execute_arithmetic_operation(op, std::divides<>());
            break;
        case Operation::JMP:
            execute_jump_operation(op);
            break;
        case Operation::JMPC:
            execute_jump_operation(op, std::equal_to<>());
            break;
        case Operation::GEQ:
            execute_comparison_operation(op, std::greater_equal<>());
            break;
        case Operation::LEQ:
            execute_comparison_operation(op, std::less_equal<>());
            break;
        case Operation::GT:
            execute_comparison_operation(op, std::greater<>());
            break;
        case Operation::LT:
            execute_comparison_operation(op, std::less<>());
            break;
        default:
            throw std::invalid_argument("Operation type not supported");
    }
}

Vm::Vm() {
    reg.resize(9);
    sp = -1;
    halted = false;
    stack.resize(4096);
}

void Vm::execute_program(const std::string &&file) {
    parse(file);
    for (current_instruction = operations.begin(); current_instruction != operations.end(); current_instruction++)
        if (halted)
            break;
        else
            execute_instruction(*current_instruction);
}

void Vm::parse(const std::string &file) {
    auto tokens = get_tokens(file);
    for (auto &line : tokens) {
        if (line.empty() || line[0][0] == ':') continue;
        const Operation op = op_table.find(line[0])->second;

        switch (line.size()) {
            case 1: {
                operations.emplace_back(std::vector<Operation_Class>({Operation_Class::SPECIAL}),
                                        op);
                break;
            }
            case 2: {
                const std::string s = line[1].substr(1);

                if (line[1][0] == '@')
                    operations.emplace_back(
                            std::vector<Operation_Class>({Operation_Class::STACK, Operation_Class::REGISTER}),
                            op,
                            static_cast<unsigned>(std::stoi(s)));
                else if (line[1][0] == ':')
                    if (line[1][1] == '@')
                        operations.emplace_back(
                                std::vector<Operation_Class>({Operation_Class::REGISTER, Operation_Class::JUMP}),
                                op,
                                static_cast<unsigned>(std::stoi(line[1].substr(2))));
                    else
                        operations.emplace_back(std::vector<Operation_Class>(
                                {Operation_Class::STACK, Operation_Class::JUMP, Operation_Class::IMMEDIATE}),
                                                op,
                                                std::stoi(s));
                else
                    operations.emplace_back(
                            std::vector<Operation_Class>({Operation_Class::STACK, Operation_Class::IMMEDIATE}),
                            op,
                            std::stoi(line[1]));
                break;
            }
            case 3: {
                const std::string s = line[1].substr(1);
                const std::string s1 = line[2].substr(1);

                if (line[1][0] == '@')
                    operations.emplace_back(std::vector<Operation_Class>({Operation_Class::REGISTER,
                                                                          Operation_Class::CONDITIONAL}),
                                            op,
                                            static_cast<unsigned>(std::stoi(s)),
                                            static_cast<unsigned>(std::stoi(s1)));
                else
                    operations.emplace_back(std::vector<Operation_Class>({Operation_Class::IMMEDIATE,
                                                                          Operation_Class::CONDITIONAL}),
                                            op,
                                            std::stoi(line[1]),
                                            static_cast<unsigned>(std::stoi(s1)));
                break;
            }
            default:
                throw std::invalid_argument("Instruction has too many arguments");
        }
    }
}