#include<iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
using namespace std;

#define ID 15   // 非关键字的标识符
#define NUM 16   // 数值量

//存放处理后的字符串 
char tempstr[255] = {};
//空格标志 
bool temp = false;
//临时数组
char word[255] = {};
//keyword关键字 
string keyword[9] = {
	"Const", "Var", "if", "else", "then",
	"while", "do", "begin", "end"
};

int keyword_num[9] = {
	1,2,3,4,5,
	6,7,8,9
};
//部分运算符，定界符等 
char symbol[10] = { '+','-','*','/',';','(',')','{','}',',' };
//对应的种码值
int symbol_num[10] = { 21,22,23,24,25,26,27,28,29,40 };



int type[1000] = { 0 };
int type_p = 0;
int q = 0;

// int parse_error = 0;   // 语法检测错误标记


bool IsLetter(char ch); //判断是否为字母 
bool IsDigit(char ch);   //判断是否为数字 
int IsSymbol(char ch);  //判断是否为定界符等
int IsKeyword(string str);  //判断是否为关键字 
void HandleSpace(char a[]);   //空格处理
void prePro();  //处理"//"注释
void Scanner(char *str);  //单句 词法扫描


void Parse(); // 语法分析
bool Statement();  // 语句
bool Assignment_Statement(); // 赋值语句  
bool Looping_Statement();  // 循环语句
bool Conditional_Statement(); // 条件语句
bool Condition();  // 条件
bool Compound_Statement(); // 复合语句
bool Const_Def();  // 常量定义
bool Const_Desc();    // 常量说明
bool Var_Def();   // 变量定义
bool Var_Desc();   // 变量说明
bool Expression();   // 表达式
bool Term();  // 加减运算
bool Factor();   // 因子

int main()
{
	char instr[255] = {}; //接收输入字符串 
	char *str = NULL;//存放每行的字符串 
	char delims[] = " ";//分割标志 
	freopen("test.cpp", "r", stdin);
	freopen("result.txt", "w", stdout);

	printf("词法分析结果如下：\n");
	printf("******************************\n");

	while ((gets_s(instr)) != NULL)
	{
		HandleSpace(instr);
		prePro();

		str = strtok(tempstr, delims);//分割字符串 

		while (str != NULL)
		{
			//头文件，宏定义
			if (*(str) == '#')
			{
				printf("#\n");
				break;
			}

			Scanner(str);

			str = strtok(NULL, delims);
		}
	}

	Parse(); // 语法分析

	return 0;
}

// 语法分析
void Parse()
{
	printf("\n\n语法分析结果如下：\n");
	printf("********************************\n");
	Const_Desc();
	Var_Desc();
	do {
		q++;
	//	Statement();
	} while (Statement() && q <= type_p);
//	printf("%d\n%d\n", q, type_p);

	if (q >= type_p)
	{
		printf("********************************\n");
		printf("Parse Success! Congratulations!\n");
		printf("********************************\n");
	}
}

// 语句
bool Statement()
{
	if (*(type + q) == ID)   // ID
	{
		if (Assignment_Statement())  // 赋值语句
			return true;
		else
		{
			return false;
		}
	}
	else if (*(type + q) == 6)    // while
	{
		if (Looping_Statement())   // 循环语句
			return true;
		else
		{
			return false;
		}
	}
	else if (*(type + q) == 3)   // if
	{
		if (Conditional_Statement())  // 条件语句
			return true;
		else
		{
			return false;
		}
	}
	else if (*(type + q) == 8)  // begin
	{ 
		if(Compound_Statement())    // 复合语句
			return true;
		else
		{
			return false;
		}
	}
	return false;
}

// 赋值语句
bool Assignment_Statement()
{
	q++;
	if (*(type + q) == 35)  // =
	{
		q++;
		if (Expression())
			return true;
		else
		{
			return false;
		}
	}
	else
	{
		printf("Assignment statement error! missing '=', cursor = %d\n", q);
		return false;
	}
}

// 循环语句
bool Looping_Statement()
{
	q++;
	if (Condition())
	{
		if (*(type + q) == 7)  // do
		{
			q++;
			if (Statement())  // 语句
				return true;
			else
			{
				return false;
			}
		}
		else
		{
			printf("Looping statement error! missing 'do', cursor = %d\n", q);
			return false; 
		}
	}
	else
	{
		return false;
	}
}

