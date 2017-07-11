#include "expression.h"
#define isNum(ch) ('0' <= ch && ch <= '9')

Expression::Expression(string _str)
{
    _str = _str[0] == '-' ? "0" + _str : _str;
    str = _str + '#';
}

int Expression::getIdx(char c)
{
    switch (c)
    {
        case '+' : return 0;
        case '-' : return 1;
        case '*' : return 2;
        case '/' : return 3;
        case '(' : return 4;
        case ')' : return 5;
        case '#' : return 6;
        case '^' : return 7;
    }

    return -1;
}

int Expression::judgePriority(char c1, char c2)
{
    static int table[20][20] =
    {
        {1, 1, -1, -1, -1, 1, 1, -1},
        {1, 1, -1, -1, -1, 1, 1, -1},
        {1, 1, 1, 1, -1, 1, 1, -1},
        {1, 1, 1, 1, -1, 1, 1, -1},
        {-1, -1, -1, -1, -1, 0, 2, -1},
        {1, 1, 1, 1, 2, 1, 1, 1},
        {-1, -1, -1, -1, -1, 2, 0, -1},
        {1, 1, 1, 1, -1, 1, 1, 2}
    };

    return table[getIdx(c1)][getIdx(c2)];
}

double Expression::getNum(int l, int r)
{
    double ret = 0, base = 0;
    int flag = 1;

    for (int i = l; i <= r; i ++)
    {
        if (str[i] == '-')
        {
            flag = -1;
            continue;
        }
        if (str[i] == '.')
        {
            base = 1;
            continue;
        }

        if (base != 0)
            base *= 0.1;

        ret = ret * 10 + str[i] - '0';
    }

    ret *= flag;
    return base == 0 ? ret : ret * base;
}

double Expression::calc(double d1, double d2, char ch)
{
    if (ch == '+')
        return d1 + d2;
    if (ch == '-')
        return d1 - d2;
    if (ch == '*')
        return d1 * d2;
    if (ch == '^')
        return pow(d1, d2);
    if (d2 != 0)
        return d1 / d2;

    str = "Invalid Expression!";
    return 0;
}

string Expression::getValue()
{
    while (not s1.empty())
        s1.pop();
    while (not s2.empty())
        s2.pop();

    s2.push('#');

    int i = -1, pre_i = 0;
    while (i < (int)str.length())
    {
        pre_i = i;
        i = str.find_first_of("+-*/()^#", pre_i + 1);
        if (i == -1)
            break;

        if (i != pre_i + 1)
            s1.push(getNum(pre_i + 1, i - 1));
		if (i == pre_i + 1 && !((('0' <= str[pre_i] && str[pre_i] <= '9')  && !('0' <= str[i] && str[i] <= '9')) || 
			(str[i] == '(' && pre_i == -1) || 
			(!('0' <= str[pre_i] && str[pre_i] <= '9') && str[i] == '(') ||
			(str[pre_i] == ')' && !('0' <= str[i] && str[i] <= '9'))))
		{
			i = str.find_first_of("+*-/()^#", pre_i + 2);
			if (i == -1)
				break;
			if (i != pre_i + 2) 
				s1.push(getNum(pre_i + 1, i - 1));
		}
        
		int flag = judgePriority(s2.top(), str[i]);

        if (flag == 1)
        {
            static double d1, d2;
            d1 = s1.top(); s1.pop();
            d2 = s1.top(); s1.pop();

            s1.push(calc(d2, d1, s2.top()));
            s2.pop();

            if (str == "Invalid Expression!")
                return str;

            i --;
        }
        else if (flag == -1)
            s2.push(str[i]);
        else if (flag == 0)
            s2.pop();
    }

    ostringstream oss;
    oss << s1.top();
    return oss.str();
}
