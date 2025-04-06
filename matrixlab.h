#ifndef MATRIXLAB_H
#define MATRIXLAB_H
#include "biginteger.h"
#include <vector>
class Fraction {
	BigInteger numerator; //числитель
	BigInteger denominator; //знаменатель
public:
	Fraction():numerator(1), denominator(1) {}
	Fraction(BigInteger& num, BigInteger& denum) : numerator(num), denominator(denum) {}
	Fraction(int num, int denum) {
		numerator = BigInteger(num);
		denominator = BigInteger(denum);
	}
	Fraction(BigInteger& a) {
		numerator = a;
		denominator = BigInteger(1);
	}
	Fraction(int a) {
		numerator = BigInteger(a);
		denominator = BigInteger(1);
	}
	Fraction(const Fraction& num) : numerator(num.numerator), denominator(num.denominator) { //копирующий конструктор
	};
	
	Fraction(Fraction&& other) noexcept {
		numerator = std::move(other.numerator);
		denominator = std::move(other.denominator);
	}
	Fraction& operator=(const Fraction& num) {
		numerator = num.numerator;
		denominator = num.denominator;
		return *this;
	}
	Fraction operator-() const {
		Fraction minus;
		minus.denominator = denominator;
		minus.numerator = -numerator;
		return minus;
	}
	Fraction operator+(const Fraction& oth) {
		Fraction temp;
		if (this->denominator == oth.denominator) {
			temp.numerator = this->numerator + oth.numerator;
			temp.denominator = this->denominator;
		}		
		else {
			//нок / число
			BigInteger div = scd(denominator, oth.denominator); //нод
			BigInteger mult = scm(denominator, oth.denominator, div); //нок
			BigInteger multA = multiple(oth.denominator,div); //множитель для числителя 
			BigInteger multB = multiple(denominator, div);//множитель для числителя num
			temp.numerator = this->numerator * multA + oth.numerator * multB;
			temp.denominator = mult;
		}
		temp.ReduceFraction();
		return temp;
	}
	Fraction operator-(const Fraction& oth) {
		Fraction temp;
		if (this->denominator == oth.denominator) {
			temp.numerator = this->numerator - oth.numerator;
			temp.denominator = this->denominator;
		}
		else {
			//нок / число
			BigInteger div = scd(denominator, oth.denominator); //нод
			BigInteger mult = scm(denominator, oth.denominator, div); //нок
			BigInteger multA = multiple(oth.denominator, div); //множитель для числителя 
			BigInteger multB = multiple(denominator, div);//множитель для числителя num
			temp.numerator = this->numerator * multA - oth.numerator * multB;
			temp.denominator = mult;
		}
		temp.ReduceFraction();
		return temp;

	}
	Fraction operator*(const Fraction& oth) {
		Fraction temp;
		temp.denominator = this->denominator * oth.denominator;
		temp.numerator = this->numerator * oth.numerator;
		if (!temp.numerator.what_sign() && !temp.denominator.what_sign() || temp.numerator.what_sign() && !temp.denominator.what_sign()) {
			temp.numerator = -temp.numerator;
			temp.denominator = -temp.denominator;
		}
		temp.ReduceFraction();
		return temp;
	}
	Fraction operator/(const Fraction& oth) {
		Fraction temp;
		temp.denominator = this->denominator * oth.numerator;
		temp.numerator = this->numerator * oth.denominator;
		if (!temp.numerator.what_sign() && !temp.denominator.what_sign() || temp.numerator.what_sign() && !temp.denominator.what_sign()) {
			temp.numerator = -temp.numerator;
			temp.denominator = -temp.denominator;
		}
		temp.ReduceFraction();
		return temp;
	}
	bool operator == (const Fraction& num) const {
		return numerator == num.numerator && denominator == num.denominator;
	}
	bool operator > (const Fraction& num) const {
		BigInteger div = scd(denominator, num.denominator); //нод
		BigInteger mult = scm(denominator, num.denominator, div); //нок
		BigInteger multA = multiple(num.denominator, div); //множитель для числителя 
		BigInteger multB = multiple(denominator, div);//множитель для числителя num
		return (multA * this->numerator) > (multB * num.numerator);
	}
	Fraction ReduceFraction() {
		if (denominator > 1) {
			BigInteger nod = scd(numerator, denominator);
			if (nod > 1) {
				numerator /= nod;
				denominator /= nod;
			}
		}
		return *this;
	}
	BigInteger what_num() const {
		return numerator;
	}
	BigInteger what_den() const {
		return denominator;
	}
	~Fraction(){}
};
std::ostream& operator<<(std::ostream& os, const Fraction& lv);
std::string to_string(const Fraction& f);

