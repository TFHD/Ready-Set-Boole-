#include <iostream>
#include <stack>
#include <map>
#include <vector>
#include <memory>
#include <typeinfo>


enum class NodeType { VAR, NOT, AND, OR, IMP, IFF, XOR };

struct Expr {
    NodeType type;
    std::string var;
    std::shared_ptr<Expr> left = nullptr, right = nullptr;
    std::shared_ptr<Expr> child = nullptr;

    static std::shared_ptr<Expr> Var(const std::string& name) { return std::make_shared<Expr>(Expr{NodeType::VAR, name}); }
    static std::shared_ptr<Expr> Not(std::shared_ptr<Expr> e) { return std::make_shared<Expr>(Expr{NodeType::NOT, "!", nullptr, nullptr, e}); }
    static std::shared_ptr<Expr> And(std::shared_ptr<Expr> l, std::shared_ptr<Expr> r) { return std::make_shared<Expr>(Expr{NodeType::AND, "&", l, r}); }
    static std::shared_ptr<Expr> Or(std::shared_ptr<Expr> l, std::shared_ptr<Expr> r) { return std::make_shared<Expr>(Expr{NodeType::OR, "|", l, r}); }
    static std::shared_ptr<Expr> Xor(std::shared_ptr<Expr> l, std::shared_ptr<Expr> r) { return std::make_shared<Expr>(Expr{NodeType::XOR, "^", l, r}); }
    static std::shared_ptr<Expr> Imp(std::shared_ptr<Expr> l, std::shared_ptr<Expr> r) { return std::make_shared<Expr>(Expr{NodeType::IMP, ">", l, r}); }
    static std::shared_ptr<Expr> Iff(std::shared_ptr<Expr> l, std::shared_ptr<Expr> r) { return std::make_shared<Expr>(Expr{NodeType::IFF, "=", l, r}); }
    static bool Not(char x) { return !x; }
    static bool And(char x, char y) { return x & y; }
    static bool Or(char x, char y) { return x | y; }
    static bool Xor(char x, char y) { return y ^ x; }
    static bool Imp(char x, char y) { return !y || x; }
    static bool Iff(char x, char y) { return x == y; }
};

std::shared_ptr<Expr> toNNF(const std::shared_ptr<Expr> expr, bool neg = false) {
    switch (expr->type) {
        case NodeType::VAR:
            return neg ? Expr::Not(expr) : expr;
        case NodeType::NOT:
            return toNNF(expr->child, !neg);
        case NodeType::AND:
            if (neg)
                return Expr::Or(toNNF(expr->left, true), toNNF(expr->right, true));
            else
                return Expr::And(toNNF(expr->left, false), toNNF(expr->right, false));
        case NodeType::OR:
            if (neg)
                return Expr::And(toNNF(expr->left, true), toNNF(expr->right, true));
            else
                return Expr::Or(toNNF(expr->left, false), toNNF(expr->right, false));
        default:
            throw std::runtime_error("Non supported type !");
    }
}

std::shared_ptr<Expr> eliminateComplexOps(const std::shared_ptr<Expr>& expr) {
    switch (expr->type) {
        case NodeType::XOR:
            // A!B& AB!& |
            return Expr::Or(
                Expr::And(Expr::Not(eliminateComplexOps(expr->left)), eliminateComplexOps(expr->right)),
                Expr::And(eliminateComplexOps(expr->left), Expr::Not(eliminateComplexOps(expr->right)))
            );
        case NodeType::IMP:
            // A!B|
            return Expr::Or(
                Expr::Not(eliminateComplexOps(expr->left)), eliminateComplexOps(expr->right)
            );
        case NodeType::IFF:
            // A!B!& AB& |
            return Expr::Or(
                Expr::And(eliminateComplexOps(expr->left), eliminateComplexOps(expr->right)),
                Expr::And(Expr::Not(eliminateComplexOps(expr->left)), Expr::Not(eliminateComplexOps(expr->right))
                )
            );
        case NodeType::NOT:
            return Expr::Not(eliminateComplexOps(expr->child));
        case NodeType::AND:
        case NodeType::OR:
            return std::make_shared<Expr>(Expr{expr->type, "|", eliminateComplexOps(expr->left), eliminateComplexOps(expr->right)});
        case NodeType::VAR:
        default:
            return expr;
    }
}

std::shared_ptr<Expr> distributeOrOverAnd(const std::shared_ptr<Expr>& expr) {
    if (!expr) return nullptr;
    if (expr->type == NodeType::OR) {
        auto A = distributeOrOverAnd(expr->left);
        auto B = distributeOrOverAnd(expr->right);

        if (A && A->type == NodeType::AND) {
            return Expr::And(
                distributeOrOverAnd(Expr::Or(A->left, B)),
                distributeOrOverAnd(Expr::Or(A->right, B)));
        }
        if (B && B->type == NodeType::AND) {
            return Expr::And(
                distributeOrOverAnd(Expr::Or(A, B->left)),
                distributeOrOverAnd(Expr::Or(A, B->right)));
        }
        return Expr::Or(A, B);
    }
    else if (expr->type == NodeType::AND) {
        return Expr::And(
            distributeOrOverAnd(expr->left),
            distributeOrOverAnd(expr->right));
    }
    else if (expr->type == NodeType::NOT)
        return Expr::Not(distributeOrOverAnd(expr->child));

    return expr;
}

