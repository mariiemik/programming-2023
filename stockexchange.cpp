/*Разработать информационно-справочную систему для работы биржи ценных бумаг.
База данных позволяет хранить описания сведений о заявках на покупку/продажу акций: дата и время заявки, тикер1, тип заявки, количество, цена.
  Должны обрабатываться следующие команды:
load имя_файла_с_БД — чтение базы данных из файла;
save имя_файла_с_БД — запись базы данных в файл;
add дата и время заявки, тикер, тип заявки, количество, цена — добавление заявки на покупку/продажу акций (добавление записи);
delete ID — удаление записи по её уникальному номеру ID;
clear — удаление всех записей из базы данных, у которых количество равно 0;
print — вывод всего списка заявок, отсортированных по дате и времени заявки (по возрастанию);
dom TICKER — биржевой стакан, вывод таблицы актуальных (количество больше 0) заявок на заданный тикер акций TICKER, отсортированных по убыванию цены (при равенстве цены сначала заявки на продажу, упорядоченные по убыванию даты и времени, а далее заявки на покупку, упорядоченные по возрастанию даты и времени);
export имя_файла — вывод всего списка заявок (отсортированных по возрастанию даты и времени) в текстовый файл;
quit — завершение работы.*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
using std::endl;
using std::cin;
using std::cout;

struct Application {
    int id_;
    char date_[11];
    char time_[9];
    char ticker_[16];
    bool buy;
    int amount_;
    double price_;
};
struct DataBase {
    int count;
    int maxsize;
    Application* data;
};

void addRecord(DataBase& DB, const char* date, const char* time, const char* ticker, const char* type, const char* amount, const char* price, const bool sort = true) {
    ++DB.count;
    if (DB.count > DB.maxsize) {
        DB.maxsize = DB.maxsize > 0 ? 2 * DB.maxsize : 1;
        Application* tmp = new Application[DB.maxsize];
        for (int i = 0; i < DB.count-1; ++i){
            tmp[i] = DB.data[i];
        }
        delete[] DB.data;
        DB.data = tmp;
    }
    Application tmp;
    tmp.id_ = DB.count;
    for (int i = 0; i < 10; ++i) {
        tmp.date_[i] = date[i];
    }
    tmp.date_[10] = '\0';
    for (int i = 0; i < 8; ++i) {
        tmp.time_[i] = time[i];
    }
    tmp.time_[8] = '\0';
    for (int i = 0; i < 15; ++i) {
        tmp.ticker_[i] = ticker[i];
    }
    tmp.ticker_[15] = '\0';
    tmp.buy = false;
    if (type[0] == 'b') {
        tmp.buy = true;
    }
    int amount_temp = std::atoi(amount);
    double price_temp = strtod(price, NULL);

    if (DB.count > 1 && sort) {
        int* index_of_same_ticker = NULL;
        int size = 0;
        //ищем заявки с одинаковыми тикерами, но другим типом
        for (int i = 0; i < DB.count - 1; ++i) {
            bool same = false;
            for (int j = 0; j < 15; ++j) {
                if (('a' < ticker[j] && ticker[j] < 'z') && ('A' < DB.data[i].ticker_[j] && DB.data[i].ticker_[j] < 'Z') && (ticker[j] == (DB.data[i].ticker_[j] + 32))
                    || (('A' < ticker[j] && ticker[j] < 'Z') && ('a' < DB.data[i].ticker_[j] && DB.data[i].ticker_[j] < 'z') && (ticker[j] == (static_cast<int>(DB.data[i].ticker_[j]) - 32)))
                    || (ticker[j] == DB.data[i].ticker_[j])) {
                    same = true;
                }
                else {
                    same = false;
                    break;
                }
            }
            if (((!tmp.buy && DB.data[i].buy) || (tmp.buy && !DB.data[i].buy)) && same && DB.data[i].amount_ != 0) {
                int* index_tmp = new int[size];
                for (int t = 0; t < size; ++t) {
                    index_tmp[t] = index_of_same_ticker[t];
                }
                delete[] index_of_same_ticker;
                ++size;
                index_of_same_ticker = new int[size];
                for (int t = 0; t < size - 1; ++t) {
                    index_of_same_ticker[t] = index_tmp[t];
                }
                index_of_same_ticker[size - 1] = i;
                delete[] index_tmp;
            }
        }
        //сортировка по возрастанию цены
        for (int i = 0; i < size - 1; ++i) {
            for (int j = 0; j < size - 1 - i; ++j) {
                if (DB.data[index_of_same_ticker[j]].price_ > DB.data[index_of_same_ticker[j + 1]].price_) {
                    std::swap(index_of_same_ticker[j], index_of_same_ticker[j + 1]);
                }
            }
        }
        if (size > 0) {
            //если у заявки тип покупка, нужно продать тикеры по наивысшей цене
            if (tmp.buy) {
                //начинаем с конца массива index_of_same_ticker
                for (int s = 0; s < size; ++s) {
                    if (price_temp >= DB.data[index_of_same_ticker[s]].price_) {
                        if (amount_temp - DB.data[index_of_same_ticker[s]].amount_ > 0) {
                            amount_temp -= DB.data[index_of_same_ticker[s]].amount_;
                            DB.data[index_of_same_ticker[s]].amount_ = 0;
                        }
                        else {
                            DB.data[index_of_same_ticker[s]].amount_ -= amount_temp;
                            amount_temp = 0;
                        }
                        if (amount_temp == 0) {
                            break;
                        }
                    }
                }
            }
            //если у завявки тип продажа, нужно купить тикеры по наименьшей цене
            else {
                for (int s = size - 1; s >= 0; --s) {
                    if (price_temp <= DB.data[index_of_same_ticker[s]].price_) {
                        if (amount_temp - DB.data[index_of_same_ticker[s]].amount_ > 0) {
                            amount_temp -= DB.data[index_of_same_ticker[s]].amount_;
                            DB.data[index_of_same_ticker[s]].amount_ = 0;
                        }
                        else {
                            DB.data[index_of_same_ticker[s]].amount_ -= amount_temp;
                            amount_temp = 0;
                        }
                        if (amount_temp == 0) {
                            break;
                        }
                    }
                }
            }
        }
        delete[] index_of_same_ticker;
    }
    tmp.amount_ = amount_temp;
    tmp.price_ = price_temp;
    DB.data[DB.count - 1] = tmp;
}

void print(const DataBase& DB) {
    for (int i = 0; i < DB.count; ++i) {
        cout << DB.data[i].id_ << ", ";
        cout << DB.data[i].date_[6] << DB.data[i].date_[7] << DB.data[i].date_[8] << DB.data[i].date_[9] << DB.data[i].date_[5];
        cout << DB.data[i].date_[3] << DB.data[i].date_[4] << DB.data[i].date_[2];
        cout << DB.data[i].date_[0] << DB.data[i].date_[1];
        cout << " " << DB.data[i].time_ << ", " << DB.data[i].ticker_ << ", ";
        if (DB.data[i].buy) {
            cout << "buy, ";
        }
        else {
            cout << "sell, ";
        }
        cout << DB.data[i].amount_ << ", " << DB.data[i].price_ << endl;
    }
}

void sortByDate(DataBase& DB, const int start=0, const int end=0) {
    int real_end;
    if (end > 0) {
        real_end = end;
    }
    else {
        real_end = DB.count;
    }
    for (int i = 0; i < real_end -start - 1; ++i) {
        for (int j = 0 + start; j < real_end - 1 - i; ++j) {
            if (strcmp(DB.data[j].date_, DB.data[j + 1].date_) > 0) {
                std::swap(DB.data[j], DB.data[j + 1]);
            }
            else if (strcmp(DB.data[j].date_, DB.data[j + 1].date_) == 0) {
                if (strcmp(DB.data[j].time_, DB.data[j + 1].time_) > 0) {
                    std::swap(DB.data[j], DB.data[j + 1]);
                }
                else if (strcmp(DB.data[j].time_, DB.data[j + 1].time_) == 0) {
                    if (strcmp(DB.data[j].ticker_, DB.data[j + 1].ticker_) > 0) {
                        std::swap(DB.data[j], DB.data[j + 1]);
                    }
                    else if (strcmp(DB.data[j].ticker_, DB.data[j + 1].ticker_) == 0) {
                        if (!DB.data[j].buy && DB.data[j + 1].buy) {
                            std::swap(DB.data[j], DB.data[j + 1]);
                        }
                        else if (!DB.data[j].buy && !DB.data[j + 1].buy) {
                            if (DB.data[j].price_ < DB.data[j + 1].price_) {
                                std::swap(DB.data[j], DB.data[j + 1]);
                            }
                        }
                        else if (DB.data[j].buy && DB.data[j + 1].buy) {
                            if (DB.data[j].price_ > DB.data[j + 1].price_) {
                                std::swap(DB.data[j], DB.data[j + 1]);
                            }
                        }
                    }
                }
            }
        }
    }
}
void sortByPrice(DataBase& DB) {
    for (int i = 0; i < DB.count - 1; ++i) {
        for (int j = 0; j < DB.count - 1 - i; ++j) {
            if (DB.data[j].price_ > DB.data[j + 1].price_) {
                std::swap(DB.data[j], DB.data[j+1]);
            }
        }
    }
}
void sortByBuySell(DataBase& DB, const int start, const int end) {
    // сначала sell , buy
    for (int i = start; i < end - 1; ++i) {
        for (int j = start; j < end - 1 - i; ++j) {
            if (DB.data[j].buy && !DB.data[j + 1].buy) {
                std::swap(DB.data[j], DB.data[j + 1]);
            }
        }
    }
    int start_sell = -1;
    int start_buy = -1;
    
    if (!DB.data[start].buy) {
        start_sell = start;
    }
    for (int i = start; i < end; ++i) {

        if (DB.data[i].buy) {
            start_buy = i;
            break;
        }
    }
    if (start_sell > 0) {
        if (start_buy > 0) {
            sortByDate(DB, start_sell, start_buy);
        }
        else {
            sortByDate(DB, start_sell, end);
        }
        for (int i = 0; i < DB.count - i - 1; i++) {
            std::swap(DB.data[i], DB.data[DB.count - i - 1]);
        }
    }
    sortByDate(DB, start_buy, end);
}
int findRecord(DataBase& DB, const int id) {
    for (int i = 0; i < DB.count; ++i) {
        if (DB.data[i].id_ == id) {
            return i;
        }
    }
    return -1;
}
void deleteRecord(DataBase& DB, const int id) {
    if(DB.data != nullptr){
        int index = findRecord(DB, id);
        DB.data[index] = DB.data[DB.count - 1];
        --DB.count;
    }
}
void clear(DataBase& DB) {
    for (int i = 0; i < DB.count; ++i) {
        if (DB.data[i].amount_ == 0) {
            deleteRecord(DB, DB.data[i].id_);
        }
    } 
}
void dom(DataBase& DB, const char* ticker) {
    DataBase domDB = { 0,0,nullptr };
    for (int i = 0; i < DB.count; ++i) {
        bool same = false;
        for (int j = 0; j < 15; ++j) {
            if ((('A' < ticker[j] && ticker[j] < 'Z') && ('a' < DB.data[i].ticker_[j] && DB.data[i].ticker_[j] < 'z') && ((ticker[j] + 32) == DB.data[i].ticker_[j]))
            || (('a' < ticker[j] && ticker[j] < 'z') && ('A' < DB.data[i].ticker_[j] && DB.data[i].ticker_[j] < 'Z') && (ticker[j] == (DB.data[i].ticker_[j] + 32))) 
            || (DB.data[i].ticker_[j] == ticker[j]) ){
                same = true;
            }
            else if (DB.data[i].ticker_[j]!='\0') {
                same = false;
                break;
            }
        }
        if(same){
            if (DB.data[i].amount_ > 0) {
                ++domDB.count;
                if (domDB.count > domDB.maxsize) {
                    domDB.maxsize = domDB.maxsize > 0 ? 2 * domDB.maxsize : 1;
                    Application* tmp = new Application[domDB.maxsize];
                    for (int i = 0; i < domDB.count - 1; ++i){
                        tmp[i] = domDB.data[i];
                    }
                    delete[] domDB.data;
                    domDB.data = tmp;
                }
                domDB.data[domDB.count - 1] = DB.data[i];
            }
        }
    }
    sortByPrice(domDB);
    for (int i = 0; i < domDB.count - i - 1; i++) {
        std::swap(domDB.data[i], domDB.data[domDB.count - i - 1]);
    }
    int start = -1;
    int end = -1;
    for (int i = 1; i < domDB.count; ++i) {
        if (domDB.data[i].price_ == domDB.data[i - 1].price_) {
            if (start == -1) {
                start = i - 1;
            }
        }
        else {
            if (start >= 0) {
                end = i - 1;
                sortByBuySell(domDB, start, end+1);
            }
            start = -1;
        }
    }
    cout << "> > ";
    for (int i = 0; i < domDB.count; ++i) {
        if (domDB.data[i].buy) {
            cout << "buy, ";
        }
        else {
            cout << "sell, ";
        }
        cout << std::fixed << std::setprecision(2) << domDB.data[i].price_ <<", " << domDB.data[i].amount_<<", ";
        cout << domDB.data[i].date_[6] << domDB.data[i].date_[7] << domDB.data[i].date_[8] << domDB.data[i].date_[9] << domDB.data[i].date_[5];
        cout << domDB.data[i].date_[3] << domDB.data[i].date_[4] << domDB.data[i].date_[2];
        cout << domDB.data[i].date_[0] << domDB.data[i].date_[1];
        cout << " " << domDB.data[i].time_ << endl;
    }
    cout << "> ";
    delete[] domDB.data;
}
void define_names(const char* input, char* date, char *time , char* ticker, char* buyORsell, char* amount, char* price, int ind = 0) {
    int i_date = 0;
    int i_time = 0;
    int i_ticker = 0;
    int i_buy = 0;
    int i_amount = 0;
    int i_price = 0;
    for (int i = ind; i < 200; ++i) {
        if (input[i] == ' ') {
            ind = i + 1;
            break;
        }
        date[i_date++] = input[i];
    }
    for (int i = ind; i < 200; ++i) {
        if (input[i] == ',') {
            ind = i + 2;
            break;
        }
        time[i_time++] = input[i];
    }
    for (int i = ind; i < 200; ++i) {
        if (input[i] == ',') {
            ind = i + 1;
            break;
        }
        ticker[i_ticker++] = input[i];
    }
    for (int i = ind; i < 200; ++i) {
        if (input[i] == ',') {
            ind = i + 1;
            break;
        }
        if (input[i] != ' ') {
            buyORsell[i_buy++] = input[i];
        }
    }
    for (int i = ind; i < 200; ++i) {
        if (input[i] == ',') {
            ind = i + 1;
            break;
        }
        if (input[i] != ' ') {
            amount[i_amount++] = input[i];
        }
    }
    for (int i = ind; i < 200; ++i) {
        if (input[i] == '\0') {
            break;
        }
        if (input[i] != ' ') {
            price[i_price++] = input[i];
        }
    }
}
int main(int argc, char* argv[])
{
    char input[200] = "";
    DataBase DB = {0, 0, nullptr};

    while (std::cin.getline(input, 200)) {
        char command[50] = "";
        char date[50] = "";
        char time[50] = "";
        char ticker[50] = "";
        char buyORsell[50] = "";
        char amount[50] = "";
        char price[50] = "";
        char nameOFfileINP[200] = "";
        char nameOFfileOUT[200] = "";
        int ind = 0;
        for (int i = 0; i < 200; ++i) {
            if (input[i] == ' ') {
                command[i] = '\0';
                ind = i + 1;
                break;
            }
            command[i] = input[i];
        }
        if (strcmp(command, "load") == 0) {
            int i_file = 0;
            for (int i = ind; i < 200; ++i) {
                nameOFfileINP[i_file++] = input[i];
            }

            std::ifstream in;
            in.open(nameOFfileINP);
    
            char symb;
            char record[200] = "";
            int i = 0;
            while (!in.eof()) {
                symb = in.get();
                
                if (symb == '\n') {
                    define_names(record, date, time, ticker, buyORsell, amount, price);
                    bool sort = false;
                    addRecord(DB, date, time, ticker, buyORsell, amount, price, false);
                    memset(date, 0, 50);
                    memset(time, 0, 50);
                    memset(ticker, 0, 50);
                    memset(buyORsell, 0, 50);
                    memset(amount, 0, 50);
                    memset(price, 0, 50);
                    i = 0;

                }
                else {
                    record[i++] = symb;
                }
            }
            in.close();
        }
        else if (strcmp(command, "save") == 0) {
            int i_file = 0;
            for (int i = ind; i < 200; ++i) {
                nameOFfileOUT[i_file++] = input[i];
            }
            std::ofstream out;
            out.open(nameOFfileOUT);
            for (int i = 0; i < DB.count; ++i) {
                out << DB.data[i].date_ << " " << DB.data[i].time_ << ", " << DB.data[i].ticker_ << ", ";
                if (DB.data[i].buy) {
                    out << "buy, ";
                }
                else {
                    out << "sell, ";
                }
                out << DB.data[i].amount_ << ", " << std::fixed << std::setprecision(2) << DB.data[i].price_ << endl;
            }
            out.close();
        }
        else if (strcmp(command, "add") == 0) {
           define_names(input, date, time, ticker, buyORsell, amount, price, ind);
           if (date[2] != '.' || date[5] != '.') {
               std::cout << "> Wrong date format: " << date << "\n> ";
           }
           else if ((date[3] == '0' && date[4] == '0') || (date[3] == '1' && date[4] > '2')) {
               std::cout << "> Month value is invalid: " << date[2] << date[3] << "\n> ";
           }
           else if ((date[0] == '0' && date[1] == '0') || (date[0] == '3' && date[1] > '1') ||(date[3]=='0' && date[4]=='2' && date[0]=='2' && date[1]>'8')) {
               std::cout << "> Day value is invalid: " << date[0] << date[1] << "\n> ";
           }
           else if (time[2] != ':' || time[5] != ':') {
               std::cout << "> Wrong time format: " << time << "\n> ";
           }
           else if ((time[0] == '2' && time[1] > '4') || (time[0] > '3') || time[3] > '5' || time[6] > '5') {
               std::cout << "> Time value is invalid: " << time << "\n> ";
           }
           else if ( buyORsell[0] != 'b' && buyORsell[0] != 's') {
               std::cout << "> Unknown command: " << buyORsell << "\n> ";
           }
           else {
               addRecord(DB, date, time, ticker, buyORsell, amount, price);
           }
        }
        else if (strcmp(command, "delete") == 0) {
            char id_str[50] = "";
            int i_id = 0;
            for (int i = ind; i < 50; ++i) {
                if (input[i] > '9' || input[i] < '0') {
                    break;
                }
                id_str[i_id++] = input[i];
            }
            int id = atoi(id_str);
            deleteRecord(DB, id);
        }
        else if (strcmp(command, "clear") == 0) {
            int* mas_of_indexes = new int[DB.count];
            int i_mas = 0;
            for (int i = 0; i < DB.count; ++i) {
                if (DB.data[i].amount_ == 0) {
                    mas_of_indexes[i_mas++]=DB.data[i].id_;
                }
            }
            for (int i = 0; i < i_mas; ++i) {
                deleteRecord(DB, mas_of_indexes[i]);
            }
            delete[] mas_of_indexes;
        }
        else if (strcmp(command, "print") == 0) {
            print(DB);
        }
        else if (strcmp(command, "dom") == 0) {
            int i_ticker = 0;
            for (int i = ind; i < ind + 15; ++i) {
                ticker[i_ticker++] = input[i];
            }
            dom(DB, ticker);
        }
        else if (strcmp(command, "export") == 0) {
            int i_file = 0;
            for (int i = ind; i < 200; ++i) {
                nameOFfileOUT[i_file++] = input[i];
            }
            std::ofstream out;
            out.open(nameOFfileOUT);
            sortByDate(DB);
            for (int i = 0; i < DB.count; ++i) {
                out << DB.data[i].date_[6] << DB.data[i].date_[7] << DB.data[i].date_[8] << DB.data[i].date_[9] << DB.data[i].date_[5];
                out << DB.data[i].date_[3] << DB.data[i].date_[4] << DB.data[i].date_[2];
                out << DB.data[i].date_[0] << DB.data[i].date_[1];
                out << " " << DB.data[i].time_ << ", " << DB.data[i].ticker_ << ", ";
                if (DB.data[i].buy) {
                    out << "buy, ";
                }
                else {
                    out << "sell, ";
                } 
                out << DB.data[i].amount_ << ", " << std::fixed << std::setprecision(2) << DB.data[i].price_ << endl;
            }
            out.close();
        }
        else if (strcmp(command, "quit") == 0) {
            delete[] DB.data;
            return 0;
        }
        else {
            std::cout << "> Unknown command: " << command<<"\n> ";
        }
    }
}