class Matrix {
	int rows_;
	int cols_;
	std::vector<std::vector<Fraction>> matrix_;
public:
	Matrix(){
		rows_ = 0;
		cols_ = 0;
	}
	Matrix(int row, int col) : rows_(row), cols_(col) {//матрица из нулей размером n строк и m столбцов
		matrix_.resize(rows_);
		for (int i = 0; i < rows_; ++i) {
			matrix_[i].resize(cols_);
		}
		for (int i = 0; i < rows_; ++i) {
			for (int j = 0; j < cols_; ++j) {
				matrix_[i][j] = Fraction(0);
			}
		}
	}
	Matrix(Fraction a) {
		rows_ = 1;
		cols_ = 1;
		matrix_.push_back(std::vector<Fraction> {std::move(a)});
	}
	void FillOne() { //заполнить матрицу единицами
		for (int i = 0; i < rows_; ++i) {
			for (int j = 0; j < cols_; ++j) {
				matrix_[i][j] = Fraction(1);
			}
		}
	}
	void add_row(std::vector<Fraction>& A) {
		
		matrix_.push_back(A);
		++rows_;
		cols_ = static_cast<int>(A.size());
	}
	void add_column(std::vector<Fraction>& A) {
		if (matrix_.size() < A.size()) {
			matrix_.resize(A.size());
		}

		++cols_;
		rows_ = static_cast<int>(A.size());
		for (int i = 0; i < A.size(); ++i) {
			matrix_[i].push_back(A[i]);
		}
	}
	void one_matrix() {
		//должна быть квадратной
		for (int i = 0; i < rows_; ++i) {
			for (int j = 0; j < cols_; ++j) {
				matrix_[i][j] = (i == j ? Fraction(1) : Fraction(0));
			}
		}
	}
	Fraction SumOfElements() {//вычисление суммы всех элементов матрицы
		Fraction sum(0);
		for (int i = 0; i < rows_; ++i) {
			for (int j = 0; j < cols_; ++j) {
				sum = sum + matrix_[i][j];
				sum.ReduceFraction();
			}
		}
		return sum;
	}
	Fraction ProdOfElements() {
		Fraction prod(1);
		for (int i = 0; i < rows_; ++i) {
			for (int j = 0; j < cols_; ++j) {
				prod = prod * matrix_[i][j];
				prod.ReduceFraction();//сократим
				if (prod == Fraction(0)) {
					return Fraction(0);
				}
			}
		}
		return prod;
	}

