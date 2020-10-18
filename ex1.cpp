#include "ex1.h"
#include <queue>
#include <stack>
#include <cstring>
#include <string>
#include <cstdlib>
#include <sstream>
using namespace std;

Value::Value(const double num){
    this->num = num;
}
double Value::calculate(){
    return num;
}
Variable::Variable(const string &name, const double &value){
    this->name = name;
    this->value = value;
}
double Variable::calculate(){
    return this->value;
}
Variable& Variable::operator++(){
    ++value;
    return *this;
}
Variable& Variable::operator--(){
    --value;
    return *this;
}
Variable& Variable::operator+=(double val){
    value += val;
    return *this;
}
Variable& Variable::operator-=(double val){
    value -= val;
    return *this;
}
Variable& Variable::operator++(int num){
    ++value;
    return *this;
}
Variable& Variable::operator--(int num){
    --value;
    return *this;
}
UnaryOperator::UnaryOperator(Expression* exp){
    this->x = exp;
}
UnaryOperator::~UnaryOperator(){
    if(this->x != NULL) {
        delete this->x;
    }
}
UPlus::UPlus(Expression* exp) : UnaryOperator(exp) {
}
double UPlus::calculate() {
    return this->x->calculate();
}
UMinus::UMinus(Expression* exp) : UnaryOperator(exp) {
}
double UMinus::calculate() {
    return this->x->calculate() * -1;
}

BinaryOperator::BinaryOperator(Expression* left, Expression* right){
    this->left = left;
    this->right = right;
}
BinaryOperator::~BinaryOperator(){
    if(this->left != NULL) {
        delete this->left;
    }
    if(this->right != NULL) {
        delete this->right;
    }
}

Plus::Plus(Expression* left, Expression* right) : BinaryOperator(left, right){
}
double Plus::calculate() {
    return this->left->calculate() + this->right->calculate();
}

Minus::Minus(Expression* left, Expression* right) : BinaryOperator(left, right){
}
double Minus::calculate() {
    return this->left->calculate() - this->right->calculate();
}

Mul::Mul(Expression* left, Expression* right) : BinaryOperator(left, right){
}
double Mul::calculate() {
    return this->left->calculate() * this->right->calculate();
}

Div::Div(Expression* left, Expression* right) : BinaryOperator(left, right){
}
double Div::calculate() {
    if(right->calculate() == 0) {
        throw "diving in zero";
    }
    cout << "rr" << endl;
    return this->left->calculate() / this->right->calculate();
}


bool isOperator(char c) {
    if(c == '*' || c == '+' || c == '-' | c == '/')
        return true;
    return false;
}
bool isParenthesis(char c) {
    if(c == '(' || c == ')')
        return true;
    return false;
}
// cast from double to string
string doubleToString(double num) {
    ostringstream helper;
    helper << num;
    return helper.str();
}
// cast from string to double
 double stringToDouble(string &s) {
    int size = s.length();
    char* charArray = new char[size + 1];
    strcpy(charArray, s.c_str());
    double x = strtod(charArray, NULL);
    return x;
}

// check if the number (value) is valid
bool numberIsValid(const string& str) {
    int i = 0, dots = 0;
    if(str[i] == '-' && str.size() > 1 && isdigit(str[i+1])) { // check negative number
        i++;
    }
    while((str[i] != '\0') && (isdigit(str[i]) || str[i] == '.')) {
        i++;
        if(str[i] == '.') { // to check count of dots in the number
            dots++;
        }
    }
    if(dots > 1) { // too many dots in number - error
        return false;
    }
    if(i == str.size()) { // the number is valid
        return true;
    }
    return false;
}

// check if the variable is valid
bool variableIsValid(const string& str) {
    int i = 1;
    if(!isalpha(str[0]) && str[0] != '_') { // first char of variable must be or alphabetic or '_'
        return false;
    }
    while((str[i] != '\0') && (isalpha(str[i]) || isdigit(str[i]) || str[i] == '_')) {
        i++;
    }
    if(i == str.size()) { // the variable is valid
        return true;
    }
    return false;
}

// enter to the map the variables (key) and the values (value)
void enterToMap(const string& s, map<string, double> &varMap) {
    int location = s.find('=');
    if(location == string::npos) { // not found '=' in the string
        throw "no '=' in text";
    }
    string s1 = s.substr(0, location); // s1 will represent the variable
    string s2 = s.substr(location + 1, s.size() - location); // s2 will represent the value
    if(variableIsValid(s1) && numberIsValid(s2)) { // if both are valid
        double x = stringToDouble(s2);
        if(varMap.find(s1) == varMap.end()) { // // variable not in map
            varMap.insert(pair<string, double>(s1, x));
        }
        else { // variable already in map
            varMap.at(s1) = x; // so we only change the value of the key
        }
    }
    else {
        throw "error!!!!";
    }
}

