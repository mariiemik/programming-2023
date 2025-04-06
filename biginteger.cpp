/*Длинная арифметика
Реализуйте класс для представления длинных целых чисел (знаковых).
Для класса BigInteger реализовать:
Конструктор по умолчанию (инициализирует число нулём).
Конструкторы от числовых типов.
Явный конструктор от std::string.
Конструктор копирования.
Конструктор перемещения.
Операторы:
присваивания копированием,
присваивания перемещением,
сравнения.
Арифметические операции: сложение, вычитание, умножение, деление, унарный минус, унарный плюс, инкременты и декременты.
Внешнюю функцию std::string to_string(const BigInteger &).
Требования:
Разряды числа должны представляться как минимум 32-битными числами, все биты в их представлении должны использоваться.
Пользоваться сторонними библиотеками при выполнении задания запрещено. Исключение std::string (при реализации to_string и конструктора) и std::numeric_limits.*/

#include "biginteger.h"
int* ConvertToBinary(const string& str, int& idx_bin, int& max_bin) {
    int* number = new int[str.length()];
    for (int i = 0; i < str.length(); i++) {
        number[i] = static_cast<int>((str[i]) - 48);
    }

    int number_size = static_cast<int>(str.length());
    int* bin = new int[max_bin];
    if (number_size > 0) {
        while (number_size != 0 && !(number_size == 1 && number[0] == 0)) { // пробегаемся по числам 

            int* quotient = new int[number_size]; // остатки
            int q_idx = 0; 

            int start = 0;
            int remainder = 0; //остаток
            int divident = 0; //делимое
            while (start <= number_size - 1) {//пробегаемся по числу для деления столбиком 
                bool we_took_2_numbers = false; //двузначное ли число взяли для деления
                divident = number[start];
                
                if (number[start] < 2 && start == number_size - 1) { //для завершения деления, если осталось последнее число 1 или 0
                    if ((number[start] == 0) ||
                        (number_size>1 && number[start] == 1 && remainder == 0)) {
                        quotient[q_idx++] = 0;
                    }
                    remainder = number[start];
                    break;
                }
                if (number[start] < 2) {
                    we_took_2_numbers = true;
                    divident = divident * 10 + number[start + 1];
                }
                if (q_idx > 0 && we_took_2_numbers && remainder==0) {
                    quotient[q_idx++] = 0;
                }
                remainder = divident % 2 == 0 ? 0 : 1;
                if (remainder == 0) {
                    if (we_took_2_numbers) {
                        start += 2;
                    }
                    else {
                        start += 1;
                    }
                }
                else {
                    if (we_took_2_numbers) {
                        start += 1;
                    }
                    number[start] = 1;
                }
                quotient[q_idx++] = divident / 2;
            }
            for (int i = 0; i < q_idx; ++i) {
                number[i] = quotient[i];
            }
            if (q_idx == 0) {
                number[0] = 0;
            }
            number_size = q_idx;
            if (idx_bin >= max_bin) {
                max_bin *= 2;
                int* tmp = new int[max_bin];
              
                for (int i = 0; i < max_bin / 2; ++i) {
                    tmp[i] = bin[i];
                }
                delete[] bin;
                bin = tmp;
            }
            bin[idx_bin++] = remainder;
            delete[] quotient;
        }
    }
    for (int i = 0; i < idx_bin / 2; i++) {
        std::swap(bin[i], bin[idx_bin - i - 1]);
    }

    if (idx_bin == 0) {
        bin[0] = 0;
    }
    delete[] number;
    return bin;
}

unsigned int* ConvertToUlArray(const string& str, unsigned int* number, int& num_size) {
    int idx_bin = 0;
    int max_bin = static_cast<int>(str.length());
    int* bin = ConvertToBinary(str, idx_bin, max_bin);

    int temp[bits_UL_occupies]; //делим искходное бинарное число на отсеки, чтобы  перевести в UL
    int start = idx_bin % 32 == 0 ? idx_bin / 32 - 1 : idx_bin / 32;
    for (int i = 0; i <= start; ++i) {
        for (int j = 0; j < bits_UL_occupies; ++j) {
            if ((idx_bin - 1 - j) - 32 * i < 0) {
                temp[bits_UL_occupies - 1 - j] = 0;
            }
            else {
                temp[bits_UL_occupies - 1 - j] = bin[(idx_bin - 1 - j) - 32 * i];
            }
        }
        unsigned int Ul = 0;

        for (int j = 0; j < bits_UL_occupies; ++j) {
            Ul += temp[j] * static_cast<unsigned int>(pow(2, j));
        }
        Ul = 0;
        for (int j = 0; j < bits_UL_occupies; ++j) {
            Ul += temp[j] * static_cast<unsigned int>(pow(2, bits_UL_occupies - 1 - j));
        }
        unsigned int* tmp_nmb = new unsigned int[++num_size];
        tmp_nmb[i] = Ul;
        for (int l = 0; l < i; ++l) {
            tmp_nmb[l] = number[l];
        }
        delete[] number;
        number = new unsigned int[num_size];
        for (int k = 0; k < num_size; ++k) {
            number[k] = tmp_nmb[k];
        }
        delete[] tmp_nmb;
    }
    delete[] bin;
    return number;
}

BigInteger operator+(const long& y, const BigInteger& x) {
    BigInteger temp(y);
    return temp + x;
}
bool operator == (const long& y, const BigInteger& num) 
{   
    BigInteger temp(y);
    return (temp == num);
}
BigInteger operator+(const int& y, const BigInteger& x) {
    BigInteger temp(y);
    return temp + x;
}

std::string to_string(const BigInteger& num) {
    string res;
    unsigned long long numb_d = 0;
    res += (num.sign_ ? "" : "-");
    if (num.size_ <= 2) {
        if (num.size_ == 1 && num.number_[0] == 0) {
            res = "0";
        }
        else if (num.size_ == 1) {
            res += std::to_string(num.number_[0]);
        }
        else if (num.size_ == 2) {
            unsigned long long a = int_max;
            ++a;
            numb_d = num.number_[1];
            numb_d *= a;
            numb_d += num.number_[0];
            res += std::to_string(numb_d);
        }
    }
    return res;
}

std::ostream& operator<<(std::ostream& os, const BigInteger& lv) {
    if (lv.what_sign()) {
        os << '+';
    }
    else {
        os << '-';
    }
    int sz = static_cast<int>(lv.what_size());
    const unsigned int* n = lv.what_num();
    for (int s = 0; s < sz; ++s) {
        os << n[s]<< " , ";
    }
    os << endl;
    return os;
}