	Matrix operator+(const Matrix& A) {
		Matrix AdMatrix;
		AdMatrix.rows_ = A.rows_;
		AdMatrix.cols_ = A.cols_;
		AdMatrix.matrix_.resize(A.rows_);
		for (int i = 0; i < A.rows_; ++i) {
			AdMatrix.matrix_[i].resize(A.cols_);
		}
		Fraction a;
		Fraction b;
		for (int i = 0; i < AdMatrix.rows_; i++) {
			for (int j = 0; j < AdMatrix.cols_; j++) {
				a = A.matrix_[i][j];
				b = this->matrix_[i][j];
				AdMatrix.matrix_[i][j] = a + b;
			}
		}
		return AdMatrix;
	}
	Matrix operator-(const Matrix& A) {
		Matrix AdMatrix;
		AdMatrix.rows_ = A.rows_;
		AdMatrix.cols_ = A.cols_;
		AdMatrix.matrix_.resize(A.rows_);
		for (int i = 0; i < A.rows_; ++i) {
			AdMatrix.matrix_[i].resize(A.cols_);
		}
		Fraction a;
		Fraction b;
		for (int i = 0; i < AdMatrix.rows_; i++) {
			for (int j = 0; j < AdMatrix.cols_; j++) {
				a = this->matrix_[i][j];
				b = A.matrix_[i][j];
				AdMatrix.matrix_[i][j] = a - b;
			}
		}
		return AdMatrix;
	}
	Matrix operator*(const Matrix& A) {
		Matrix mult;
		mult.rows_ = this->rows_;
		mult.cols_ = A.cols_;

		mult.matrix_.resize(mult.rows_);
		for (int i = 0; i < mult.rows_; ++i) {
			mult.matrix_[i].resize(mult.cols_);
		}
		Fraction a;
		Fraction b;
		for (int i = 0; i < this->rows_; i++) {
			for (int j = 0; j < A.cols_; j++) {
				mult.matrix_[i][j] = Fraction(0);
				for (int k = 0; k < this->cols_; k++) {
					a = this->matrix_[i][k];
					b = A.matrix_[k][j];
					mult.matrix_[i][j] = mult.matrix_[i][j] +  a*b;
				}
			}
		}
		return mult;
	}
	Matrix operator/(const Matrix& A) {
		//A * B-1
		if (A.cols_ == 1 && A.rows_ == 1) {
			Matrix b;
			b.cols_ = 1;
			b.rows_ = 1;
			b.matrix_.push_back(std::vector<Fraction>{this->matrix_[0][0] / A.matrix_[0][0]});
			return b;
		}
		Matrix invA = Inv(A);
		Matrix res = *this * invA;
		return res;
	}
	Matrix operator-() const {
		Matrix minus;
		minus.cols_ = cols_;
		minus.rows_ = rows_;
		minus.matrix_.resize(minus.rows_);
		for (int i = 0; i < minus.rows_; ++i) {
			minus.matrix_[i].resize(minus.cols_);
		}
		for (int i = 0; i < rows_; ++i) {
			for (int j = 0; j < cols_; ++j) {
				minus.matrix_[i][j] = -matrix_[i][j];
			}
		}
		return minus;
	}
	int what_cols() const{
		return cols_;
	}
	int what_rows() const {
		return rows_;
	}
	std::vector<std::vector<Fraction>> what_matrix() const {
		return matrix_;
	}
	friend Matrix MultiplicationElByEl(const Matrix& A, const Matrix& B); //поэлементное умножение матриц A и B одинакового размера (получается матрица такого же размера как матрица A с элементами aij * bij)
	friend Matrix DivisionElByEl(const Matrix& A, const Matrix& B);//(./ A B) — поэлементное деление матриц A и B одинакового размера (получается матрица такого же размера как матрица A с элементами aij / bij),
	friend Matrix Minimal(const Matrix& A, const Matrix& B);
	friend Matrix Maximum(const Matrix& A, const Matrix& B);
	friend Matrix ConcatVert(const std::vector<Matrix>& m);
	friend Matrix ConcatHoriz(const std::vector<Matrix>& m);
	friend Fraction Det(const Matrix& m);
	friend Matrix Inv(const Matrix& m);
	friend Matrix Transp(const Matrix& m);
	friend Matrix Diag(const Matrix& m);
	void PrintMatrix() {
		for (int r = 0; r < rows_; ++r) {
			for (int c = 0; c < cols_; ++c) {
				cout << matrix_[r][c] << " ";
			}
			cout << endl;
		}
	}

	~Matrix(){}
};

std::ostream& operator<<(std::ostream& os, const Matrix& lv);
#endif
