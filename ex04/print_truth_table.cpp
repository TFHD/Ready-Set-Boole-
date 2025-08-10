#include <iostream>
#include <stack>
#include <map>
#include <vector>

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

std::string replaceAll(std::string str, char oldChar, char newChar) {
    for (char& c : str)
        if (c == oldChar) c = newChar;
    return str;
}

bool eval_formula(std::string str) {

    std::stack<char> stack;
    for (int i = (int)str.size() - 1; i >= 0; i--)
        stack.push(str[i]);

    try { return solveRPN(stack); }
    catch(std::exception &e) { std::cout << e.what() << std::endl; return false;}
}

void printHeaderTable(std::vector<char> letters) {

    for (int i = 0; i < (int)letters.size(); i++)
        std::cout << "| " << letters[i] << " ";
    std::cout << "| = |" << std::endl;
    for (int i = 0; i < (int)letters.size() + 1; i++)
        std::cout << "|---";
    std::cout << "|" << std::endl;
}

void print_truth_table(std::string &str) {

    int nb_possibility = 0;
    int nb_letter = 0;
    std::map<char, int> mapLetter;
    std::vector<char> letters;
    for (int i = 0; i < (int)str.length(); i++) {
        if (std::isalpha(str[i]) && mapLetter.find(str[i]) == mapLetter.end()) {
            nb_letter++;
            mapLetter.insert({str[i], 0});
            letters.push_back(str[i]);
        }
    }
    nb_possibility = 1 << nb_letter;

    printHeaderTable(letters);

    for (int i = 0; i < nb_possibility; i++) {
        for (int j = 0; j < nb_letter; j++) {
            char c = letters[j];
            mapLetter[c] = (i >> (nb_letter - j - 1)) & 1;
        }
        for (int i = 0; i < (int)letters.size(); i++)
            std::cout << "| " << mapLetter[letters[i]] << " ";
        std::cout << "| ";

        std::string str1 = str;
        for (auto it = mapLetter.begin(); it != mapLetter.end(); it++) {
            str1 = replaceAll(str1, it->first, it->second + 48);
        }
        std::cout << eval_formula(str1) << " |" << std::endl;
    }
}

int main(int ac, char **av) {

    if (ac != 2) { std::cout << "need 1 parameter" << std::endl; return 1; }

    std::string str = av[1];
    print_truth_table(str);
    return 0;
}