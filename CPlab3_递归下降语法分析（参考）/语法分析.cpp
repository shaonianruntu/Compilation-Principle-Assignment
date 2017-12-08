#include<stdio.h>
#include<string.h>
#include<iostream>
using namespace std;


void Print();
void Scanner();
bool Statement();
bool Condition();
bool While_Statement();
bool Expression();
bool Item_expression();
bool Factor();
bool Conditional_statements();
bool Assignment_statement();
bool Compound_statements();


int syn;//存放单词的类型
int p;
int numof_ = 0;
char ch;
int sum;//用来保存数字的值 
char token[10];
char program[300] = " \
Const x=8,y=7; \
Var a, b, c; \
begin \
a = b + x; \
	if a > 0 \
	then	\
		begin  \
		 c = y - 1;  \
		 a = a + 2;  \
	end  \
	while a>0  \
 	do  a = a - 1; \
end#";

char *rwtab[9] = { "begin","if","then",
"else","while","do","Const"
,"Var","end" };
int m;

void printNumOf() {
	for (int i = 0; i < numof_; ++i)
		cout << " ";
}

bool isDigital(char ch)
{
	if (ch <= '9'&&ch >= '0')
		return true;
	else
		return false;
}

bool isAlpha(char ch)
{
	if (ch >= 'a'&&ch <= 'z' || ch >= 'A'&&ch <= 'Z')
		return true;
	else
		return false;
}




void Scanner()
{
	int num = 0;
	for (m = 0; m<10; m++)
	{
		token[m] = '\0';
	}
	m = 0;
	ch = program[p++];

	while (ch == ' ' || ch == '\n' || ch == '\t')
	{
		ch = program[p++];
	}
	if (isAlpha(ch)) {
		do {
			token[m++] = ch;
			ch = program[p++];
		} while (isAlpha(ch) || isDigital(ch));
		p--;
		syn = 10;
		token[m++] = '\0';
		for (int n = 0; n <= 8; n++)
		{
			if (strcmp(token, rwtab[n]) == 0)
			{
				syn = n + 1;
				break;
			}
		}
		return;
	}
	else if (isDigital(ch))
	{
		sum = 0;
		while (isDigital(ch))
		{
			sum = sum * 10 + ch - '0';
			ch = program[p++];
		}
		p--;
		syn = 11;
		if (isAlpha(ch))
			syn = -1;
		return;
	}
	else
	{
		token[0] = ch;
		switch (ch)
		{
		case '<':ch = program[p++];
			if (ch == '>')
			{
				syn = 22;
				token[1] = ch;
			}
			else if (ch == '=')
			{
				syn = 18;
				token[1] = ch;
			}
			else {
				syn = 19;
				p--;
			}
			break;
		case '>':ch = program[p++];
			if (ch == '=')
			{
				syn = 21;
				token[1] = ch;
			}
			else {
				syn = 20;
				p--;
			}
			break;
		case '=':ch = program[p++];
			if (ch == '=')
			{
				syn = 17;
				token[1] = ch;
			}
			else {
				syn = 16;
				p--;
			}
			break;
		case '+':syn = 12; break;
		case '-':syn = 13; break;
		case '*':syn = 14; break;
		case '/':syn = 15; break;
		case ';':syn = 23; break;
		case '(':syn = 24; break;
		case ')':syn = 25; break;
		case ',':syn = 26; break;
		case '#':syn = 0; break;
		default:syn = -1; break;
		}
		return;
	}
}

bool Constan_Defined()
{
	if (syn == 10)
	{
		printNumOf();
		cout << "常量定义" << endl;
		numof_ += 4;
		printNumOf();
		cout << token << endl;
		Scanner();
		if (syn == 16)
		{
			printNumOf();
			cout << "等于" << token << endl;
			Scanner();
			if (syn == 11)
			{
				printNumOf();
				cout << "无符号整数" << sum << endl;
				numof_ -= 4;
				return true;
			}
			return false;
		}
		return false;
	}
	return false;
}

bool Constan_Description()
{
	Scanner();
	if (syn == 7)
	{
		printNumOf();
		cout << "常量说明" << token << endl;
		numof_ += 4;
		Scanner();
		while (Constan_Defined())
		{
			Scanner();
			if (syn == 23)
			{
				printNumOf();
				cout << "分号" << token << endl;
				numof_ -= 4;
				return true;
			}
			else if (syn == 26)
			{
				printNumOf();
				cout << "逗号" << token << endl;
				Scanner();
				continue;
			}
			cout << "常量说明错误" << endl;
		}
	}
	else
		return false;
}


bool  Variable_Defined()
{
	if (syn == 10)
	{
		printNumOf();
		cout << "变量定义" << token << endl;
		return true;
	}
	else
		return false;
}

bool Variable_Description()
{
	if (syn == 10 || syn == 2 || syn == 5 || syn == 1 || syn == 0)
	{
		return true;
	}
	Scanner();
	if (syn == 8)
	{
		printNumOf();
		cout << "变量说明" << token << endl;
		numof_ += 4;
		Scanner();
		while (Variable_Defined())
		{
			Scanner();
			if (syn == 23)
			{
				printNumOf();
				cout << "分号" << token << endl;
				numof_ -= 4;
				return true;
			}
			else if (syn == 26)
			{
				printNumOf();
				cout << "逗号" << token << endl;
				Scanner();
				continue;
			}

		}
	}
}



