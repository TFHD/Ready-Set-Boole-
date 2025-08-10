#include <iostream>
#include <stack>
#include <vector>
#include <map>
#include <algorithm>

typedef std::vector<int> tab;
typedef std::vector<tab> d_tab;

struct LogicExpr {

    static tab Or(tab y, tab x) {
        tab res = x;
        for (int i = 0; i < (int)y.size(); i++)
            if (std::find(res.begin(), res.end(), y[i]) == res.end())
                res.push_back(y[i]);
        return res;
    }
    static tab And(tab y, tab x) {
        tab res;
        for (int i = 0; i < (int)x.size(); i++)
            if (std::find(x.begin(), x.end(), y[i]) != x.end()) {
                res.push_back(y[i]);
            }
        return res;
    }
    static tab Xor(tab y, tab x) {
        tab res = LogicExpr::Not(LogicExpr::And(y, x), {y, x});
        return res;
    }
    static tab Not(tab x, d_tab ys) {
        tab res;
        for (int i = 0; i < (int)ys.size(); i++) {
            for (int j = 0; j < (int)ys[i].size(); j++) {
                if (std::find(x.begin(), x.end(), ys[i][j]) == x.end() &&
                    std::find(res.begin(), res.end(), ys[i][j]) == res.end())
                res.push_back(ys[i][j]);
            }
        }
        return res;
    }
    static tab Imp(tab y, tab x, d_tab all) {
        tab res = LogicExpr::Or(LogicExpr::Not(y, all), x);
        return res;
    }
    static tab Iff(tab x, tab y) {
        for (int i = 0; i < (int)y.size(); i++)
            if (std::find(x.begin(), x.end(), y[i]) == x.end())
                return {};
        return x;
    }
};

bool isOperator(char c) {
    if (c == '|' || c == '&'
    || c == '^' || c == '!'
    || c == '>' || c == '=')
        return true;
    return false;
}

void printTab(d_tab res) {

    for (int i = 0; i < (int)res.size(); i++) {
        std::cout << "[";
        for (int j = 0; j < (int)res[i].size(); j++) {
            std::cout << res[i][j];
            if (j + 1 < (int)res[i].size())
                std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }
}

void printTab2(tab &res) {
    std::cout << "[";
    for (int i = 0; i < (int)res.size(); i++) {
        std::cout << res[i];
        if (i + 1 < (int)res.size())
            std::cout << ", ";
        }
    std::cout << "]" << std::endl;
}

tab chooseFormula(tab x, tab y, d_tab all, char op) {

    if (op == '|')
        return LogicExpr::Or(x, y);
    if (op == '&')
        return LogicExpr::And(x, y);
    if (op == '^')
        return LogicExpr::Xor(x, y);
    if (op == '>')
        return LogicExpr::Imp(x, y, all);
    if (op == '!')  
        return LogicExpr::Not(x, all);
    if (op == '=')
        return LogicExpr::Iff(x, y);
    return {};
}

tab solveRPN(std::stack<char> &stack, d_tab &vec, std::map<char, tab> &map)
{
    std::stack<tab> rpn;

    for (int i = 0; stack.size() != 0; i++) {
        if (isOperator(stack.top())) {
            if ((rpn.size() <= 1 && stack.top() != '!')
            || (rpn.size() < 1 && stack.top() == '!')) throw std::runtime_error("Invalid formula");
            if (stack.top() == '!') {
                auto x = rpn.top(); rpn.pop();
                rpn.push(chooseFormula(x, x, vec, stack.top()));
            } else {
                auto x = rpn.top(); rpn.pop();
                auto y = rpn.top(); rpn.pop();
                rpn.push(chooseFormula(x, y, vec, stack.top()));
            }
        }
        else if (stack.top() < 'A' || stack.top() > 'Z') throw std::runtime_error("Unknow parameter");
        else
            rpn.push(map.at(stack.top()));
        stack.pop();
    }
    if (rpn.size() != 1) throw std::runtime_error("Invalid formula");
    return rpn.top();
}

tab eval_set(std::string &str, d_tab *vec) {

    std::stack<char> stack;
    std::map<char, tab> mapLetter;
    int nbLetter = 0;

    for (int i = (int)str.size() - 1; i >= 0; i--)
        stack.push(str[i]);

    try {
        for (int i = 0; i < (int)str.size(); i++) {
            if (str[i] >= 'A' && str[i] <= 'Z' && mapLetter.find(str[i]) == mapLetter.end()) {
                if (nbLetter + 1 > (int)vec->size())
                    throw std::runtime_error("Missings sets");
                mapLetter.insert({str[i], ( *vec)[nbLetter++]});
            }
        }
        if (nbLetter != (int)vec->size())
            throw std::runtime_error("Missings sets");
        tab res = solveRPN(stack, *vec, mapLetter);
        return res;
    }
    catch(std::exception &e) {
        std::cout << e.what() << std::endl;
        return {};
    }
}

int main(int ac, char **av) {

    if (ac <= 1) { std::cout << "need 1 parameter" << std::endl; return 1; }

    std::string str(av[1]);
    d_tab vec = { {0, 1, 2}, {0, 3, 4} };

    tab res = eval_set(str, &vec);
    printTab2(res);

    return 0;
}