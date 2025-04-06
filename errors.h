#ifndef ERRORS_H
#define ERRORS_H
#include<string>
class Error {
public:
	virtual std::string what() const {
		return std::string("error");
	}
};
class  InvVar : public Error {
public:
	std::string what() const {
		return std::string("inv var");
	}
};
class  InvOper : public Error {
public:
	std::string what() const {
		return std::string("inv operation");
	}
};
class  InvExpr : public Error {
public:
	std::string what() const {
		return std::string("inv expression");
	}
};
#endif
