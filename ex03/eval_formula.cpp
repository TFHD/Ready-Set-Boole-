#include <iostream>
#include <stack>

bool isOperator(char c) {
    if (c == '|' || c == '&'
    || c == '^' || c == '!'
    || c == '>' || c == '=')
        return true;
    return false;
}

int chooseFormula(char x, char y, char op) {
    if (op == '|')
        return (y | x);
    if (op == '&')
        return (y & x);
    if (op == '^')
        return (y ^ x);
    if (op == '>')
        return (!y || x);
    if (op == '!')  
        return (!x);
    if (op == '=')
        return (x == y);
    return 0;
}

int solveRPN(std::stack<char> &stack)
{
    std::stack<char> rpn;
    char x = 0;
    char y = 0;

    for (int i = 0; stack.size() != 0; i++) {
        if (isOperator(stack.top())) {
            if ((rpn.size() <= 1 && stack.top() != '!')
            || (rpn.size() < 1 && stack.top() == '!')) throw std::runtime_error("Invalid formula");
            if (stack.top() == '!') {
                x = rpn.top(); rpn.pop();
                rpn.push(chooseFormula(x, 0, stack.top()));
            } else {
            x = rpn.top(); rpn.pop();
            y = rpn.top(); rpn.pop();
            rpn.push(chooseFormula(x, y, stack.top())); }
        }
        else if (stack.top() != '0' && stack.top() != '1') throw std::runtime_error("Unknow parameter");
        else
            rpn.push(stack.top() - '0');
        stack.pop();
    }
    if (rpn.size() != 1) throw std::runtime_error("Invalid formula");
    return rpn.top();
}

bool eval_formula(std::string &str) {

    std::stack<char> stack;
    for (int i = (int)str.size() - 1; i >= 0; i--)
        stack.push(str[i]);

    try { return solveRPN(stack); }
    catch(std::exception &e) { std::cout << e.what() << std::endl; return false; }
}

int main(int ac, char **av) {
    if (ac != 2) {
        std::cout << "need 1 parameter" << std::endl;
        return 1;
    }
    std::string str = av[1];
    bool res = eval_formula(str);

    if (res)    std::cout << "true" << std::endl;
    else        std::cout << "false" << std::endl;

    return 0;
}