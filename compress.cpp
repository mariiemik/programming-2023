/*Архиватор
Необходимо реализовать архиватор, который поддерживает преобразование MTF (Move-To-Front), алгоритм сжатия LZ77 и алгоритм Хаффмана.*/
#include <iostream>
#include <fstream>
#include <cstring>
using std::cout;
using std::endl;
using std::string;

void MtfCode(const char* inp_name, const char* out_name, const string& coding) {
    std::ifstream inp(inp_name, std::ios::binary);
    std::ofstream out(out_name, std::ios::binary);
    if (coding == "m") {
        for (int i = 0; i < 8; ++i) {
            if (i == 7) {
                out << static_cast<char>(1);;
            }
            else {
                out << static_cast<char>(0);
            }
        }
    }
    unsigned char table[256]{};
    for (int i = 0; i < 256; ++i) {
        table[i] = static_cast<unsigned char>(i);
    }
    unsigned char s;
    int code = 0;
    s = static_cast<unsigned char>(inp.get());
    while (!inp.eof()) {
        for (int i = 0; i < 256; ++i) {
            if (table[i] == s) {
                code = i;
                break;
            }
        }
        out << static_cast<unsigned char>(code);
        for (int i = code; i > 0; i--) {
            table[i] = table[i - 1]; // сдвигаем элементы вправо
        }
        table[0] = s;
        s = static_cast<unsigned char>(inp.get());
    }
    inp.close();
    out.close();
}
void MtfDecode(const char* inp_name, const char* out_name, const bool extra) {
    std::ifstream inp(inp_name, std::ios::binary);
    std::ofstream out(out_name, std::ios::binary);
    unsigned char table[256]{};
    for (int i = 0; i < 256; ++i) {
        table[i] = static_cast<unsigned char>(i);
    }
    unsigned char s;
    unsigned char symb;
    if (extra) {
        for (int i = 0; i < 8; ++i) {
            inp.get();
        }
    }
    s = static_cast<unsigned char>(inp.get());
    while (!inp.eof()) {
        symb = table[static_cast<int>(s)];
        out << symb;
        for (int i = static_cast<int>(s); i > 0; i--) {
            table[i] = table[i - 1]; // сдвигаем элементы вправо
        }
        table[0] = symb;
        s = static_cast<unsigned char>(inp.get());
    }
}
void buf256(string& buffer, string& buffer_length256) {
    while (buffer.length() != 0 && buffer_length256.length() < 255) {
        buffer_length256 += buffer;
    }
}
void Lz77Code(const char* inp_n, const char* out_n, const string& coding) {
    std::ifstream inp(inp_n, std::ios::binary);
    std::ofstream out(out_n, std::ios::binary);
    if (coding == "l") {
        for (int i = 0; i < 8; ++i) {
            if (i == 6) {
                out << static_cast<char>(1);
            }
            else { out << static_cast<char>(0); }
        }
    }
    else if (coding == "ml") {
        for (int i = 0; i < 8; ++i) {
            if (i == 7 || i == 6) {
                out << static_cast<char>(1);
            }
            else { out << static_cast<char>(0); }
        }
    }
    string file;
    unsigned char s;
    s = static_cast<unsigned char>(inp.get());
    while (!inp.eof()) { //положим файл в строку
        file += static_cast<char>(s);
        s = static_cast<unsigned char>(inp.get());
    }
    string buffer;
    unsigned char offset = static_cast<unsigned char>(0); //смещение назад от текущей позиции
    unsigned char length = static_cast<unsigned char>(0); //длинa совпадающей подстроки
    string chain; //цепочка которую будем искать в буффере
    int file_idx = 0;
    bool seek_in_buffer_from_beg = false;
    int chain_split = 0;
    string add_to_buffer;
    string buffer_length256; //для того чтобы отлавливать цепочки которые больше длины буфера или когда мы цепь обрывается после конца и мы часть цепи ищем с начала буффера
    while (file_idx < file.length()) { //после каждой итерации возвраащем тройку значений
        buf256(buffer, buffer_length256);
        buffer_length256 = buffer_length256.substr(0, 255);
        chain += file[file_idx++];
        //добавляем пока 1)цепочка != 255 или 2)цепочка не найдена в буфере 
        while (((buffer.rfind(chain) != string::npos && chain.length() != 256) || seek_in_buffer_from_beg && chain.length() != 256)) {
            if (chain.length() == 256) {
                break;
            }
            if (!seek_in_buffer_from_beg) {
                offset = buffer.length() - buffer.rfind(chain); // откуда началась цепочка в буффере
            }
            if (buffer.find(chain + file[file_idx]) == string::npos && buffer_length256.find(chain + file[file_idx]) != string::npos) {
                seek_in_buffer_from_beg = true;
            }
            else if (seek_in_buffer_from_beg && buffer_length256.find(chain + file[file_idx]) != string::npos) {
                break;
            }
            else if (seek_in_buffer_from_beg) {
                offset = static_cast<unsigned char>(buffer.length() - buffer_length256.find(chain));
                ++length;
                chain += file[file_idx++];
                break;
            }
            ++length;
            if (file_idx == file.size()) {  //добавить ниже условия на конец
                break;
            }
            chain += file[file_idx++];
            //посмотрим в конце ли буффера последний символ цепи
        }

        if (chain.length() == 256) {
            if (add_to_buffer.length() != 0) {
                buffer += add_to_buffer;
                add_to_buffer = "";
            }
            buffer += chain;
            if (buffer.length() > 128) {
                buffer = buffer.substr(buffer.length() - 128, 128);
            }
            out << '(' << offset << length << ',' << chain[chain.length() - 1] << ')';

            length = static_cast<unsigned char>(0);
            offset = static_cast<unsigned char>(0);
            chain = "";
            seek_in_buffer_from_beg = false;
        }
        else if (file_idx >= file.length()) {
            if (static_cast<int>(length) < chain.length() && !seek_in_buffer_from_beg) {
                out << '(' << offset << length << ',' << chain[chain.length() - 1] << ')';
            }
            else {
                out << '(' << offset << length << ',' << ')';
            }

        }
        else if (buffer.rfind(chain) == string::npos && chain.length() == 1)
        {
             //цепочка не нашлась в буффере, убираем один символ из цепи, если ее длина>1
            offset = static_cast<unsigned char>(0);
            length = static_cast<unsigned char>(0);
            buffer += chain;
            if (buffer.length() > 128) {
                buffer = buffer.substr(1, buffer.length() - 1);
            }
            out << '(' << offset << length << ',' << chain[0] << ')';
            length = static_cast<unsigned char>(0);
            offset = static_cast<unsigned char>(0);
            chain = "";
            seek_in_buffer_from_beg = false;
        }
        else if (seek_in_buffer_from_beg) {
            if (chain.length() == 1) {
                buffer += add_to_buffer;
                buffer += chain;
                out << '(' << offset << length << ',' << chain << ')';
            }
            else {
                buffer += add_to_buffer;
                buffer += chain;
                out << '(' << offset << length << ',' << chain[chain.length() - 1] << ')';
            }
            chain = "";
            length = static_cast<unsigned char>(0);
            offset = static_cast<unsigned char>(0);
            seek_in_buffer_from_beg = false;
            add_to_buffer = "";
            if (buffer.length() > 128) {
                buffer = buffer.substr(buffer.length() - 128, 128);
            }
        }
        else if (buffer.rfind(chain) == string::npos) {
            out << '(' << offset << length << ',';
            if (file_idx == file.size()) {
                out << ')';
            }
            else {
                out << chain[chain.length() - 1] << ')';
            }
            if (add_to_buffer.length() != 0) {
                buffer += add_to_buffer;
                add_to_buffer = "";
            }
            buffer += chain;
            if (buffer.length() > 128) {
                buffer = buffer.substr(buffer.length() - 128, 128);
            }
            length = static_cast<unsigned char>(0);
            offset = static_cast<unsigned char>(0);
            chain = "";
            seek_in_buffer_from_beg = false;
        }
        buffer_length256 = "";
    }
    out << '\n';
    out.close();
}
void Lz77Decode(const char* inp_n, const char* out_n, const bool extra) {
    std::ifstream inp(inp_n, std::ios::binary);
    std::ofstream out(out_n, std::ios::binary);
    string buffer; // макс размер 128
    unsigned char s;
    if (extra) {
        for (int i = 0; i < 8; ++i) {
            s = static_cast<unsigned char>(inp.get());
        }
    }
    unsigned char offset = 0;
    unsigned char length = 0;
    unsigned char next = 0;
    bool end = false;
    while (!inp.eof()) {
        inp.read((char*)&s, sizeof(s));
        if (s != '(') {
            break;
        }
        inp.read((char*)&s, sizeof(s));
        offset = s;
        inp.read((char*)&s, sizeof(s));
        length = s;
        inp.read((char*)&s, sizeof(s));
        inp.read((char*)&s, sizeof(s));
        next = s;
        inp.read((char*)&s, sizeof(s));
        if (inp.peek() == EOF || inp.peek() == '\n') {
            end = true;
        }
        if (static_cast<int>(offset) == 0) {
            buffer += static_cast<char>(next);
            if (buffer.length() > 128) {
                buffer = buffer.substr(buffer.length() - 128, 128);
            }
            out << next;
        }
        else {
            string addTObuffer;
            int buffer_idx = static_cast<int>(buffer.length()) - static_cast<int>(offset);
            for (int i = 0; i < static_cast<int>(length); ++i) {
                if (buffer_idx >=  static_cast<int>(buffer.length())) {
                    buffer_idx = 0;
                }
                addTObuffer += buffer[buffer_idx];
                out << buffer[buffer_idx++];
            }

            buffer += addTObuffer;
            if (!end || (end && next != ')'))
            {
                out << next;
            }
            buffer +=static_cast<char>(next);
            if (buffer.length() > 128) {
                buffer = buffer.substr(buffer.length() - 128, 128);
            }
            if (end) { break; }
        }
    }
}
void Help() {
    cout << "enter needed keys:\n";
    cout << "\tyou can use -mtf/-lz77/-huffman codings";
    cout << "\tif you need to decode use -d key";
}
void count_frequency(const char* filename, int frequency[]) { //частотность символов
    std::ifstream inp(filename, std::ios::binary);
    unsigned char s;
    s = static_cast<unsigned char>(inp.get());
    while (!inp.eof()) { //получаем частотность символов
        ++frequency[static_cast<int>(s)];
        s = static_cast<unsigned char>(inp.get());
    }
    inp.close();
}

