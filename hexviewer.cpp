#include <iostream>
#include <fstream>
using std::cout;
using std::endl;
void convert_strnum(int number, unsigned char count_hex[10]) {
    for (int i = 0; i < 10; ++i) {
        count_hex[i] = '0';
    }
    const int syst = 16;
    unsigned char invres[10] = "";
    int idx = 0;
    int remain;
    while (number != 0){
        remain = number % syst;
        if (10 <= remain && remain <= 15){
            unsigned char letter = static_cast<unsigned char>(remain + 55);
            invres[idx++] = letter;
        }
        else {
            invres[idx++] = static_cast<unsigned char>(remain + 48);
        }
        number /= syst;
    }
    int end_ind = 8;
    for (int i = 0; i < idx; ++i) {
        count_hex[end_ind--] = invres[i];
    }
}
void convert_symbol(int number, unsigned char symb_hex[2]) {
    for (int i = 0; i < 2; ++i) {
        symb_hex[i] = '0';
    }
    const int syst = 16;
    unsigned char invres[2] = "";
    int idx = 0;
    int remain;
    while (number != 0) {
        remain = number % syst;
        if (10 <= remain && remain <= 15) {
            unsigned char letter = static_cast<unsigned char>(remain + 55);
            invres[idx++] = letter;
        }
        else {
            invres[idx++] = static_cast<unsigned char>(remain + 48);
        }
        number /= syst;
    }
    int end_ind = 1;
    for (int i = 0; i < idx; ++i) {
        symb_hex[end_ind--] = invres[i];
    }
}

int main(int argc, char* argv[]) {
    unsigned char element;
    std::ifstream f_inp(argv[1], std::ios::binary);
    std::ofstream f_outp;
    f_outp.open(argv[2]);
    int str_num = 0; //номер строки
    unsigned char str_hex[10] = "";
    unsigned char symb_hex[2] = "";
    int int_element = 0;
    unsigned char buffer[16];
    int idx_buffer = 0;
    unsigned char char_element;

    while (f_inp.read((reinterpret_cast<char*>(&element)), sizeof(element))) {
        int_element = static_cast<int>(element);
        if (int_element >= 32) {
            char_element = static_cast<unsigned char>(int_element);
        }
        else {
            char_element = '.';
        }
        buffer[idx_buffer++] = char_element;
        if (idx_buffer % 16 == 1) {
            convert_strnum(str_num, str_hex);
            for (int i = 0; i < 10; ++i) {
                f_outp << str_hex[i];
            }
            f_outp << ':' << ' ';
            ++str_num;
        }
        convert_symbol(int_element, symb_hex);
        for (int i = 0; i < 2; ++i) {
            f_outp << symb_hex[i];
        }
        f_outp << ' ';
        if (idx_buffer % 16 == 8) {
            f_outp << '|' << ' ';
        }
        if (idx_buffer % 16 == 0) {
            f_outp << ' ';
            idx_buffer = 0;
            for (int i = 0; i < 16; ++i) {
                f_outp << buffer[i];
            }
            f_outp << endl;
        }
    }

    int add_spaces = 0;
    int idx_buffer_copy = idx_buffer;
    if (idx_buffer % 16 != 0) {
        add_spaces = (16 - idx_buffer);
        for (int i = 0; i < add_spaces; ++i) {
            ++idx_buffer;
            f_outp << ' ' << ' ' << ' ';
            if (idx_buffer % 16 == 8) {
                f_outp << '|' << ' ';
            }
        }
        f_outp << ' ';
        for (int i = 0; i < idx_buffer_copy; ++i) {
            f_outp << buffer[i];
        }
    }
    f_inp.close();
    f_outp.close();
    return 0;
}
