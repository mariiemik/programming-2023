#include "boolexpr.h"
#include <stack>
#include "boolcalcnodes.h"
#include <algorithm>


BooleanExpressionNode* BooleanExpression::Postfix2Tree(const char* str, char** table, int row) {
	int index = 0; // Индекс элемента во входной строке
	std::stack<BooleanExpressionNode*> S;
	char ch;
	BooleanExpressionNode* result;
	BooleanExpressionNode* left;
	BooleanExpressionNode* right;
	char* paramname;
	int ind_name = 0;
	int perem = 0; // номер переменной

	while ((ch = str[index]) != 0) {
		left = right = nullptr;
		switch (ch) {
		case '~':
			left = S.top(); S.pop();
			result = new Negation(left);
			break;
		case '&':
			right = S.top(); S.pop();
			left = S.top(); S.pop();
			result = new Conjunction(left, right);
			break;
		case 'v':
			right = S.top(); S.pop();
			left = S.top(); S.pop();
			result = new Disjunction(left, right);
			break;
		case '+':
			right = S.top(); S.pop();
			left = S.top(); S.pop();
			result = new Xor(left, right);
			break;
		case '>':
			right = S.top(); S.pop();
			left = S.top(); S.pop();
			result = new Implication(left, right);
			break;
		case '<':
			right = S.top(); S.pop();
			left = S.top(); S.pop();
			result = new ReverseImplication(left, right);
			break;
		case '=':
			right = S.top(); S.pop();
			left = S.top(); S.pop();
			result = new Equivalence(left, right);
			break;
		case '|':
			right = S.top(); S.pop();
			left = S.top(); S.pop();
			result = new Nand(left, right);
			break;
		case '^':
			right = S.top(); S.pop();
			left = S.top(); S.pop();
			result = new Nor(left, right);
			break;
		default:
			paramname = new char[10];
			while ((ch = str[index]) != 0 && (str[index] >= '0' && str[index] <= '9') || ch == 'x' || ch == 'y' || ch == 'z') {
				paramname[ind_name++] = ch;
				if ((str[index + 1]) != 0 && (str[index + 1] >= '0' && str[index + 1] <= '9') || ch == 'x') {
					++index;
				}
				else { break; }
			}
			paramname[ind_name] = '\0';
			if (paramname[0] == 'y') {
				--perem;
				result = new VarNode(paramname, '0');
			}
			else if (paramname[0] == 'z') {
				--perem;
				result = new VarNode(paramname, '1');
			}
			else {
				std::string numb;
				int pni = 0;
				while ((paramname[pni]) != '\0') {
					if (paramname[pni] != 'x') {
						numb += paramname[pni];
					}
					++pni;
				}
				auto it = std::find(x_numbers.begin(), x_numbers.end(), stoi(numb));
				int indd = static_cast<int>(std::distance(x_numbers.begin(), it));
				result = new VarNode(paramname, static_cast<char>(table[row][indd]));
			}
			if(row!=0){
				++perem;
			}
		}
		S.push(result);
		++index;
		ind_name = 0;
	}
	result = S.top();
	return result;
}