struct node
{
    int freq_;
    string code_;
    unsigned char ch{};
    unsigned char zero_one{}; //для дерева при декодировании
    node* left;
    node* right;
    node() : freq_(0), left(nullptr), right(nullptr) {}
    node(int fr, string cod, node* l, node* r) : freq_(fr), code_(std::move(cod)), left(l), right(r) {}
    node(int fr, unsigned char cod) : freq_(fr), ch(cod), left(nullptr), right(nullptr) {
        code_ = static_cast<char>(cod);
    }
    node(unsigned char zo) :  freq_(0), zero_one(zo), left(nullptr), right(nullptr) {}
};
void clear_node(node* root) {
    if (root!=nullptr)
    {
        clear_node(root->left);
        clear_node(root->right);
        delete root;
        root = nullptr;
    }
}
void make_code(node& root, const string& str, string code_haff[]) {
    if (root.left != nullptr) {
        make_code(*root.left, str + '0', code_haff);
    }
    if (root.right != nullptr) {
        make_code(*root.right, str + '1', code_haff);
    }
    if (root.left == nullptr && root.right == nullptr) {
        code_haff[static_cast<int>(root.ch)] = str;
    }
}
void free_node(node& root) {
    if (root.left != nullptr) {
        free_node(*root.left);
    }
    if (root.right != nullptr) {
        free_node(*root.right);
    }
    if (root.left == nullptr && root.right == nullptr) {
        delete& root;
    }
}
void write_to_file(const char* filename_in, const char* filename_out, string code_haff[]) {
    std::ifstream inp(filename_in, std::ios::binary);
    std::ofstream out(filename_out, std::ios::binary);
    unsigned char s;
    s = static_cast<unsigned char>(inp.get());
    while (!inp.eof()) {
        out << code_haff[static_cast<int>(s)];
        s = static_cast<unsigned char>(inp.get());
    }
    inp.close();
    out.close();
}
void HuffmanCode(const char* inp_n, const char* out_n, const string& coding) {
    std::ofstream out(out_n, std::ios::binary);
    if (coding == "mh") {
        for (int i = 0; i < 8; ++i) {
            if (i == 7 || i == 5) {
                out << static_cast<char>(1);
            }
            else {
                out << static_cast<char>(0);
            }
        }
    }
    else if (coding == "mlh") {
        for (int i = 0; i < 8; ++i) {
            if (i == 7 || i == 5 || i == 6) {
                out << static_cast<char>(1);
            }
            else {
                out << static_cast<char>(0);
            }
        }
    }
    else if (coding == "lh") {
        for (int i = 0; i < 8; ++i) {
            if (i == 6 || i == 5) {
                out << static_cast<char>(1);
            }
            else {
                out << static_cast<char>(0);
            }
        }
    }
    else if (coding  == "h") {
        for (int i = 0; i < 8; ++i) {
            if (i == 5) {
                out << static_cast<char>(1);
            }
            else {
                out << static_cast<char>(0);
            }
        }
    }

    int frequency[256] = {};
    count_frequency(inp_n, frequency); //частотность символов
    if (coding == "mlh") {
        frequency[static_cast<int>('\n')] -= 1;
    }
    string code_haff[256]{};
    int cur_number_of_nodes = 0;
    for (int i = 0; i < 256; ++i) {
        if (frequency[i] != 0) {
            ++cur_number_of_nodes;
        }
    }
    if (cur_number_of_nodes == 1) {
        for (int i = 0; i < 256; ++i) {
            if (frequency[i] != 0) {
                code_haff[i] = '0';
            }
        }
    }
    else {
        node* temp1[256];
        for (int i = 0; i < 256; ++i) {
            temp1[i] = nullptr;
        }
        int idx = 0;
        for (int i = 0; i < 256; ++i) {
            if (frequency[i] != 0) {
                temp1[idx++] = new node(frequency[i], static_cast<char>(i));
            }
        }

        //сортируем узлы по возрастаемости частотности и сливаем по 2
        while (cur_number_of_nodes != 1) {
             //сортируем temp1 сначала самые большие узлы
            for (int i = 0; i < cur_number_of_nodes - 1; i++) {
                for (int j = 0; j < cur_number_of_nodes - i - 1; j++) {
                    if (temp1[j]->freq_ < temp1[j + 1]->freq_) {
                        node* temp = temp1[j];
                        temp1[j] = temp1[j + 1];
                        temp1[j + 1] = temp;
                    }
                }
            }
            //сливаем по 2 или 1(если остается при нечет кол-ве) узлу
            int sum_freq = temp1[cur_number_of_nodes - 1]->freq_ + temp1[cur_number_of_nodes - 2]->freq_;
            string str = temp1[cur_number_of_nodes - 1]->code_ + temp1[cur_number_of_nodes - 2]->code_;
            temp1[cur_number_of_nodes - 2] = new node(sum_freq, str, temp1[cur_number_of_nodes - 2], temp1[cur_number_of_nodes - 1]);
            --cur_number_of_nodes;
        }

        make_code(*temp1[0], "", code_haff);
        //удаляем рекурусивно temp1
        clear_node(temp1[0]);
    }

    std::ofstream table("attached_haffman_table.txt", std::ios::binary);
    for (int i = 0; i < 256; ++i) {
        if (!code_haff[i].empty()) {
            table << static_cast<char>(i) << " " << code_haff[i] << endl;
        }
    }
    table.close();

    std::ifstream inp(inp_n, std::ios::binary);
    unsigned char s;
    s = static_cast<unsigned char>(inp.get());
    while (!inp.eof()) {
        if (coding == "mlh") {
            if (inp.peek() == EOF) {
                break;
            }
        }
        out << code_haff[static_cast<int>(s)] << ' ';;
        s = static_cast<unsigned char>(inp.get());

    }
    inp.close();
    out.close();
}
void haffman_file_to_array(const char* haff_file, string haffman_table[]) {
    std::ifstream haffman_table_file("attached_haffman_table.txt", std::ios::binary);
    unsigned char s;
    s = static_cast<unsigned char>(haffman_table_file.get());
    int idx = 0;
    while (!haffman_table_file.eof()) { //читаем по одной строчке, записываем таблицу хаффмана в массив
        idx = static_cast<int>(s);
        string code;
        haffman_table_file.get();
        s = static_cast<unsigned char>(haffman_table_file.get());

        while (s != '\n' && !haffman_table_file.eof()) {
            code += static_cast<char>(s);
            s = static_cast<unsigned char>(haffman_table_file.get());
        }
        haffman_table[idx] = code;
        s = static_cast<unsigned char>(haffman_table_file.get());
    }
    haffman_table_file.close();
}
void make_branch(node& root, string code, unsigned char symb) {
    if (code[0] == '0') {       
        if (root.left == nullptr)
        {
            root.left = new node('0');
        }
        if (code.length() == 1) {
            root.left->ch = symb;
            return;
        }

        make_branch(*root.left, code.substr(1, code.length() - 1), symb);
    }
    else {
        if (root.right == nullptr)
        {
            root.right = new node('1');
        }
        if (code.length() == 1) {
            root.right->ch = symb;
            return;
        }
        make_branch(*root.right, code.substr(1, code.length() - 1), symb);
    }
}
unsigned char find_symb(node& root, const string& code) {
    if (code[0] == '0') {
        if (code.length() == 1) {
            return root.left->ch;
        }
        return find_symb(*root.left, code.substr(1, code.length()));
    }
    if (code.length() == 1) {
        return root.right->ch;
    }
    return find_symb(*root.right, code.substr(1, code.length()));
}

