#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>
#include <iostream>

std::string le_compte_est_bon(unsigned target, const std::vector<unsigned> &operands);

enum Operator
{
    ADDITION,
    SUBSTRACTION,
    MULTIPLICATION,
    DIVISION,
    NUMBER_OF_OPERATORS,
    NO_OPERATOR
}; // enum Operator

class Operand
{
public:
    unsigned value;
    unsigned leftGenerativeOperand;
    unsigned rightGenerativeOperand;
    Operator generativeOperator;

    Operand()
    {
        value = 0;
        leftGenerativeOperand = 0;
        rightGenerativeOperand = 0;
        generativeOperator = Operator::NO_OPERATOR;
    }

    Operand(unsigned v)
    {
        value = v;
        leftGenerativeOperand = 0;
        rightGenerativeOperand = 0;
        generativeOperator = Operator::NO_OPERATOR;
    }

    Operand(Operand leftOperand, Operand rightOperand)
    {
        value = 0;
        leftGenerativeOperand = leftOperand.value;
        rightGenerativeOperand = rightOperand.value;
        generativeOperator = Operator::NO_OPERATOR;
    }
};

class Operation
{
public:
    Operand *leftOperand;
    Operand *rightOperand;
    unsigned result;
    unsigned leftOperandValue;
    unsigned rightOperandValue;
    Operator operator_;

    Operation()
    {
        leftOperand = nullptr;
        rightOperand = nullptr;
        result = 0;
        leftOperandValue = 0;
        rightOperandValue = 0;
        operator_ = Operator::NO_OPERATOR;
    }

    Operation(Operand *lOperandPtr, Operand *rOperandPtr)
    {
        leftOperand = lOperandPtr;
        rightOperand = rOperandPtr;
        result = 0;
        leftOperandValue = lOperandPtr->value;
        rightOperandValue = rOperandPtr->value;
        operator_ = Operator::ADDITION;
    }
};

Operator nextOperator(Operator operator_);

bool find_target_in_operands(const unsigned target, const std::vector<unsigned> &operands);

std::vector<Operand> from_unsigned_vector_to_operand_vector(const std::vector<unsigned> &operands);

bool find_solution(std::vector<Operation> &realisedOperations, const unsigned target, const std::vector<Operand> &currentOperands);

bool test_couple_of_operands(std::vector<Operation> &realisedOperations, const unsigned target, std::vector<Operand> &newOperands, Operation &currentOperation, const bool swapHappened, const int rightOperandIndex);

unsigned realise_operation(const Operation &operation, const bool swapHappened);

bool condition_for_addition(const Operation &operation, const bool swapHappened);

bool condition_for_substraction(const Operation &operation, const bool swapHappened);

bool condition_for_multiplication(const Operation &operation, const bool swapHappened);

bool condition_for_division(const Operation &operation, const bool swapHappened);

bool potential_swap(Operand *leftOperandPtr, Operand *rightOperandPtr);

void potential_swap_back(Operand *leftOperandPtr, Operand *rightOperandPtr, bool *swapHappenedPtr);

std::string generate_solution(const std::vector<Operation> &realisedOperations);

std::string generate_expression(const std::vector<Operation> &realisedOperations, const int currentOperationIndex, std::vector<bool> &alreadyUsed);

void potentially_change_expression(const std::vector<Operation> &realisedOperations, bool *isModified, const bool isRightOperand, const int generativeOperationIndex, const Operation &currentOperation, std::string *expression, std::vector<bool> &alreadyUsed);

bool put_parenthises(const Operator generativeOperator, const Operator currentOperator, const bool isRightOperand);

std::vector<Operand> generate_new_operands_vector(const std::vector<Operand> &currentOperands, const int leftOperandIndex);

std::string from_operator_to_string(const Operator op_);

void log_operation(const Operation operation);

void log_operationsVector(const std::vector<Operation> &operation);

#endif // FUNCTIONS_H
