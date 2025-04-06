#ifndef BIGINTEGER_H
#define BIGINTEGER_H
#include <iostream>
#include <string>
#include <limits>
#include <cmath>
using std::endl;
using std::cin;
using std::cout;
using std::string;
const unsigned int int_max = std::numeric_limits<unsigned int>::max(); //система счисления
const string str_max = std::to_string(int_max); //макс число в сист счисления
const unsigned long check_how_many_bytes_UL_occupies = 0;
const int bits_UL_occupies = sizeof(check_how_many_bytes_UL_occupies) * 8;


int* ConvertToBinary(const string& str, int& idx_bin, int& max_bin);

unsigned int* ConvertToUlArray(const string& str, unsigned int* number, int& num_size);

class BigInteger {
private:
    bool sign_; //знаковость числа 
    unsigned int* number_; //число, храним с конца
    int size_; // длина массива
public:
    BigInteger() : sign_(true), size_(1), number_(new unsigned int[1] {0}) {}

    BigInteger(bool sign, int size, const unsigned int* number) :sign_(sign), size_(size) { //конструктор по умолчаниию 
        number_ = new unsigned int[size_];
        if (number == nullptr) {
            number_[0] = 0;
        }
        else if (size == 1 && number[0] == 0) {
            sign_ = true;
        }
        else {
            for (int i = 0; i < size; ++i) {
                if (size == 1 && number[0] == 0) {
                    sign_ = true;;
                }
                number_[i] = number[i];
            }
        }
    }
    BigInteger(const unsigned int& n) { //конструктор от инта
        sign_ = true;
        size_ = 1;
        number_ = new unsigned int[size_];
        number_[0] = n;
    }
    BigInteger(const long long& n) { //конструктор от ll
        n < 0 ? sign_ = false : sign_ = true;
        if (std::abs(n) <= int_max) {
            size_ = 1;
            number_ = new unsigned int[size_];
            number_[0] = std::abs(n);
        }
        else {
            size_ = 0;
            number_ = nullptr; 
            number_ = ConvertToUlArray(std::to_string(std::abs(n)), number_, size_);
        }
    }
    BigInteger(const unsigned long long& n) { 
        sign_ = true;
        size_ = 1;
        number_ = new unsigned int[size_];
        number_[0] = n;
    }
    BigInteger(const int& n) { 
        n < 0 ? sign_ = false : sign_ = true;
        size_ = 1;
        number_ = new unsigned int[size_];
        number_[0] = std::abs(n);
    }
    BigInteger(const long int& n) {
        n < 0 ? sign_ = false : sign_ = true;
        size_ = 1;
        number_ = new unsigned int[size_];
        number_[0] = std::abs(n);
    }
    BigInteger(const unsigned long int& n) { 
        n < 0 ? sign_ = false : sign_ = true;
        size_ = 1;
        number_ = new unsigned int[size_];
        number_[0] = n;
    }

    BigInteger(const string& str) {
        if (str.length() == 1 && str[0] == '-' || str.length() == 0) {
            throw std::invalid_argument("");
        }
        for (int i = 0; i < str.length(); ++i) {
            if (i == 0 && str[i] == '-') {
                continue;
            }
            if (str[i] > '9' || str[i] < '0') {
                throw std::invalid_argument("");
            }
        }
        if (str[0] == '-') {
            sign_ = false;
            if (str.substr(1, str.length() - 1).length() <= str_max.length() && str_max.compare(str.substr(1, str.length() - 1)) >= 0) {
                size_ = 1;
                number_ = new unsigned int[size_];
                number_[0] = static_cast<unsigned int>(std::stoul(str.substr(1, str.length() - 1)));
            }
            else {
                //перевод в другую систему
                size_ = 0;
                number_ = nullptr; 
                number_ = ConvertToUlArray(str.substr(1, str.length() - 1), number_, size_);
            }
        }
        else {
            sign_ = true;
            if (str.length() <= str_max.length() && str_max.compare(str) >= 0) {
                size_ = 1;
                number_ = new unsigned int[size_];
                number_[0] = static_cast<unsigned int>(std::stoul(str));
            }
            else {
                //перевод в другую систему
                size_ = 0;
                number_ = nullptr;
                number_ = ConvertToUlArray(str, number_, size_);
            }
        }
    }
    BigInteger(const BigInteger& num) : sign_(num.sign_), size_(num.size_) { //копирующий конструктор
        number_ = new unsigned int[size_];
        for (int i = 0; i < size_; ++i) {
            number_[i] = num.number_[i];
        }
    };
    BigInteger(BigInteger&& moved) noexcept { // ссылка rvalue   конструктор перемещения
        sign_ = moved.sign_;
        size_ = moved.size_;
        number_ = moved.number_;
        moved.number_ = nullptr;
        moved.size_ = 0;
    }

