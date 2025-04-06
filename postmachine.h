#ifndef POSTMACHINE_H
#define POSTMACHINE_H
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
using std::cout;
using std::endl;

class PostMachine {
	int lenta_; //размер ленты
	int** prog_; // программа
	int first_com_;
public:
	PostMachine(int lenta, int** progrm, int first_command) : lenta_(lenta), prog_(progrm), first_com_(first_command) {}
	std::string calc(std::string const& init, int maxsteps) {
	int cur_com = first_com_;
	int idx = 0;
	bool stop = false; //индекс становится отрицательным, выходим за границы массива 
	std::string res = init;
	int count = 0;
	res.resize(lenta_, '0');
	for (int i = 0; i < init.length(); ++i) {
		res[i] = init[i];
	}
	while (maxsteps > 0) {
		++count;
		--maxsteps;
		if (prog_[cur_com][0] == '0') {
			res = "Error";
			break;
		}
		switch (prog_[cur_com][0]) {
		case 1:
		{
			res[idx] = '1';
			cur_com = prog_[cur_com][1];
			break;
		}
		case 2:
		{
			res[idx] = '0';
			cur_com = prog_[cur_com][1];
			break;
		}
		case 3:
			--idx;
			if (idx == -1) {
				idx = static_cast<int>(res.length()) - 1;
			}
			cur_com = prog_[cur_com][1];
			break;
		case 4:
			++idx;
			if (idx == res.length()) {
				idx = 0;
			}
			cur_com = prog_[cur_com][1];
			break;
		case 5:
			if (res[idx] == '0') {
				cur_com = prog_[cur_com][1];
			}
			else {
				cur_com = prog_[cur_com][2];
			}
			break;
		case 6:
			stop = true;
			break;
		}
		if (stop) {
			break;
		}
		if (maxsteps == 0) {
			res = "Not applicable";
			return res;
		}
	}
	
	std::string ret;
	int end = idx; //last '1'
	for (int i = idx; i <= res.length(); ++i) {
		if (res[i] == '1') {
			end = i;
		}
	}
	for (int i = 0; i < idx; ++i) {
		if (res[i] == '1') {
			end = i;
		}
	}

	if (end < idx) {
		for (int i = idx; i < res.length(); ++i) {
			ret += res[i];
		}
		for (int i = 0; i <= end; ++i) {
			ret += res[i];
		}
	}
	else {
		for (int i = idx; i <= end; ++i) {
			ret += res[i];
		}
	}
	return ret;
}

	~PostMachine() {}
};
#endif
