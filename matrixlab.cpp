/*Matrix Lab
Необходимо разработать программное обеспечение, позволяющее производить вычисления с матрицами. 
Элементами матрицы могут быть рациональные числа, у которых числитель и знаменатель — длинные целые.
Для записи команд в разрабатываемом языке матричных вычислений используется кембриджская префиксная нотация (это польская нотация со скобками). В префиксной записи оператор располагается слева от операндов, 
но так как у операторов и функций может быть разное количество операндов, аргументов, то в записи используются круглые скобки, чтобы избежать неоднозначности при интерпретации.

Должны поддерживаться следующие операторы и функции:\
(= var value) — присваивание переменной var значения value,
(+ A B) — сложение A+B,
(- A B) — вычитание A-B,
(.* A B) — поэлементное умножение матриц A и B одинакового размера (получается матрица такого же размера как матрица A с элементами aij * bij),
(./ A B) — поэлементное деление матриц A и B одинакового размера (получается матрица такого же размера как матрица A с элементами aij / bij),
(* A B) — матричное умножение A * B,
(/ A B) — матричное деление A * B-1,
(- A) — унарный минус,
(zeros n m) — матрица из нулей размером n строк и m столбцов,
(ones n m) — матрица из единиц размером n строк и m столбцов,
(eye n) — единичная матрица n x n,
(linspace x0 x1 n) — создание вектора строки с n элементами от x0 до x1,
(vertcat A1 A2 ... An) — конкатенация матриц вертикально,
(horzcat A1 A2 ... An) — конкатенация матриц горизонтально,
(transpose A) — транспонирование матрицы,
(det A) — вычисление определителя матрицы,
(inv A) — вычисление обратной матрицы,
(sum A) — вычисление суммы всех элементов матрицы,
(prod A) — вычисление произведения всех элементов матрицы,
(min A B) — для матриц A и В равных размеров вычисление матрицы такого же размера, элементами которой будут минимальные из соответствующих элементов матриц A и В,
(max A B) — для матриц A и В равных размеров вычисление матрицы такого же размера, элементами которой будут максимальные из соответствующих элементов матриц A и В,
(disp A) — вывод значения A в выходной файл.*/

#include <fstream>
#include "matrixlab.h"
#include <algorithm>
#include <iomanip>
std::ostream& operator<<(std::ostream& os, const Fraction& lv) {
	if (lv.what_den() == BigInteger(1) || lv.what_num() == BigInteger(0)) {
		os << lv.what_num();
	}
	else {
		os << lv.what_num() << '/' << lv.what_den();
	}
	return os;
}
std::string to_string(const Fraction& f) {
	if (f.what_den() == BigInteger(1)) {
		return to_string(f.what_num());
	}
	if (f.what_num() == BigInteger(0)) {
		return "0";
	}
	return to_string(f.what_num()) + '/' + to_string(f.what_den());
}
std::ostream& operator<<(std::ostream& os, const Matrix& lv) {
	if (lv.what_cols() == 1 && lv.what_rows() == 1) {
		Fraction res = lv.what_matrix()[0][0].ReduceFraction();
		os << res;
	}
	else {
		std::vector<std::vector<std::string>> matr;
		int r = lv.what_rows();
		int c = lv.what_cols();
		matr.resize(r);
		for (int i = 0; i < r; ++i) {
			matr[i].resize(c);
		}
		os << "[\n";
		for (int i = 0; i < r; ++i) {
			for (int j = 0; j < c; ++j) {
				Fraction res = lv.what_matrix()[i][j];
				matr[i][j] = to_string(res);
			}
		}
		std::vector<int> mx; //максимальная длина числа в столбике
		mx.resize(c);
		for (int i = 0; i < c; ++i) {
			mx[i] = 0;
		}
		for (int j = 0; j < c; ++j) {
			for (int i = 0; i < r; ++i) {
				if (mx[j] < matr[i][j].size()) {
					mx[j] = static_cast<int>(matr[i][j].size());
				}
			}
		}
		for (int i = 0; i < r; ++i) {
			for (int j = 0; j < c; ++j) {
				os << std::setw(mx[j]) << std::right << matr[i][j];
				if (j != c - 1) {
					os << " ";
				}
			}
			os << "\n";
		}
		os << "]";
	}
	return os;
}