    bool what_sign() const {
        return sign_;
    }
    int what_size() const {
        return size_;
    }
    unsigned int* what_num() const { 
        return number_;
    }
    BigInteger& operator=(const BigInteger& num) { //оператор копирования
        if (this != &num) {
            sign_ = num.sign_;
            size_ = num.size_;
            delete[] number_;
            number_ = new unsigned int[size_];
            for (int i = 0; i < size_; ++i) {
                number_[i] = num.number_[i];
            }
        }
        return *this;
    }
    BigInteger& operator=(BigInteger&& num) noexcept {
        //rvalue_func(std::move(val))
        if (this != &num) {
            size_ = num.size_;
            sign_ = num.sign_;
            delete[] number_;
            number_ = num.number_;
            num.number_ = nullptr;
        }

        return *this;
    }
    bool operator == (const BigInteger& num) const
    {
        if (size_ == num.size_ && sign_ == num.sign_) {
            for (int i = 0; i < size_; ++i) {
                if (num.number_[i] != number_[i]) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
    bool operator >(const BigInteger& num) const {
        if ((num.size_ > size_ && sign_ && num.sign_)
            || (num.size_ < size_ && !sign_  && !num.sign_ )
            || (!sign_ && num.sign_ )) {
            return false;
        }
        if ((size_ > num.size_ && sign_ && num.sign_)
            || (size_ < num.size_ && !sign_ && !num.sign_ )
            || (sign_ && !num.sign_ )) {
            return true;
        }
        if (!sign_  && !num.sign_ ) {
            for (int i = size_ - 1; i >= 0; --i) {
                if (number_[i] < num.number_[i]) {
                    return true;
                }
                if (number_[i] > num.number_[i]) {
                    return false;
                }
            }
        }
        for (int i = size_ - 1; i >= 0; --i) {
            if (number_[i] < num.number_[i]) {
                 return false;
            }
            if (number_[i] > num.number_[i]) {
                return true;
            }
        }

        return false;
    }
    bool operator !=(const BigInteger& num) const {
        return !(num == *this);
    }
    bool operator < (const BigInteger& num) const {
        return (num > *this && (num != *this));
    }
    bool operator <= (const BigInteger& num) const {
        return !(num < *this);
    }
    bool operator >= (const BigInteger& num) const {
        return !(num > *this);
    }
    BigInteger operator-() const {
        if (this->size_ == 1 && this->number_[0] == 0) {
            return *this;
        }
        return BigInteger(!sign_, size_, number_);
    }
    BigInteger operator+() const {
        return *this;
    }

    BigInteger& operator+=(const BigInteger& num) {
        if (num.size_ == 1 && num.number_[0] == 0) {
            return *this;
        }
        if (size_ == 1 && number_[0] == 0) {
            *this = num;
            return *this;
        }
        if (sign_ == num.sign_) {
            //размер массива максимум может увелиться на 1 
            unsigned long long addition = 0;
            unsigned long long compar = int_max;
            const unsigned long long base = compar + 1;
            bool perenos = false;
            unsigned long long* result = new unsigned long long[(size_ > num.size_ ? size_ : num.size_) + 1];
            for (int i = 0; i < (size_ > num.size_ ? size_ : num.size_) + 1; ++i) {
                result[i] = 0;
            }
            for (int i = 0; i < (size_ < num.size_ ? size_ : num.size_); ++i) { //проходимся по числу меньшего размера
                addition = static_cast<unsigned long long>(num.number_[i]);
                addition += static_cast<unsigned long long>(number_[i]); 
                result[i] = addition;
            }
            if (size_ != num.size_) {
                for (int i = size_ < num.size_ ? size_ : num.size_; i < (size_ > num.size_ ? size_ : num.size_); ++i) {
                    if (size_ > num.size_) {
                        result[i] = static_cast<unsigned long long>(number_[i]);
                    }
                    else {
                        result[i] = static_cast<unsigned long long>(num.number_[i]);
                    }
                }
            }
            bool dop = false;
            for (int i = 0; i < (size_ > num.size_ ? size_ : num.size_) + 1; ++i) {
                if (dop) {
                    ++result[i];
                    dop = false;
                }
                if (result[i] > compar) {
                    dop = true;
                    result[i] -= base;
                }
            }
            for (int i = (size_ > num.size_ ? size_ : num.size_); i >= 0; --i) {
                if (result[i] != 0) {
                    size_ = i + 1; 
                    break;
                }
            }
            delete[] number_;
            number_ = new unsigned int[size_];
            for (int i = 0; i < size_; ++i) {
                number_[i] = static_cast<unsigned int>(result[i]);
                cout << number_[i];
            }
            cout << endl;
            delete[] result;
            result = nullptr;
        }
        else if (-*this == num) {
            *this = BigInteger(0);
        }
        else {
            unsigned int* big = nullptr;   // большее по модулю число
            unsigned int* small = nullptr; // меньшее по модулю число
            int big_sz = 0;
            int small_sz = 0;
            if ((!sign_ && (-*this > num)) || (sign_ && (*this > -num))) {
                cout << "heere\n";
                big = number_;
                big_sz = size_;
                small = num.number_;
                small_sz = num.size_;
            }
            else {
                small = number_;
                small_sz = size_;
                big = num.number_;
                big_sz = num.size_;
                sign_ = num.sign_;
            }
            //из большего нужно вычесть меньшее
            bool perenos = false; //забираем ли единицу из след разряда
            unsigned int substraction = 0;
            unsigned int compar = int_max;
            unsigned int* result = new unsigned int[big_sz];
            for (int i = 0; i < big_sz; ++i) {
                result[i] = 0;
            }
            for (int i = 0; i < small_sz; ++i) {
                substraction = big[i];
                if (perenos && substraction > 0) {
                    --substraction;
                    perenos = false;
                }
                else if (perenos) {
                    --substraction;
                }
                if (substraction < small[i]) {
                    perenos = true;
                }
                substraction -= small[i];
                result[i] = substraction;
            }
            for (int i = small_sz; i < big_sz; ++i) {
                if (perenos) {
                    if (big[i] > 0) {
                        result[i] = big[i] - 1;
                        perenos = false;
                    }
                    else {
                        result[i] = compar;
                    }
                }
                else {
                    result[i] = big[i];
                }
            }
            for (int i = big_sz - 1; i >= 0; --i) {
                if (result[i] != 0) {
                    size_ = i + 1; 
                    break;
                }
            }
            delete[] number_;
            number_ = new unsigned int[size_];
            for (int i = 0; i < size_; ++i) {
                number_[i] = result[i];
            }
            delete[] result;
            result = nullptr;
        }
         return *this;

    }

    BigInteger& operator-=(const BigInteger& num) {
        BigInteger temp = (-num);
        return (*this += temp);
    }

    BigInteger& operator*=(const BigInteger& other) {
        unsigned int* res_int = new unsigned int[(size_ + other.size_)];
        for (int i = 0; i < (size_ + other.size_); ++i) {
            res_int[i] = 0;
        }
        unsigned int* this_int = this->number_;
        unsigned int* other_int = other.number_;

        unsigned long long cur = 0; //число в этом разряде
        unsigned long cur_int = 0;
        unsigned long long prev = 0;
        unsigned long prev_overflow = 0;//перенос с предыд разряда
        unsigned long long res_i = 0;
        unsigned long res_overflow = 0;
        unsigned long long int_base = std::numeric_limits<unsigned int>::max();
        ++int_base;
        unsigned long long tmp = 0;
        int idx = 0;
        for (int i = 0; i < other.size_; ++i) { // проход по other number
            for (int t = 0; t < size_; ++t) {   //проход по this
                cur = this_int[t];
                cur *= other_int[i]; 
                cur += prev_overflow; 
                prev_overflow = cur / int_base;
                cur_int = cur % int_base; 
                tmp += res_int[i + t]; 
                tmp += cur_int;
                res_int[i + t] = (tmp % int_base); 
                prev_overflow += (tmp / int_base); 
                ++idx;

                tmp = 0;
            }
            if (idx < size_ + other.size_) {
                res_int[idx] += prev_overflow;
            }
            prev_overflow = 0;
            idx = i + 1; //1
        }
        int end_of_num = 0;
        for (int h = (size_ + other.size_) - 1; h >= 0; --h) {
            if (res_int[h] != 0) {
                end_of_num = h;
                break;
            }
        }
        unsigned int* res_int_without_extra_zeros = new unsigned int[end_of_num + 1];
        for (int i = 0; i <= end_of_num; ++i) {
            res_int_without_extra_zeros[i] = res_int[i];
        }
        delete[] res_int;

        *this = BigInteger((sign_ == other.sign_), end_of_num + 1, res_int_without_extra_zeros);

        delete[] res_int_without_extra_zeros;
        return *this;
    }

    BigInteger& operator/=(const BigInteger& num) {
        if ((*this < num && this->sign_  && num.sign_ ) ||
            (*this > num && !this->sign_  && !num.sign_ ) ||
         (-*this < num && !this->sign_ && num.sign_) ||
        (*this < -num && this->sign_ && !num.sign_ )){
            *this = BigInteger(0);
        }
        else if (*this == num) {
            *this = BigInteger(1);
        }
        else if (*this == -num) {
            *this = BigInteger(-1);
        }
        else {
            BigInteger left(0);
            BigInteger right(*this >= 0 ? *this : -*this);          

            bool s = (num.sign_ == sign_);
            while (left <= right) {
                //вычислить среднее значение
                //поделим aver на 2
                unsigned long long tmp = 0;
                unsigned long long base = int_max;
                ++base;
                BigInteger aver = left + right;

                for (int k = 0; k < aver.size_; ++k) {
                }
                for (int j = aver.size_ - 1; j >= 0; --j) {
                    tmp *= base;
                    tmp += aver.number_[j];
                    aver.number_[j] = static_cast<unsigned int>(tmp / 2);
                    tmp %= 2;
                }
                //убираем лишний 0 если есть
                if (aver.number_[aver.size_ - 1] == 0) {
                    --aver.size_;
                    unsigned int* temp = new unsigned int[aver.size_];
                    for (int j = 0; j < aver.size_; ++j) {
                        temp[j] = aver.number_[j];
                    }
                    delete[] aver.number_;
                    aver.number_ = temp;
                    temp = nullptr;
                }
                if (aver * (num.sign_ ? num : -num) < (this->sign_ ? *this : -*this)) {
                    if (left == aver) {
                        *this = aver;
                        break;
                    }
                    left = aver;
                }
                else if (aver * (num.sign_ ? num : -num) > (this->sign_ ? *this : -*this)) {
                    if (right == aver) {
                        *this = aver;
                        break;
                    }
                    right = aver;
                }
                else {
                    *this = aver;
                    break;
                }
            }
            sign_ = s;
        }
        return *this;
    }

    BigInteger& operator%=(const BigInteger& num) { //23 -5
        BigInteger div = *this / num; //-4
        BigInteger compare = div * num;
        if (compare == *this) {
            *this = 0;
            return *this;
        }
        BigInteger res = (*this) - compare;
        *this = res;
        return *this;
    }
    BigInteger operator+(const BigInteger& y) const {
        if (*this == -y) {
            return BigInteger(0);
        }
        BigInteger temp;
        temp = *this;
        temp += y;
        return temp;
    }
    BigInteger operator-(const BigInteger& y) const {
        BigInteger temp(*this);
        // temp = (*this);
        temp -= y;
        return temp;
    }
    BigInteger operator*(const BigInteger& y) const {
        BigInteger temp(*this);
        temp *= y;
        return temp;
    }
    BigInteger operator/(const BigInteger& y) const {
        BigInteger temp = *this;
        temp /= y;
        return temp;
    }
    BigInteger operator%(const BigInteger& y) const {
        BigInteger temp = *this;
        temp %= y;
        return temp;
    }
    BigInteger operator++() {
        *this += 1;
        return *this;
    }
    BigInteger operator++(int) {   // Postfix ++
        BigInteger temp(*this);
        ++(*this);
        return temp;
    }
    BigInteger operator--() {
        *this -= 1;
        return *this;
    }
    BigInteger operator--(int) {   // Postfix ++
        BigInteger temp(*this);
        --(*this);
        return temp;
    }
    friend std::string to_string(const BigInteger& num);
    ~BigInteger() {
        delete[] number_;
    }
};

BigInteger operator+(const long& y, const BigInteger& x);
bool operator == (const long& y, const BigInteger& num);
BigInteger operator+(const int& y, const BigInteger& x);

std::string to_string(const BigInteger& num);
std::ostream& operator<<(std::ostream& os, const BigInteger& lv);
#endif