const unsigned char BooleanExpression::ActionsTable[11][13] = {
	/*- 1 - добавить символ в выходную строку
	- 2 - добавить символ в стек
	- 3 - добавить символ с вершины стека в выходную строку и удалить его из стека
	- 7 - конец выражения
	- 9 - ошибка: неизвестный символ*/
	//  {0    +   -   *   /   ^ ( ) P F =} НА ВХОД
	  //{\0   ~  &    v   +   >   <   =   P   ^   |   (    )
		{'7','2','2','2','2','2','2','2','1','2','2','2','0'},//0
		{'3','2','3','3','3','3','3','3','1','3','2','2','3'},//~
		{'3','2','3','3','3','3','3','3','1','3','3','2','3'},//&
		{'3','2','2','3','3','3','3','3','1','3','3','2','3'},//v
		{'3','2','2','3','3','3','3','3','1','3','3','2','3'},//+
		{'3','2','2','2','2','3','3','3','1','3','3','2','3'},//>
		{'3','2','2','2','2','3','3','3','1','3','3','2','3'},//<
		{'3','2','2','2','2','2','2','3','1','3','3','2','3'},//=
		{'3','2','2','2','2','2','2','2','1','3','3','2','3'},//^
		{'3','2','2','2','2','2','2','2','1','3','3','2','3'},//|
		{'0','2','2','2','2','2','2','2','1','2','2','2','4'}// (

};
int BooleanExpression::actionsRowNumber(char ch) {
	switch (ch) {
	case '~': return 1;
	case '&': return 2;
	case 'v': return 3;
	case '+': return 4;
	case '>': return 5;
	case '<': return 6;
	case '=': return 7;
	case '|': return 9;
	case '^': return 8;
	case '(': return 10;
	}
	return 7;
}
int BooleanExpression::actionColNumber(char ch) {
	switch (ch) {
	case '\0': return 0;
	case '~': return 1;
	case '&': return 2;
	case 'v': return 3;
	case '+': return 4;
	case '>': return 5;
	case '<': return 6;
	case '=': return 7;
	case '|': return 10;
	case '^': return 9;
	case '(': return 11;
	case ')': return 12;
	}
	return 7;
}

void BooleanExpression::Infix2Postfix(const char* instr, char* outstr, int& num) {
	int i = 0; // индекс во входной строке
	int j = 0; // индекс в выходной строке
	int row;
	int col;
	unsigned char action = 0;
	std::stack<char> S;
	std::string num_of_x;
	int kol = 0; 
	do {
		if ((instr[i] != '\0' && instr[i] != '~' && instr[i] != '&' && instr[i] != 'v' && instr[i] != '+' && instr[i] != '>' && instr[i] != '<' && instr[i] != '=' && instr[i] != '|' && instr[i] != '^' && instr[i] != '(' && instr[i] != ')')) {
			if (instr[i] != 'x' && instr[i] != 'y' && instr[i] != 'z' && (instr[i] >= '9' || instr[i] <= '0')) {
				throw InvOper();
			}
			if (instr[i] <= '9' && instr[i] >= '0') {
				num_of_x += instr[i];
			}
			if (instr[i] == 'x') {
				++kol;
				if (kol > 1) {
					throw InvExpr();
				}
				++num;
			}
			outstr[j++] = instr[i++]; //записываем переменную во входную строку
			continue;
		}
		kol = 0;
		if (!num_of_x.empty()) {
			auto it = std::find(x_numbers.begin(), x_numbers.end(), stoi(num_of_x));
			if (it != x_numbers.end()) {
				--num;
			}
			else {
				x_numbers.push_back(stoi(num_of_x));
			}
			num_of_x.clear();
		}
		col = actionColNumber(instr[i]); 
		row = S.empty() ? 0 : actionsRowNumber(S.top());
		action = ActionsTable[row][col];
		switch (action) {
		case '1': outstr[j] = instr[i]; 	++j; ++i; break;
		case '2': S.push(instr[i]); ++i;	break;
		case '3': outstr[j] = S.top(); S.pop(); ++j; break;
		case '4': S.pop(); ++i; break;
		}
	} while (action != '7');
	outstr[j] = '\0';
}

void BooleanExpression::InfixFilter(const char* instr, char* outstr) {//упростить сложные логич операторы, убрать пробелы}
	int i = 0; // Индекс во входной строке
	int j = 0; // Индех в выходной строке
	char ch;
	char buf[256];
	int bufLen = 0;
	char prev = 0; // Предыдущий символ во входной строке
	bool space = false;
	bool x = false;
	while ((ch = instr[i++]) != 0) {
		// Пропустить пробелы и табуляции
		if (ch == ' ' || ch == '\t' || ch == static_cast<char>(13)) {
			x = false;
			space = true;
			continue;
		}
		if (ch <= '9' && ch >= '0' && !x) {
			if (ch == '0') {
				ch = 'y';
			}
			else if (ch == '1') {
				ch = 'z';
			}
		}
		else if (ch == 'x') {
			if (instr[i] <= '0' || instr[i] >= '9') {
				throw InvVar();
			}
			x = true;
		}
		else if (ch >= '0' && ch <= '9' && x && space) {
			j = 0;
			break;
		}
		outstr[j++] = ch;
		space = false;
	}
	outstr[j] = '\0';
}