Matrix MultiplicationElByEl(const Matrix& A, const Matrix& B) {//поэлементное умножение матриц A и B одинакового размера (получается матрица такого же размера как матрица A с элементами aij * bij)
	//должен быть одинаковый размер
	Matrix multMatrix;
	multMatrix.cols_ = A.cols_;
	multMatrix.rows_ = A.rows_;
	multMatrix.matrix_.resize(A.rows_);
	for (int i = 0; i < A.rows_; ++i) {
		multMatrix.matrix_[i].resize(A.cols_);
	}

	Fraction a;
	Fraction b;
	for (int i = 0; i < multMatrix.rows_; i++) {
		for (int j = 0; j < multMatrix.cols_; j++) {
			a = A.matrix_[i][j];
			b = B.matrix_[i][j];
			multMatrix.matrix_[i][j] = a * b;
		}
	}
	return multMatrix;
}
Matrix DivisionElByEl(const Matrix& A, const Matrix& B){//(./ A B) — поэлементное деление матриц A и B одинакового размера (получается матрица такого же размера как матрица A с элементами aij / bij),
	Matrix multMatrix;
	multMatrix.cols_ = A.cols_;
	multMatrix.rows_ = A.rows_;
	multMatrix.matrix_.resize(A.rows_);
	for (int i = 0; i < A.rows_; ++i) {
		multMatrix.matrix_[i].resize(A.cols_);
	}

	Fraction a;
	Fraction b;
	for (int i = 0; i < multMatrix.rows_; i++) {
		for (int j = 0; j < multMatrix.cols_; j++) {
			a = A.matrix_[i][j];
			b = B.matrix_[i][j];
			multMatrix.matrix_[i][j] = a / b;
		}
	}
	return multMatrix;
}
Matrix Minimal(const Matrix& A, const Matrix& B) {
	//матрицы должны быть одинакового размера
	Matrix min;
	min.rows_ = A.rows_;
	min.cols_ = A.cols_;
	min.matrix_.resize(A.rows_);
	for (int i = 0; i < A.rows_; ++i) {
		min.matrix_[i].resize(A.cols_);
	}
	for (int i = 0; i < min.rows_; ++i) {
		for (int j = 0; j < min.cols_; ++j) {
			//sum = sum + matrix_[i][j];
			min.matrix_[i][j] = A.matrix_[i][j] > B.matrix_[i][j] ? B.matrix_[i][j] : A.matrix_[i][j];
		}
	}
	return min;
}
Matrix Maximum(const Matrix& A, const Matrix& B) {
	//матрицы должны быть одинакового размера
	Matrix min;
	min.rows_ = A.rows_;
	min.cols_ = A.cols_;
	min.matrix_.resize(A.rows_);
	for (int i = 0; i < A.rows_; ++i) {
		min.matrix_[i].resize(A.cols_);
	}
	for (int i = 0; i < min.rows_; ++i) {
		for (int j = 0; j < min.cols_; ++j) {
			//sum = sum + matrix_[i][j];
			min.matrix_[i][j] = A.matrix_[i][j] > B.matrix_[i][j] ? A.matrix_[i][j] : B.matrix_[i][j];
		}
	}
	return min;
}
Matrix ConcatVert(const std::vector<Matrix>& m) {
	//должны совпадать колич столбцов
	Matrix vert;
	vert.cols_ = m[0].cols_;
	for (int i = 0; i < m.size(); ++i) {
		vert.rows_ += m[i].rows_;
	}
	for (int j = 0; j < m.size(); ++j) {
		for (int i = 0; i < m[j].rows_; ++i) {
			vert.matrix_.push_back(m[j].matrix_[i]);
		}
	}
	return vert;
}
Matrix ConcatHoriz(const std::vector<Matrix>& m) {
	//должны совпадать колич строк
	Matrix hor;
	hor.rows_ = m[0].rows_;
	for (int i = 0; i < m.size(); ++i) {
		hor.cols_ += m[i].cols_;
	}
	hor.matrix_.resize(hor.rows_);
	for (int i = 0; i < hor.rows_; ++i) {
		hor.matrix_[i].resize(hor.cols_);
	}
	for (int j = 0; j < hor.rows_; ++j){
		for (int i = 0; i < m.size(); ++i) {
			for (int k = 0; k < m[0].cols_; ++k) {
				hor.matrix_[j][m[0].cols_ * i + k] = m[i].matrix_[j][k];
			}
		}
	}

	return hor;
}
Matrix Transp(const Matrix& m) {
	Matrix tr;
	tr.rows_ = m.cols_;
	tr.cols_ = m.rows_;
	tr.matrix_.resize(tr.rows_);
	for (int i = 0; i < tr.rows_; ++i) {
		tr.matrix_[i].resize(tr.cols_);
	}
	for (int i = 0; i < tr.rows_; ++i) {
		for (int j = 0; j < tr.cols_; ++j) {
			tr.matrix_[i][j] = m.matrix_[j][i];
		}
	}
	return tr;
}
Matrix Diag(const Matrix& m) {
	std::vector<Fraction> add;
	add.resize(m.rows_);
	for (int i = 0; i < m.rows_; ++i) {
		add[i] = (m.matrix_[i][i]);
	}
	Matrix res;
	res.add_row(add);
	return res;
}
Fraction Det(const Matrix& m) {
	if (m.rows_ == 2) {
		Fraction res(0);
		Fraction a= m.matrix_[0][0];
		Fraction b= m.matrix_[1][1];
		res = a * b;
		a = m.matrix_[0][1];
		b = m.matrix_[1][0];

		res = res - a * b;
		return res;
	}
	if (m.rows_ == 3) {
		Fraction res(0);
		Fraction a = m.matrix_[0][0];
		Fraction b = m.matrix_[1][1];
		Fraction c = m.matrix_[2][2];
		res = a * b * c;
		a = m.matrix_[0][1];
		b = m.matrix_[1][2];
		c = m.matrix_[2][0];
		res = res + a * b * c;
		a = m.matrix_[0][2];
		b = m.matrix_[1][0];
		c = m.matrix_[2][1];
		res = res + a * b * c;

		a = m.matrix_[0][2];
		b = m.matrix_[1][1];
		c = m.matrix_[2][0];
		res = res - a * b * c;
		a = m.matrix_[0][1];
		b = m.matrix_[1][0];
		c = m.matrix_[2][2];
		res = res - a * b * c;
		a = m.matrix_[0][0];
		b = m.matrix_[1][2];
		c = m.matrix_[2][1];
		res = res - a * b * c;
		return res;
	}
	int n = m.cols_;
	Fraction det(1);

	std::vector<std::vector<Fraction>> matrix = m.matrix_;

	for (int i = 0; i < n - 1; ++i) {
		for (int j = i + 1; j < n; ++j) {
			Fraction ratio = matrix[j][i] / matrix[i][i];
			ratio.ReduceFraction();
			for (int k = i; k < n; ++k) {
				matrix[j][k] = matrix[j][k] - ratio * matrix[i][k];
				matrix[j][k].ReduceFraction();
			}
		}
	}
	for (int i = 0; i < n; ++i) {
		det = det * matrix[i][i];
		det.ReduceFraction();
	}
	return det;
}
Matrix Inv(const Matrix& m) {
	//квадратная и определитель не равен 0
	Matrix alg;//м алг дополнений
	alg.rows_ = m.rows_;
	alg.cols_ = m.cols_;
	alg.matrix_.resize(alg.rows_);
	for (int i = 0; i < alg.rows_; ++i) {
		alg.matrix_[i].resize(alg.cols_);
	}
	Matrix temp;
	temp.rows_ = alg.rows_-1;
	temp.cols_ = alg.cols_-1;
	temp.matrix_.resize(temp.rows_);
	for (int i = 0; i < temp.rows_; ++i) {
		temp.matrix_[i].resize(temp.cols_);
	}
	int i_t = 0;
	int j_t = 0;
	for (int i = 0; i < alg.rows_; ++i) {
		for (int j = 0; j < alg.cols_; ++j) {
			for (int i1 = 0; i1 < alg.rows_; ++i1) {
				for (int j1 = 0; j1 < alg.cols_; ++j1) {
					if (i == i1 || j == j1) {
						continue;
					}
					temp.matrix_[i_t][j_t] = m.matrix_[i1][j1];
					++j_t;
				}
				if (i != i1) { ++i_t; }
				j_t = 0;
			}
			alg.matrix_[i][j] = (i + j) % 2 == 0 ? Det(temp) : -Det(temp);
			i_t = 0;
			j_t = 0;
		}
	}
	Matrix tr = Transp(alg);
	tr.PrintMatrix();
	Fraction d = Det(m);
	for (int i = 0; i < tr.rows_; ++i) {
		for (int j = 0; j < tr.cols_; ++j) {
			tr.matrix_[i][j] = tr.matrix_[i][j] / d;
		}
	}
	tr.PrintMatrix();
	return tr;
}


