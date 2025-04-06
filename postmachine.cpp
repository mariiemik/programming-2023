/*Интерпретатор Машин Поста*/
#include "postmachine.h"
#include <fstream>
int main(int argc, char* argv[])
{
    if (argc < 3) {
        return  1;
    }
    char* NameFileProgram = argv[1]; //Имя файла с программой
    char* NameFileInitialState = argv[2]; //имя файла с начальным состоянием
    char* NameFileOutput = argv[3]; //имя выходного файла
    std::ifstream FileProgram(NameFileProgram);
    std::ifstream FileInitState(NameFileInitialState);
    std::ofstream FileOutput(NameFileOutput);
    char ch;
    //найдем программу с наименьшим номером
    char min_com_num[]{ "000" };
    char max_com_num[]{ "000" };
    char com_num[]{ "000" };
    int temp_idx = 2;
    while (!FileProgram.eof()) {
        ch = static_cast<char>(FileProgram.get());
        if (ch == EOF) { break; }
        if (ch == '%') { // пропускаем последующие символы до конца строки
            while ((ch = static_cast<char>(FileProgram.get())) != '\n') {}
        }
        if ('0' <= ch && ch <= '9') {
            com_num[temp_idx--] = ch;
            while ((ch = static_cast<char>(FileProgram.get())) != '\n' && ch != EOF) {
                if (temp_idx != 2 && '0' <= ch && ch <= '9') {
                    if (temp_idx == 1) {
                        com_num[temp_idx] = com_num[temp_idx + 1];
                        com_num[temp_idx + 1] = ch;
                    }
                    if (temp_idx == 0) {
                        com_num[0] = com_num[1];
                        com_num[1] = com_num[2];
                        com_num[2] = ch;
                    }
                    --temp_idx;
                }
                else {
                    temp_idx = 2;
                }
            }
            if (strcmp(com_num, min_com_num) < 0 || (min_com_num[0] == '0' && min_com_num[1] == '0' && min_com_num[2] == '0')) {
                for (int i = 0; i < 3; ++i) {
                    min_com_num[i] = com_num[i];
                }
            }
            if (strcmp(com_num, max_com_num) > 0 || (max_com_num[0] == '0' && max_com_num[1] == '0' && max_com_num[2] == '0')) {
                for (int i = 0; i < 3; ++i) {
                    max_com_num[i] = com_num[i];
                }
            }
            com_num[0] = '0'; com_num[1] = '0'; com_num[2] = '0';
        }
    }

    int min_com_int = std::stoi(min_com_num);//чтобы знать с какой комманды начинать
    int max_com_int = std::stoi(max_com_num);// чтобы создать массив комманд
    int** prog = new int* [max_com_int + 1];
    for (int i = 0; i < max_com_int + 1; ++i) {
        prog[i] = new int[3];
        for (int j = 0; j < 3; ++j) {
            prog[i][j] = 0;
        }
    }
    FileProgram.close();
    FileProgram.open(NameFileProgram);
    com_num[0] = '0'; com_num[1] = '0'; com_num[2] = '0';

    while (!FileProgram.eof()) { //проходимся по файлу чтобы положить команды в массив
        ch = static_cast<char>(FileProgram.get());
        if (ch == '%') { // пропускаем последующие символы до конца строки
            while ((ch = static_cast<char>(FileProgram.get())) != '\n') {}
        }
        if ('0' <= ch && ch <= '9') {
            com_num[temp_idx--] = ch;
            //узнаем номер команды
            while ((ch = static_cast<char>(FileProgram.get())) != '\n' && ch != EOF) {
                if (temp_idx != 2 && '0' <= ch && ch <= '9') {
                    if (temp_idx == 1) {
                        com_num[temp_idx] = com_num[temp_idx + 1];
                        com_num[temp_idx + 1] = ch;
                    }
                    if (temp_idx == 0) {
                        com_num[0] = com_num[1];
                        com_num[1] = com_num[2];
                        com_num[2] = ch;
                    }
                    --temp_idx;
                }
                else {
                    temp_idx = 2;
                    break;
                }
            }
            do { ch = static_cast<char>(FileProgram.get()); }
            while (ch == ' ');
            
            int cur_com = std::stoi(com_num);
            //какая команда
            switch (ch)
            {
            case 'V':
                prog[cur_com][0] = 1;
                break;
            case 'X':
                prog[cur_com][0] = 2;
                break;
            case 'L':
                prog[cur_com][0] = 3;
                break;
            case 'R':
                prog[cur_com][0] = 4;
                break;
            case '?':
                prog[cur_com][0] = 5;
                break;
            case '!':
                prog[cur_com][0] = 6;
                break;
            default:    //если неизвестная команда
                break;
            }
            //к какой по счету команде переходить
            com_num[0] = '0'; com_num[1] = '0'; com_num[2] = '0';
            if (1 <= prog[cur_com][0] && prog[cur_com][0] <= 4 || prog[cur_com][0] == 5) {
                do { ch = static_cast<char>(FileProgram.get()); } while (ch == ' ');
                com_num[temp_idx--] = ch;
                while (ch != ' ') {
                    ch = static_cast<char>(FileProgram.get());
                    if (temp_idx != 2 && '0' <= ch && ch <= '9') {
                        if (temp_idx == 1) {
                            com_num[temp_idx] = com_num[temp_idx + 1];
                            com_num[temp_idx + 1] = ch;
                        }
                        if (temp_idx == 0) {
                            com_num[0] = com_num[1];
                            com_num[1] = com_num[2];
                            com_num[2] = ch;
                        }
                        --temp_idx;
                    }
                    else {
                        temp_idx = 2;
                        break;
                    }
                }
                prog[cur_com][1] = std::stoi(com_num);
                com_num[0] = '0'; com_num[1] = '0'; com_num[2] = '0';

                if (prog[cur_com][0] == 5) {// для ? записывем 2 номера команд (записываем еще один номер)
                    com_num[0] = '0'; com_num[1] = '0'; com_num[2] = '0';
                    do { ch = static_cast<char>(FileProgram.get()); } while (ch == ' ');
                    com_num[temp_idx--] = ch;
                    while (ch != ' ') {
                        ch = static_cast<char>(FileProgram.get());
                        if (temp_idx != 2 && '0' <= ch && ch <= '9') {
                            if (temp_idx == 1) {
                                com_num[temp_idx] = com_num[temp_idx + 1];
                                com_num[temp_idx + 1] = ch;
                            }
                            if (temp_idx == 0) {
                                com_num[0] = com_num[1];
                                com_num[1] = com_num[2];
                                com_num[2] = ch;
                            }
                            --temp_idx;
                        }
                        else {
                            temp_idx = 2;
                            break;
                        }
                    }
                    prog[cur_com][2] = std::stoi(com_num);
                }
                else {
                    prog[cur_com][2] = 0;
                }
            }

            else if(prog[cur_com][0] == 6){ //для комманды стоп
                prog[cur_com][1] = 0;
                prog[cur_com][2] = 0;
            }
        }
        com_num[0] = '0'; com_num[1] = '0'; com_num[2] = '0';
    }
    
    std::string str_lenta;
    std::string str_tact;
    int start = 0;
    std::string init;
    while (!FileInitState.eof()) {
        ch = static_cast<char>(FileInitState.get());
        ++start;
        while (ch != ' ') {
            str_lenta += ch;
            ch = static_cast<char>(FileInitState.get());
            ++start;
        }
        ch = static_cast<char>(FileInitState.get());
        ++start;
        while (ch != ' ') {
            str_tact += ch;
            ch = static_cast<char>(FileInitState.get());
            ++start;
        }
        ch = static_cast<char>(FileInitState.get());
        while ('0' <= ch && ch <= '9') {
            init += ch;
            ch = static_cast<char>(FileInitState.get());
        }
    }
    int lenta = std::stoi(str_lenta);
    int tact = std::stoi(str_tact);
    PostMachine PM(lenta, prog, min_com_int);
    std::string result;
    for (int i = 0; i <= max_com_int; ++i) {
         if (prog[i][0] != 0) {
            if (prog[i][0] == 5) {
                if (prog[prog[i][1]][0] == 0 || prog[prog[i][2]][0] == 0) {
                    result = "Error";
                }
            }
        }
    }
    if (result.length() == 0) {
        result = PM.calc(init, tact);
    }

    FileOutput << result;
    FileOutput.close();
    FileInitState.close();
    FileOutput.close();
    for (int i = 0; i < max_com_int + 1; ++i) {
        delete[] prog[i];
    }
    delete[] prog;
    return 0;
}
