#include <iostream>
#include <stack>
#include <map>
#include <vector>
#include <memory>


enum class NodeType { VAR, NOT, AND, OR, IMP, IFF, XOR };

struct Expr {
    NodeType type;
    std::string var;
    std::shared_ptr<Expr> left = nullptr, right = nullptr;
    std::shared_ptr<Expr> child = nullptr;

    static std::shared_ptr<Expr> Var(const std::string& name) {
        return std::make_shared<Expr>(Expr{NodeType::VAR, name});
    }
    static std::shared_ptr<Expr> Not(std::shared_ptr<Expr> e) {
        return std::make_shared<Expr>(Expr{NodeType::NOT, "!", nullptr, nullptr, e});
    }
    static std::shared_ptr<Expr> And(std::shared_ptr<Expr> l, std::shared_ptr<Expr> r) {
        return std::make_shared<Expr>(Expr{NodeType::AND, "&", l, r});
    }
    static std::shared_ptr<Expr> Or(std::shared_ptr<Expr> l, std::shared_ptr<Expr> r) {
        return std::make_shared<Expr>(Expr{NodeType::OR, "|", l, r});
    }
    static std::shared_ptr<Expr> Xor(std::shared_ptr<Expr> l, std::shared_ptr<Expr> r) {
    return std::make_shared<Expr>(Expr{NodeType::XOR, "^", l, r});
    }
    static std::shared_ptr<Expr> Imp(std::shared_ptr<Expr> l, std::shared_ptr<Expr> r) {
        return std::make_shared<Expr>(Expr{NodeType::IMP, ">", l, r});
    }
    static std::shared_ptr<Expr> Iff(std::shared_ptr<Expr> l, std::shared_ptr<Expr> r) {
        return std::make_shared<Expr>(Expr{NodeType::IFF, "=", l, r});
    }
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

bool isOperator(char c) {
    if (c == '|' || c == '&'
    || c == '^' || c == '!'
    || c == '>' || c == '=')
        return true;
    return false;
}

std::shared_ptr<Expr> chooseFormula(std::shared_ptr<Expr> x, std::shared_ptr<Expr> y, char op) {
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

std::shared_ptr<Expr> solveRPN(std::stack<char> &stack)
{
    std::stack<std::shared_ptr<Expr>> rpn;

    for (int i = 0; stack.size() != 0; i++) {
        if (isOperator(stack.top())) {
            if ((rpn.size() <= 1 && stack.top() != '!')
            || (rpn.size() < 1 && stack.top() == '!')) throw std::runtime_error("Invalid formula");
            if (stack.top() == '!') {
                auto x = rpn.top(); rpn.pop();
                rpn.push(chooseFormula(x, 0, stack.top()));
            } else {
            auto x = rpn.top(); rpn.pop();
            auto y = rpn.top(); rpn.pop();
            rpn.push(chooseFormula(x, y, stack.top()));
            }
        }
        else if (stack.top() < 'A' || stack.top() > 'Z') throw std::runtime_error("Unknow parameter");
        else {
            std::string str(1, stack.top());
            rpn.push(Expr::Var(str));
        }
        stack.pop();
    }
    if (rpn.size() != 1) throw std::runtime_error("Invalid formula");
    return rpn.top();
}

std::string displayRPN(const std::shared_ptr<Expr>& expr, std::string str) {
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

std::string negation_normal_form(std::string &str) {

    std::stack<char> stack;
    for (int i = (int)str.size() - 1; i >= 0; i--)
        stack.push(str[i]);

    try {
        std::shared_ptr<Expr> expr = solveRPN(stack);
        expr = eliminateComplexOps(expr);
        expr = toNNF(expr);
        std::string a = displayRPN(expr, "");
        std::cout << a << std::endl;
    }
    catch(std::exception &e) { std::cout << e.what() << std::endl; return ""; }  
    return "";  
}

int main(int ac, char **av) {

    if (ac != 2) { std::cout << "need 1 parameter" << std::endl; return 1; }

    std::string str = av[1];
    negation_normal_form(str);
    return 0;
}