int main(int argc, char* argv[]) {
	std::vector<Matrix> glob;
	std::vector<Matrix> loc;
	std::vector<string> glob_name;
	std::vector<string> loc_name; //начинаются с _
	std::ifstream inp(argv[1]);
	std::ofstream out(argv[2]);
	char ch;
	std::string cur;// = "";//текущая строчка

	do {
		ch = static_cast<char>(inp.get());

		if (ch == EOF || ch == '\n') {//в таком случае очищаем вектор с локальными переменными
			// Поиск индекса символа, который нужно удалить
			while (!cur.empty()) {
				size_t index = cur.find(')'); //находим закрывающую скобку
				//пойдем назад до открывающей
				while (cur[index] != '(') {
					--index;
				}
				cur.erase(index, 1);
				//удаляем символы между скобками и выполняем действия которые в них
				std::string command;
				while (cur[index] != ' ') { //смотрим какая комманда
					command += cur[index];
					cur.erase(index, 1);
				}

				cur.erase(index, 1);//удаляем пробел после комманды
				if (command == "=") {
					std::string name;
					while (cur[index] != ' ') {//читаем имя
						name += cur[index];
						cur.erase(index, 1);
					}
					cur.erase(index, 1); //удаляем пробел после имени
					glob_name.push_back(name);
					std::string value;
					while (cur[index] != ')') {//читаем value
						value += cur[index];
						cur.erase(index, 1);
					}
					if (value[0] == '_') {//значит справа локальная переменная
						auto it = std::find(loc_name.begin(), loc_name.end(), value);
						glob.push_back(loc[std::distance(loc_name.begin(), it)]);//добавляем локальную переменную в массив к глобальным
					}
					else {
						glob.push_back(Matrix(Fraction(stoi(value))));
					}
				}
				else if (command=="disp") {
					std::string name;
					while (cur[index] != ')') {//читаем имя
						name += cur[index];
						cur.erase(index, 1);
					}
					if (name[0] != '_') {
						auto it = std::find(glob_name.begin(), glob_name.end(), name);
						out << name << " = " << (glob[std::distance(glob_name.begin(), it)]);
					}
					else {
						auto it = std::find(loc_name.begin(), loc_name.end(), name);
						out << "ans = " << (loc[std::distance(loc_name.begin(), it)]);
					}
					out<<'\n';
				}
				else if (command=="+") {
					std::string value1;
					while (cur[index] != ' ') {//читаем имя 1
						value1 += cur[index];
						cur.erase(index, 1);
					}
					cur.erase(index, 1); //удаляем пробел после имени
					std::string value2;
					while (cur[index] != ')') {//читаем value
						value2 += cur[index];
						cur.erase(index, 1);
					}
					std::vector<std::string>::iterator it1;
					std::vector<std::string>::iterator it2;

					if (value1[0] != '_') {
						it1 = std::find(glob_name.begin(), glob_name.end(), value1);
					}
					else {
						it1 = std::find(loc_name.begin(), loc_name.end(), value1);
					}
					if (value2[0] != '_') {
						it2 = std::find(glob_name.begin(), glob_name.end(), value2);
					}
					else {
						it2 = std::find(loc_name.begin(), loc_name.end(), value2);
					}
					Matrix a1;
					Matrix a2;
					if (value1[0] != '_' && it1 == glob_name.end()  || value1[0] == '_' && it1 == loc_name.end()) {//если числа 
						a1 = Fraction(stoi(value1));
					}
					else if(value1[0] != '_' && it1 != glob_name.end()) {
						a1 = glob[std::distance(glob_name.begin(), it1)];
					}
					else {
						a1 = loc[std::distance(loc_name.begin(), it1)];
					}
					if (value2[0] != '_' && it2 == glob_name.end() || value2[0] == '_' &&  it2 == loc_name.end()) {//если числа 
						a2 = Fraction(stoi(value2));
					}
					else if (value2[0] != '_' && it2 != glob_name.end()) {
						a2 = glob[std::distance(glob_name.begin(), it2)];
					}
					else {
						a2 = loc[std::distance(loc_name.begin(), it2)];
					}
					loc.push_back(a1 + a2);
					loc_name.push_back('_' + std::to_string(loc.size()));
					cur.insert(index, "_" + std::to_string(loc.size()));
					index = index + 1 + std::to_string(loc.size()).size();
				}
				else if (command =="-") {
					std::string value1;
					while (cur[index] != ' ' && cur[index] != ')') {//читаем имя 1
						value1 += cur[index];
						cur.erase(index, 1);
					}
					if (cur[index] != ')') {
						cur.erase(index, 1); //удаляем пробел после имени
						std::string value2;
						while (cur[index] != ')') {//читаем value
							value2 += cur[index];
							cur.erase(index, 1);
						}
						std::vector<std::string>::iterator it1;
						std::vector<std::string>::iterator it2;

						if (value1[0] != '_') {
							it1 = std::find(glob_name.begin(), glob_name.end(), value1);
						}
						else {
							it1 = std::find(loc_name.begin(), loc_name.end(), value1);
						}
						if (value2[0] != '_') {
							it2 = std::find(glob_name.begin(), glob_name.end(), value2);
						}
						else {
							it2 = std::find(loc_name.begin(), loc_name.end(), value2);
						}
						Matrix a1;
						Matrix a2;
						if (value1[0] != '_' && it1 == glob_name.end() || value1[0] == '_' && it1 == loc_name.end()) {//если числа 
							a1 = Fraction(stoi(value1));
						}
						else if (value1[0] != '_' && it1 != glob_name.end()) {
							a1 = glob[std::distance(glob_name.begin(), it1)];
						}
						else {
							a1 = loc[std::distance(loc_name.begin(), it1)];
						}
						if (value2[0] != '_' && it2 == glob_name.end() || value2[0] == '_' && it2 == loc_name.end()) {//если числа 
							a2 = Fraction(stoi(value2));
						}
						else if (value2[0] != '_' && it2 != glob_name.end()) {
							a2 = glob[std::distance(glob_name.begin(), it2)];
						}
						else {
							a2 = loc[std::distance(loc_name.begin(), it2)];
						}
						loc.push_back(a1 - a2);
						loc_name.push_back('_' + std::to_string(loc.size()));
						cur.insert(index, "_" + std::to_string(loc.size()));
						index = index + 1 + std::to_string(loc.size()).size();
					}
					else {
						//значит унарный минус
						std::vector<std::string>::iterator it1;
						Matrix add;
						if (value1[0] != '_') {
							it1 = std::find(glob_name.begin(), glob_name.end(), value1);
						}
						else {
							it1 = std::find(loc_name.begin(), loc_name.end(), value1);
						}
						if (value1[0] != '_' && it1 != glob_name.end()) {
							add = (glob[std::distance(glob_name.begin(), it1)]);
						}
						else {
							add = (loc[std::distance(loc_name.begin(), it1)]);
						}
						loc.push_back(-add);
						loc_name.push_back('_' + std::to_string(loc.size()));
						cur.insert(index, "_" + std::to_string(loc.size()));
						index = index + 1 + std::to_string(loc.size()).size();
					}
				}
				else if (command =="*") {
					std::string value1;
					while (cur[index] != ' ') {//читаем имя 1
						value1 += cur[index];
						cur.erase(index, 1);
					}
					cur.erase(index, 1); //удаляем пробел после имени
					std::string value2;
					while (cur[index] != ')') {//читаем value
						value2 += cur[index];
						cur.erase(index, 1);
					}
					std::vector<std::string>::iterator it1;
					std::vector<std::string>::iterator it2;
					if (value1[0] != '_') {
						it1 = std::find(glob_name.begin(), glob_name.end(), value1);
					}
					else {
						it1 = std::find(loc_name.begin(), loc_name.end(), value1);
					}
					if (value2[0] != '_') {
						it2 = std::find(glob_name.begin(), glob_name.end(), value2);
					}
					else {
						it2 = std::find(loc_name.begin(), loc_name.end(), value2);
					}
					Matrix a1;
					Matrix a2;
					if (value1[0] != '_' && it1 == glob_name.end() || value1[0] == '_' && it1 == loc_name.end()) {//если числа 
						a1 = Fraction(stoi(value1));
					}
					else if (value1[0] != '_' && it1 != glob_name.end()) {
						a1 = glob[std::distance(glob_name.begin(), it1)];
					}
					else {
						a1 = loc[std::distance(loc_name.begin(), it1)];
					}
					
					if ((value2[0] != '_' && it2 == glob_name.end()) || value2[0] == '_' && it2 == loc_name.end()) {//если числа 
						a2 = Fraction(stoi(value2));
					}
					else if (value2[0] != '_' && it2 != glob_name.end()) {
						a2 = glob[std::distance(glob_name.begin(), it2)];
					}
					else {
						a2 = loc[std::distance(loc_name.begin(), it2)];
					}
					loc.push_back(a1 * a2);
					loc_name.push_back('_' + std::to_string(loc.size()));
					cur.insert(index, "_" + std::to_string(loc.size()));
					index = index + 1 + std::to_string(loc.size()).size();
				}
				else if (command =="/") {
					std::string value1;
					while (cur[index] != ' ') {//читаем имя 1
						value1 += cur[index];
						cur.erase(index, 1);
					}
					cur.erase(index, 1); //удаляем пробел после имени
					std::string value2;
					while (cur[index] != ')') {//читаем value
						value2 += cur[index];
						cur.erase(index, 1);
					}
					std::vector<std::string>::iterator it1;
					std::vector<std::string>::iterator it2;
					if (value1[0] != '_') {
						it1 = std::find(glob_name.begin(), glob_name.end(), value1);
					}
					else {
						it1 = std::find(loc_name.begin(), loc_name.end(), value1);
					}
					if (value2[0] != '_') {
						it2 = std::find(glob_name.begin(), glob_name.end(), value2);
					}
					else {
						it2 = std::find(loc_name.begin(), loc_name.end(), value2);
					}
					Matrix a1;
					Matrix a2;
					if (value1[0] != '_' && it1 == glob_name.end() || value1[0] == '_' && it1 == loc_name.end()) {//если числа 
						a1 = Fraction(stoi(value1));
					}
					else if (value1[0] != '_' && it1 != glob_name.end()) {
						a1 = glob[std::distance(glob_name.begin(), it1)];
					}
					else {
						a1 = loc[std::distance(loc_name.begin(), it1)];
					}
					if (value2[0] != '_' && it2 == glob_name.end() || value2[0] == '_' && it2 == loc_name.end()) {//если числа 
						a2 = Fraction(stoi(value2));
					}
					else if (value2[0] != '_' && it2 != glob_name.end()) {
						a2 = glob[std::distance(glob_name.begin(), it2)];
					}
					else {
						a2 = loc[std::distance(loc_name.begin(), it2)];
					}
					loc.push_back(a1 / a2);
					loc_name.push_back('_' + std::to_string(loc.size()));
					cur.insert(index, "_" + std::to_string(loc.size()));
					index = index + 1 + std::to_string(loc.size()).size();
				}
				else if (command =="horzcat") {
					std::vector<Matrix> add;
					std::string value;
					while (cur[index] != ')') {//читаем value
						while (cur[index] != ' ' && cur[index] != ')') {
							value += cur[index];
							cur.erase(index, 1);
						}
						std::vector<std::string>::iterator it1;
						if (value[0] != '_') {
							it1 = std::find(glob_name.begin(), glob_name.end(), value);
						}
						else {
							it1 = std::find(loc_name.begin(), loc_name.end(), value);
						}
						if (value[0] != '_' && it1 == glob_name.end() || value[0] == '_' && it1 == loc_name.end()) {//если числа 
							add.push_back(Fraction(stoi(value)));
						}
						else if (value[0] != '_' && it1 != glob_name.end()) {
							add.push_back(glob[std::distance(glob_name.begin(), it1)]);
						}
						else {
							add.push_back(loc[std::distance(loc_name.begin(), it1)]);
						}
						if (cur[index] != ')') { cur.erase(index, 1); }
						value.clear();
					}
					loc.push_back(ConcatHoriz(add));
					loc_name.push_back('_' + std::to_string(loc.size()));
					cur.insert(index, "_" + std::to_string(loc.size()));
					index = index + 1 + std::to_string(loc.size()).size();

				}
				else if (command =="vertcat") {
					std::vector<Matrix> add;
					std::string value;
					while (cur[index] != ')') {//читаем value
						while (cur[index] != ' ' && cur[index] != ')') {
							value += cur[index];
							cur.erase(index, 1);
						}
						std::vector<std::string>::iterator it1;
						if (value[0] != '_') {
							it1 = std::find(glob_name.begin(), glob_name.end(), value);
						}
						else {
							it1 = std::find(loc_name.begin(), loc_name.end(), value);
						}
						if (value[0] != '_' && it1 == glob_name.end() || value[0] == '_' && it1 == loc_name.end()) {//если числа 
							add.push_back(Fraction(stoi(value)));
						}
						else if (value[0] != '_' && it1 != glob_name.end()) {
							add.push_back(glob[std::distance(glob_name.begin(), it1)]);
						}
						else {
							add.push_back(loc[std::distance(loc_name.begin(), it1)]);
						}
						if (cur[index] != ')') { cur.erase(index, 1); }
						value.clear();
					}
					loc.push_back(ConcatVert(add));
					loc_name.push_back('_' + std::to_string(loc.size()));
					cur.insert(index, "_" + std::to_string(loc.size()));
					index = index + 1 + std::to_string(loc.size()).size();

				}
				else if (command == "transpose") {
					Matrix add;
					std::string value;
					while (cur[index] != ')') {//читаем value
						while (cur[index] != ' ' && cur[index] != ')') {
							value += cur[index];
							cur.erase(index, 1);
						}
						std::vector<std::string>::iterator it1;
						if (value[0] != '_') {
							it1 = std::find(glob_name.begin(), glob_name.end(), value);
						}
						else {
							it1 = std::find(loc_name.begin(), loc_name.end(), value);
						}
						if (value[0] != '_' && it1 != glob_name.end()) {
							add = (glob[std::distance(glob_name.begin(), it1)]);
						}
						else {
							add = (loc[std::distance(loc_name.begin(), it1)]);
						}
					}
					loc.push_back(Transp(add));
					loc_name.push_back('_' + std::to_string(loc.size()));
					cur.insert(index, "_" + std::to_string(loc.size()));
					index = index + 1 + std::to_string(loc.size()).size();
				}
				else if (command =="inv") {
					Matrix add;
					std::string value;
					while (cur[index] != ')') {
						value += cur[index];
						cur.erase(index, 1);
					}
					std::vector<std::string>::iterator it1;
					if (value[0] != '_') {
						it1 = std::find(glob_name.begin(), glob_name.end(), value);
					}
					else {
						it1 = std::find(loc_name.begin(), loc_name.end(), value);
					}
					if (value[0] != '_' && it1 != glob_name.end()) {
						add = (glob[std::distance(glob_name.begin(), it1)]);
					}
					else {
						add = (loc[std::distance(loc_name.begin(), it1)]);
					}
					loc.push_back(Inv(add));
					loc_name.push_back('_' + std::to_string(loc.size()));
					cur.insert(index, "_" + std::to_string(loc.size()));
					index = index + 1 + std::to_string(loc.size()).size();
				}
				else if (command=="linspace") {
					//(linspace x0 x1 n) — создание вектора строки с n элементами от x0 до x1,
					std::string value1;
					while (cur[index] != ' ') {//читаем имя 1
						value1 += cur[index];
						cur.erase(index, 1);
					}
					cur.erase(index, 1); //удаляем пробел после имени
					std::string value2;
					while (cur[index] != ' ') {//читаем value
						value2 += cur[index];
						cur.erase(index, 1);
					}
					std::string n;
					cur.erase(index, 1); //удаляем пробел после имени
					while (cur[index] != ')') {//читаем value
						n += cur[index];
						cur.erase(index, 1);
					}
					std::vector<std::string>::iterator it1;
					std::vector<std::string>::iterator it2;
					std::vector<std::string>::iterator it3;
					if (value1[0] != '_') {
						it1 = std::find(glob_name.begin(), glob_name.end(), value1);
					}
					else {
						it1 = std::find(loc_name.begin(), loc_name.end(), value1);
					}
					if (value2[0] != '_') {
						it2 = std::find(glob_name.begin(), glob_name.end(), value2);
					}
					else {
						it2 = std::find(loc_name.begin(), loc_name.end(), value2);
					}
					if (n[0] != '_') {
						it3 = std::find(glob_name.begin(), glob_name.end(), n);
					}
					else {
						it3 = std::find(loc_name.begin(), loc_name.end(), n);
					}
					Fraction a1;
					Fraction a2;
					Fraction a3;
					if (value1[0] != '_' && it1 == glob_name.end() || value1[0] == '_' && it1 == loc_name.end()) {//если числа 
						a1 = stoi(value1);
					}
					else if (value1[0] != '_' && it1 != glob_name.end()) {
						a1 = (glob[std::distance(glob_name.begin(), it1)].what_matrix()[0][0]);
					}
					else {
						a1 = (loc[std::distance(loc_name.begin(), it1)].what_matrix()[0][0]);
					}

					if ((value2[0] != '_' && it2 == glob_name.end()) || value2[0] == '_' && it2 == loc_name.end()) {//если числа 
						a2 = (stoi(value2));
					}
					else if (value2[0] != '_' && it2 != glob_name.end()) {
						a2 =(glob[std::distance(glob_name.begin(), it2)].what_matrix()[0][0]);
					}
					else {
						a2 =(loc[std::distance(loc_name.begin(), it2)].what_matrix()[0][0]);
					}
					if ((n[0] != '_' && it2 == glob_name.end()) || n[0] == '_' && it2 == loc_name.end()) {//если числа 
						a3 = (stoi(n));
					}
					else if (value2[0] != '_' && it2 != glob_name.end()) {
						a3 = (glob[std::distance(glob_name.begin(), it3)].what_matrix()[0][0]);
					}
					else {
						a3 = (loc[std::distance(loc_name.begin(), it3)].what_matrix()[0][0]);
					}
					Fraction start(a1);
					Fraction end(a2);
					Fraction dif;
					dif = end - start + Fraction(1);
					Fraction kol(a3);
					Fraction step(dif / kol);
					step.ReduceFraction();
					std::vector<Matrix> lin;
					for (int i = 0; i < static_cast<int>(a3.what_num().what_num()[0]); ++i) {
						lin.push_back(start);
						start = start + step;
					}
					loc.push_back(ConcatHoriz(lin));
					loc_name.push_back('_' + std::to_string(loc.size()));
					cur.insert(index, "_" + std::to_string(loc.size()));
					index = index + 1 + std::to_string(loc.size()).size();
				}
				else if (command =="ones") {
					//(ones n m) — матрица из единиц размером n строк и m столбцов,
					std::string value1;
					while (cur[index] != ' ') {//читаем имя 1
						value1 += cur[index];
						cur.erase(index, 1);
					}
					cur.erase(index, 1); //удаляем пробел после имени
					std::string value2;
					while (cur[index] != ')') {//читаем value
						value2 += cur[index];
						cur.erase(index, 1);
					}
					std::vector<std::string>::iterator it1;
					std::vector<std::string>::iterator it2;
					if (value1[0] != '_') {
						it1 = std::find(glob_name.begin(), glob_name.end(), value1);
					}
					else {
						it1 = std::find(loc_name.begin(), loc_name.end(), value1);
					}
					if (value2[0] != '_') {
						it2 = std::find(glob_name.begin(), glob_name.end(), value2);
					}
					else {
						it2 = std::find(loc_name.begin(), loc_name.end(), value2);
					}
					int a1 = 0;
					int a2 = 0;
					if (value1[0] != '_' && it1 == glob_name.end() || value1[0] == '_' && it1 == loc_name.end()) {//если числа 
						a1 = stoi(value1);
					}
					else if (value1[0] != '_' && it1 != glob_name.end()) {
						a1 = static_cast<int>( glob[std::distance(glob_name.begin(), it1)].what_matrix()[0][0].what_num().what_num()[0]);
					}
					else {
						a1 = static_cast<int>(loc[std::distance(loc_name.begin(), it1)].what_matrix()[0][0].what_num().what_num()[0]);
					}

					if ((value2[0] != '_' && it2 == glob_name.end()) || value2[0] == '_' && it2 == loc_name.end()) {//если числа 
						a2 = (stoi(value2));
					}
					else if (value2[0] != '_' && it2 != glob_name.end()) {
						a2 = static_cast<int>(glob[std::distance(glob_name.begin(), it2)].what_matrix()[0][0].what_num().what_num()[0]);
					}
					else {
						a2 = static_cast<int>(loc[std::distance(loc_name.begin(), it2)].what_matrix()[0][0].what_num().what_num()[0]);
					}
					Matrix add(a1, a2);
					add.FillOne();
					loc.push_back(add);
					loc_name.push_back('_' + std::to_string(loc.size()));
					cur.insert(index, "_" + std::to_string(loc.size()));
					index = index + 1 + std::to_string(loc.size()).size();
				}
				else if (command =="sum") {
					//(sum A) — вычисление суммы всех элементов матрицы,
					std::string value;
					while (cur[index] != ')') {//читаем имя 1
						value += cur[index];
						cur.erase(index, 1);
					}
					std::vector<std::string>::iterator it1;
					if (value[0] != '_') {
						it1 = std::find(glob_name.begin(), glob_name.end(), value);
					}
					else {
						it1 = std::find(loc_name.begin(), loc_name.end(), value);
					}
					Fraction sum;
					Matrix m;
					if (value[0] != '_' && it1 != glob_name.end()) {
						m = glob[std::distance(glob_name.begin(), it1)];
					}
					else {
						m = loc[std::distance(loc_name.begin(), it1)];
					}
					loc.push_back(m.SumOfElements());
					loc_name.push_back('_' + std::to_string(loc.size()));
					cur.insert(index, "_" + std::to_string(loc.size()));
					index = index + 1 + std::to_string(loc.size()).size();
				}
				else if (command =="prod") {
					//(prod A) — вычисление произведения всех элементов матрицы,
					std::string value;
					while (cur[index] != ')') {//читаем имя 1
						value += cur[index];
						cur.erase(index, 1);
					}
					std::vector<std::string>::iterator it1;
					if (value[0] != '_') {
						it1 = std::find(glob_name.begin(), glob_name.end(), value);
					}
					else {
						it1 = std::find(loc_name.begin(), loc_name.end(), value);
					}
					Matrix m;
					if (value[0] != '_' && it1 != glob_name.end()) {
						m = glob[std::distance(glob_name.begin(), it1)];
					}
					else {
						m = loc[std::distance(loc_name.begin(), it1)];
					}
					loc.push_back(m.ProdOfElements());
					loc_name.push_back('_' + std::to_string(loc.size()));
					cur.insert(index, "_" + std::to_string(loc.size()));
					index = index + 1 + std::to_string(loc.size()).size();
				}
				else if (command == "det") {
					//(det A) — вычисление определителя матрицы,
					std::string value;
					while (cur[index] != ')') {//читаем имя 1
						value += cur[index];
						cur.erase(index, 1);
					}
					std::vector<std::string>::iterator it1;
					if (value[0] != '_') {
						it1 = std::find(glob_name.begin(), glob_name.end(), value);
					}
					else {
						it1 = std::find(loc_name.begin(), loc_name.end(), value);
					}
					Matrix m;
					if (value[0] != '_' && it1 != glob_name.end()) {
						m = glob[std::distance(glob_name.begin(), it1)];
					}
					else {
						m = loc[std::distance(loc_name.begin(), it1)];
					}
					loc.push_back(Det(m));
					loc_name.push_back('_' + std::to_string(loc.size()));
					cur.insert(index, "_" + std::to_string(loc.size()));
					index = index + 1 + std::to_string(loc.size()).size();
				}
				else if (command == "./") {
					//(./ A B) — поэлементное деление матриц A и B одинакового размера (получается матрица такого же размера как матрица A с элементами aij / bij),
					std::string value1;// = "";
					while (cur[index] != ' ') {//читаем имя 1
						value1 += cur[index];
						cur.erase(index, 1);
					}
					cur.erase(index, 1); //удаляем пробел после имени
					std::string value2;
					while (cur[index] != ')') {//читаем value
						value2 += cur[index];
						cur.erase(index, 1);
					}
					std::vector<std::string>::iterator it1;
					std::vector<std::string>::iterator it2;
					if (value1[0] != '_') {
						it1 = std::find(glob_name.begin(), glob_name.end(), value1);
					}
					else {
						it1 = std::find(loc_name.begin(), loc_name.end(), value1);
					}
					if (value2[0] != '_') {
						it2 = std::find(glob_name.begin(), glob_name.end(), value2);
					}
					else {
						it2 = std::find(loc_name.begin(), loc_name.end(), value2);
					}
					Matrix a1;
					Matrix a2;
					if (value1[0] != '_' && it1 != glob_name.end()) {
						a1 = glob[std::distance(glob_name.begin(), it1)];
					}
					else {
						a1 = loc[std::distance(loc_name.begin(), it1)];
					}
					if (value2[0] != '_' && it2 != glob_name.end()) {
						a2 = glob[std::distance(glob_name.begin(), it2)];
					}
					else {
						a2 = loc[std::distance(loc_name.begin(), it2)];
					}
					Matrix d;
					d = DivisionElByEl(a1, a2);
					loc.push_back(DivisionElByEl(a1, a2));
					loc_name.push_back('_' + std::to_string(loc.size()));
					cur.insert(index, "_" + std::to_string(loc.size()));
					index = index + 1 + std::to_string(loc.size()).size();
				}
				else if (command == "min") {
					std::string value1;
					while (cur[index] != ' ') {//читаем имя 1
						value1 += cur[index];
						cur.erase(index, 1);
					}
					cur.erase(index, 1); //удаляем пробел после имени
					std::string value2;
					while (cur[index] != ')') {//читаем value
						value2 += cur[index];
						cur.erase(index, 1);
					}
					std::vector<std::string>::iterator it1;
					std::vector<std::string>::iterator it2;
					if (value1[0] != '_') {
						it1 = std::find(glob_name.begin(), glob_name.end(), value1);
					}
					else {
						it1 = std::find(loc_name.begin(), loc_name.end(), value1);
					}
					if (value2[0] != '_') {
						it2 = std::find(glob_name.begin(), glob_name.end(), value2);
					}
					else {
						it2 = std::find(loc_name.begin(), loc_name.end(), value2);
					}
					Matrix a1;
					Matrix a2;
					if (value1[0] != '_' && it1 == glob_name.end() || value1[0] == '_' && it1 == loc_name.end()) {//если числа 
						a1 = Fraction(stoi(value1));
					}
					else if (value1[0] != '_' && it1 != glob_name.end()) {
						a1 = glob[std::distance(glob_name.begin(), it1)];
					}
					else {
						a1 = loc[std::distance(loc_name.begin(), it1)];
					}

					if ((value2[0] != '_' && it2 == glob_name.end()) || value2[0] == '_' && it2 == loc_name.end()) {//если числа 
						a2 = Fraction(stoi(value2));
					}
					else if (value2[0] != '_' && it2 != glob_name.end()) {
						a2 = glob[std::distance(glob_name.begin(), it2)];
					}
					else {
						a2 = loc[std::distance(loc_name.begin(), it2)];
					}
					loc.push_back(Minimal(a1,a2));
					loc_name.push_back('_' + std::to_string(loc.size()));
					cur.insert(index, "_" + std::to_string(loc.size()));
					index = index + 1 + std::to_string(loc.size()).size();
				}
				else if (command =="max") {
					std::string value1;// = "";
					while (cur[index] != ' ') {//читаем имя 1
						value1 += cur[index];
						cur.erase(index, 1);
					}
					cur.erase(index, 1); //удаляем пробел после имени
					std::string value2;// = "";
					while (cur[index] != ')') {//читаем value
						value2 += cur[index];
						cur.erase(index, 1);
					}
					std::vector<std::string>::iterator it1;
					std::vector<std::string>::iterator it2;
					if (value1[0] != '_') {
						it1 = std::find(glob_name.begin(), glob_name.end(), value1);
					}
					else {
						it1 = std::find(loc_name.begin(), loc_name.end(), value1);
					}
					if (value2[0] != '_') {
						it2 = std::find(glob_name.begin(), glob_name.end(), value2);
					}
					else {
						it2 = std::find(loc_name.begin(), loc_name.end(), value2);
					}
					Matrix a1;
					Matrix a2;
					if (value1[0] != '_' && it1 == glob_name.end() || value1[0] == '_' && it1 == loc_name.end()) {//если числа 
						a1 = Fraction(stoi(value1));
					}
					else if (value1[0] != '_' && it1 != glob_name.end()) {
						a1 = glob[std::distance(glob_name.begin(), it1)];
					}
					else {
						a1 = loc[std::distance(loc_name.begin(), it1)];
					}

					if ((value2[0] != '_' && it2 == glob_name.end()) || value2[0] == '_' && it2 == loc_name.end()) {//если числа 
						a2 = Fraction(stoi(value2));
					}
					else if (value2[0] != '_' && it2 != glob_name.end()) {
						a2 = glob[std::distance(glob_name.begin(), it2)];
					}
					else {
						a2 = loc[std::distance(loc_name.begin(), it2)];
					}
					loc.push_back(Maximum(a1, a2));
					loc_name.push_back('_' + std::to_string(loc.size()));
					cur.insert(index, "_" + std::to_string(loc.size()));
					index = index + 1 + std::to_string(loc.size()).size();
				}
				else if (command == "diag") {
					Matrix add;
					std::string value;
					while (cur[index] != ')') {//читаем value
						while (cur[index] != ' ' && cur[index] != ')') {
							value += cur[index];
							cur.erase(index, 1);
						}
						std::vector<std::string>::iterator it1;
						if (value[0] != '_') {
							it1 = std::find(glob_name.begin(), glob_name.end(), value);
						}
						else {
							it1 = std::find(loc_name.begin(), loc_name.end(), value);
						}
						if (value[0] != '_' && it1 != glob_name.end()) {
							add = (glob[std::distance(glob_name.begin(), it1)]);
						}
						else {
							add = (loc[std::distance(loc_name.begin(), it1)]);
						}
					}
					loc.push_back(Diag(add));
					loc_name.push_back('_' + std::to_string(loc.size()));
					cur.insert(index, "_" + std::to_string(loc.size()));
					index = index + 1 + std::to_string(loc.size()).size();
				}

				cur.erase(index, 1); //стираем )
			}
			loc.clear();
			loc_name.clear();
		}
		else {
			cur += ch;
		}

	} while (!inp.eof()); 

	return 0;
}
