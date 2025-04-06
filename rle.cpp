/*Кодирование длин серий (run-length encoding, RLE) — алгоритм сжатия данных, заменяющий повторяющиеся символы на один символ и число его повторов.
Для записи длин серий (для подсчёта количества символов) используем переменную целочисленного типа со знаком signed char. В такую переменную можно записать числа от -128 до 127 включительно. 
Учтем, что нет необходимости записывать, что какой-то символ повторился 0 раз. Тогда, если длина серии будет отрицательное число, 
то его модуль будет означать сколько символов далее в последовательности встречаются по одному разу. 
А если длина серии является неотрицательным числом Х, то оно означает, что следующий символ встретился в последовательности X+1 раз.
Как же быть, если длина серии равна 129 символам и более? В этом случае серию разделяют на части так, чтобы длина части не превышала 128 символов. Например, серия, состоящая из 200 символов «A»,
  будет закодирована следующей строкой (200=128+72): 127A71A (здесь число 128 — это один байт, число 71 тоже, то есть в шестнадцатеричном виде строка 7F 41 47 41).
Необходимо написать программу, которая выполняет RLE: кодирование и декодирование.*/
#include <iostream>
#include <fstream>
using std::cout;
using std::endl;
int main(int argc, char* argv[]) {
	char dORe = argv[1][1];
	std::ifstream f_inp(argv[2], std::ios::binary);
	if (!f_inp)
	{
		std::cout << "file error\n";
		return 1;
	}
	std::ofstream f_outp(argv[3], std::ios::binary);

	if (dORe == 'e') {
		unsigned char element;
		unsigned char next_element;
		unsigned char dif_els[100] = "";
		int amount_of_dif_els = 0;
		int one_letter_sequence_in_the_end = 0;
		char count = 0;
		unsigned char buffer[128] = "";
		int idx = 0;
		int check = 0;
		unsigned char prev_el='0';
		char zero = 0;
		do {
			element = f_inp.get();
			next_element = f_inp.peek();
			if (element == next_element) {
				if (count < 0) {
					f_outp << count;
					for (int i = 0; i < idx; ++i) {
						f_outp << buffer[i];
					}
					idx = 0;
					count = 0;
				}
				if (count == 127) {
					f_outp << count << element;
					count = -1;
					idx = 0;
				}
				else {
					buffer[idx++] = element;
				}
				++count;
			}
			else if (element != next_element) {
				if (f_inp.eof() && count > 0) {
					if (prev_el == element && count == 1) {
						f_outp << count << element;
					}
					else {
						f_outp << count;
						f_outp << buffer[0];
					}
				}
				else if (f_inp.eof()) {
					if (count == 0 && prev_el == element) {
						f_outp << count << element;
					}
					else {
						buffer[idx++] = element;
						--count;
						f_outp << count;
						for (int i = 0; i < idx; ++i) {
							f_outp << buffer[i];
						}
					}
				}
				else if (count > 0) {
					buffer[idx++] = element;
					f_outp << count << element;
					count = 0;
					idx = 0;
				}
				else {
					if (prev_el == element) {
						f_outp << zero << prev_el;
					}
					else if (static_cast<int>(element) != EOF) {
						--count;
						buffer[idx++] = element;
					}
					if (count == -128) {
						f_outp << count;
						for (int i = 0; i < idx; ++i) {
							f_outp << buffer[i];
						}
						count = 0;
						idx = 0;
					}
				}
			}
			prev_el = element;
		} while (!f_inp.eof());
	}
	else {
		unsigned char element;
		int seq = 0;
		char count;
		while (f_inp.peek()!=EOF) {
			f_inp.read((reinterpret_cast<char*>(&count)), sizeof(count));
			seq = int(count)+0;
			if (count < 0) {
				for (int i = 0; i < (-seq); ++i) {
					element = f_inp.get();
					f_outp << element;
				}
			}
			else{
				element = f_inp.get();
				for (int i = 0; i <= seq; ++i) {
					f_outp << element;
				}
			}
		}
	}
	return 0;
}
