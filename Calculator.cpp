// ConsoleApplication2.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <stack>
#include <queue>
using namespace std;

bool getAnswer(string, double &);
bool isNum(char ch);
struct op {
	double opera;
	int priority;
};

int main()
{
	cout << "请依次输入数学表达式，或q退出" << endl;
	string expression;
	double result;
	while (true) {
		getline(cin, expression);
		if (expression == "q") {
			break;
		}
		if (getAnswer(expression, result)) {
			cout << "计算结果是：" << result << endl;
		}
	}
	return 0;
}

bool getAnswer(string expression, double & result) {
	stack<op> actions;										// 存放操作符的栈
	queue<op> suffix;										// 存放转换后后缀表达式的队列
	stack<double> numbers;									// 存放后缀表达式中操作数的栈
	int N = expression.size();
	int bracket = 0;										// 保存符号栈中括号数量
	bool operand = true;
	char ch;
	for (int i = 0; i < N; i++) {
		ch = expression[i];
		if (ch == ' ')										// 允许空格
			continue;
		if (ch == ')') {									// 读到 ），将前一个（之前的所有操作符弹出到后缀表达式队列中
			bracket--;
			if (bracket < 0) {
				cout << "syntax error" << endl;
				return false;
			}
			while (!actions.empty()) {
				op & tmp = actions.top();
				if (tmp.opera != ((double)'(')) {
					suffix.push(tmp);
					actions.pop();
				}
				else break;
			}
			actions.pop();
			continue;
		}
		else if (ch == '(') {								// 读到（，直接将其放入操作符栈中
			bracket++;
			op leftbracket{ (double)ch, 2 };
			actions.push(leftbracket);
			continue;
		}
		if (!operand) {
			if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {		// 读到加减乘除符号，从操作符栈中依次弹出所有优先级大于等于它的操作符，入队
				op tmp;
				tmp.opera = ch;
				tmp.priority = 1;
				if (ch == '+' || ch == '-') {
					tmp.priority = 0;
				}
				else {
					tmp.priority = 1;
				}
				while (!actions.empty()) {
					op & theTop = actions.top();
					if (theTop.priority >= tmp.priority && theTop.priority != 2) {
						suffix.push(theTop);
						actions.pop();
					}
					else break;
				}
				actions.push(tmp);										//最后将此操作符入队
				operand = !operand;
			}
			else {
				cout << "syntax error" << endl;
				return false;
			}
		}
		else {
			bool isInteger{ true }, positive{ true };				// 遇到小数点，设置成false
			double integer = 0, fraction = 0;						// integer保存数字的整数部分，fraction保存数字小数部分
			int n = 0;
			char ch = expression[i];
			while ( ch == '+' || ch == '-') {
				if (ch == '-')
					positive = !positive;
				i++;
				if (i >= N) {
					break;
				}
				ch = expression[i];
			}
			if (i >= N || !isNum(expression[i])) {
				cout << "syntax error" << endl;
				return false;
			}
			while (i<N && isNum(expression[i])) {					// 碰到数字或小数点，循环读取直到碰到操作符
				if (expression[i] == '.') {
					isInteger = false;
				}
				else if (isInteger) {
					integer = integer * 10 + expression[i] - 48;
				}
				else {
					n++;
					fraction = fraction * 10 + expression[i] - 48;
				}
				i++;
			}
			op num{ 0.0, 3 };
			positive ? num.opera = (integer+fraction/pow(10,n)) : num.opera = -(integer + fraction / pow(10, n));
			suffix.push(num);
			operand = !operand;
			i--;
		}
	}
	if (bracket != 0 || operand) {
		cout << "syntax error" << endl;
		return false;
	}
	while (!actions.empty()) {								// 将其余符号入队
		suffix.push(actions.top());
		actions.pop();
	}
	while (!suffix.empty()) {								// 依次将后缀表达式队列元素出队
		op tmp = suffix.front();
		double result = 0;
		if (tmp.priority == 3) {							// 如果是数字，入数字栈
			numbers.push(tmp.opera);
		}
		else {												// 如果是操作符，弹出数字栈顶两元素计算，将结果重新入栈
			char act = (char)tmp.opera;
			result = numbers.top();
			numbers.pop();
			switch (act) {
			case'+':
				result += numbers.top();
				break;
			case'-':
				result = numbers.top() - result;
				break;
			case'*':
				result *= numbers.top();
				break;
			case'/':
				result = numbers.top() / result;
				break;
			}
			numbers.pop();
			numbers.push(result);
		}
		suffix.pop();
	}
	result = numbers.top();
	return true;								
}

bool isNum(char ch) {									// 数字返回true，操作符返回false
	if (ch >= '0'&&ch <= '9')
		return true;
	if (ch == '.')
		return true;
	return false;
}

