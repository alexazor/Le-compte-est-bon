#include <vector>
#include <iostream>
#include <algorithm>
#include "main.h"

/*
 * Command line arguments:
 * [target] [operands]
 */
int main(int argc, const char **argv)
{
    std::vector<unsigned> operands = {13, 19, 29, 43, 47, 53, 61, 79};
    unsigned target = 10921140;

    std::string solution = le_compte_est_bon(target, operands);

    std::cout << solution << std::endl;
    return EXIT_SUCCESS;
}

std::string le_compte_est_bon(unsigned target, const std::vector<unsigned> &operands)
{
    if (find_target_in_operands(target, operands))
        return std::to_string(target);

    if (operands.size() < 2)
        return std::string("");

    std::vector<Operation> realisedOperations = {};
    std::vector<Operand> currentOperands = from_unsigned_vector_to_operand_vector(operands);

    find_solution(realisedOperations, target, currentOperands);

    return generate_solution(realisedOperations);
}

Operator nextOperator(Operator operator_)
{
    switch (operator_)
    {
    case Operator::ADDITION:
        return Operator::SUBSTRACTION;
        break;
    case Operator::SUBSTRACTION:
        return Operator::MULTIPLICATION;
        break;
    case Operator::MULTIPLICATION:
        return Operator::DIVISION;
        break;
    case Operator::DIVISION:
        return Operator::NUMBER_OF_OPERATORS;
        break;
    default:
        std::cout << "Unexpected operator:\n"
                  << operator_ << std::endl;
        return Operator::NUMBER_OF_OPERATORS;
        break;
    }
}

bool find_target_in_operands(const unsigned target, const std::vector<unsigned> &operands)
{
    return (std::find(operands.begin(), operands.end(), target) != operands.end());
}

std::vector<Operand> from_unsigned_vector_to_operand_vector(const std::vector<unsigned> &operands)
{
    int numberOfOperands = static_cast<int>(operands.size());
    std::vector<Operand> operandsVec(numberOfOperands);

    for (int i = 0; i < numberOfOperands; i++)
        operandsVec[i] = Operand(operands[i]);

    return operandsVec;
}

bool find_solution(std::vector<Operation> &realisedOperations, const unsigned target, const std::vector<Operand> &currentOperands)
{
    Operand leftOperand, rightOperand, newOperand;
    Operation currentOperation;
    std::vector<Operand> newOperands;
    int numberOfOperands = static_cast<int>(currentOperands.size());
    bool swapHappened = false;

    for (int leftOperandIndex = 0; leftOperandIndex < numberOfOperands - 1; leftOperandIndex++)
    {
        leftOperand = currentOperands[leftOperandIndex];
        newOperands = generate_new_operands_vector(currentOperands, leftOperandIndex);

        for (int rightOperandIndex = leftOperandIndex + 1; rightOperandIndex < numberOfOperands; rightOperandIndex++)
        {
            rightOperand = currentOperands[rightOperandIndex];

            swapHappened = potential_swap(&leftOperand, &rightOperand);

            currentOperation = Operation(&leftOperand, &rightOperand);

            if (test_couple_of_operands(realisedOperations, target, newOperands, currentOperation, swapHappened, rightOperandIndex))
                return true;

            potential_swap_back(&leftOperand, &rightOperand, &swapHappened);
        }
    }
    return false;
}

bool test_couple_of_operands(std::vector<Operation> &realisedOperations, const unsigned target, std::vector<Operand> &newOperands, Operation &currentOperation, const bool swapHappened, const int rightOperandIndex)
{
    Operand newOperand(currentOperation.leftOperandValue, currentOperation.rightOperandValue);

    while (currentOperation.operator_ < Operator::NUMBER_OF_OPERATORS)
    {
        // std::cout << "    " << currentOperation.leftOperandValue << from_operator_to_string(currentOperation.operator_) << currentOperation.rightOperandValue << std::endl;

        newOperand.generativeOperator = currentOperation.operator_;
        newOperand.value = realise_operation(currentOperation, swapHappened);

        if (newOperand.value != 0)
        {
            currentOperation.result = newOperand.value;

            realisedOperations.push_back(currentOperation);

            if (newOperand.value == target)
                return true;

            newOperands[rightOperandIndex - 1] = newOperand;

            if (find_solution(realisedOperations, target, newOperands))
                return true;

            realisedOperations.pop_back();
        }

        currentOperation.operator_ = nextOperator(currentOperation.operator_);
    }

    if (swapHappened)
        newOperands[rightOperandIndex - 1] = *(currentOperation.leftOperand);
    else
        newOperands[rightOperandIndex - 1] = *(currentOperation.rightOperand);

    return false;
}

