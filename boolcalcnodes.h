#ifndef BOOLCALCNODES_H
#define BOOLCALCNODES_H
#include <iostream>
#include <cstring>
#include <cmath>
#include "errors.h"

class BooleanExpressionNode {
public:
	virtual char calc() const = 0;
	virtual std::string str() const = 0;
	virtual ~BooleanExpressionNode() {}
};

class Var {
	const char* var; //имя переменной
	char value; //значение
public:
	Var(const char* x, char val) : var(x), value(val) {}
	char calc() const { return value; }
    const char* getName() const { return var; }
	bool operator<(const Var& X)const {
		char* t1 = new char[strlen(var) - 1];
		char* t2 = new char[strlen(X.var) - 1];
		for (int i = 1; i < strlen(var); ++i) {
			t1[i - 1] = var[i];
		}
		for (int i = 1; i < strlen(X.var); ++i) {
			t2[i - 1] = X.var[i];
		}
		int th = atoi(t1);
		int oth = atoi(t2);
		delete[] t1;
		delete[] t2;
		return th < oth; }
	bool operator==(const Var& X) const {
		char* t1 = new char[strlen(var) - 1];
		char* t2 = new char[strlen(X.var) - 1];
		for (int i = 1; i < strlen(var); ++i) {
			t1[i - 1] = var[i];
		}
		for (int i = 1; i < strlen(X.var); ++i) {
			t2[i - 1] = X.var[i];
		}
		int th = atoi(t1);
		int oth = atoi(t2); 
		delete[] t1;
		delete[] t2;
		return th==oth; }
	~Var() {
		delete[] var;
	}
};


class VarNode : public BooleanExpressionNode {
	const char* var;
	char value;
public:

	VarNode(const char* v, char val) :var(v), value(val) {}
	char calc() const { return value; }
	std::string str() const {
		return std::string(var);
	}
	~VarNode() {
		delete[] var;
	}
};


class OperationNode : public BooleanExpressionNode {
protected:
	BooleanExpressionNode* left;
	BooleanExpressionNode* right;
public:
	OperationNode(BooleanExpressionNode* L, BooleanExpressionNode* R) : left(L), right(R) {}
	~OperationNode() {
		delete left;
		delete right;
		left = right = nullptr;
	}
};
class BinOperationNode : public BooleanExpressionNode {
protected:
	BooleanExpressionNode* left;
public:
	BinOperationNode(BooleanExpressionNode* L) : left(L) {}
	~BinOperationNode() {
		delete left;
		left = nullptr;
	}
};


class Negation : public BinOperationNode {
public:
	Negation(BooleanExpressionNode* L) : BinOperationNode(L) {}
	char calc() const { return left->calc() == '1' ? '0' : '1'; }
	std::string str() const {
		return '~' + left->str();
	}
};


class Conjunction : public OperationNode {
public:
	Conjunction(BooleanExpressionNode* L, BooleanExpressionNode* R): OperationNode(L,R){}
	char calc() const { return (left->calc() == right->calc() && left->calc() == '1') ? '1' : '0'; }
	std::string str() const {
		return left->str() + "&" + right->str();
	}
};
class Disjunction : public OperationNode {
public:
	Disjunction(BooleanExpressionNode* L, BooleanExpressionNode* R) : OperationNode(L, R) {}
	char calc() const { return (left->calc() == '1' || right->calc() == '1') ? '1' : '0'; }
	std::string str() const {
		return left->str() + "v" + right->str();
	}
};
class Xor : public OperationNode {
public:
	Xor(BooleanExpressionNode* L, BooleanExpressionNode* R) : OperationNode(L, R) {}
	char calc() const { return (left->calc() == right->calc()) ? '0' : '1'; }
	std::string str() const {
		return left->str() + "+" + right->str();
	}
};
class Implication : public OperationNode {
public:
	Implication(BooleanExpressionNode* L, BooleanExpressionNode* R) : OperationNode(L, R) {}
	char calc() const { return (left->calc() == '1' && right->calc() == '0') ? '0' : '1'; }
	std::string str() const {
		return left->str() + ">" + right->str();
	};
};
class ReverseImplication : public OperationNode {
public:
	ReverseImplication(BooleanExpressionNode* L, BooleanExpressionNode* R) : OperationNode(L, R) {}
	char calc() const { return (left->calc() == '1' && right->calc() == '0') ? '0' : '1'; }
	std::string str() const {
		return left->str() + "<" + right->str();
	};
};
class Equivalence : public OperationNode {
public:
	Equivalence(BooleanExpressionNode* L, BooleanExpressionNode* R) : OperationNode(L, R) {}
	char calc() const { return (left->calc() == right->calc()) ? '1' : '0'; }
	std::string str() const {
		return left->str() + "=" + right->str();
	};
};
class Nand : public OperationNode { //Штрих Шеффера
public:
	Nand(BooleanExpressionNode* L, BooleanExpressionNode* R) : OperationNode(L, R) {}
	char calc() const { return (left->calc() == right->calc()) && left->calc() == '1' ? '0' : '1'; }
	std::string str() const {
		return left->str() + "|" + right->str();
	};
};
class Nor : public OperationNode {//стрелка пирса
public:
	Nor(BooleanExpressionNode* L, BooleanExpressionNode* R) : OperationNode(L, R) {}
	char calc() const { return (left->calc() == right->calc()) && left->calc() == '0' ? '1' : '0'; }
	std::string str() const {
		return left->str() + "^" + right->str();
	};
};
#endif