// 条件语句
bool Conditional_Statement()
{
	if (*(type + q) == 3)  // if
	{
		q++;
		if (Condition())   // 条件
		{
			if (*(type + q) == 5)   // then
			{
				q++;
				if (Statement())   // 语句
				{
					if (*(type + q) == 4)   // else
					{
						q++;
						if (Statement())
							return true;
						else
						{
							return false;
						}
					}
					else
						return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				printf("Conditional statement error! missing 'then', cursor = %d\n", q);
				return false;
			}
		}
		else
		{
			return false;
		}
	}
}

// 条件
bool Condition()
{
	Expression();  // 表达式
	// 关系运算符
	if ((*(type + q) == 30) || (*(type + q) == 31) || (*(type + q) == 32) || (*(type + q) == 33) || (*(type + q) == 34) || (*(type + q) == 36))
	{
		q++;
	}
	else 
	{
		printf("Condition statement error! relational operator error, cursor = %d\n", q);
		return false;
	}
	Expression();  // 表达式
}

// 复合语句
bool Compound_Statement()
{
	q++;
	while (Statement())
	{
		if (*(type + q) == 25)   // ;
		{
			q++;
			if (*(type + q) == 9)  // end
			{
				break;
			}
		}
	} 
	if (*(type + q) == 9)  // end
	{
		q++;
		return true;
	}
	else
	{
		printf("Compound statement error! missing 'end', cursor = %d\n", q);
		return false;
	}
}

// 常量定义
bool Const_Def()
{
	if (*(type + q) == ID)  // ID
	{
		q++;
		if (*(type + q) == 35)  // =
		{
			q++;
			if (*(type + q) == NUM)
			{
				printf("Const define success!\n");
				return true;
			}
			else
			{
				printf("Const define error! missing unsigned integer, cursor = %d\n", q);
				return false;
			}
		}
		else
		{
			printf("Const define error! missing '=', cursor = %d\n", q);
			return false;
		}
	}
	else
	{
		printf("Const define error! missing ID, cursor = %d\n", q);
		return false;
	}
}

// 常量说明
bool Const_Desc()
{
//	q++;
	if (*(type + q) == 1)  // Const
	{
		q++;
		while (Const_Def())
		{
			q++;
			if (*(type + q) == 25)  // ;
			{
				printf("Const description success!\n");
				return true;
			}
			else if (*(type + q) == 40)   // ,
			{
				q++;
				continue;
			}
			else
			{
				printf("Const description error! missing ';', cursor = %d\n", q);
				return false;
			}
		}
	}
	else
	{
		printf("Const description error! missing 'Const', cursor = %d\n", q);
		return false;
	}
}

// 变量定义
bool Var_Def()
{
	if (*(type + q) == ID)  // ID
	{
		printf("Var define success!\n");
		return true;
	}
	else
	{
		printf("Var define error! cursor = %d\n", q);
		return false;
	}
}

// 变量说明
bool Var_Desc()
{
	if ((*(type + q) == 3) || (*(type + q) == 6) || (*(type + q) == 8) || (*(type + q) == ID)) // if, while, begin, ID
	{
		return true;
	}
	q++;
	if (*(type + q) == 2)   // Var
	{
		q++;
		while (Var_Def()) {
			q++;
			if (*(type + q) == 25)  // ;
			{
				printf("Var description success!\n");
				return true;
			}
			else if (*(type + q) == 40)   // ,
			{
				q++;
				continue;
			}
		}
	}
	else
	{
		printf("Var description error! missing 'Var', cursor = %d\n", q);
		return false;
	}
}

// 表达式
bool Expression()
{
	do {
		if ((*(type + q) == 21) || (*(type + q) == 22))  // [+/-]
		{
			q++;
			Term();  // 项
		}
		else
		{
			Term();  //项
		}

	} while ((*(type + q) == 21) || (*(type + q) == 22));  // +, -

	return true;
}

// 项
bool Term()
{
	while (Factor())
	{
		if ((*(type + q) == 23) || (*(type + q) == 24)) // *, /
		{
			q++;
		}
		else
			return true;
	}
	printf("Term error! Cannot find factor, cursor = %d\n", q);
	return false;
}

// 因子
bool Factor()
{
	if (*(type + q) == ID)   // 标识符
	{
		q++;
		return true; 
	}
	else if (*(type + q) == NUM)  // 无符号整数
	{
		q++;
		return true;
	}
	else if(*(type + q) == 26)  // (
	{
		q++;
		if (Expression())  // 重新检测 小括号 内部的表达式
		{
			if (*(type + q) == 27)  // )
			{
				q++;
				return true;
			}
			else
			{
				printf("Factor error! missing ')', cursor = %d\n", q);
				return false;
			}
		}
		else
			return false;
	}
	else
	{
		printf("Factor error! missing '('!, cursor = %d\n", q);
		return false;
	}
}

/*******************************  词法分析  **************************************/
// 单句 词法扫描
void Scanner(char *str) {
	bool flag = false; //多行注释标志,false为未处于注释区域 
	string Token;//存放字符串 

	for (int i = 0; i<strlen(str); i++)
	{
		if (*(str + i) == '/')
		{
			if (*(str + i + 1) == '*')
			{
				flag = true;
				break;
			}
		}
		//注释处理: */,注释区域结束 
		if (*(str + i) == '*'&&flag)
		{
			if (*(str + i + 1) == '/')
			{
				flag = false;
				i++;
				break;
			}
		}

		//标识符，关键词 
		if (IsLetter(*(str + i)) && (!flag))
		{
			while (IsLetter(*(str + i)) || IsDigit(*(str + i)) || *(str + i) == '_')
			{
				Token += *(str + i);
				i++;
			}
			if (IsKeyword(Token) != ID)
			{
				int syb = keyword_num[IsKeyword(Token)];
				*(type + type_p++) = syb;
			}
			else
			{
				*(type + type_p++) = ID;
			}
			printf("%2d: < %s , %d >\n", type_p, Token.c_str(), *(type + type_p - 1));
			Token = "";
		}

		if (IsDigit(*(str + i)) && (!flag))
		{
			while (IsDigit(*(str + i)))
			{
				Token += *(str + i);
				i++;
			}
			*(type + type_p++) = NUM;
			printf("%2d: < %s , %d >\n", type_p, Token.c_str(), *(type + type_p - 1));
			Token = "";
		}

		//<,<=,<>
		if (*(str + i) == '<' && (!flag))
		{
			if (*(str + i + 1) == '=')
			{
				*(type + type_p++) = 32;
				printf("%2d: < %s , %d >\n", type_p, "<=", *(type + type_p - 1));
			}
			if (*(str + i + 1) == '>')
			{
				*(type + type_p++) = 31;
				printf("%2d: < %s , %d >\n", type_p, "<>", *(type + type_p - 1));
			}
			else
			{
				*(type + type_p++) = 30;
				printf("%2d: < %s , %d >\n", type_p, "<", *(type + type_p - 1));
			}
		}
		//>,>=
		else if (*(str + i) == '>' && (!flag))
		{
			if (*(str + i + 1) == '=')
			{
				*(type + type_p++) = 34;
				printf("%2d: < %s , %d >\n", type_p, ">=", *(type + type_p - 1));
			}
			else
			{
				*(type + type_p++) = 33;
				printf("%2d: < %s , %d >\n", type_p, ">", *(type + type_p - 1));
			}
		}
		//=,==
		else  if (*(str + i) == '=' && (!flag))
		{
			if (*(str + i + 1) == '=')
			{
				*(type + type_p++) = 36;
				printf("%2d: < %s , %d >\n", type_p, "==", *(type + type_p - 1));
			}
			else
			{
				*(type + type_p++) = 35;
				printf("%2d: < %s , %d >\n", type_p, "=", *(type + type_p - 1));
			}
		}

		//余下定界符等
		else if (IsSymbol(*(str + i)) != -1 && (!flag))
		{
			int syb = symbol_num[IsSymbol(*(str + i))];
			*(type + type_p++) = syb;
			printf("%2d: < %c , %d >\n", type_p, *(str + i), syb);
		}
	}
}

//判断是否为字母 
bool IsLetter(char ch)
{
	if ((ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z'))
		return true;
	return false;
}

//判断是否为数字 
bool IsDigit(char ch)
{
	if (ch >= '0'&&ch <= '9')
		return true;
	return false;
}

//判断是否为定界符等
int IsSymbol(char ch)
{
	for (int i = 0; i<10; i++)
	{
		if (ch == symbol[i])
			return i;
	}
	return -1;
}

//判断是否为关键字 
int IsKeyword(string str)
{
	for (int i = 0; i<9; i++)
	{
		if (str == keyword[i])
		{
			return i;
		}
	}
	//不是关键字即为ID
	return ID;
}

//空格处理
void HandleSpace(char a[])
{
	int j = 0;
	memset(word, 0, 255);//需要清空，不然可能残留上次的字符串
	temp = false;
	for (int i = 0; i < strlen(a); i++)
	{

		if (a[i] != ' ' && a[i] != '\t')
		{
			word[j++] = a[i];
			temp = false;
		}
		else
		{

			if (!temp && a[i] != '\t')
			{
				word[j++] = a[i];
				temp = true;
			}

		}
	}
}

//处理"//"注释
void prePro()
{
	int j = 0;
	memset(tempstr, 0, 255);
	for (int i = 0; i < strlen(word); i++)
	{
		if (word[i] == '/' && word[i + 1] == '/')
		{
			while (i < strlen(word))
			{
				i++;
			}
		}

		else {
			tempstr[j++] = word[i];
		}
	}
}