void BooleanExpression::TruthTableVar(char** table, int numVar) {
	int step = 1;
	for (int i = 0; i < numVar; ++i) {
		step *= 2;
	}
	int kur = step / 2;
	bool zero = true;
	for (int i = 0; i < numVar; ++i) {
		for (int j = 0; j < step; ++j) {
			table[j][i] = zero ? '0' : '1';
			if ((j + 1) % kur == 0) {
				zero = !zero;
			}
		}
		kur /= 2;
	}
	
}
std::string BooleanExpression::cnf() {
	std::string res;
	if (NumberOfVariables > 0) {
		res = "(";
		int kol = 0;
		int size = static_cast<int>(pow(2, NumberOfVariables));
		for (int i = 0; i < size; ++i) {
			if (table[i][NumberOfVariables] == '0') {
				++kol;
				if (kol > 1) {
					res += ") & (";
				}
				for (int j = 0; j < NumberOfVariables; ++j) {

					if (table[i][j] != '0') {
						res += "~";
					}
					res += 'x';
					res += to_string(j + 1);
					if (j != NumberOfVariables - 1) {
						res += "v";
					}
				}
			}
		}
		res += ")";
	}
	else {
		res += root->calc();
	}
	return res;
}
std::string BooleanExpression::dnf() {
	std::string res;
	if (NumberOfVariables > 0) {
		res = "";
		int kol = 0;
		int size = static_cast<int>(pow(2, NumberOfVariables));
		for (int i = 0; i < size; ++i) {
			if (table[i][NumberOfVariables] == '1') {
				++kol;
				if (kol > 1) {
					res += " v ";
				}
				for (int j = 0; j < NumberOfVariables; ++j) {
					if (table[i][j] != '1') {
						res += "~";
					}
					res += 'x';
					res += to_string(j + 1);
					if (j != NumberOfVariables - 1) {
						res += "&";
					}
				}
			}
		}
	}
	else {
		res += root->calc();
	}
	return res;
}
std::string BooleanExpression::zhegalkin() const {
	std::string zheg_str;
	if (NumberOfVariables > 0) {
		int kol = 1;
		int ckol = 0;
		bool change = false;
		int size = static_cast<int>(pow(2, NumberOfVariables));
		char* dop_col = new char[size];
		char* res = new char[size];
		for (int i = 0; i < size; ++i) {
			res[i] = table[i][NumberOfVariables];
		}

		while (kol < size) {
			for (int i = 0; i < size; ++i) {
				++ckol;
				if (change) {
					dop_col[i] = ((dop_col[i - kol] == '1' || res[i] == '1') && (dop_col[i - kol] == '0' || res[i] == '0') ? '1' : '0');
				}
				else {
					dop_col[i] = res[i];
				}
				if (ckol == kol) {
					change = !change;
					ckol = 0;
				}
			}
			kol *= 2;
			for (int i = 0; i < size; ++i) {
				res[i] = dop_col[i];
			}
		}
		int n = 0;
		int kolich = 0;
		for (int i = 0; i < size; ++i) {
			if (res[i] == '1') {
				++n;
				if (n != 1) {
					zheg_str += " + ";
				}
				if (i == 0) {
					zheg_str += '1';
				}
				else {
					for (int j = 0; j < NumberOfVariables; ++j) {
						if (i == 0) {
							zheg_str += '1';
						}
						else if (table[i][j] == '1') {
							++kolich;
							if (kolich > 1) {
								zheg_str += '&';
							}
							zheg_str += "x";
							zheg_str += to_string(j + 1);

						}
					}
					kolich = 0;
				}
			}
		}
		delete[] dop_col;
		delete[] res;
	}
	else {
		zheg_str = root->calc();
	}
	return zheg_str;
}