void HuffmanDecode(const char* inp_n, const char* out_n, const bool extra, bool lz = false) {
    std::ifstream inp(inp_n, std::ios::binary);
    std::ofstream outp(out_n, std::ios::binary);
    string haffman_table[256]{};
    haffman_file_to_array("attached_haffman_table.txt", haffman_table);
    //уменьшим таблицу хаффмана
    int haf_t_size = 0;
    for (int i = 0; i < 256; ++i) {
        if (!haffman_table[i].empty()) {
            ++haf_t_size;
        }
    }
    int max_length = 0;
    int size = 0;
    string* haf_table_code = new string[haf_t_size];
    int* haf_table_idx = new int[haf_t_size];
    int min_length_of_code = haf_t_size; // минимальная длина кода символа

    for (int i = 0; i < 256; ++i) {
        if (!haffman_table[i].empty()) {
            min_length_of_code = static_cast<int>(haffman_table[i].length()) < min_length_of_code ? static_cast<int>(haffman_table[i].length()) : min_length_of_code;
            max_length = static_cast<int>(haffman_table[i].length()) > max_length ? static_cast<int>(haffman_table[i].length()) : max_length;
            haf_table_code[size] = haffman_table[i];
            haf_table_idx[size++] = i;
        }
    }
    char s;
    if (extra) {
        for (int i = 0; i < 8; ++i) {
            inp.get();
        }
    }
    s = static_cast<char>(inp.get());
    string cur_code;
    cur_code.resize(max_length);
    int cur_idx = 0;
    bool there_is_such_code_in_table = false;
    int idx_of_found_code = 0;
    int count = 0;
    node root(0, "", nullptr, nullptr);
    for (int i = 0; i < haf_t_size; ++i) {
        make_branch(root, haf_table_code[i], static_cast<unsigned char>(haf_table_idx[i]));
    }
    unsigned char symb;
    while (!inp.eof()) {
        while (s != ' ') {
            cur_code[cur_idx++] = s;
            s = static_cast<char>(inp.get());
        }
        //поиск в дереве:
        symb = find_symb(root, cur_code.substr(0, cur_idx));
        outp << symb;
        cur_idx = 0;
        s = static_cast<char>(inp.get());
        //s = static_cast<unsigned char>(inp.get());
    }
    if (lz) { outp << '\n'; }// remove
}
void Decompr(const char* inp_name, const char* out_name) {
    std::ifstream inp(inp_name, std::ios::binary);
    std::ofstream out(out_name, std::ios::binary);

    unsigned char s;
    bool mtf = false;
    bool lz77 = false;
    bool huffman = false;
    for (int i = 0; i < 8; ++i) {
        s = static_cast<unsigned char>(inp.get());
        if (i == 5 && 1 == static_cast<int>(s)) {
            huffman = true;
        }
        if (i == 6 && 1 == static_cast<int>(s)) {
            lz77 = true;
        }
        if (i == 7 && 1 == static_cast<int>(s)) {
            mtf = true;
        }

    }
    if (mtf && !huffman && !lz77) {
        MtfDecode(inp_name, out_name, true);
    }
    else if (mtf && lz77 && !huffman) {
        const char* tmp = "temp.txt";
        Lz77Decode(inp_name, tmp, true);
        MtfDecode(tmp, out_name, false);
    }
    else if (mtf && !lz77 && huffman) {
        const char* tmp = "temp.txt";
        HuffmanDecode(inp_name, tmp, true);
        MtfDecode(tmp, out_name, false);
    }
    else if (mtf && lz77 && huffman) {
        const char* tmp = "temp.txt";
        const char* tmp2 = "temp2.txt";
        HuffmanDecode(inp_name, tmp, true, true);
        Lz77Decode(tmp, tmp2, false);
        MtfDecode(tmp2, out_name, false);
    }
    else if (!mtf && lz77 && !huffman) {
        Lz77Decode(inp_name, out_name, true);
    }
    else if (!mtf && lz77 && huffman) {
        const char* tmp = "temp.txt";
        HuffmanDecode(inp_name, tmp, true);
        Lz77Decode(tmp, out_name, false);
    }
    else if (!mtf && !lz77 && huffman) {
        HuffmanDecode(inp_name, out_name, true);
    }
}

