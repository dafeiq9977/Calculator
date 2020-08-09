// ConsoleApplication2.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <stack>
#include <queue>
using namespace std;

double getAnswer(string expression);
bool isNum(char ch);
struct op {
	double opera;
	int priority;
};

int main()
{
	cout << "请依次输入数学表达式，或q退出" << endl;
	string expression;
	while (true) {
		getline(cin, expression);
		if (expression == "q") {
			break;
		}
		cout << "计算结果是：" << getAnswer(expression) << endl;
	}
	return 0;
}

double getAnswer(string expression) {
	stack<op> actions;										// 存放操作符的栈
	queue<op> suffix;										// 存放转换后后缀表达式对的队列
	stack<double> numbers;									// 存放后缀表达式中操作数的栈
	int N = expression.size();
	char ch;
	for (int i = 0; i < N; i++) {
		ch = expression[i];
		if (ch == ')') {									// 读到 ），将前一个（之前的左右操作符弹出到后缀表达式队列中
			while (!actions.empty()) {
				op & tmp = actions.top();
				if (tmp.opera != ((double)'(')) {
					suffix.push(tmp);
					actions.pop();
				}
				else break;
			}
			actions.pop();
		}
		else if (ch == '(') {								// 读到（，直接将其放入操作符栈中
			op leftbracket{ (double)ch, 2 };
			actions.push(leftbracket);
		}
		else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {		// 读到加减乘除符号，从操作符栈中依次弹出所有大于等于它的操作符，入后缀队列
			op tmp;
			tmp.opera = ch;
			if (ch == '+' || ch == '-') {
				tmp.priority = 0;
			}
			else {
				tmp.priority = 1;
			}
			while (!actions.empty()) {
				op theTop = actions.top();
				if (theTop.priority >= tmp.priority && theTop.priority != 2) {
					suffix.push(theTop);
					actions.pop();
				}
				else break;
			}
			actions.push(tmp);										//最后将此操作符入
		}
		else {
			bool isInteger{ true };									// 遇到小数点，设置成false
			double integer = 0, fraction = 0;						// integer保存数字的整数部分，fraction保存数字小数部分
			int n = 0;
			while (i<N && isNum(expression[i])) {					// 碰到数字或小数点，循环读取知道碰到操作符
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
			op num{ integer + fraction / pow(10,n),3 };
			suffix.push(num);
			i--;
		}
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
			switch (act) {
			case'+':
				result = numbers.top();
				numbers.pop();
				result += numbers.top();
				break;
			case'-':
				result = numbers.top();
				numbers.pop();
				result = numbers.top() - result;
				break;
			case'*':
				result = numbers.top();
				numbers.pop();
				result *= numbers.top();
				break;
			case'/':
				result = numbers.top();
				numbers.pop();
				result = numbers.top() / result;
				break;
			}
			numbers.pop();
			numbers.push(result);
		}
		suffix.pop();
	}
	return numbers.top();								// 最后，栈顶元素即为结果
}

bool isNum(char ch) {									// 数字返回true，操作符返回false
	if (ch >= '0'&&ch <= '9')
		return true;
	if (ch == '.')
		return true;
	return false;
}

