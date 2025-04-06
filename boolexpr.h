#ifndef BOOLEXPR_H
#define BOOLEXPR_H
#include "boolcalcnodes.h"
#include <stack>
#include <vector>
using namespace std;

class BooleanExpression {
	BooleanExpressionNode* root;
	BooleanExpressionNode* Postfix2Tree(const char*, char** table, int row);
	void Infix2Postfix(const char* instr, char* outstr, int& num);
	static void InfixFilter(const char*, char* outstr);
	static void TruthTableVar(char** table, int numVar);
	static const unsigned char ActionsTable[11][13];
	static int actionsRowNumber(char);
	static int actionColNumber(char);
	int NumberOfVariables;
	std::vector<int> x_numbers;
	char** table;
public:
	BooleanExpression(const char* str) {

		size_t Len = strlen(str);
		Len = (Len < 10) ? 20 : 2 * Len;
		char* str_infix = new char[Len];
		char* str_posfix = new char[Len];
		bool error = false;

		try {
			InfixFilter(str, str_infix);
			NumberOfVariables = 0;
			if (!error) {
				Infix2Postfix(str_infix, str_posfix, NumberOfVariables);
				int size = static_cast<int>(pow(2, NumberOfVariables));
				root = nullptr;
				table = new char* [size];
				for (int i = 0; i < size; ++i) {
					table[i] = new char[NumberOfVariables + 1];
				}
				TruthTableVar(table, NumberOfVariables);
				for (int i = 0; i < size; ++i) {
					delete root;
					root = nullptr;
					root = Postfix2Tree(str_posfix, table, i);
					table[i][NumberOfVariables] = root->calc();
				}
			}
		}
		catch (...) {
			root = nullptr;
			table = nullptr;
			delete[] str_infix;
			delete[] str_posfix;
			throw;
		}

		delete[] str_infix;
		delete[] str_posfix;
	}
	BooleanExpression(const BooleanExpression& X) {
		char* tmp = new char[10];
		tmp[0] = 'y';
		tmp[1] = '\0';
		root = nullptr;
		NumberOfVariables = X.NumberOfVariables;
		int size = static_cast<int>(pow(2, NumberOfVariables));
		root = new VarNode(tmp, X.root->calc());

		table = new char* [size];
		for (int i = 0; i < size; ++i) {
			table[i] = new char[NumberOfVariables + 1];
			for (int j = 0; j <= NumberOfVariables; ++j) {
				table[i][j] = X.table[i][j];
			}
		}
	}
	char calc() const {
		if (root!= nullptr){ return root->calc(); }
		return 0;
	}
	std::string str() const {
		if (root!= nullptr){
			return root->str();
		}
		return std::string("");

	}
	bool operator==(const string& oth) const{
		BooleanExpression a(oth.c_str());
		if (NumberOfVariables != a.NumberOfVariables) {
			return false;
		}
		int size = static_cast<int>(pow(2, NumberOfVariables));
		for (int i = 0; i < size; ++i) {
			if (table[i][NumberOfVariables] != a.table[i][NumberOfVariables]) {
				return false;
			}
		}
		return true;
	}

	std::string cnf();//строит конъюнктивную нормальную форму.
	std::string dnf();//строит дизъюнктивную нормальную форму.
	std::string zhegalkin() const;// строит полином Жегалкина.
	friend bool isFullSystem(const std::vector<BooleanExpression>& sys);
	~BooleanExpression() {
		int size = static_cast<int>(pow(2, NumberOfVariables));
		for(int i = 0; i<size; ++i){
			delete[] table[i];
		}
		delete[] table;
		table = nullptr;
		delete root;
		root = nullptr;
	}
};
#endif