bool Condition()
{
	printNumOf();
	cout << "条件" << endl;
	Expression();
	if (syn == 17 || syn == 18 || syn == 19 || syn == 20 || syn == 21
		|| syn == 22)
	{
		printNumOf();
		cout << "关系运算符" << token << endl;
		Scanner();
	}
	else
	{
		printNumOf();
		cout << "关系运算符错误" << endl;
		return false;
	}
	Expression();
}

bool Expression()
{
	printNumOf();
	cout << "表达式" << endl;
	numof_ += 4;
	do {
		if (syn == 12 || syn == 13)
		{
			printNumOf();
			cout << "加法运算符" << token << endl;
			Scanner();
			Item_expression();
		}
		else
		{
			Item_expression();
		}
	} while (syn == 12 || syn == 13);
	numof_ -= 4;
	return true;
}

bool Item_expression()
{
	printNumOf();
	cout << "项" << endl;
	numof_ += 4;
	while (Factor())
	{
		//Scanner();
		if (syn == 14 || syn == 15)
		{
			printNumOf();
			cout << "乘法运算符" << token << endl;
			Scanner();

		}
		else {
			numof_ -= 4;
			return true;
		}
	}
	numof_ -= 4;
	return false;
}

bool Factor()
{
	printNumOf();
	cout << "因子" << endl;
	numof_ += 4;
	if (syn == 10)
	{
		printNumOf();
		cout << "标识符" << token << endl;
		Scanner();
		numof_ -= 4;
		return true;
	}
	else if (syn == 11)
	{
		printNumOf();
		cout << "无符号数字" << sum << endl;
		Scanner();
		numof_ -= 4;
		return true;
	}
	else if (syn == 24)
	{
		printNumOf();
		cout << "左括号" << token << endl;
		Scanner();
		Expression();
		if (syn == 25)
		{
			printNumOf();
			cout << "右括号" << token << endl;
			Scanner();
			numof_ -= 4;
			return true;
		}
		else
		{
			cout << "没有），错误" << endl;
			numof_ -= 4;
			return false;
		}
	}
	else
	{
		cout << "没有左括号" << endl;
		numof_ -= 4;
		return false;
	}
	numof_ -= 4;
	return false;
}

bool Assignment_statement()
{
	printNumOf();
	cout << "赋值语句" << endl;
	numof_ += 4;
	printNumOf();
	cout << "标识符" << token << endl;
	Scanner();
	if (syn == 16)
	{
		printNumOf();
		cout << "赋值语句=" << endl;
		numof_ += 4;
		Scanner();
		Expression();
		numof_ -= 8;
		return true;
	}
	else
	{
		cout << "没有=" << endl;
	}
}

bool Compound_statements()
{
	printNumOf();
	cout << "复合语句" << token << endl;
	numof_ += 4;
	Scanner();
	while (Statement())
	{
		if (syn == 23)
		{
			printNumOf();
			cout << "复合语句中的分割符" << token << endl;
			Scanner();
			if (syn == 9) {

				break;
			}
		}
	}
	if (syn == 9)
	{
		numof_ -= 4;
		printNumOf();
		cout << "复合语句" << token << endl;
		Scanner();

		return true;
	}
	else
	{
		cout << "复合语句缺乏" << endl;
		return false;
	}
	if (syn == 0)
	{
		printNumOf();
		cout << "复合语句" << token << endl;
	}
}


bool Conditional_statements()
{
	if (syn == 2)
	{
		printNumOf();
		cout << "条件语句if" << endl;
		Scanner();
		Condition();
		if (syn == 3)
		{
			printNumOf();
			cout << "then" << endl;
			Scanner();
			Statement();
			if (syn == 4)
			{
				Scanner();
				Statement();
			}
			else
			{
				return true;
			}
		}
		else
		{
			cout << "条件语句中缺少 then" << endl;
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool While_Statement()
{
	printNumOf();
	cout << "循环语句" << token << endl;
	Scanner();

	Condition();
	if (syn == 6)
	{
		printNumOf();
		cout << "while循环的do" << endl;
		Scanner();
		Statement();
		return true;
	}
	//      else 
	//          return false;


	else
		return false;
}

bool Statement()
{
	if (syn == 10)
	{
		Assignment_statement();
		return true;
	}
	else if (syn == 5)
	{
		While_Statement();
		return true;
	}
	else if (syn == 1)
	{
		Compound_statements();
		return true;
	}
	else if (syn == 2)
	{
		Conditional_statements();
		return true;
	}
	else
	{
		return false;
	}
}



int main()
{
	freopen("result.txt", "w", stdout);
	printf("语法分析：\n");
	p = 0;
	cout << "********************************************" << endl;
	cout << "程序开始" << endl;
	numof_ += 4;
	Constan_Description();
	Variable_Description();
	do {
		Scanner();
		Statement();
	} while (syn != 0);
	cout << "********************************************" << endl;
	cout << "程序结束" << endl;
	cout << "按任意键退出" << endl;
	return 0;
}