unsigned realise_operation(const Operation &operation, const bool swapHappened)
{
    switch (operation.operator_)
    {
    case Operator::ADDITION:
        if (condition_for_addition(operation, swapHappened))
            return operation.leftOperand->value + operation.rightOperand->value;
        break;

    case Operator::SUBSTRACTION:
        if (condition_for_substraction(operation, swapHappened))
            return operation.leftOperand->value - operation.rightOperand->value;
        break;

    case Operator::MULTIPLICATION:
        if (condition_for_multiplication(operation, swapHappened))
            return operation.leftOperand->value * operation.rightOperand->value;
        break;

    case Operator::DIVISION:
        if (condition_for_division(operation, swapHappened))
            return operation.leftOperand->value / operation.rightOperand->value;
        break;

    default:
        std::cout << "Unknown Operator\n"
                  << operation.leftOperand->value << " -- "
                  << operation.operator_ << " -- "
                  << operation.rightOperand->value << std::endl;
        break;
    }
    return 0;
}

bool condition_for_addition(const Operation &operation, const bool swapHappened)
{
    if (swapHappened)
        return operation.leftOperand->generativeOperator != ADDITION && operation.leftOperand->generativeOperator != SUBSTRACTION;
    else
        return operation.rightOperand->generativeOperator != ADDITION && operation.rightOperand->generativeOperator != SUBSTRACTION;
}

bool condition_for_substraction(const Operation &operation, const bool swapHappened)
{
    if (swapHappened)
    {
        if (operation.leftOperand->generativeOperator == SUBSTRACTION)
            return false;

        if (operation.leftOperand->generativeOperator != ADDITION)
            return true;

        if (operation.rightOperand->value > std::max(operation.leftOperand->leftGenerativeOperand, operation.leftOperand->rightGenerativeOperand))
            return true;
        else
            return false;
    }
    else
        return operation.rightOperand->generativeOperator != ADDITION && operation.rightOperand->generativeOperator != SUBSTRACTION;
}

bool condition_for_multiplication(const Operation &operation, const bool swapHappened)
{
    if (operation.leftOperand->value == 1 || operation.rightOperand->value == 1)
        return false;

    if (swapHappened)
        return operation.leftOperand->generativeOperator != MULTIPLICATION && operation.leftOperand->generativeOperator != DIVISION;
    else
        return operation.rightOperand->generativeOperator != MULTIPLICATION && operation.rightOperand->generativeOperator != DIVISION;
}

bool condition_for_division(const Operation &operation, const bool swapHappened)
{
    if (operation.rightOperand->value == 1 || operation.leftOperand->value % operation.rightOperand->value != 0)
        return false;

    if (swapHappened)
    {
        switch (operation.leftOperand->generativeOperator)
        {
        case Operator::DIVISION:
            return false;
            break;

        case Operator::MULTIPLICATION:
            if (operation.rightOperand->value % operation.leftOperand->leftGenerativeOperand == 0)
                return false;

            if (operation.rightOperand->value % operation.leftOperand->rightGenerativeOperand == 0)
                return false;
            break;

        default:
            break;
        }
        return true;
    }

    else
    {
        switch (operation.rightOperand->generativeOperator)
        {
        case Operator::DIVISION:
            return false;
            break;

        case Operator::MULTIPLICATION:
            if (operation.leftOperand->leftGenerativeOperand % operation.leftOperand->value == 0)
                return false;

            if (operation.leftOperand->rightGenerativeOperand % operation.leftOperand->value == 0)
                return false;
            break;

        default:
            break;
        }
        return true;
    }
}

bool potential_swap(Operand *leftOperandPtr, Operand *rightOperandPtr)
{
    if (leftOperandPtr->value >= rightOperandPtr->value)
        return false;

    unsigned leftOperandFormerValue = leftOperandPtr->value;
    leftOperandPtr->value = rightOperandPtr->value;
    rightOperandPtr->value = leftOperandFormerValue;

    return true;
}

void potential_swap_back(Operand *leftOperandPtr, Operand *rightOperandPtr, bool *swapHappened)
{
    if (*swapHappened)
        potential_swap(rightOperandPtr, leftOperandPtr);

    *swapHappened = false;
}

std::string generate_solution(const std::vector<Operation> &realisedOperations)
{
    int numberOfOperations = static_cast<int>(realisedOperations.size());
    int currentOperationIndex = numberOfOperations - 1;
    std::vector<bool> alreadyUsed(numberOfOperations, false);
    std::string solution;

    if (numberOfOperations == 0)
        return std::string("");

    solution = generate_expression(realisedOperations, currentOperationIndex, alreadyUsed);
    solution += std::string(" = ") + std::to_string(realisedOperations[numberOfOperations - 1].result);

    return solution;
}

