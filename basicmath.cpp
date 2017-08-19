#include <iostream>
#include <string>
#include <vector>
#include "CustomExpression.h"

using namespace std;

bool opHandler(string op, float a, float b, float &res) {
	if (op == "+")
		res = a + b;
	else if (op == "-")
		res = a - b;
	else if (op == "*")
		res = a * b;
	else if (op == "/") {
		if (right == 0) {
			cout << "Error: Divide by zero" << endl;
			return false;
		}
		else {
			res = a / b;
		}
	}
	else {
		cout << "Error: Unknown operation " + op << endl;
		return false;
	}

	return true;
}

int main(int argc, char **argv) {
	cout << "Please enter expression:" << endl;
	string expr;
	getline(cin, expr);
	//expr = "2 - -3 * (4 - 3) - 5 + 22 - 5 * 3";

	CustomExpression exp(expr);

	exp.addOp("+", 1);
	exp.addOp("-", 1);
	exp.addOp("*", 2);
	exp.addOp("/", 2);

	if (exp.createTree() && exp.calculateResult(opHandler))
		cout << "Result : " << exp.getResult() << endl;
	else
		cout << exp.getError();
}