template <typename T>
bool isOperator(T c) {
    if (c == '|' || c == '&'
    || c == '^' || c == '!'
    || c == '>' || c == '=')
        return true;
    return false;
}

static std::string displayRPN(const std::shared_ptr<Expr>& expr, std::string str) {
    if (!expr) return str;
    if (expr->type == NodeType::VAR) {
        str.append(expr->var);
    } else if (expr->type == NodeType::NOT) {
        str = displayRPN(expr->child, str);
        str.append(expr->var);
    } else {
        str = displayRPN(expr->left, str);
        str = displayRPN(expr->right, str);
        str.append(expr->var);
    }
    return str;
}

template <typename T>
T chooseFormula(T x, T y, char op) {
    if (op == '|')
        return (Expr::Or(y, x));
    if (op == '&')
        return (Expr::And(y, x));
    if (op == '^')
        return (Expr::Xor(y, x));
    if (op == '>')
        return (Expr::Imp(y, x));
    if (op == '!')
        return (Expr::Not(x));
    if (op == '=')
        return (Expr::Iff(y, x));
    return 0;
}

std::shared_ptr<Expr> solveRPNExpr(std::stack<char> &stack) {
    std::stack<std::shared_ptr<Expr>> rpn;
    while (!stack.empty()) {
        if (isOperator<char>(stack.top())) {
            if ((rpn.size() <= 1 && stack.top() != '!') || (rpn.size() < 1 && stack.top() == '!'))
                throw std::runtime_error("Invalid formula");
            if (stack.top() == '!') {
                auto x = rpn.top(); rpn.pop();
                rpn.push(chooseFormula(x, x, stack.top()));
            } else {
                auto x = rpn.top(); rpn.pop();
                auto y = rpn.top(); rpn.pop();
                rpn.push(chooseFormula(x, y, stack.top()));
            }
        } else {
            if (stack.top() < 'A' || stack.top() > 'Z') throw std::runtime_error("Unknow parameter");
            std::string str(1, stack.top());
            rpn.push(Expr::Var(str));
        }
        stack.pop();
    }
    if (rpn.size() != 1) throw std::runtime_error("Invalid formula");
    return rpn.top();
}

char solveRPNChar(std::stack<char> &stack) {
    std::stack<char> rpn;
    while (!stack.empty()) {
        if (isOperator<char>(stack.top())) {
            if ((rpn.size() <= 1 && stack.top() != '!') || (rpn.size() < 1 && stack.top() == '!'))
                throw std::runtime_error("Invalid formula");
            if (stack.top() == '!') {
                char x = rpn.top(); rpn.pop();
                rpn.push(chooseFormula(x, x, stack.top()));
            } else {
                char x = rpn.top(); rpn.pop();
                char y = rpn.top(); rpn.pop();
                rpn.push(chooseFormula(x, y, stack.top()));
            }
        } else {
            if (stack.top() != '0' && stack.top() != '1') throw std::runtime_error("Unknow parameter");
            rpn.push(stack.top() - '0');
        }
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

std::string conjunctive_normal_form(std::string &str) {

    std::stack<char> stack;
    for (int i = (int)str.size() - 1; i >= 0; i--)
        stack.push(str[i]);

    try {
        std::shared_ptr<Expr> expr = solveRPNExpr(stack);
        expr = eliminateComplexOps(expr);
        expr = toNNF(expr);
        expr = distributeOrOverAnd(expr);
        std::string res = displayRPN(expr, "");
        return res;
    }
    catch(std::exception &e) { std::cout << e.what() << std::endl; return "" ; }
    return "";   
}

bool eval_formula(std::string str) {

    std::stack<char> stack;
    for (int i = (int)str.size() - 1; i >= 0; i--)
        stack.push(str[i]);

    try { return solveRPNChar(stack); }
    catch(std::exception &e) { std::cout << e.what() << std::endl; return false; }
}

int getTruthTable(std::string &str) {

    int nb_possibility = 0;
    int nb_letter = 0;
    int efficiant = 0;
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

    for (int i = 0; i < nb_possibility; i++) {
        for (int j = 0; j < nb_letter; j++) {
            char c = letters[j];
            mapLetter[c] = (i >> (nb_letter - j - 1)) & 1;
        }
        std::string str1 = str;
        for (auto it = mapLetter.begin(); it != mapLetter.end(); it++) {
            str1 = replaceAll(str1, it->first, it->second + 48);
        }
        if (eval_formula(str1))
            efficiant = 1;
    }
    return efficiant;
}

bool sat(std::string &str) {
    std::string Cnf = conjunctive_normal_form(str);
    bool res = getTruthTable(Cnf);
    return res;
}

int main(int ac, char **av) {

    if (ac != 2) { std::cout << "need 1 parameter" << std::endl; return 1; }

    std::string str = av[1];

    if (sat(str))   std::cout << "true" << std::endl;
    else            std::cout << "false" << std::endl;

    return 0;
}