// calculate and return the final expression
Expression* interpreterCalculate(queue<string> &queue1) {
    stack<Expression*> stack1; // stack to use for the expressions calculate
    Expression *e, *left, *right;
    while (!queue1.empty()) {
        if (isdigit(queue1.front()[0]) || (queue1.front().size() > 1 && isdigit(queue1.front()[1]))) { // the '||' is for the case of negative value in map
            e = new Value(stringToDouble(queue1.front()));
            stack1.push(e);
        } else { // queue front is operator
            right = stack1.top();
            stack1.pop();
            if(stack1.empty() && queue1.front()[0] == '$') {
                return new UMinus(right);
            }
            if(stack1.empty() && queue1.front()[0] == '@') {
                return new UPlus(right);
            }
            if(stack1.empty() && isOperator(queue1.front()[0])) {
                throw "wrong number of operators";
            }
            if(queue1.front()[0] != '$' && queue1.front()[0] != '@') { // because '$' and '@' represent unary operation
                left = stack1.top();
                stack1.pop();
            }
            switch (queue1.front()[0]) { // the operator
                case '+':
                    e = new Plus(left, right);
                    stack1.push(e);
                    break;
                case '-':
                    e = new Minus(left, right);
                    stack1.push(e);
                    break;
                case '*':
                    e = new Mul(left, right);
                    stack1.push(e);
                    break;
                case '/':
                    e = new Div(left, right);
                    stack1.push(e);
                    break;
                case '@':
                    e = new UPlus(right);
                    stack1.push(e);
                    break;
                case '$':
                    e = new UMinus(right);
                    stack1.push(e);
                    break;
            }
        }
        queue1.pop();
    }
    if (stack1.size() > 1) {
        throw "wrong input";
    }
    return stack1.top(); // the final result
}
Expression* Interpreter::interpret(const string&  infix) {
    int i = 0, dot = 0;
    while (infix[i] != '\0') {
        if (isOperator(infix[0]) && infix[0] == '*' && infix[0] == '/') { // wrong input
            throw "wrong start text";
        }
        if (isOperator(infix[i])) {
            if(i + 1 < infix.size() && isOperator(infix[i+1])) { // wrong expression
                throw "more than one operator - one after one";
            }
            if((infix[i] == '-' && i == 0) || (infix[i] == '-' && infix[i-1] == '(')) { // check if Unary UMinus
                stack1.push("$"); // '$' will represent UMinus in stack
            }
            else if((infix[i] == '+' && i == 0) || (infix[i] == '+' && infix[i-1] == '(')) { // check if Unary UPlus
                stack1.push("@"); // '@' will represent UPlus in stack
            }
            else if (!((!stack1.empty() && stack1.top() == "*" && (infix[i] == '+' || infix[i] == '-')) ||
                  (!stack1.empty() && stack1.top() == "/" && (infix[i] == '+' || infix[i] == '-')))) {
                string c(1, infix[i]); // cast infix[i] to string
                stack1.push(c);
            } else {
                while ((!stack1.empty() && stack1.top() == "*") || (!stack1.empty() && stack1.top() == "/")) {
                    queue1.push(stack1.top());
                    stack1.pop();
                }
                string c(1, infix[i]); // cast infix[i] to string
                stack1.push(c);
            }
        } else if (infix[i] == '(') {
            string c(1, infix[i]); // cast infix[i] to string
            stack1.push(c);
        } else if (infix[i] == ')') {
            while (stack1.top() != "(") {
                queue1.push(stack1.top());
                stack1.pop();
                if (stack1.empty()) {
                    throw "wrong parenthesis text";
                }
            }
            stack1.pop();
        } else { // infix[i] is part of value or variable
            if (isdigit(infix[i])) {
                string x;
                while (isdigit(infix[i]) || (infix[i] == '.')) {
                    if(infix[i] == '.') { // check number of dots in the number
                        if(i < infix.size() && !isdigit(infix[i+1])) { // wrong value
                            throw "problem in double number";
                        }
                        dot++;
                    }
                    if(dot > 1) { // invalid value
                        throw "too much dots in number";
                    }
                    x += infix[i];
                    i++;
                    if (isalpha(infix[i])) { // invalid value
                        throw "wrong var text";
                    }
                }
                dot = 0;
                queue1.push(x);
                i--;
            } else if (isalpha(infix[i])) {
                string y;
                while (!isOperator(infix[i]) && !isParenthesis(infix[i]) && infix[i] != '\0') {
                    y += infix[i];
                    i++;
                }
                if (varMap.find(y) == varMap.end()) { // the variable not found in map - error
                    throw "error - not in dictionary";
                } else {
                    string toQ = doubleToString(varMap.at(y));
                    queue1.push(toQ); // enter to queue the value of the variable (and not the variable)
                }
                i--;
            }
        }
        i++;
    }
    while (!stack1.empty()) { // now move all the rest members from the stack to the queue
        queue1.push(stack1.top());
        stack1.pop();
    }
    return interpreterCalculate(queue1); // will return the final expression
}

//separate the string to parts - each part will have variable and his value
void Interpreter::setVariables(const string&  str) {
    string temp = str;
    vector<string> vars;
    while (temp.find(';') != string::npos) { // while there are more ';' in string
        int x = temp.find(';');
        vars.push_back(temp.substr(0, x)); // enter to the variables string vector
        temp = temp.substr(x + 1, temp.size() - x); // move temp forward on string
    }
    if (temp.size() > 1) { // in case the string not ended with ';'
        vars.push_back(temp);
    }
    for (int i = 0; i < vars.size(); i++) { // will enter all variables to the map
        enterToMap(vars[i], varMap);
    }
}
