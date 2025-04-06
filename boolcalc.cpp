/*Калькулятор булевских выражений
Реализуйте класс для работы с булевскими выражениями.

Операции:
~ отрицание,
& конъюнкция,
v дизъюнкция,
+ сложение по модулю 2,
> импликация (x → y),
< обратная импликация (x ← y),
= эквиваленция,
| штрих Шеффера,
^ стрелка Пирса.
Калькулятор должен строить конъюнктивную и дизъюнктивную нормальные формы, полином Жегалкина, проверять систему функций на полноту.*/

#include "boolexpr.h"
#include <iostream>
#include <fstream>
int main(int argc, char* argv[])
{
	if (argc > 1) {
		std::vector<BooleanExpression> B;
		std::ifstream inp(argv[2]);
		std::ofstream out(argv[3]);
		char* tp = new char[150];
		int NumberOfExpressions = 0;
		char c;
		do {
			c = static_cast<char>(inp.get());
			if (c == EOF || c == '\n') {
				++NumberOfExpressions;
			}
		} while (!inp.eof());
		B.reserve(NumberOfExpressions);
		inp.close();
		inp.open(argv[2]);
		try{
		char ch;
		int i_tp = 0;
		int size = 0;
			do {
				ch = static_cast<char>(inp.get());
				if (ch != EOF && ch != '\n') {
					tp[i_tp++] = ch;
				}
				else if(ch == EOF && i_tp==0){
					break;
				}
				else {
					++size;
					tp[i_tp] = '\0';
					B.emplace_back(tp);
					i_tp = 0;
				}
			} while (!inp.eof());
		
		switch (argv[1][1]) {
		case 'd':
			for (int i = 0; i < B.size(); ++i) {
				out << B[i].dnf();
				if (i != B.size() - 1) {
					out << '\n';
				}
			}
			break;
		case 'c':
			for (int i = 0; i < B.size(); ++i) {
				out << B[i].cnf();
				if (i != B.size() - 1) {
					out << '\n';
				}
			}
			break;
		case 'z':
			for (int i = 0; i < B.size(); ++i) {
				out << B[i].zhegalkin();
				if (i != B.size() - 1) {
					out << '\n';
				}
			}
			break;
		case 'i':
			if (isFullSystem(B)) {
				out <<'y'<<'e'<<'s';
			}
			else {
				out << 'n' << 'o';
			}
			break;
		}
		delete[] tp;
		tp= nullptr;
		}
		catch(...){
			out << "error";
			delete[] tp;
			tp= nullptr;
		}
				
	}
	else {
	cout << "HELP:\n" << "first, create file and write there system of boolean expressions\nthen enter in command line:\n";
}
	return 0;
}