std::string generate_expression(const std::vector<Operation> &realisedOperations, const int currentOperationIndex, std::vector<bool> &alreadyUsed)
{
    Operation generativeOperation;
    Operation currentOperation = realisedOperations[currentOperationIndex];
    std::string leftExpression = std::to_string(currentOperation.leftOperandValue);
    std::string rightExpression = std::to_string(currentOperation.rightOperandValue);
    std::string currentOperatorExpression = from_operator_to_string(currentOperation.operator_);
    std::string expression;
    bool isModifiedLeft = false;
    bool isModifiedRight = false;

    alreadyUsed[currentOperationIndex] = true;

    for (int generativeOperationIndex = currentOperationIndex - 1; generativeOperationIndex >= 0 && (!isModifiedLeft || !isModifiedRight); generativeOperationIndex--)
    {
        potentially_change_expression(realisedOperations, &isModifiedLeft, false, generativeOperationIndex, currentOperation, &leftExpression, alreadyUsed);
        potentially_change_expression(realisedOperations, &isModifiedRight, true, generativeOperationIndex, currentOperation, &rightExpression, alreadyUsed);
    }
    expression = leftExpression + currentOperatorExpression + rightExpression;
    return expression;
}

void potentially_change_expression(const std::vector<Operation> &realisedOperations, bool *isModified, const bool isRightOperand, const int generativeOperationIndex, const Operation &currentOperation, std::string *expression, std::vector<bool> &alreadyUsed)
{
    Operation generativeOperation = realisedOperations[generativeOperationIndex];
    unsigned operandValueToConsider = currentOperation.leftOperandValue;

    if (isRightOperand)
        operandValueToConsider = currentOperation.rightOperandValue;

    if (!(*isModified) && generativeOperation.result == operandValueToConsider && !alreadyUsed[generativeOperationIndex])
    {
        *isModified = true;
        *expression = generate_expression(realisedOperations, generativeOperationIndex, alreadyUsed);
        if (put_parenthises(generativeOperation.operator_, currentOperation.operator_, isRightOperand))
            *expression = std::string("(") + *expression + std::string(")");
    }
}

bool put_parenthises(const Operator generativeOperator, const Operator currentOperator, const bool isRightOperand)
{
    bool isAdditionOrSubstraction = generativeOperator == Operator::ADDITION || generativeOperator == Operator::SUBSTRACTION;
    switch (currentOperator)
    {
    case Operator::ADDITION:
        break;

    case Operator::SUBSTRACTION:
        return isRightOperand && isAdditionOrSubstraction;
        break;

    case Operator::MULTIPLICATION:
        return isAdditionOrSubstraction || (!isRightOperand && generativeOperator == Operator::DIVISION);
        break;

    case Operator::DIVISION:
        if (isRightOperand)
            return true;
        return isAdditionOrSubstraction;
        break;

    default:
        std::cout << "Oh no !" << currentOperator << std::endl;
        break;
    };

    return false;
}

std::vector<Operand> generate_new_operands_vector(const std::vector<Operand> &currentOperands, const int leftOperandIndex)
{
    std::vector<Operand> newOperands(currentOperands.size() - 1);

    for (int i = 0; i < leftOperandIndex; i++)
        newOperands[i] = currentOperands[i];

    for (int j = leftOperandIndex; j < currentOperands.size() - 1; j++)
        newOperands[j] = currentOperands[j + 1];

    return newOperands;
}

std::string from_operator_to_string(const Operator op_)
{
    std::string operatorString = std::string("??");

    switch (op_)
    {
    case Operator::ADDITION:
        operatorString = std::string(" + ");
        break;

    case Operator::SUBSTRACTION:
        operatorString = std::string(" - ");
        break;

    case Operator::MULTIPLICATION:
        operatorString = std::string(" * ");
        break;

    case Operator::DIVISION:
        operatorString = std::string(" / ");
        break;

    default:
        break;
    }

    return operatorString;
}

void log_operation(const Operation operation)
{
    std::cout << operation.leftOperandValue
              << from_operator_to_string(operation.operator_)
              << operation.rightOperandValue
              << " = "
              << operation.result
              << std::endl;
}

void log_operationsVector(const std::vector<Operation> &realisedOperations)
{
    for (int i = 0; i < realisedOperations.size(); i++)
        log_operation(realisedOperations[i]);

    if (realisedOperations.size() == 0)
        std::cout << "No solution\n########" << std::endl;
    else
        std::cout << "########" << std::endl;
}