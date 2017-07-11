#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <cmath>
#include <stack>
#include <string>
#include <sstream>
using namespace std;

#include <QDebug>
#include <QString>

class Expression
{
	private: 
		stack<double> s1;
		stack<char> s2;
		string str;

		double getNum(int l, int r);
		int getIdx(char c);
		int judgePriority(char c1, char c2);
		double calc(double d1, double d2, char ch);

	public :
		Expression(string _str);
        string getValue();
};

#endif
