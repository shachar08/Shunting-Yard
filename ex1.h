#ifndef EX1_H
#define EX1_H
#include "Expression.h"
#include <iostream>
#include <stdlib.h>
#include <queue>
#include <stack>
#include <map>
using namespace std;

class Value : public Expression {
private:
    double num;
public:
    Value(const double num);
    double calculate() override;
};

class Variable : public Expression {
private:
    string name;
    double value;
public:
    //double getValue(){return value;}
    Variable(const string &name, const double &value);
    double calculate() override;
    Variable& operator++();
    Variable& operator--();
    Variable& operator+=(double val);
    Variable& operator-=(double val);
    Variable& operator++(int num);
    Variable& operator--(int num);
};

class UnaryOperator : public Expression {
protected:
    Expression* x;
public:
    UnaryOperator(Expression* exp);
    virtual ~UnaryOperator();
};

class UPlus : public UnaryOperator {
public:
    UPlus(Expression* x);
    double calculate() override;
};

class UMinus : public UnaryOperator {
public:
    UMinus(Expression* x);
    double calculate() override;
};

class BinaryOperator : public Expression {
protected:
    Expression* left;
    Expression* right;
public:
    BinaryOperator(Expression* left, Expression* right);
    virtual ~BinaryOperator();
};

class Plus : public BinaryOperator {
public:
    Plus(Expression* left, Expression* right);
    double calculate() override;
};

class Minus : public BinaryOperator {
public:
    Minus(Expression* left, Expression* right);
    double calculate() override;
};

class Mul : public BinaryOperator {
public:
    Mul(Expression* left, Expression* right);
    double calculate() override;
};

class Div : public BinaryOperator {
public:
    Div(Expression* left, Expression* right);
    double calculate() override;
};

class Interpreter {
private:
    stack<string> stack1;
    queue<string> queue1;
    map<string, double> varMap;
public:
    Expression* interpret(const string&  infix);
    void setVariables(const string&  str);
};
#endif //EX1_H