bool isFullSystem(const std::vector<BooleanExpression>& sys) {
	bool** syst;
	syst = new bool* [sys.size()];
	for (int i = 0; i < sys.size(); ++i) {
		syst[i] = new bool[5];
		for (int j = 0; j < 5; ++j) {
			syst[i][j] = false;
		}
	}
	for (int i = 0; i < sys.size(); ++i) {
		if (sys[i].NumberOfVariables == 0) {
			if (sys[i].table[0][0] == '0') {
				syst[i][0] = true;
			}
			else {
				syst[i][0] = false;
			}
		}
		else {
			if (sys[i].table[0][sys[i].NumberOfVariables] == '0') {
				syst[i][0] = true;
			}
			else {
				syst[i][0] = false;
			}
		}
	}
	for (int i = 0; i < sys.size(); ++i) {
		if (sys[i].NumberOfVariables == 0) {
			if (sys[i].table[0][0] == '1') {
				syst[i][1] = true;
			}
			else {
				syst[i][1] = false;
			}
		}
		else {
			int k = static_cast<int>(pow(2, sys[i].NumberOfVariables));
			if (sys[i].table[k - 1][sys[i].NumberOfVariables] == '1') {
				syst[i][1] = true;
			}
			else {
				syst[i][1] = false;
			}
		}
	}
	for (int i = 0; i < sys.size(); ++i) {
		if (sys[i].NumberOfVariables == 0) {
			syst[i][2] = true;
		}
		else {
			std::string zheg = sys[i].zhegalkin();
			int kol = 0;
			syst[i][2] = true;
			for (int i2 = 0; i2 < zheg.size(); ++i2) {
				if (zheg[i2] == 'x') {
					++kol;
					if (kol > 1) {
						syst[i][2] = false;
						break;
					}
				}
				else if (zheg[i2] == ' ' && kol != 0) {
					if (kol > 1) {
						syst[i][2] = false;
						break;
					}
					kol = 0;
				}
			}
		}
	}
	for (int i = 0; i < sys.size(); ++i) {
		if (sys[i].NumberOfVariables == 0) {
			syst[i][3] = true;
		}
		else {
			int k = static_cast<int>(pow(2, sys[i].NumberOfVariables));
			syst[i][3] = true;
			for (int i2 = 0; i2 < k; ++i2) {
				for (int j = 0; j < k; ++j) {
					if (i2 == j) {
						continue;
					}
					bool comp = true;
					for (int n = 0; n < sys[i].NumberOfVariables; ++n) {
						if ((sys[i].table[i2][n] != sys[i].table[j][n]) && (sys[i].table[i2][n] != '0' && sys[i].table[j][n] != '1')) {
							comp = false;
							break;
						}
					}
					if (comp) {
						if ((sys[i].table[i2][sys[i].NumberOfVariables] != sys[i].table[j][sys[i].NumberOfVariables]) && (sys[i].table[i2][sys[i].NumberOfVariables] != '0' && sys[i].table[j][sys[i].NumberOfVariables] != '1')) {
							syst[i][3] = false;
							break;
						}
					}
				}
				if (!syst[i][3]) {
					break;
				}
			}
		}
	}
	for (int i = 0; i < sys.size(); ++i) {
		if (sys[i].NumberOfVariables == 0) {
			syst[i][4] = false;
		}
		else {
			int k = static_cast<int>(pow(2, sys[i].NumberOfVariables));
			char tmp;
			int i_rev = 0;
			syst[i][4] = true;
			for (int i2 = k - 1; i2 >= 0; --i2) {
				tmp = (sys[i].table[i2][sys[i].NumberOfVariables] == '1') ? '0' : '1';
				if (sys[i].table[i_rev++][sys[i].NumberOfVariables] != tmp) {
					syst[i][4] = false;
					break;
				}
			}
		}
	}
	int count = 0;
	for (int j = 0; j < 5; ++j) {
		for (int i = 0; i < sys.size(); ++i) {
			if (!syst[i][j]) {
				++count;
				break;
			}
		}
	}
	for (int i = 0; i < sys.size(); ++i) {
		delete[] syst[i];
	}
	delete[] syst;

	return count == 5;
} // проверяет на полноту систему функций
