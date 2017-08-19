/*
	Halil ibrahim Azak
	August 19, 2017
*/

#ifndef CUSTOM_EXPRESSION_H
#define CUSTOM_EXPRESSION_H

// Must be highest priorities, change as you wish
#define NUM_PRIORITY 9999
#define PARA_PRIORITY 9998

#include <string>
#include <vector>

struct Node {
	std::string item = "";
	Node *left = NULL;
	Node *right = NULL;
	int priority = NUM_PRIORITY;
};

class CustomExpression {
public:
	CustomExpression();
	CustomExpression(std::string expr);
	~CustomExpression();

	bool createTree();
	std::string getError();
	void addOp(std::string op, int priority);
	void addOp(std::vector<std::string> ops, std::vector<int> priorities);
	float getResult();
	bool calculateResult(bool opHandler(std::string, float, float, float&));

private:
	std::string expr;
	Node *root;
	float result = 0;
	std::string error = "";
	std::vector<std::string> ops;
	std::vector<int> priorities;
	bool calculateResult(Node *root, float &res, bool opHandler(std::string, float, float, float&));
	bool getOp(std::string op, Node *&nod);
	bool createExprTree(bool isInner, int &index, Node *&root);
	bool addToTree(Node *&root, Node *&nod);
};

CustomExpression::CustomExpression() {
	this->expr = "";
	this->root = NULL;
}

CustomExpression::CustomExpression(std::string expr) {
	this->expr = expr + "         ";
	this->root = NULL;
}

CustomExpression::~CustomExpression() {}

bool CustomExpression::createTree() {
	int index = 0;
	if (!createExprTree(false, index, this->root))
		return false;

	return true;
}

std::string CustomExpression::getError() {
	return this->error;
}

void CustomExpression::addOp(std::string op, int priority) {
	ops.push_back(op);
	priorities.push_back(priority);
}
void CustomExpression::addOp(std::vector<std::string> ops, std::vector<int> priorities) {
	this->ops.insert(this->ops.end(), ops.begin(), ops.end());
	this->priorities.insert(this->priorities.end(), priorities.begin(), priorities.end());
}

float CustomExpression::getResult() {
	return result;
}

bool CustomExpression::calculateResult(bool opHandler(std::string, float, float, float&)) {
	float res = 0;
	if (!calculateResult(root, res, opHandler))
		return false;

	this->result = res;
	return true;
}

bool CustomExpression::calculateResult(Node *root, float &res, bool opHandler(std::string, float, float, float&)) {
	if (root == NULL)
		return false;

	if (root->priority == NUM_PRIORITY) {
		res = stoi(root->item);
		return true;
	}

	if (root->left == NULL || root->right == NULL) {
		error = "Error: Expression tree is not in valid structure";
		return false;
	}

	float left = 0;
	float right = 0;
	if (!calculateResult(root->left, left, opHandler) || !calculateResult(root->right, right, opHandler))
		return false;

	if (!opHandler(root->item, left, right, res)) {
		this->error = "Operation handler returned an error";
		return false;
	}
	return true;
}

bool CustomExpression::getOp(std::string op, Node *&nod) {
	for (int i = 0; i < ops.size(); i++) {
		if (ops[i] == op) {
			if (priorities.size() < (i + 1)) {
				error = "Error: Less priority defined than operation number";
				return false;
			}

			Node *newNode = new Node;
			newNode->item = op;
			newNode->priority = priorities[i];
			nod = newNode;
			return true;
		}
	}
	error = "Syntax error: Could not find operation '" + op + "'";
	return false;
}

bool CustomExpression::createExprTree(bool isInner, int &index, Node *&root) {
	Node *op = NULL;

	int size = expr.size() - 5;
	for (; index < size; index++) {
		char c = expr[index];
		char nc = expr[index + 1];

		if(c == ' ') {}
		else if ((c > 47 && c < 58) || (c == '-' && nc > 47 && nc < 58)) {
			std::string temp = "";
			temp.push_back(c);
			while (nc > 47 && nc < 58 && index < size) {
				temp.push_back(nc);
				nc = expr[++index + 1];
			}

			Node *newNode = new Node;
			newNode->item = temp;

			if (root == NULL) {
				root = newNode;
			}
			else {
				if (op == NULL) {
					error = "Syntax error: A number cannot follow another (Column " + std::to_string(index) + ")";
					return false;
				}

				op->right = newNode;

				if (!addToTree(root, op))
					return false;

				op = NULL;
			}
		}
		else if (c == '(') {
			if (op == NULL) {
				if (root != NULL) {
					error = "Syntax error: '(' cannot follow number (Column " + std::to_string(index) + ")";
					return false;
				}

				if (!createExprTree(true, index, root))
					return false;
			}
			else {
				Node *newNode = NULL;

				if (!createExprTree(true, ++index, newNode))
					return false;

				op->right = newNode;

				if (!addToTree(root, op))
					return false;

				op = NULL;
			}
		}
		else if(c == ')'){
			if (!isInner) {
				error = "Syntax error: No match found for ')' (Column " + std::to_string(index) + ")";
				return false;
			}

			if (op != NULL) {
				error = "Syntax error: ')' cannot follow an operation (Column " + std::to_string(index) + ")";
				return false;
			}

			if (root == NULL) {
				error = "Syntax error: Parantheses cannot be empty (Column " + std::to_string(index) + ")";
				return false;
			}

			root->priority = PARA_PRIORITY;
			return true;
		}
		else {
			if(op != NULL) {
				error = "Syntax error: An operation cannot follow another (Column " + std::to_string(index) + ")";
				return false;
			}

			std::string temp = "";
			temp.push_back(c);
			while ((nc < 48 || nc > 57) && nc != ' ' && index < size ) {
				if (nc == '-' && expr[index + 2] > 47 && expr[index + 2] < 58)
					break;

				temp.push_back(nc);
				nc = expr[++index + 1];
			}

			if (!getOp(temp, op))
				return false;
		}
	}


	if (op != NULL) {
		error = "Syntax error: Expression cannot end with an operation (Column " + std::to_string(index) + ")";
		return false;
	}

	return true;
}

bool CustomExpression::addToTree(Node *&root, Node *&nod) {
	if (root == NULL) {
		root = nod;
		return true;
	}

	if (nod->priority <= root->priority) {
		nod->left = root;
		root = nod;
		return true;
	}

	Node *parent = root;
	Node *rchild = root->right;
	if (rchild == NULL) {
		error = "Error: Expression tree is not in valid structure";
		return false;
	}

	while (nod->priority > rchild->priority) {
		if (rchild->right == NULL) {
			error = "Error: Expression tree is not in valid structure";
			return false;
		}
		parent = rchild;
		rchild = rchild->right;
	}

	nod->left = rchild;
	parent->right = nod;
	return true;
}

#endif