int main(int argc, char* argv[])
{
    bool mtf = false;
    bool lz77 = false;
    bool huffman = false;
    bool decompr = false;
    bool help = false;
    for (int i = 1; i < argc - 2; ++i) {
        if (argv[i][1] == 'm') {
            mtf = true;
        }
        else if (argv[i][1] == 'l') {
            lz77 = true;
        }
        else if (strlen(argv[i]) < 3 && (argv[i][1] == 'h' || argv[i][0] == '?')) {
            help = true;
        }
        else if (argv[i][1] == 'h') {
            huffman = true;
        }
        else if (argv[i][1] == 'd') {
            decompr = true;
        }
    }
    const char* inp = argv[argc - 2];
    const char* out = argv[argc - 1];
    
    if (help) {
        Help();
    }
    else if (mtf && !huffman && !lz77) {
        MtfCode(inp, out, "m");
    }
    else if (mtf && lz77 && !huffman) {

        const char* tmp = "temp.txt";
        MtfCode(inp, tmp, "");
        Lz77Code(tmp, out, "ml");
    }
    else if (mtf && !lz77 && huffman) {

        const char* tmp = "temp.txt";
        MtfCode(inp, tmp, "");
        HuffmanCode(tmp, out, "mh");
    }
    else if (mtf && lz77 && huffman) {
        const char* tmp = "temp.txt";
        const char* tmp2 = "temp2.txt";
        MtfCode(inp, tmp, "");
        Lz77Code(tmp, tmp2, "");
        HuffmanCode(tmp2, out, "mlh");
    }
    else if (!mtf && lz77 && !huffman) {
        Lz77Code(inp, out, "l");
    }
    else if (!mtf && lz77 && huffman) {

        const char* tmp = "temp.txt";
        Lz77Code(inp, tmp, "");
        HuffmanCode(tmp, out, "lh");
    }
    else if (!mtf && !lz77 && huffman) {
        HuffmanCode(inp, out, "h");
    }
    else if (decompr) {
        Decompr(inp, out);
    }
    return 0;
}
